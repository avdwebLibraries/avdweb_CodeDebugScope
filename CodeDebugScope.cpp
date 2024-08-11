/*
  LET OP: Serial.begin(115200) IN DE scope LIB

  The library <Streaming.h> has to be installed too, download here: http://arduiniana.org/libraries/streaming/
  The library <avdweb.h> has to be installed too, download here: nog doen

  Copyright (C) 2016  Albert van Dalen http://www.avdweb.nl
  This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License at http://www.gnu.org/licenses .

  HISTORY:
  1.0.0 1-1-2016
  1.0.1 14-1-2016 sample0_us
  1.0.2 10-2-2017 1, 2, 3, 4 channels
  1.0.3 10-2-2017 probeA sampleB probeABC probeABCD to avoid mistakes, all unsigned int -> int, added stop(), sizeof(int)
                  SerialUSB -> Serial (incl. <Albert.h>), short
  1.0.4 10-3-2017 for ISR measurements do show() in loop()
  1.0.5 28-1-2018 renamed sample to probe, Scope to SWscope

  start           ___|____________________________________|________

  probe           |_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_

  trigger         ____________|________________________________________
                             ___________________________
  triggered       ____________|                           |___________

  samples         0 0 1 2 3 . . .
                                         ________
  show            ________________________|        |______________
                    ____________________                 ___________
  samplingOn      ___|                    |_______________|

  canShow         ________________________|___________________________

*/

#include "CodeDebugScope.h"
#include <Streaming.h>


template<class T> // easy printing multiple variables with separator ',' example: Serial << endl << a, b, c;
inline Print &operator , (Print &stream, const T arg)
{ stream.print(" ");
  stream.print(arg);
  return stream;
}

void SWscope::start(byte _channels, int _preSamples, unsigned int _recordLenght)
{ ptr = samples = triggerPtr = preSamples = triggered = 0;
  stopPtr = 32767;
  samplingOn = 1;
  channels = _channels;
  recordLenght = maxScopeBytes / (sizeof(short) * channels);
  if (_recordLenght <= recordLenght) recordLenght = _recordLenght;
  if (abs(_preSamples) <= recordLenght) preSamples = _preSamples;
}

void SWscope::showIfReady()
{ if (!canShow) return;
  canShow = 0;
  Serial.begin(115200); // HIER doen Serial part moet buiten de bibliotheek
  //interrupts(); // hoeft niet
  while (!Serial); // wait on Serial Monitor
  Serial << "\nusPerDiv: " << usPerDiv << "\nptr, values ";
  ptr = stopPtr;
  do
  { if (channels == 1) Serial << endl << ptr, ringBuffer.chA[ptr];
    if (channels == 2) Serial << endl << ptr, ringBuffer.chAB[ptr][0], ringBuffer.chAB[ptr][1];
    if (channels == 3) Serial << endl << ptr, ringBuffer.chABC[ptr][0], ringBuffer.chABC[ptr][1], ringBuffer.chABC[ptr][2];
    if (channels == 4) Serial << endl << ptr, ringBuffer.chABCD[ptr][0], ringBuffer.chABCD[ptr][1], ringBuffer.chABCD[ptr][2], ringBuffer.chABCD[ptr][3];
    if (ptr == triggerPtr) Serial << " trigger";
    ptr = calcPtr(ptr + 1);
  }
  while (ptr != stopPtr);
  stopPtr = 32767;
}

void SWscope::stop()
{ stopPtr = calcPtr(ptr + 1);
}

void SWscope::probeA(short valueA) // 1 channel
{ if (samplingOn)
  { ringBuffer.chA[ptr] = valueA;
    sampleControl();
  }
}

void SWscope::probeAB(short valueA, short valueB) // 2 channels
{ if (samplingOn)
  { ringBuffer.chAB[ptr][0] = valueA;
    ringBuffer.chAB[ptr][1] = valueB;
    sampleControl();
  }
}

void SWscope::probeABC(short valueA, short valueB, short valueC) // 3 channels
{ if (samplingOn)
  { ringBuffer.chABC[ptr][0] = valueA;
    ringBuffer.chABC[ptr][1] = valueB;
    ringBuffer.chABC[ptr][2] = valueC;
    sampleControl();
  }
}

void SWscope::probeABCD(short valueA, short valueB, short valueC, short valueD) // 4 channels
{ if (samplingOn)
  { ringBuffer.chABCD[ptr][0] = valueA;
    ringBuffer.chABCD[ptr][1] = valueB;
    ringBuffer.chABCD[ptr][2] = valueC;
    ringBuffer.chABCD[ptr][3] = valueD;
    sampleControl();
  }
}

void SWscope::sampleControl()
{ //doen micros weggelaten if(samples == 0) sample0_us = micros();
  samples++;
  ptr = calcPtr(ptr + 1);
  if (ptr == stopPtr)
  { samplingOn = 0;
    canShow = 1;
    unsigned long stop_us;//doen = micros(); // to avoid delay, start with this
    usPerDiv = samples > 1 ? (stop_us - sample0_us) / (samples - 1) : 0; // is not exact?
  }
}

unsigned int SWscope::calcPtr(int _ptr)
{ if (_ptr >= recordLenght) return (_ptr - recordLenght); // happens most frequent
  if (_ptr < 0) return (_ptr + recordLenght);
  return _ptr;
}

void SWscope::trigger()
{ if (!triggered)
  { triggerPtr = ptr;
    stopPtr = calcPtr(triggerPtr - preSamples);
    triggered = 1;
  }
}

void SWscope::testBuffer()
{ start(1);
  for (int i = 0; i < 25000; i++)
  { if (i == 0) trigger();
    probeA(i);
  }
}

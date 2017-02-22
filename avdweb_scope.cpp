/*                                                     
Scope.cpp

The library <Streaming.h> has to be installed too, download here: http://arduiniana.org/libraries/streaming/
The library <Albert.h> has to be installed too, download here: http://www.avdweb.nl/arduino/albert-library.html
                                                     
Copyright (C) 2016  Albert van Dalen http://www.avdweb.nl
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License at http://www.gnu.org/licenses .

Version 1-1-2016
Version 14-1-2016 sample0_us
Version 10-2-2017 1, 2, 3, 4 channels
Version 10-2-2017 sampleA sampleB sampleABC sampleABCD to avoid mistakes
                  all unsigned int -> int
                  added stop()
                  sizeof(int)

start           ___|____________________________________|________

sample          |_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_
                             
trigger         ____________|________________________________________
                             ___________________________
triggered       ____________|                           |___________

samples         0 0 1 2 3 . . . 
                                         ________
show            ________________________|        |______________
                    ____________________                 ___________
samplingOn      ___|                    |_______________|

*/

#include "avdweb_scope.h"
#include <Albert.h> // this library has to be installed too
#include <Streaming.h> // this library has to be installed too

void Scope::start(byte _channels, int _preSamples, unsigned int _recordLenght)  
{ ptr = samples = triggerPtr = preSamples = triggered = 0;
  stopPtr=32767;
  samplingOn=1;
  channels = _channels;
  recordLenght = maxBytes/(sizeof(int)*channels);
  if(_recordLenght <= recordLenght) recordLenght = _recordLenght;
  if(abs(_preSamples) <= recordLenght) preSamples = _preSamples; 
}

void Scope::stop()
{ stopPtr = calcPtr(ptr+1);
}

void Scope::show() 
{ unsigned long stop_us = micros(); // to avoid delay, start with this
  unsigned long usPerDiv = samples > 1 ? (stop_us - sample0_us)/(samples-1) : 0; 
  while(!SerialUSB); // wait on Serial Monitor 
  SerialUSB << "\nusPerDiv " << usPerDiv;
  samplingOn=0;
  SerialUSB << "\nptr value ";
  ptr=stopPtr;
  do 
  { if(channels==1) SerialUSB << endl << ptr, ringBuffer.chA[ptr]; 
    if(channels==2) SerialUSB << endl << ptr, ringBuffer.chAB[ptr][0], ringBuffer.chAB[ptr][1]; 
    if(channels==3) SerialUSB << endl << ptr, ringBuffer.chABC[ptr][0], ringBuffer.chABC[ptr][1], ringBuffer.chABC[ptr][2]; 
    if(channels==4) SerialUSB << endl << ptr, ringBuffer.chABCD[ptr][0], ringBuffer.chABCD[ptr][1], ringBuffer.chABCD[ptr][2], ringBuffer.chABCD[ptr][3]; 
    if(ptr==triggerPtr) SerialUSB << " trigger";
    ptr = calcPtr(ptr+1);  
  } 
  while (ptr!=stopPtr);
  stopPtr = 32767;  
}

void Scope::sampleA(int valueA) // 1 channel
{ if(samplingOn)
  { ringBuffer.chA[ptr] = valueA;
    sampleControl();
  }
}

void Scope::sampleAB(int valueA, int valueB) // 2 channels
{ if(samplingOn)
  { ringBuffer.chAB[ptr][0] = valueA;
    ringBuffer.chAB[ptr][1] = valueB;
    sampleControl();
  }
}

void Scope::sampleABC(int valueA, int valueB, int valueC) // 3 channels
{ if(samplingOn)
  { ringBuffer.chABC[ptr][0] = valueA;
    ringBuffer.chABC[ptr][1] = valueB;
    ringBuffer.chABC[ptr][2] = valueC;
    sampleControl();
  }
}

void Scope::sampleABCD(int valueA, int valueB, int valueC, int valueD) // 4 channels
{ if(samplingOn)
  { ringBuffer.chABCD[ptr][0] = valueA;
    ringBuffer.chABCD[ptr][1] = valueB;
    ringBuffer.chABCD[ptr][2] = valueC;
    ringBuffer.chABCD[ptr][3] = valueD;
    sampleControl();
  }
}

void Scope::sampleControl()
{ if(samples == 0) sample0_us = micros();  
  samples++;
  ptr = calcPtr(ptr+1); 
  if(ptr==stopPtr) show();
}

unsigned int Scope::calcPtr(int _ptr)
{ if(_ptr>=recordLenght) return (_ptr - recordLenght); // happens most frequent
  if(_ptr<0) return (_ptr + recordLenght);
  return _ptr;
}

void Scope::trigger()
{ if(!triggered)
  { triggerPtr = ptr;
    stopPtr = calcPtr(triggerPtr-preSamples);
    triggered=1;
  }
}

void Scope::testBuffer()
{ start(1);
  for(int i=0; i<25000; i++)
  { if(i==0) trigger(); 
    sampleA(i);
  }  
}
  






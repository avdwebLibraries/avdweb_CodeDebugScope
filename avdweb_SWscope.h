/*                                          
Copyright (C) 2016  Albert van Dalen http://www.avdweb.nl
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License at http://www.gnu.org/licenses .
*/
 
#ifndef SWscope_H
#define SWscope_H

#include <Arduino.h>

#if defined(__arm__) 
const unsigned int maxBytes = 7000; // SAMD21: max ~ 7000
#else
const int maxBytes = 700; // AVR: max ~ 780
#endif

class SWscope
{
public:
  void start(byte _channels, int _preSamples=0, unsigned int _recordLenght=65535);
  void probeA(short valueA);
  void probeAB(short valueA, short valueB);
  void probeABC(short valueA, short valueB, short valueC);
  void probeABCD(short valueA, short valueB, short valueC, short valueD);
  void trigger();
  void stop();
  void showIfReady();
  void testBuffer();

  bool canShow;
   
  protected:
  void sampleControl();
  unsigned int calcPtr(int ptr);

  union{short chA[maxBytes/2]; 
        short chAB[maxBytes/4][2]; 
        short chABC[maxBytes/3][3]; 
        short chABCD[maxBytes/8][4];} ringBuffer; 

  unsigned long sample0_us, usPerDiv;
  bool triggered, samplingOn; 
  byte channels; // 1, 2, 3, 4 channels
  int recordLenght, preSamples, ptr, samples, triggerPtr, stopPtr;
};

#endif



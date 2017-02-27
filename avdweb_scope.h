/*
Scope.h
                                                     
Copyright (C) 2016  Albert van Dalen http://www.avdweb.nl
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License at http://www.gnu.org/licenses .
*/
 
#ifndef SCOPE_H
#define SCOPE_H

#include <Arduino.h>

#if defined(__arm__) 
const unsigned int maxBytes = 2000; // SAMD21: max ~ 7000
#else
const int maxBytes = 700; // AVR: max ~ 780
#endif

class Scope
{
public:
  void start(byte _channels, int _preSamples=0, unsigned int _recordLenght=65535);
  void sampleA(short valueA);
  void sampleAB(short valueA, short valueB);
  void sampleABC(short valueA, short valueB, short valueC);
  void sampleABCD(short valueA, short valueB, short valueC, short valueD);
  void trigger();
  void stop();
  void testBuffer();
   
protected:
  void sampleControl();
  unsigned int calcPtr(int ptr);
  void show();

  union{short chA[maxBytes/2]; 
        short chAB[maxBytes/4][2]; 
        short chABC[maxBytes/3][3]; 
        short chABCD[maxBytes/8][4];} ringBuffer; 

  unsigned long sample0_us;
  bool triggered, samplingOn; 
  byte channels; // 1, 2, 3, 4 channels
  int recordLenght, preSamples, ptr, samples, triggerPtr, stopPtr;
};

#endif



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

const unsigned int maxBytes = 2000; // SAMD21: max ~ 7000
//const int maxBytes = 700;         // AVR: max ~ 780

class Scope
{
public:
  void start(byte _channels, int _preSamples=0, unsigned int _recordLenght=65535);
  void sample(int valueA);
  void sample(int valueA, int valueB);
  void sample(int valueA, int valueB, int valueC);
  void sample(int valueA, int valueB, int valueC, int valueD);
  void trigger();
  void testBuffer();
   
protected:
  void sampleControl();
  unsigned int calcPtr(int ptr);
  void show();

  union{int chA[maxBytes/2]; 
        int chAB[maxBytes/4][2]; 
        int chABC[maxBytes/3][3]; 
        int chABCD[maxBytes/8][4];} ringBuffer; 

  unsigned long sample0_us;
  bool triggered, samplingOn; 
  byte channels; // 1, 2, 3, 4 channels
  unsigned int recordLenght, ptr, samples, triggerPtr, stopPtr; 
  int preSamples;
};

#endif



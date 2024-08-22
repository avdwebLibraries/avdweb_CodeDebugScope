/**
 * CodeDebugScope.h
 *
 * CodeDebugScope - Arduino Fast Debug Oscilloscope Library.
 * ===========================================================
 *
 * Sources
 * -------
 * https://github.com/avandalen/avdweb_CodeDebugScope
 *
 * License
 * --------
 * SPDX: GPL-3.0-or-later
 *
 *  CodeDebugScope - Arduino Fast Debug Oscilloscope Library.
 *  Copyright (C) 2016-2024 Albert van Dalen <http://www.avdweb.nl>
 *  Copyright (C) 2024 Abhijit Bose (aka. Boseji) :contributor:
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef CodeDebugScope_H
#define CodeDebugScope_H

#include <Arduino.h>

#if defined(__arm__)
const unsigned int maxScopeBytes = 7000; // SAMD21: max ~ 7000
#else
const int maxScopeBytes = 500; // ATTENTION SET HERE, AVR ATmega328: max ~ 780 ATmega168: max ~ 320
// const int maxScopeBytes = 100; // ATTENTION SET HERE, AVR ATmega328: max ~
// 780 ATmega168: max ~ 320
#endif

class SWscope {
public:
    void start(byte _channels, int _preSamples = 0,
        unsigned int _recordLenght = 65535);
    void probeA(short valueA);
    void probeAB(short valueA, short valueB);
    void probeABC(short valueA, short valueB, short valueC);
    void probeABCD(short valueA, short valueB, short valueC, short valueD);
    void trigger();
    void stop();
    void showIfReady();
    void testBuffer();

    volatile bool canShow;

protected:
    void sampleControl();
    unsigned int calcPtr(int ptr);

    volatile union {
        short chA[maxScopeBytes / 2];
        short chAB[maxScopeBytes / 4][2];
        short chABC[maxScopeBytes / 3][3];
        short chABCD[maxScopeBytes / 8][4];
    } ringBuffer;

    volatile unsigned long sample0_us, usPerDiv;
    volatile bool triggered, samplingOn;
    volatile byte channels; // 1, 2, 3, 4 channels
    volatile int recordLenght, preSamples, ptr, samples, triggerPtr, stopPtr;
};

#endif

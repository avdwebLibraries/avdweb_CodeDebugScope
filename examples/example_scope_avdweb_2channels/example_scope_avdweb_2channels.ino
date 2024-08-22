#include "CodeDebugScope.h" // https://avdweb.nl/arduino/measurement/oscilloscope

// Attention: set in avdweb_SWscope.h: const int maxScopeBytes = 100; // or more
// if possible

SWscope scope;

void setup(void)
{
    Serial.begin(115200);
    scope.start(2, 10); // 2 integer channels , preSamples = 10, buffer is
                        // 100/4=25 integer values
    // scope.testBuffer(); // print 100/2 = 50 integer values with only channel A
}

void loop(void)
{
    static int internalValue;
    internalValue += 2;
    scope.probeAB(internalValue,
        analogRead(A0)); // channel A = internalValue, channel B = ADC
    if (internalValue > 10)
        scope.trigger();
    scope.showIfReady();
}

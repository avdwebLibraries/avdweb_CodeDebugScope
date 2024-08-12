#include "CodeDebugScope.h"
#include <FrequencyTimer2.h> // mainISR https://github.com/PaulStoffregen/FrequencyTimer2

// Attention: set in avdweb_SWscope.h: const int maxScopeBytes = 100; // or more
// if possible

SWscope scope;

void setup(void) {
  Serial.begin(115200);
  FrequencyTimer2::setPeriod(250);         // period = 250us
  FrequencyTimer2::setOnOverflow(mainISR); // start ISR here
  scope.start(2, 10); // 2 integer channels , preSamples = 10, buffer is
                      // 100/4=25 integer values
  scope.trigger();
}

void mainISR(void) {
  static int internalValue;
  internalValue += 2;
  scope.probeAB(internalValue,
                analogRead(A0)); // channel A = internalValue, channel B = ADC
  if (internalValue > 10)
    scope.trigger();
}

void loop(void) { scope.showIfReady(); }

#pragma once

// methods for activating the LEDs
#define USE_PWM true
#define USE_DIG false
// sub-demos of the LEDs 
#define PWM_TABLE false  // ignored when USE_PWM is false
// pins
#define BUTTON  4   // GPIO4 
#define RED_LED 15  // GPIO15
#define GRN_LED 12  // GPIO12
#define BLU_LED 13  // GPIO13
#define LDR_IN  A0  // ADC
// change to 'true' to see more console output
#define DBGOUT false
// LED timing
#if DBGOUT
//      500ms between LED state changes, note
//      that the LED static time value will have
//      an effect on the appearance. the static
//      times are: PWM_ZERO_DELAY & PWM_MAX_DELAY,
//      and DIGLED_DELAY
#define LED_INTERVAL    500
//      change the PWM ratio by 10
#define PWMSTEP         10
//      250ms between LDR(analog) readings
#define LDR_INTERVAL    250
// run-time delays
#define PWM_ZERO_DELAY  3000
#define PWM_MAX_DELAY   2000
#else   // DBGOUT
#define LED_INTERVAL    25
#define PWMSTEP         1
#define LDR_INTERVAL    25
#define PWM_ZERO_DELAY  1500
#define PWM_MAX_DELAY   1000
#endif  // DBGOUT

// the amount of time (in milliseconds) that
// the LED will stay in its current state. note
// that the LED_INTERVAL value will have an 
// effect on the appearance.
#define DIGLED_FASTEST  1
#define DIGLED_FAST     50
#define DIGLED_SLOW     250
#define DIGLED_SLOWER   500
#define DIGLED_SLOWEST  1000
#define DIGLED_DELAY    DIGLED_SLOWER

// indices into the PWM color table
#define PWMLED_RED      0
#define PWMLED_GRN      1
#define PWMLED_BLU      2

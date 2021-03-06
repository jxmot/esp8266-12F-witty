/*
    esp8266-12F-witty.ino - demo sketch intended to
    run on the "Witty" board (ESP8266-12F, with a 
    3-wire RGB LED, LDR(photocell), and a push button.

    NOTE: Select the "WeMos D1(Retired)" board to build
    this sketch.
*/
#include "esp8266-12F-witty.h"

// for choosing the method for activating the LEDs
//      default is PWM, USE_DIG is the other choice
bool ledMode = USE_PWM;
// set time for the next LED access
//      LED_INTERVAL milliseconds in the future
unsigned long nextLED = LED_INTERVAL + millis();
//      0 = LED is off
int pwm = 0;
//      0 = R,G, or B is OFF
//      for digital control only, the code will determine
//      how the individual LEDs are turned on or off
// 
//      different patterns have somewhat different effects,
//      the green led on the board I used was much brighter
//      than the other two. so most of the time it looked
//      very green.
int rgbLED[3] = {HIGH, LOW, LOW};
//int rgbLED[3] = {HIGH, LOW, HIGH};
//int rgbLED[3] = {HIGH, HIGH, LOW};

int pwmIdx = 0;
int pwmLED[11][3] = {
    {255,  0,  0},  // red 
    {  0,255,  0},  // green 
    {  0,  0,255},  // blue 
    {255,255,255},  // white
    {237,109,  0},  // orange
    {  0, 46, 90},  // indigo
    {255,  0,255},  // magenta
    {153, 50,204},
    {255,  0,255},  // yellow
    {  0,255,255},  // cyan
    {  0,  0,  0}   // off
};
// set time for the next LDR reading
//      LDR_INTERVAL milliseconds in the future
unsigned long nextLDR = LDR_INTERVAL + millis();
int LDRvalue = 0;
// loop() will toggle between the LED and LDR
bool toggle = false;
// The "witty" board will start with the button HIGH
int lastButton = LOW;
int currButton;

/**********************************************************
*/
void setup() 
{
    // initialize and output some blank lines
    Serial.begin(115200);
    Serial.println();
    Serial.println();
    Serial.println();

    // initialize the button...
    initButton();

    // reset all 3 colors to OFF, and use digital
    // 
    resetLED(ledMode);
}

void loop()
{
    yield();

    readButton();

    if(toggle) runLED(ledMode);
    else readLDR();

    toggle = !toggle;
}

/*****************************************
    Initialize the button input to use a
    pull-up. That means that when the 
    button isn't pressed the input will
    read as HIGH. When the button is pressed
    it will ground the line and a LOW will
    be read.
*/
void initButton()
{
    pinMode(BUTTON, INPUT_PULLUP);
}

/*
    Read the button state. If it is different
    from the last time it was read then its
    state has changed. Don't continue to 
    announcing the button state if it has not
    changed.
*/
void readButton()
{
    // read the current state of the button
    currButton = digitalRead(BUTTON);
    // if the button is pressed and the last 
    // time the button was read it was HIGH then...
    if((currButton == LOW) && (lastButton != LOW))
    {
        if(DBGOUT) 
        {
            // announce the change in the button state
            Serial.println("BUTT: LOW " + String(currButton));
        } else {
            Serial.println("LDR : "+String(LDRvalue));
            // PWM or digital mode?
            //      PWM table or sweep display?
            if(ledMode == USE_PWM) 
                if(PWM_TABLE) Serial.println("LEDS: "+String(pwmIdx));
                else Serial.println("LEDS: "+String(pwm));
            else Serial.println("LEDS: "+String(rgbLED[0])+" "+String(rgbLED[1])+" "+String(rgbLED[2]));
        }
    } else {
        // if the button is released and the last 
        // time the button was read it was LOW then...
        if((currButton == HIGH) && (lastButton != HIGH))
        {
            // announce the change in the button state & PWM
            Serial.println("BUTT: HIGH " + String(currButton));
        }
    }

    // save the button state
    lastButton = currButton;
}

/*
    Manage the LED states
*/
void runLED(bool pwmORdig)
{
    // is it time to change the LED?
    if(nextLED < millis())
    {
        // PWM or digital mode?
        //      PWM table or sweep display?
        if(pwmORdig == USE_PWM) 
            if(PWM_TABLE) colorsPWM();
            else sweepPWM();
        else cycleDIG();

        // get the time of the next LED change...
        nextLED = LED_INTERVAL + millis();
    }
}

/* 
    Sweeps from 0 to PWMRANGE, pauses and then resets
    to 0 and pauses. Then starts again.
*/
void sweepPWM()
{
    if(DBGOUT) Serial.println("LEDS: "+String(pwm));

    // yep, adjust the same PWM for each of the colors
    analogWrite(RED_LED, pwm);
    analogWrite(GRN_LED, pwm);
    analogWrite(BLU_LED, pwm);
    // when the PWM is OFF wait for a while
    if(pwm == 0) delay(PWM_ZERO_DELAY);
    // increment the PWM value
    pwm = pwm + PWMSTEP;
    // did we reach the end of the PWM range?
    if(pwm > PWMRANGE) 
    {
        // yep, set the PWM value to OFF(0) and 
        // leave the LED turned ON for a while
        pwm = 0;
        delay(PWM_MAX_DELAY);
    }
}

/*
    Steps circularly though an array of RGB PWM values
    and sets the LEDs to that color mix
*/
void colorsPWM()
{
    analogWrite(RED_LED, pwmLED[pwmIdx][PWMLED_RED]);
    analogWrite(GRN_LED, pwmLED[pwmIdx][PWMLED_GRN]);
    analogWrite(BLU_LED, pwmLED[pwmIdx][PWMLED_BLU]);

    if((pwmLED[pwmIdx][PWMLED_RED] == pwmLED[pwmIdx][PWMLED_GRN])
       && (pwmLED[pwmIdx][PWMLED_GRN] == pwmLED[pwmIdx][PWMLED_BLU])
       && (pwmLED[pwmIdx][PWMLED_BLU] == 0))
    {
        pwmIdx = 0;
        delay(PWM_MAX_DELAY);
    } else  {
        pwmIdx += 1;
        delay(PWM_ZERO_DELAY);
    }
}


/*
    Rotates a pattern of ON/OFF to the LEDs via digital
    I/O. 
*/
void cycleDIG()
{
int temp;

    // rotate the pattern through the 3 LEDs
    temp = rgbLED[2];
    rgbLED[2] = rgbLED[1];
    rgbLED[1] = rgbLED[0];
    rgbLED[0] = temp;

    // write the pattern of ON/OFF to the LEDs
    digitalWrite(RED_LED, rgbLED[0]);
    digitalWrite(GRN_LED, rgbLED[1]);
    digitalWrite(BLU_LED, rgbLED[2]);

    delay(DIGLED_DELAY);
}

/*
    Reset the LEDs to OFF
*/
void resetLED(bool pwmORdig)
{
    ledMode = pwmORdig;

    if(pwmORdig == USE_PWM)
    {
        analogWrite(RED_LED, 0);
        analogWrite(GRN_LED, 0);
        analogWrite(BLU_LED, 0);
    } else {
        pinMode(RED_LED, OUTPUT);
        digitalWrite(RED_LED, LOW);
        delay(100);
        pinMode(GRN_LED, OUTPUT);
        digitalWrite(GRN_LED, LOW);
        delay(100);
        pinMode(BLU_LED, OUTPUT);
        digitalWrite(BLU_LED, LOW);
        delay(100);
    }
}

/*
    Read the LDR if it's time
*/
void readLDR()
{
    // is it time to read the LDR?
    if(nextLDR < millis())
    {
        // yep, read it
        LDRvalue = analogRead(LDR_IN);
        if(DBGOUT) Serial.println("LDR : "+String(LDRvalue));
        // get the time of the next LDR reading...
        nextLDR = LDR_INTERVAL + millis();
    }
}


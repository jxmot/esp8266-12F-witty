# Witty Board Sketch
This sketch contains code that demonstrates the use of a Witty(GizWits) ESP8266-12F & breakout board.

The "Witty" board is similar to many other boards with the exception of containing an LDR, 3-wire RGB LED, and a momentary push button. And like most of the other boards it comes equipped with a daughter board that provides a USB<->serial(CH340G) connection, and a flash and reset button for flashing.

# History

I happened to find a couple of the Witty boards for $5 each. And the ESP8266-12F is a variant that I haven't used yet. So why not?

# Audience Requirements

This project will require *some* knowledge of the ESP8266 and - 

* how to use the Arduino IDE or other tool for preparing a sketch for execution
* how to compile and download a sketch
* a basic understanding of the `setup()` and `loop()` functions of the ESP8266
* a basic understanding of 3-wire RGB LEDs
* a basic understanding of what an LDR(*light dependent resistor, aka "photocell"*) is
* a basic understanding of what PWM (*pulse width modulation*) is
* how to install the driver(*only if you  need it*) for the **`CH340G`**, it is the circuit that allows communication with the board via USB.

# Sketch Overview

The sketch exercises the RBG LED, LDR and the push button. It has the option of using PWM or digital I/O to operate the LEDs. The code is commented to make it easy to adjust the options to get different effects from the LEDs.

## Miscellaneous Details

There are some `#define` statements that can be used for adjusting specific aspects of the sketch. They are - 

* use PWM or digital I/O to run the LEDs :
    *  `USE_PWM`
    *  `USE_DIG`

Find the call to `resetLED()`,  it's the last function call in `setup()`. Change the argument to one of the following - 

`    resetLED(USE_PWM)` - Use PWM and calls to `analogWrite()` to operate the LEDs.
`    resetLED(USE_DIG)` - Uses digial I/O calls to `digitalWrite()` to operate the LEDs.

* enable some additional PWM functionality :
    * `PWM_TABLE` - when `#define`d as `true` the sketch will adjust the LED colors using a table of RGB values.

<br>

* to enable or disable additional output to the console :
    * `DBGOUT` - when `true` additional information will be sent to the console, if `false` the extra output is muted.
    
There are some `#define`d values that can be used to change the timing of the LEDs :

* PWM operation -
    * `LED_INTERVAL` - the amount of time in milliseconds that is between LED operations
    * `PWMSTEP` - the amount that the PWM step value changes
    * `LDR_INTERVAL` - the amount of time in milliseconds that is between LED
    * `PWM_ZERO_DELAY` - the amount of time that the LEDs will stay off during a complete cycle of dark to light
    * `PWM_MAX_DELAY` - the amount of time that the LEDs will stay on before returning to an off state

* digital I/O operations - 
    * `DIGLED_FASTEST` 
    * `DIGLED_FAST`
    * `DIGLED_SLOW`
    * `DIGLED_SLOWER`
    * `DIGLED_SLOWEST`
    * `DIGLED_DELAY` - this can be `#define`d as one of the values above, they range from 1 to 1000 milliseconds. It is the amount of time that the LEDs will stay in a particular pattern.

## LED Operations

### PWM

The PWM operation of the LEDs is a graduated change in state from off to full on. This is accomplished by increasing the PWM value over time. 

### Digital

The digital I/O operation is different. It's because there is no control over the brightness of the LEDs. They're either on or off. Instead a 3 bit pattern is *rotated* through the LEDs. For example:

|  Step |  Red |  Grn |  Blu |
|:-----:|:----:|:----:|:----:|
| Start | HIGH |  LOW |  LOW |
|       |   1  |   0  |   0  |
|   1   |   0  |   1  |   0  |
|   2   |   0  |   0  |   1  |
|   3   |   1  |   0  |   0  |

HIGH or 1 = the LED is on

Starting with different patterns have somewhat different effects.

|  Red |  Grn |  Blu | Color Combos |
|:----:|:----:|:----:|:------------:|
| HIGH |  LOW | HIGH |              |
|   1  |   0  |   1  | red + blue   |
|   1  |   1  |   0  | red + green  |
|   0  |   1  |   1  | green + blue |

## Button & LDR

Pressing the button on the ESP8266 board will be detected and the current input from the LDR will be read. The LDR values can range from about 20 (*when dark*) to 1024 when fully illuminated.

## The loop

The majority of sketch's logic is executed from within the `loop()` function. In this skectch that isn't an issue only because this sketch is simple and doesn't require any precision timing or fast execution of logic within `loop()`.

## Other Things to Note

The green LED on the board I used was much brighter than the red and the blue. A mix of all three LEDs should produce white, but mine did not. Even when diffused. But reproducing specific color mixes wasn't my goal so it was no real issue.

The best way to see mixed colors is to place a piece of white paper over the LED. That will diffuse the light and the mixed color should be easier to see.




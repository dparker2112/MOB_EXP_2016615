/*
 MOB Expansion 2016615
  Board:  ArduinoR3
  Lighting:

    Test Tubes:       3@  https://www.adafruit.com/product/2226
    DNA Sample Tray:  3@  https://www.adafruit.com/product/1426
    Pipette:          6@  https://www.adafruit.com/product/1938
    Pipette Switch:   1@  https://www.mouser.com/ProductDetail/CK-Components/PNP8E3D2Y03QE/?qs=%2fha2pyFadugSqn7RireNfLBzy61sQP%2fFy5X2XMWh5YRy%2fMPDqMplag%3d%3d
    Monitor Switch:   1@  https://www.mouser.com/ProductDetail/CK-Components/AP4D202TZBE/?qs=%2fha2pyFaduiTlLh%252b9I2UnqgY%2fR780juLpEHFjth5Whnd3I5fQOksOA%3d%3d

Pin Assignments:

D0  RX
D1  TX
D2  Free
D3  Test Tube LH (1) Switch
D4  Test Tube MID (2) Switch
D5  Test Tube RH (3) Switch
D6  DNA Receiver Tray LH (1) Switch
D7  DNA Receiver Tray MID (2) Switch
D8  DNA Receiver Tray RH (3) Switch
D9  Pipette lights
D10 Test Tube lights
D11 DNA Receiver Tray Lights
D12 DNA Receiver Tray Lighted Button
D13 Free

  1.	Screen tells you what to do to start, Pipette lights red and first vial lights green.
  2. Touch stylus to vial while holding button, stylus tip goes from red light to green. vile light turns red.
  3. Touch tray through hole, screen light goes green, stylus light goes red. next vile lights green. Repeat three times (touching all three vials).
  4. Once all wells filled, button light turns on.  Push button to initiate sequence - can see it in tray.
  5. Screen now tells you results, including bar graph.
*/

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIP_PIN 9       //Pipette Lights
#define TT_PIN 10       //Test Tube Lights
#define TRAY_PIN 11     //DNA Tray Lights

int pp;
int tt;
int tl;
int read_tt1;
int read_tt2;
int read_tt3;
int read_tr1;
int read_tr2;
int read_tr3;
int previous = LOW;    // the previous reading from the input pin
int state = HIGH;      // the current state of the output pin
int tt1sw = 3;
int tt2sw = 4;
int tt3sw = 5;
int tr1sw = 6;
int tr2sw = 7;
int tr3sw = 8;

// the following variables are long's because the time, measured in milliseconds,
// will quickly become a bigger number than can be stored in an int.
long time = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel PipLites = Adafruit_NeoPixel(6, PIP_PIN, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel TTLites = Adafruit_NeoPixel(21, TT_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel TrayLites = Adafruit_NeoPixel(24, TRAY_PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  //#if defined (__AVR_ATtiny85__)
    //if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  //#endif
  // End of trinket special code
  pinMode(tt1sw, INPUT_PULLUP);

  PipLites.begin();
  PipLites.show(); // Initialize all pixels to 'off'
  TTLites.begin();
  TTLites.show(); // Initialize all pixels to 'off'
  TrayLites.begin();
  TrayLites.show(); // Initialize all pixels to 'off'

}


void loop() {
  for(int tt=15; tt<21; tt++){
  TTLites.setPixelColor(tt, 0, 255, 0);       //Test Tube 1 GREEN
  TTLites.show();
  }
  for(int pp=0; pp<5; pp++){
  PipLites.setPixelColor(pp, 255, 0, 0);      //Pipette RED
  PipLites.show();
  }

  read_tt1 = digitalRead(tt1sw);

  if (read_tt1 == HIGH){

    for(int tt=15; tt<21; tt++){
    TTLites.setPixelColor(tt, 0, 0, 0);
    TTLites.show();                             // Initialize all pixels to 'off'
    }
    for(int pp=0; pp<5; pp++){
    PipLites.setPixelColor(pp, 0, 255, 0);      //Pipette GREEN
    PipLites.show();
    }
    }


}

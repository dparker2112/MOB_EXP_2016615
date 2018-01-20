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
// Uduino Default Board
#include<Uduino.h>
Uduino uduino("uduinoBoard"); // Declare and name your object
Adafruit_NeoPixel pipette = Adafruit_NeoPixel(6, 9, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel tube = Adafruit_NeoPixel(21, 10, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel dna = Adafruit_NeoPixel(24, 11, NEO_GRB + NEO_KHZ800);
int rxpins[7]={6,7,8,3,4,5,12};
int buttonState = -1;
int prevButtonState = -1;
void setup()
{
Serial.begin(9600);
while(!Serial);
uduino.addCommand("SetPipette", SetPipette);
uduino.addCommand("SetDNA", SetDNA);
uduino.addCommand("SetTube", SetTube);
uduino.addCommand("GetContacts", GetContacts);
pipette.begin(); // 6 neodots
pipette.show();
tube.begin(); // 3 neosticks(7)
tube.show();
dna.begin(); // 3 jewels (8)
dna.show();
for(int a=0;a<7;a++){
pinMode(rxpins[a],INPUT_PULLUP);
}
}
void GetContacts()
{
buttonState = -1;
for(int i = 0 ; i < 7; i++)
{
if(!digitalRead(rxpins[i]))
{
buttonState = i;
}
}
Serial.println(buttonState);
}
void loop()
{
uduino.readSerial();
delay(15);
}
void SetPipette(){
char *arg;

arg = uduino.next();
int led = atoi(arg);
arg = uduino.next();
int r = atoi(arg);

arg = uduino.next();
int g = atoi(arg);

arg = uduino.next();
int b = atoi(arg);
pipette.setPixelColor(led, r, g, b);
pipette.show();
}
void SetDNA(){
char *arg;

arg = uduino.next();
int led = atoi(arg);
arg = uduino.next();
int r = atoi(arg);
arg = uduino.next();
int g = atoi(arg);
arg = uduino.next();
int b = atoi(arg);

dna.setPixelColor(led, r, g, b);
dna.show();
}
void SetTube(){
char *arg;

arg = uduino.next();
int led = atoi(arg);
arg = uduino.next();
int r = atoi(arg);
arg = uduino.next();
int g = atoi(arg);
arg = uduino.next();
int b = atoi(arg);

tube.setPixelColor(led, r, g, b);
tube.show();
}
void printValue(int pin, int value, bool isBundle) {
Serial.print(pin);
Serial.print(" ");
if (isBundle) {
Serial.print(value);
Serial.print("-");
} else {
Serial.println(value);
}
}

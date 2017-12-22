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
D2  DNA tray pushbutton LED : HIGH is yellow, LOW is red
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
D13 DNA tray pushbutton LED : HIGH is red, LOW is yellow
  1.	Screen tells you what to do to start, Pipette lights red and first vial lights green.
  2. Touch stylus to vial while holding button, stylus tip goes from red light to green. vile light turns red.
  3. Touch tray through hole, screen light goes green, stylus light goes red. next vile lights green. Repeat three times (touching all three vials).
  4. Once all wells filled, button light turns on.  Push button to initiate sequence - can see it in tray.
  5. Screen now tells you results, including bar graph.
*/

#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel pipette = Adafruit_NeoPixel(1, 9, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel vial = Adafruit_NeoPixel(3, 10, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel DNA = Adafruit_NeoPixel(3, 11, NEO_GRB + NEO_KHZ800);

int rxpins[7]={3,4,5,6,7,8,12};
long unsigned timeout;
int fader;
int dir=1;
int set=3;
int notify=1;

#define TimeOutInSecs 20

void setup() {
  Serial.begin(9600);
  pipette.begin();  // 6 neodots
  pipette.show();
  vial.begin();  // 3 neosticks(8)
  vial.show();
  DNA.begin();     // 3 jewels (7)
  DNA.show();
  for(int a=0;a<7;a++){
    pinMode(rxpins[a],INPUT_PULLUP);
  }
}

void loop(){

  while(Serial.available()>0){
    Serial.print(Serial.read());  // empty serial buffer
  }
  timeout=millis();  // set the "inactivity timer"

  while((!timer||set==3)&&!Serial.available()>0){ // wait for any serial data
    fader+=dir;
    if(fader>=255){dir=-1;}
    if(fader<=0){dir=1;}
    for(int a=0;a<6;a++){
      pipette.setPixelColor(a,fader,fader,fader);  // this pulses the pipette white,, sorta saying come play with me
    }
    if(notify){   // this is so it only says waiting once
      Serial.println("Waiting for input");
      notify=0;
    }
  }
  delay(200);
  while(Serial.available()>0){
    Serial.print(Serial.read()); // empties the serial buffer
  }
  Serial.println("");
  Serial.println("Starting");
  if(!notify){
    notify=1;  // resets the notify variable
    set=0;     //starts the program at vial LH "0"
  }
  Serial.print("set: ");
  Serial.println(set);



  for(int a=0;a<256;a++){
    setpipette(a,0,0);  //fades the pipette from off to red
    setvial(set,0,a,0); //fades the current vial from off to green
    delay(4);
  }
  timeout=millis();    // resets the timout counter
  fader=127;
  Serial.println("waiting for vial touch");
  while(timer&&digitalRead(rxpins[set])){   //waits until you touch the right vial
    setvial(set,0,fader,0);
    delay(4);
    fader++;
    if(fader>=255){fader=160;}
  }
  Serial.print("vial ");
  Serial.print(set);
  Serial.println(" touched");

  for(int a=0;a<256;a++){
    setpipette(255-a,a,0);  //fades the pipette to green
    setvial(set,0,255-a,0); //fades the vial off
    delay(4);
  }
  timeout=millis();
  fader=0;
  Serial.println("waiting for DNA touch");
  while(timer&&digitalRead(rxpins[set+3])){  //waits for you to touch the right DNA tray
    setDNA(set,fader,0,0);  //fades the DNA tray to red then pulses it
    delay(4);
    fader++;
    if(fader>=255){fader=160;}
  }
  for(int a=0;a<256;a++){
    setpipette(0,255-a,0);  //fades the pipette off
    setDNA(set,255-a,a,0);  //fades the DNA tray from red to green
    delay(4);
  }

  Serial.print("set ");
  Serial.print(set);
  Serial.println(" complete");
  set++;                  //goes to next set off DNA/vials
  if(set==3){
    Serial.println("All complete");
    Serial.println("Now restarting in 8 seconds");
    for(int a=0;a<256;a++){
      for(int b=0;b<3;b++){
        setDNA(b,0,255-a,0);
      }
      delay(28);
    }
  }
}



boolean timer(){
  boolean timez=(timeout+(TimeOutInSecs*1000))<millis();
  return(timez);
}

void setpipette(byte rz, byte gz, byte bz){
  for(int a=0;a<6;a++){
    pipette.setPixelColor(a,rz,gz,bz);
  }
  pipette.show();
}
void setDNA(byte poz, byte rz, byte gz, byte bz){
  for(int a=0;a<7;a++){
    DNA.setPixelColor((poz*7)+a,rz,gz,bz);
  }
  DNA.show();
}
void setvial(byte poz, byte rz, byte gz, byte bz){
  for(int a=0;a<8;a++){
    vial.setPixelColor((poz*8)+a,rz,gz,bz);
  }
  vial.show();
}

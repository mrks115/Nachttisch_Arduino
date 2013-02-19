/**
 * PIN 2    RCSwitch Receiver (Interrupt 0)
 * PIN 3    RCSwitch Transmitter
 */
#define SWITCH_IN  0 // Interrupt
#define SWITCH_OUT 13

// Library für 433 MHz Funk
#include <RCSwitch.h>


RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial3.begin(9600);  // Raspberry Pi
  Serial2.begin(9600);  // Colorduino
  Serial.begin(9600);   // Debugging
  
  mySwitch.enableReceive(SWITCH_IN);
  mySwitch.enableTransmit(SWITCH_OUT);
}


void loop() {
  radioReceive();
  receiveSerial();
}





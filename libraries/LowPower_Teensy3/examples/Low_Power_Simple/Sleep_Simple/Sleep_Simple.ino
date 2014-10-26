/**********************************************************
 *  Puts the processor into Wait (VLPW) mode aka... Sleep. 
 *  All system interrupts will exit this sleep mode. Users
 *  must call attachInterrupt for the configured pin or have
 *  whatever interrupt handling routine.
 *
 *  This example shows using the digital pin 0 as wakeup
 *  source.  
 *
 *  Tested and compiled under Arduino 1.0.5 and 
 *  Teensyduino 1.18 
 *********************************************************/
#include <LowPower_Teensy3.h>

TEENSY3_LP LP = TEENSY3_LP();

void callbackhandler() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(0, INPUT_PULLUP);
  attachInterrupt(0, callbackhandler, RISING);
}

void loop() {
  /*****************************************************
   * Setup wake source as Digital Pin 0, using 
   * attachInterrupt function.
   *****************************************************/
  LP.Sleep();
}

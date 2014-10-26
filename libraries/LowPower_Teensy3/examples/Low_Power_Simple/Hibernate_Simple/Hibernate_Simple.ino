/**********************************************************  
 *  Puts the processor into Very Low Leakage Stop (VLLS3) 
 *  mode aka... Hibernate. All I/O is held in a pre 
 *  Hibernate state and USB is disabled. Once Hibernate 
 *  mode is entered any automatic programing will not work,
 *  this requires you to use the reset button to program. 
 *  Exiting from Hibernate will be through reset flow. 
 *
 *  This example shows using the digital pin 22 as a wakeup 
 *  source for the Teensy3.
 *
 *  Tested and compiled under Arduino 1.0.5 and 
 *  Teensyduino 1.18 
 *********************************************************/
#include <LowPower_Teensy3.h>

TEENSY3_LP LP = TEENSY3_LP();

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(0);
  blink();
  while(!Serial);
  delay(1000);
  Serial.print("\nHibernate Simple\n\n");
  
  /*****************************************************
   * Print the reset source and current power mode
   *****************************************************/
  LP.PrintSRS(&Serial);
  delay(50);
  
  /*****************************************************
   * Set digital pin 22 as wakeup source and then enter 
   * Hibernate sleep mode. Exiting Hibernate is through 
   * a reset so code will not execute after Hibernate.
   * Reopen the serial monitor after waking Teensy. 
   *****************************************************/
  pinMode(22, INPUT_PULLUP);
  LP.Hibernate(GPIO_WAKE, PIN_22);
}

void loop() {
  blink();// Notice, this never gets executed.
}

void blink() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
}



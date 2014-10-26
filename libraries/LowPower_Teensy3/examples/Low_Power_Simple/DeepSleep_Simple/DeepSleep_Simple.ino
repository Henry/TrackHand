/**********************************************************
 *  Puts the processor into Low Leakage Stop (LLS) mode
 *  aka... DeepSleep. All I/O is held in a pre DeepSleep
 *  state and USB is disabled. Once DeepSleep mode is 
 *  entered any automatic programing will not work, this 
 *  requires you to use the reset button to program.
 *
 *  This example shows using the Low-Power Timer, digital 
 *  pin 22, RTC and TSI (Touch Sense) as a wakeup source.
 *  Must have the RTC crystal installed for RTC to work.
 *  If not commit out RTC DeepSleep portion. For TSI I 
 *  used a wire connected pin 0. 
 *
 *  Tested and compiled under Arduino 1.0.5 and 
 *  Teensyduino 1.18
 *********************************************************/
#include <LowPower_Teensy3.h>

uint16_t threshold;
long blinkRate = 50000;

TEENSY3_LP LP = TEENSY3_LP();
IntervalTimer timer0;

void intervalTimerCallback() {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  
  /*****************************************************
   * Initialize TSI wakeup threshold by reading the value
   * of touchRead + 256. While sleeping the TSI value 
   * will have to be greater than this value to wake the
   * mcu. Make sure you are not touching the electrode
   * while getting wakeup threshold.
   *****************************************************/
  threshold = touchRead(0) + 256;

  /*****************************************************
   * Setup IntervalTimer to toggle led every 50ms. Notice 
   * that the led will not toggle when in DeepSleep and 
   * will held at whatever state when entering DeepSleep.
   *****************************************************/
  timer0.begin(intervalTimerCallback, blinkRate);
}

void loop() {
  /*****************************************************
   * Awake for 5 secs then go into DeepSleep for 5 secs 
   * using the Low-Power timer as a wakeup source. 
   *****************************************************/
  delay(5000);
  LP.DeepSleep(LPTMR_WAKE, 5000);
  
  /*****************************************************
   * Awake for 5 secs then go into DeepSleep using digital 
   * pin 22 as a wakeup source. Drive pin 22 to ground to 
   * trigger wakeup.
   *****************************************************/
  delay(5000);
  pinMode(22, INPUT_PULLUP);
  LP.DeepSleep(GPIO_WAKE, PIN_22);

  /*****************************************************
   * Awake for 5 secs then go into DeepSleep for 5 secs 
   * using RTC alarm as a wakeup source. 
   *****************************************************/
  delay(5000);
  LP.DeepSleep(RTCA_WAKE, 5);

  /*****************************************************
   * Awake for 5 secs then go into DeepSleep using TSI 
   * as a wakeup source. 
   *****************************************************/
  delay(5000);
  LP.DeepSleep(TSI_WAKE, 0, threshold);
}

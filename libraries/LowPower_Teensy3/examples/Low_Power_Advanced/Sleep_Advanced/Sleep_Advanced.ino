/**********************************************************
 *  ** NOTICE!!! You must make IntervalTimer class "Private"
 *  ** members "Protected" members. Then uncomment
 *  ** IntervalTimer_LP class in LowPower_Teensy3.h file.
 *
 *  Puts the processor into Wait (VLPW) mode aka... Sleep.
 *  All system interrupts will exit this sleep mode. Since
 *  Sleep clocks are running at 2MHz all peripherals are
 *  clocked at that frequency also, you have take that into
 *  account when setting up your interrupt source especially
 *  if using the different timers.
 *
 *  This example puts the CPU at 2MHz then uses
 *  LowPower_Teensy3 "IntervalTimer_LP" port to wake the
 *  processor from Sleep mode. Serial1 is also reconfigured
 *  to work at 2MHz using the "HardwareSerial_LP" class.
 *
 *  Tested and compiled under Arduino 1.0.5 and
 *  Teensyduino 1.18
 *********************************************************/
#include <LowPower_Teensy3.h>

uint16_t threshold;
long blinkRate = 5000000;

TEENSY3_LP LP = TEENSY3_LP();
// allows use of the serial port at speeds below 24MHz
HardwareSerial_LP Uart_lp = HardwareSerial_LP();
// allows use of the IntervalTimer at speeds below 24MHz
IntervalTimer_LP timer_lp;

void intervalTimerCallback() {
    digitalWrite(LED_BUILTIN, HIGH);
    LP.delay(100, TWO_MHZ);
    digitalWrite(LED_BUILTIN, LOW);
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    /*****************************************************
     * Start off at 2MHz so the IntervalTimer_LP class 
     * BUS clock configuration is initialized at the same
     * BUS frequency as Sleep BUS clock.
     *****************************************************/
    LP.CPU(TWO_MHZ);
    /*****************************************************
     * Start serial port using 2MHz CPU speed.
     *****************************************************/
    Uart_lp.begin(115200);
    Uart_lp.print("Sleep Advanced \n");
    Uart_lp.flush();
    /*****************************************************
     * Setup IntervalTimer_LP as a wake source for Sleep mode.
     *****************************************************/
    timer_lp.begin(intervalTimerCallback, blinkRate);
}

void loop() {
    /*****************************************************
     * Sleep for 5 secs, thanks to the IntervalTimer_LP class
     *****************************************************/
    LP.Sleep();
    Uart_lp.println("Wakey Wakey");
    Uart_lp.flush();
}
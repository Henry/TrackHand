/**********************************************************
 *  Puts the processor into Low Leakage Stop mode (LLS),
 *  aka... DeepSleep. All I/O is held in a pre DeepSleep
 *  state and the USB is disabled.
 *
 *  This example shows the use of the struct to setup 
 *  multiple wake sources with multiple configurations. 
 *  Debugging messages are printed through Hardware Serial1.
 *
 *  Tested with Arduino 1.0.5 and Teensyduino 1.18
 *********************************************************/
#include <LowPower_Teensy3.h>
#include <Time.h> 

HardwareSerial Uart = HardwareSerial();

TEENSY3_LP LP = TEENSY3_LP();

sleep_block_t* LP_config;// sleep configuration


// User callback handler
void callbackhandler() {
  setSyncProvider(getTeensy3Time);
}

void setup() {
  Serial.begin(0);
  pinMode(LED_BUILTIN, OUTPUT);
  Uart.begin(115200);
  setSyncProvider(getTeensy3Time);
  Uart.println("DeepSleep Advanced Example");
  if (timeStatus()!= timeSet) Uart.println("Unable to sync with the RTC"); 
  else Uart.println("RTC has set the system time");
  Uart.flush();
}

void loop() {
  sleep_config_1();// function to setup first configuration
  sleep_config_2();// function to setup second configuration
}

/*****************************************************
 * The first configuration sets up RTC & TSI- Touch 
 * Sense as the wake up sources. 
 *****************************************************/
void sleep_config_1() {
  // config1 struct in memory
  LP_config = (sleep_block_t*) calloc(1,sizeof(sleep_block_t));
  // OR together different wake sources
  LP_config->modules = (RTCA_WAKE | TSI_WAKE);
  // RTC alarm wakeup in 20 seconds
  LP_config->rtc_alarm = 20;
  // enable pin 15 as TSI wakeup, only one pin can be used
  LP_config->tsi_pin = 15;
  // configure TSI wakeup threshold
  LP_config->tsi_threshold = touchRead(LP_config->tsi_pin) + 256;
  // user callback function
  LP_config->callback = callbackhandler;
  // sleep
  LP.DeepSleep(LP_config);
 
  blink();
  
  digitalClockDisplay();
  
  Uart.print(" - Config1: ");
  
  printWakeSource();
  
  free(LP_config);
  LP_config = NULL;
}

/*****************************************************
 * The second configuration sets up Low-Power Timer, 
 * TSI and Digital Pins 6,7 as wakeup sources. 
 *****************************************************/
void sleep_config_2() {
  // configure pin 6
  pinMode(6, INPUT_PULLUP);
  // configure pin 7
  pinMode(7, INPUT_PULLUP);
  // config2 struct in memory
  LP_config = (sleep_block_t*) calloc(1,sizeof(sleep_block_t));
  // OR together different wake sources
  LP_config->modules = (GPIO_WAKE | LPTMR_WAKE | TSI_WAKE);
  // enable pin 6,7 as wake up source
  LP_config->gpio_pin = (PIN_6 | PIN_7);
  // Low-Power Timer wakeup in 20 secs
  LP_config->lptmr_timeout =  20000;
  // enable pin 16 as TSI wakeup, only one pin can be used
  LP_config->tsi_pin = 16;
  // configure TSI wakeup threshold
  LP_config->tsi_threshold = touchRead(LP_config->tsi_pin) + 256;
  // user callback function
  LP_config->callback = callbackhandler;
  // sleep
  LP.DeepSleep(LP_config);
  
  blink();
  
  digitalClockDisplay();
  
  Uart.print(" - Config2: ");
  
  printWakeSource();
  
  free(LP_config);
  LP_config = NULL;
}

void blink() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);
}

void digitalClockDisplay() {
  Uart.print(month());
  Uart.print("/");
  Uart.print(day());
  Uart.print("/");
  Uart.print(year());
  Uart.print(" ");
  Uart.print(hourFormat12());
  printDigits(minute());
  printDigits(second());
  Uart.flush();
}

time_t getTeensy3Time() {
  return Teensy3Clock.get();
}

void printDigits(int digits) {
  Uart.print(":");
  if(digits < 10)
    Uart.print('0');
  Uart.print(digits);
}

void printWakeSource() {
  if (LP_config->wake_source == PIN_2) {
    Uart.println("GPIO WAKE pin 2");
  }
  if (LP_config->wake_source == PIN_4) {
    Uart.println("GPIO WAKE pin 4");
  }
  if (LP_config->wake_source == PIN_6) {
    Uart.println("GPIO WAKE pin 6");
  }
  if (LP_config->wake_source == PIN_7) {
    Uart.println("GPIO WAKE pin 7");
  }
  if (LP_config->wake_source == PIN_9) {
    Uart.println("GPIO WAKE pin 9");
  }
  if (LP_config->wake_source == PIN_10) {
    Uart.println("GPIO WAKE pin 10");
  }
  if (LP_config->wake_source == PIN_11) {
    Uart.println("GPIO WAKE pin 11");
  }
  if (LP_config->wake_source == PIN_13) {
    Uart.println("GPIO WAKE pin 13");
  }
  if (LP_config->wake_source == PIN_16) {
    Uart.println("GPIO WAKE pin 16");
  }
  if (LP_config->wake_source == PIN_21) {
    Uart.println("GPIO WAKE pin 21");
  }
  if (LP_config->wake_source == PIN_22) {
    Uart.println("GPIO WAKE pin 22");
  }
  if (LP_config->wake_source == PIN_26) {
    Uart.println("GPIO WAKE pin 26");
  }
  if (LP_config->wake_source == PIN_30) {
    Uart.println("GPIO WAKE pin 30");
  }
  if (LP_config->wake_source == PIN_33) {
    Uart.println("GPIO WAKE pin 33");
  }
  if (LP_config->wake_source == LPTMR_WAKE) {
    Uart.println("LPTMR WAKE");
  }
  if (LP_config->wake_source == TSI_WAKE) {
    Uart.print("TSI WAKE pin ");
    Uart.println(LP_config->tsi_pin);
  }
  if (LP_config->wake_source == RTCA_WAKE) {
    Uart.println("RTCA WAKE");
  }
  Uart.flush();
}










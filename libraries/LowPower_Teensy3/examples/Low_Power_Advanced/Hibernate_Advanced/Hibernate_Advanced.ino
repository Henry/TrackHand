/**********************************************************  
 *  Puts the processor into Very Low Leakage Stop (VLLS3) 
 *  mode aka... Hibernate. All I/O is held in a pre 
 *  Hibernate state and USB is disabled. Exiting from 
 *  Hibernate will be through reset flow. 
 *
 *  This example shows the use of the "config struct" to  
 *  setup multiple wake sources with multiple configurations.
 *  The EEPROM stores what sleep configuration gets called
 *  next and uses Serial1 to print debug messages.
 *
 *  Tested and compiled under Arduino 1.0.5 and 
 *  Teensyduino 1.18
 *********************************************************/
#include <LowPower_Teensy3.h>
#include <Time.h> 
#include <EEPROM.h>

HardwareSerial Uart = HardwareSerial();

TEENSY3_LP LP = TEENSY3_LP();
sleep_block_t* LP_config;// first Hibernate configuration

void callbackhandler() {
  EEPROM.write(0, EEPROM.read(0)+1);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(0);
  Uart.begin(115200);
  setSyncProvider(getTeensy3Time);
  blink();
  Uart.print("Hibernate Advanced \n");
  Uart.flush();
  /*****************************************************
   * Use the eeprom to cycle through the three different
   * Hibernate configurations. 
   *****************************************************/
  if (EEPROM.read(0) > 2) EEPROM.write(0, 0);

  if (EEPROM.read(0) == 0) {
    digitalClockDisplay();
    Uart.print(" - Config1 \n");
    Uart.flush();
    // function to use first configuration
    sleep_config_1();
  }

  if (EEPROM.read(0) == 1) {
    digitalClockDisplay();
    Uart.print(" - Config2 \n");
    Uart.flush();
    // function to use second configuration
    sleep_config_2();
  }

  if (EEPROM.read(0) == 2) {
    digitalClockDisplay();
    Uart.print(" - Config3 \n");
    Uart.flush();
    // function to use third configuration
    sleep_config_3();
  }
}

void loop() {
  blink();// never should get here
}

/*****************************************************
 * The first configuration sets up Low-Power Timer and
 * Digital Pin 2 as the wake up sources. 
 *****************************************************/
void sleep_config_1() {
  // configure pin 2
  pinMode(2, INPUT_PULLUP);
  // config1 struct in memory
  LP_config = (sleep_block_t*) calloc(1,sizeof(sleep_block_t));
  // OR together different wake sources
  LP_config->modules = (LPTMR_WAKE | GPIO_WAKE);
  // Low-Power Timer wakeup in 10 secs
  //config1->rtc_alarm = 5;
  LP_config->lptmr_timeout = 5000;
  // set pin 7 or pin 9 as wake up
  LP_config->gpio_pin = (PIN_2);
  // user callback function
  LP_config->callback = callbackhandler;
  // go to bed
  LP.Hibernate(LP_config);
}

/*****************************************************
 * The second configuration sets up Digital Pin 7, 
 * Pin 9 and RTC as the wake up sources. Note GPIO pins
 * are not configured as pullups.
 *****************************************************/
void sleep_config_2() {
  // configure pin 7, this is also RX3 so serial could be used instead
  pinMode(7, INPUT);
  // configure pin 9, this is also RX2 so serial could be used instead
  pinMode(9, INPUT);
  // config2 struct in memory
  LP_config = (sleep_block_t*) calloc(1,sizeof(sleep_block_t));
  // OR together different wake sources
  LP_config->modules = (GPIO_WAKE | RTCA_WAKE);
  // set pin 7 or pin 9 as wake up
  LP_config->gpio_pin = (PIN_7 | PIN_9);
  // RTC alarm wakeup in 20 seconds
  LP_config->rtc_alarm = 5;
  // user callback function
  LP_config->callback = callbackhandler;
  // go to bed
  LP.Hibernate(LP_config);
}

/*****************************************************
 * The third configuration sets up RTC, TSI and Digital 
 * Pin 16 as wakeup sources. 
 *****************************************************/
void sleep_config_3() {
  // configure pin 16
  pinMode(16, INPUT_PULLUP);
  // config3 struct in memory
  LP_config = (sleep_block_t*) calloc(1,sizeof(sleep_block_t));
  // OR together different wake sources
  LP_config->modules = (RTCA_WAKE | TSI_WAKE | GPIO_WAKE);
  // RTC alarm wakeup in 20 seconds
  LP_config->rtc_alarm = 5;
  // setup pin 0 as TSI pin
  LP_config->tsi_pin = 0;
  // configure wakeup threshold
  LP_config->tsi_threshold = touchRead(LP_config->tsi_pin) + 256;
  // set pin 16 as wake up
  LP_config->gpio_pin = (PIN_16);
  // user callback function
  LP_config->callback = callbackhandler;
  // go to bed
  LP.Hibernate(LP_config);
}

void blink() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(20);
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
}

void printDigits(int digits) {
  Uart.print(":");
  if(digits < 10)
    Uart.print('0');
  Uart.print(digits);
}

time_t getTeensy3Time() {
  return Teensy3Clock.get();
}























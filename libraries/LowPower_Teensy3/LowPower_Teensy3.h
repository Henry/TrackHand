/*
 ||
 || @file 		LowPower_Teensy3.h
 || @version 	4
 || @author 	Colin Duffy
 || @contact 	cmduffy@engr.psu.edu
 ||
 || @description
 || # Low Power Library for Teensy 3.0/3.1.
 ||
 || @license
 || | Copyright (c) 2014 Colin Duffy
 || | This library is free software; you can redistribute it and/or
 || | modify it under the terms of the GNU Lesser General Public
 || | License as published by the Free Software Foundation; version
 || | 2.1 of the License.
 || |
 || | This library is distributed in the hope that it will be useful,
 || | but WITHOUT ANY WARRANTY; without even the implied warranty of
 || | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 || | Lesser General Public License for more details.
 || |
 || | You should have received a copy of the GNU Lesser General Public
 || | License along with this library; if not, write to the Free Software
 || | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 || #
 ||
 */

#ifndef LowPower_Teensy3
#define LowPower_Teensy3
#include "Arduino.h"
#include "mk20dx128_ext.h"
#include "utility/t3core2lp.h"
#include "utility/module.h"
#include "utility/llwu.h"

/* Define LLS & VLLS Wakeup Pin */
#define PIN_2           LLWU_PIN_2
#define PIN_4           LLWU_PIN_4
#define PIN_6           LLWU_PIN_6
#define PIN_7           LLWU_PIN_7
#define PIN_9           LLWU_PIN_9
#define PIN_10          LLWU_PIN_10
#define PIN_11          LLWU_PIN_11
#define PIN_13          LLWU_PIN_13
#define PIN_16          LLWU_PIN_16
#define PIN_21          LLWU_PIN_21
#define PIN_22          LLWU_PIN_22
#define PIN_26          LLWU_PIN_26
#define PIN_30          LLWU_PIN_30
#define PIN_33          LLWU_PIN_33

/* Define Pin Interrupt Type */
#define PIN_DISABLED    LLWU_PIN_DIS
#define PIN_RISING      LLWU_PIN_RIGIN
#define PIN_FALLING     LLWU_PIN_FALLING
#define PIN_ANY         LLWU_PIN_ANY

/* Module wakeup sources */
/* GPIO_WAKE is a made up flag (not used by LLWU), used here
   to make the software interface more uniform. */
#define GPIO_WAKE       0x80000000UL
#define LPTMR_WAKE      LLWU_LPTMR_MOD
#define RTCA_WAKE       LLWU_RTCA_MOD
#define RTCS_WAKE       LLWU_RTCS_MOD
#define CMP0_WAKE       LLWU_CMP0_MOD
#define CMP1_WAKE       LLWU_CMP1_MOD
#define TSI_WAKE        LLWU_TSI_MOD

/* Hardware Serial Baud VLPR Mode */
#define TWO_MHZ         2000000
#define FOUR_MHZ        4000000
#define EIGHT_MHZ       8000000
#define SIXTEEN_MHZ     16000000

#define LP_BAUD2DIV(baud, cpu) (((cpu * 2) + ((baud) >> 1)) / (baud))
#define LP_BAUD2DIV3(baud, bus) (((bus * 2) + ((baud) >> 1)) / (baud))

class TEENSY3_LP;

typedef struct sleep_block_struct {
    /* Structure wake source */
    volatile uint32_t wake_source;
    /* Structure RTC Config */
    unsigned long rtc_alarm;
    /* Module Wake Type */
    uint32_t modules;
    /* Structure GPIO Config */
    uint16_t gpio_pin;
    uint16_t gpio_mode;
    /* Structure LPTMR Config */
    uint16_t lptmr_timeout;
    /* Structure TSI Config */
    uint16_t tsi_threshold;
    uint8_t tsi_pin;
    /* pointer to callback function */
    void (*callback)();
    sleep_block_struct() : wake_source(0), rtc_alarm(0), modules(0),
                           gpio_pin(0), gpio_mode(0), lptmr_timeout(0),
                           tsi_threshold(0), tsi_pin(0), callback(NULL) {};
} sleep_block_t;

class TEENSY3_LP {
public:
    typedef void (*ISR)();
    typedef enum {
            sleep_DeepSleep,
            sleep_Hibernate
    } sleep_type_t;

private:

    /* Handle a specific sleep command */
    inline bool sleepHandle(sleep_type_t type, sleep_block_t *configuration) __attribute__((always_inline, unused));
    /* TSI Initialize  */
    void tsiIntialize(void);
    /* private class access to wakeup ISR  */
    friend void wakeup_isr(void);
    /* handle call to user callback  */
    static ISR CALLBACK;
    /* default callback ISR  */
    static void defaultCallback() { yield(); };

    static volatile uint32_t stopflag;// hold module wake up sources for wakeup isr
    static DMAMEM volatile uint8_t lowLeakageSource;// hold lowleakage mode for wakeup isr

    friend class HardwareSerial_LP;
    friend class HardwareSerial2_LP;
    friend class HardwareSerial3_LP;
    friend class IntervalTimer_LP;

    static volatile uint32_t _cpu;
    static volatile uint32_t _bus;
    static volatile uint32_t _mem;

public:
    // Constructor
    TEENSY3_LP(void);

    ~TEENSY3_LP(void) { NVIC_DISABLE_IRQ(IRQ_LLWU); }// disable wakeup isr
    //---------------------------------------------------------------------------------------
    /* Sleep Functions */
    static volatile uint32_t wakeSource;// hold llwu wake up source
    //----------------------------------------CPU--------------------------------------------
    int CPU(uint32_t freq);
    //----------------------------------------idle--------------------------------------------
    void Idle();
    //---------------------------------------Sleep-------------------------------------------
    void Sleep();
    //--------------------------------------DeepSleep----------------------------------------
    uint32_t DeepSleep(uint32_t wakeType, uint32_t time_pin, uint16_t threshold = 0, ISR myCallback = defaultCallback);
    uint32_t DeepSleep(uint32_t wakeType, uint32_t time_pin, ISR myCallback) { DeepSleep(wakeType, time_pin, 0, myCallback);  return wakeSource; }
    void DeepSleep(sleep_block_t* configuration);
    //--------------------------------------Hibernate----------------------------------------
    void Hibernate(uint32_t wakeType, uint32_t time_pin, uint16_t threshold = 0, ISR myCallback = defaultCallback);
    void Hibernate(uint32_t wakeType, uint32_t time_pin, ISR myCallback) { Hibernate(wakeType, time_pin, 0, myCallback); }
    void Hibernate(sleep_block_t* configuration);
    //---------------------------------------PrintSRS----------------------------------------
    void PrintSRS(Stream *port);
    //-----------------------------------------Core------------------------------------------
    //const volatile uint32_t *currentCPU = (&_cpu);

    static inline uint32_t micros(uint32_t cpu) __attribute__((always_inline, unused)) { return micros_lp(cpu); }
    static inline void delay(uint32_t msec, uint32_t cpu) { delay_lp(msec, cpu); }

    static inline void delayMicroseconds(uint32_t usec) __attribute__((always_inline, unused))
    { delayMicroseconds(usec, _cpu); }
    static inline void delayMicroseconds(uint32_t usec, const uint32_t cpu) __attribute__((always_inline, unused)) {
        if  (cpu == 96000000) {
            delayMicroseconds96(usec);
        }
        else if (cpu == 48000000) {
            delayMicroseconds48(usec);
        }
        else if (cpu == 24000000) {
            delayMicroseconds24(usec);
        }
        else if (cpu == 16000000) {
            delayMicroseconds16(usec);
        }
        else if (cpu == 8000000) {
            delayMicroseconds8(usec);
        }
        else if (cpu == 4000000) {
            delayMicroseconds4(usec);
        }
        else if (cpu == 2000000) {
            delayMicroseconds2(usec);
        }
    }

};

/**** !!!!!Must make interval timer private members protected for this to work!!!! *****/
/*class IntervalTimer_LP : public IntervalTimer {
private:
public:
    bool begin(ISR newISR, unsigned int newPeriod) {
        if (newPeriod == 0 || newPeriod > MAX_PERIOD) return false;
        uint32_t newValue = (TEENSY3_LP::_cpu / 1000000) * newPeriod - 1;
        return beginCycles(newISR, newValue);
    }
};*/


class HardwareSerial_LP : public HardwareSerial {
private:
public:
    void begin(uint32_t baud) { serial_begin(LP_BAUD2DIV(baud, TEENSY3_LP::_cpu)); }
    void begin(uint32_t baud, uint32_t format) {
        serial_begin(LP_BAUD2DIV(baud, TEENSY3_LP::_cpu));
        serial_format(format);
    }
    void end(void) {
        serial_end();
        uart1Disable();
    }
};

class HardwareSerial2_LP : public HardwareSerial2 {
private:
public:
    void begin(uint32_t baud) { serial2_begin(LP_BAUD2DIV(baud, TEENSY3_LP::_cpu)); }
    void begin(uint32_t baud, uint32_t format) {
        serial2_begin(LP_BAUD2DIV(baud, TEENSY3_LP::_cpu));
        serial2_format(format);
    }
    void end(void) {
        serial2_end();
        uart2Disable();
    }
};

class HardwareSerial3_LP : public HardwareSerial3 {
private:
public:
    void begin(uint32_t baud) { serial3_begin(LP_BAUD2DIV3(baud, TEENSY3_LP::_bus)); }
    void begin(uint32_t baud, uint32_t format) {
        serial3_begin(LP_BAUD2DIV3(baud, TEENSY3_LP::_bus));
        serial3_format(format);
    }
    void end(void) {
        serial3_end();
        uart3Disable();
    }
};

#endif

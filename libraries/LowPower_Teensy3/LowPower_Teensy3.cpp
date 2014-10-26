/*******************************************************************************
 *  LowPower_Teensy3.cpp
 *  Teensy3
 *
 * Purpose:    Provides routines for configuring the Teensy3 for low power.
 *
 * NOTE:       None
 *******************************************************************************/

#include "LowPower_Teensy3.h"
#include "utility/lptmr.h"
#include "utility/llwu.h"
#include "utility/smc.h"
#include "utility/mcg.h"
#include "utility/rtc.h"
#include "utility/tsi.h"
#include "utility/cmp.h"
#include "utility/module.h"
#include "util/atomic.h"
#include "utility/bitband.h"

/* Define Low Leakage Source */
#define LLS         0x01
#define VLLS        0x02

/* CPU Freq in VLPR mode */
#define BLPI_CPU    2000000
#define BLPI_BUS    2000000
#define BLPI_MEM    1000000

#define BLPE_CPU    4000000
#define BLPE_BUS    4000000
#define BLPE_MEM    1000000

volatile DMAMEM uint8_t  TEENSY3_LP::lowLeakageSource;// hold lowleakage mode for wakeup isr
volatile DMAMEM uint32_t TEENSY3_LP::wakeSource;// hold llwu wake up source for wakeup isr
volatile uint32_t TEENSY3_LP::stopflag;// hold module wake up sources for wakeup isr
volatile uint32_t TEENSY3_LP::_cpu;
volatile uint32_t TEENSY3_LP::_bus;
volatile uint32_t TEENSY3_LP::_mem;

TEENSY3_LP::TEENSY3_LP() {
    // Enable all wakeup types - SMC_PMPROT: AVLP=1,ALLS=1,AVLLS=1
    SMC_PMPROT = SMC_PMPROT_ALLS_MASK | SMC_PMPROT_AVLLS_MASK | SMC_PMPROT_AVLP_MASK;
    /*
     This is an advanced feature, usb standby during low power modes
     is disabled by default. The regulator can only supply a limited
     amount of current if you choose use this feature, use with caution!
     */
    // allows write to SIM_SOPT1CFG USBSSTBY register
    SIM_SOPT1CFG |= SIM_SOPT1CFG_USSWE_MASK;
    // clear usb regulator standby bit in LLS & VLLS modes, no standby
    SIM_SOPT1 &= ~SIM_SOPT1_USBSSTBY_MASK;
    // allows write to SIM_SOPT1CFG USBVSTBY register
    SIM_SOPT1CFG |= SIM_SOPT1CFG_UVSWE_MASK;
    // clear usb regulator standby bit in VLPR
    SIM_SOPT1 &= ~SIM_SOPT1_USBVSTBY_MASK;
    // clear llwu flags
    //wakeSource = llwu_clear_flags();
    // initialize Low Power Timer
    lptmr_init();
    // clear any wakeup flags
    llwu_clear_flags();
    // initialize current cpu, bus and mem speeds variable
    _cpu = F_CPU;
    _bus = F_BUS;
    _mem = F_MEM;
}
/****************************** Func *******************************
 * Routines to enable different sleep modes on the teensy3.
 *
 **void wake_isr() -
 *      handles wake ups from LLS or VLLS. It clears LLWU flags
 *      sets MCG_C1 to PEE run mode clears flags and sets wakeup
 *      variable.
 * Arguments: NONE
 *
 **void defaultCallback() -
 *      gets called if no user callback function is defined.
 * Arguments: NONE
 *
 **void CPU(uint32_t cpu) -
 *      dynamically configures the core cpu(2, 4, 8, 16, 24, 48, 96 MHZ),
 *      bus and flash clocks and also configures the SysTick for the
 *      selected freq.
 * Arguments:  cpu -> TWO_MHZ, FOUR_MHZ, EIGHT_MHZ, SIXTEEN_MHZ, F_CPU
 *
 **void Idle() -
 *      Non-Blocking power saving, put in loops like waiting for Serial
 *      data. It will go into and out of low power mode lowering overall
 *      current by a couple of mA.
 * Arguments: NONE
 *
 **void Sleep() -
 *      Very versatile sleep option that the can be woken up by many more sources than
 *      other modes. This puts the processor into Wait Mode and disables the systick,
 *      any digital pin using attachInterrupt or any of the timers can wake it from
 *      this Sleep Mode.
 * Arguments: NONE //TODO: add option to disable modules
 *
 **void DeepSleep(uint8_t wakeType, uint32_t var, uint16_t var2, void (*callbackfunc)()) -
 *      set teensy3 to LLS sleep mode. Exited by Low-Power Timer,
 *      Digital Pin, RTC and TSI; Code execution begins after call
 *      to DeepSleep function. User callback function will proceed from wakeup_isr()
 * Arguments:  wakeType -> LPTMR_WAKE, GPIO_WAKE, RTCA_WAKE, TSI_WAKE defines
 *              var -> LPTMR_WAKE = timeout in msec
 *              var -> GPIO_WAKE = wake pin, which are defined in the header
 *              var -> RTCA_WAKE = alarm in secs
 *              var -> TSI_WAKE = wake pin, only one pin can selected
 *              var2 -> TSI wakeup threshold
 *              void (*callbackfunc)() -> pointer to user callback function from wakeup_isr()
 *
 **void DeepSleep(volatile struct configSleep* config) -
 *      uses the struct to configure the teensy wake sources. This
 *      allows the teensy to have multiple wakeup sources and
 *      multiple configurations.
 * Arguments:  configSleep* struct - defined in header
 *
 *
 **void Hibernate(uint8_t wakeType, uint32_t var, uint16_t var2, void (*callbackfunc)()) -
 *      set teensy3 to VLLS3 sleep mode. Exited by Low-Power Timer,
 *      Digital Pin, RTC and TSI; Code execution begins after call
 *      to DeepSleep function. Code execution begins through reset flow.
 * Arguments:  wakeType -> LPTMR_WAKE, GPIO_WAKE, RTCA_WAKE, TSI_WAKE defines
 *              var -> LPTMR_WAKE = timeout in msec
 *              var -> GPIO_WAKE = wake pin, which are defined in the header
 *              var -> RTCA_WAKE = alarm in secs
 *              var -> TSI_WAKE = wake pin, only one pin can selected
 *              var2 -> TSI threshold value
 *              void (*callbackfunc)() -> pointer to user callback function from wakeup_isr()
 *
 **void Hibernate(volatile struct configSleep* config) -
 *      uses the struct to configure the teensy wake sources. This
 *      allows the teensy to have multiple wakeup sources and
 *      multiple configurations.
 * Arguments:  configSleep* struct - defined in header
 *
 *
 **void PrintSRS() -
 *      prints the reset type and current power mode.
 ***********************************************************************/
/******************************** ISR: *********************************/
TEENSY3_LP::ISR TEENSY3_LP::CALLBACK;

void wakeup_isr(void) {

    NVIC_DISABLE_IRQ(IRQ_LLWU); // disable wakeup isr

    if (TEENSY3_LP::lowLeakageSource == LLS) {

        uint32_t llwuFlag;

        __disable_irq();
        llwuFlag = llwu_clear_flags();// clear llwu flags after wakeup a/ store wakeup source
        __enable_irq();

        pbe_pee();// mcu is in PBE from LLS wakeup, transition back to PEE (if exiting from normal RUN mode)

        // clear wakeup module and stop them
        if ((TEENSY3_LP::stopflag & LPTMR_WAKE)) lptmr_stop();
        else if ((TEENSY3_LP::stopflag & RTCA_WAKE)) rtc_stop();
        else if ((TEENSY3_LP::stopflag & TSI_WAKE)) tsi_stop();
        else if ((TEENSY3_LP::stopflag & CMP0_WAKE)) cmp_stop();

        TEENSY3_LP *p;

        p->wakeSource = llwuFlag;

    }

    TEENSY3_LP::CALLBACK();
}
/******************************* PUBLIC: *******************************/
int TEENSY3_LP::CPU(uint32_t cpu) {
    if (_cpu == cpu) return 0;
    /********************************************/
    /* First check if we are in blpi or blpe, if */
    /* so transition to pee at F_CPU, F_BUS, F_MEM. */
    /********************************************/

    if (mcg_mode() == BLPI) {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            // exit low power Run
            exit_vlpr();
            blpi_pee();
        }
        usbEnable();
    }
    else if (mcg_mode() == BLPE) {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            // exit low power Run
            if (SMC_PMSTAT == 0x04) exit_vlpr();
            blpe_pee();
        }
        usbEnable();
    }

    if (cpu >= 24000000) {
        // config divisors: F_CPU core, F_BUS bus, F_MEM flash
        _cpu = F_CPU;
        _bus = F_BUS;
        _mem = F_MEM;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            systick_millis_count = 0;
            SYST_CVR = 0;
        };
        return F_CPU;
    }
    else if (cpu == TWO_MHZ) {
        _cpu = BLPI_CPU;
        _bus = BLPI_BUS;
        _mem = BLPI_MEM;
        usbDisable();
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            // transition from PEE to BLPI
            pee_blpi();
            // config divisors: 2 MHz core, 2 MHz bus, 1 MHz flash
            mcg_div(0x00, 0x00, 0x01, 1999);
            // now safe to enter vlpr
            enter_vlpr(0);
            systick_millis_count = 0;
            SYST_CVR = 0;
        }
        return TWO_MHZ;
    }
    else if (cpu == FOUR_MHZ) {
        _cpu = BLPE_CPU;
        _bus = BLPE_BUS;
        _mem = BLPE_MEM;
        usbDisable();
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            // transition from PEE to BLPE
            pee_blpe();
            // config divisors: 4 MHz core, 4 MHz bus, 1 MHz flash
            mcg_div(0x03, 0x03, 0x0F, 3999);
            // now safe to enter vlpr
            enter_vlpr(0);
            systick_millis_count = 0;
            SYST_CVR = 0;
        }
        return FOUR_MHZ;
    }
    else if (cpu == EIGHT_MHZ) {
        _cpu = EIGHT_MHZ;
        _bus = EIGHT_MHZ;
        _mem = EIGHT_MHZ;
        usbDisable();
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            // transition from PEE to BLPE
            pee_blpe();
            // config divisors: 8 MHz core, 8 MHz bus, 8 MHz flash
            mcg_div(0x01, 0x01, 0x01, 7999);
            systick_millis_count = 0;
            SYST_CVR = 0;
        }
        return EIGHT_MHZ;
    }
    else if (cpu == SIXTEEN_MHZ) {
        _cpu = SIXTEEN_MHZ;
        _bus = SIXTEEN_MHZ;
        _mem = SIXTEEN_MHZ;
        usbDisable();
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            // transition from PEE to BLPE
            pee_blpe();
            // config divisors: 16 MHz core, 16 MHz bus, 16 MHz flash
            mcg_div(0x00, 0x00, 0x00, 15999);
            systick_millis_count = 0;
            SYST_CVR = 0;
            Serial.println("16_MHZ");
        }
        return SIXTEEN_MHZ;
    }
    else {
        _cpu = F_CPU;
        _bus = F_BUS;
        _mem = F_MEM;
        return -1;
    }
}
//----------------------------------------------------------------------------------------------------------
void TEENSY3_LP::Idle() {
    enter_wait();
}
//----------------------------------------------------------------------------------------------------------
void TEENSY3_LP::Sleep() {

    const uint32_t prev_cpu = _cpu;
    //usbDisable();
    //vrefDisable();
    //adcDisable();
    //rtcDisable();
    CPU(TWO_MHZ);
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { enter_wait(); }
    if (prev_cpu == FOUR_MHZ) {
        CPU(FOUR_MHZ);
    }
    else if (prev_cpu == EIGHT_MHZ) {
        CPU(EIGHT_MHZ);
    }
    else if (prev_cpu == SIXTEEN_MHZ) {
        CPU(SIXTEEN_MHZ);
    }
    else if (prev_cpu == F_CPU) {
        CPU(F_CPU);
        //usbEnable();
    }
    //rtcEnable();
    //adcEnable();
    //vrefEnable();

}
//----------------------------------------------------------------------------------------------------------
/* Helper to build a sleep_block_t configuration from individual parameters. */
static inline bool buildConfig(sleep_block_t *config, uint32_t wakeType,uint32_t time_pin, uint16_t threshold,TEENSY3_LP::ISR callback) __attribute__((always_inline, unused));
static inline bool buildConfig(sleep_block_t *config, uint32_t wakeType,uint32_t time_pin, uint16_t threshold,TEENSY3_LP::ISR callback) {
    /* Exactly one bit must be set in wakeType */
    if (!(wakeType & 0x80770000) || ((wakeType & (wakeType - 1)) != 0))
        return false;

    /* Set up config */
    //memset(config, 0, sizeof(sleep_block_t));
    config->modules = wakeType;
    config->callback = callback;

    /* Since only one wakeType is set, time_pin will only get used once;
       it's save to put it in extra places. */
    config->gpio_pin = time_pin;
    config->lptmr_timeout = time_pin;
    config->rtc_alarm = time_pin;
    config->tsi_pin = time_pin;
    config->tsi_threshold = threshold;

    return true;
}

//----------------------------------------------------------------------------------------------------------
uint32_t TEENSY3_LP::DeepSleep(uint32_t wakeType, uint32_t time_pin, uint16_t threshold, ISR callback) {

    sleep_block_t config;
    if (!buildConfig(&config, wakeType, time_pin, threshold, callback))
        return 0;

    sleepHandle(sleep_DeepSleep, &config);
    return config.wake_source;
}

void TEENSY3_LP::DeepSleep(sleep_block_t* configuration) {
    sleepHandle(sleep_DeepSleep, configuration);
}

//----------------------------------------------------------------------------------------------------------
void TEENSY3_LP::Hibernate(uint32_t wakeType, uint32_t time_pin, uint16_t threshold, ISR callback) {
    sleep_block_t config;
    if (!buildConfig(&config, wakeType, time_pin, threshold, callback))
        return;

    sleepHandle(sleep_Hibernate, &config);
    /* shouldn't return */
}

void TEENSY3_LP::Hibernate(sleep_block_t* configuration) {
    sleepHandle(sleep_Hibernate, configuration);
    /* shouldn't return */
}

//----------------------------------------------------------------------------------------------------------
bool TEENSY3_LP::sleepHandle(sleep_type_t type, sleep_block_t *configuration) {
    int gpio_pin = 0;

    CALLBACK = configuration->callback;

    stopflag = configuration->modules;

    if (configuration->modules & GPIO_WAKE) {
        gpio_pin = configuration->gpio_pin;
    }
    if (configuration->modules & LPTMR_WAKE) {
        lptmr_start(configuration->lptmr_timeout);// start timer in msec
    }
    if (configuration->modules & RTCA_WAKE) {
        rtc_alarm(configuration->rtc_alarm);// alarm in secs
    }
    if (configuration->modules & RTCS_WAKE) {

    }
    if (configuration->modules & CMP0_WAKE) {
        //pinMode(11, INPUT);
        //pinMode(12, INPUT);
        //cmp_init();// still not working right!!!
    }
    if (configuration->modules & CMP1_WAKE) {

    }
    if (configuration->modules & TSI_WAKE) {
        tsi_init(configuration->tsi_pin, configuration->tsi_threshold);
    }

    llwu_configure(gpio_pin, PIN_ANY, configuration->modules);// configure llwu

    NVIC_ENABLE_IRQ(IRQ_LLWU);// enable llwu isr

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (type == sleep_DeepSleep) {
            lowLeakageSource = LLS;
            enter_lls();// enter lls sleep mode
        }
        else if (type == sleep_Hibernate) {
            lowLeakageSource = VLLS;
            enter_vlls3();// enter vlls3 sleep mode*/
        }
    };

    configuration->wake_source = wakeSource;// who woke me up?

    return true;
}

//----------------------------------------------------------------------------------------------------------
void TEENSY3_LP::PrintSRS(Stream *port) {
    port->println("------------------------------------------");
    if (RCM_SRS1 & RCM_SRS1_SACKERR_MASK)   port->println("[RCM_SRS0]\t- Stop Mode Acknowledge Error Reset");
    if (RCM_SRS1 & RCM_SRS1_MDM_AP_MASK)    port->println("[RCM_SRS0]\t- MDM-AP Reset");
    if (RCM_SRS1 & RCM_SRS1_SW_MASK)        port->println("[RCM_SRS0]\t- Software Reset");
    if (RCM_SRS1 & RCM_SRS1_LOCKUP_MASK)    port->println("[RCM_SRS0]\t- Core Lockup Event Reset");
    if (RCM_SRS0 & RCM_SRS0_POR_MASK)       port->println("[RCM_SRS0]\t- Power-on Reset");
    if (RCM_SRS0 & RCM_SRS0_PIN_MASK)       port->println("[RCM_SRS0]\t- External Pin Reset");
    if (RCM_SRS0 & RCM_SRS0_WDOG_MASK)      port->println("[RCM_SRS0]\t- Watchdog(COP) Reset");
    if (RCM_SRS0 & RCM_SRS0_LOC_MASK)       port->println("[RCM_SRS0]\t- Loss of External Clock Reset");
    if (RCM_SRS0 & RCM_SRS0_LOL_MASK)       port->println("[RCM_SRS0]\t- Loss of Lock in PLL Reset");
    if (RCM_SRS0 & RCM_SRS0_LVD_MASK)       port->println("[RCM_SRS0]\t- Low-voltage Detect Reset");
    if (RCM_SRS0 & RCM_SRS0_WAKEUP_MASK) {
        port->println("[RCM_SRS0]\t- Wakeup bit set from low power mode ");
        if ((SMC_PMCTRL & SMC_PMCTRL_STOPM_MASK)== 3) port->println("[SMC_PMCTRL]\t- LLS exit ") ;
        if (((SMC_PMCTRL & SMC_PMCTRL_STOPM_MASK)== 4) && ((SMC_VLLSCTRL & SMC_VLLSCTRL_VLLSM_MASK)== 0)) port->println("[SMC_PMCTRL]\t- VLLS0 exit ") ;
        if (((SMC_PMCTRL & SMC_PMCTRL_STOPM_MASK)== 4) && ((SMC_VLLSCTRL & SMC_VLLSCTRL_VLLSM_MASK)== 1)) port->println("[SMC_PMCTRL]\t- VLLS1 exit ") ;
        if (((SMC_PMCTRL & SMC_PMCTRL_STOPM_MASK)== 4) && ((SMC_VLLSCTRL & SMC_VLLSCTRL_VLLSM_MASK)== 2)) port->println("[SMC_PMCTRL]\t- VLLS2 exit") ;
        if (((SMC_PMCTRL & SMC_PMCTRL_STOPM_MASK)== 4) && ((SMC_VLLSCTRL & SMC_VLLSCTRL_VLLSM_MASK)== 3)) port->println("[SMC_PMCTRL]\t- VLLS3 exit ") ;
    }
    if (SMC_PMSTAT == 0x01) port->println("[SMC_PMSTAT]\t- Current Power Mode RUN") ;
    if (SMC_PMSTAT == 0x02) port->println("[SMC_PMSTAT]\t- Current Power Mode STOP") ;
    if (SMC_PMSTAT == 0x04) port->println("[SMC_PMSTAT]\t- Current Power Mode VLPR") ;
    if (SMC_PMSTAT == 0x08) port->println("[SMC_PMSTAT]\t- Current Power Mode VLPW") ;
    if (SMC_PMSTAT == 0x10) port->println("[SMC_PMSTAT]\t- Current Power Mode VLPS") ;
    if (SMC_PMSTAT == 0x20) port->println("[SMC_PMSTAT]\t- Current Power Mode LLS") ;
    if (SMC_PMSTAT == 0x40) port->println("[SMC_PMSTAT]\t- Current Power Mode VLLS") ;

    if (wakeSource == PIN_2)        port->println("[LLWU_SOURCE]\t- Digital pin 2");
    if (wakeSource == PIN_4)        port->println("[LLWU_SOURCE]\t- Digital pin 4");
    if (wakeSource == PIN_6)        port->println("[LLWU_SOURCE]\t- Digital pin 6");
    if (wakeSource == PIN_7)        port->println("[LLWU_SOURCE]\t- Digital pin 7");
    if (wakeSource == PIN_9)        port->println("[LLWU_SOURCE]\t- Digital pin 9");
    if (wakeSource == PIN_10)       port->println("[LLWU_SOURCE]\t- Digital pin 10");
    if (wakeSource == PIN_11)       port->println("[LLWU_SOURCE]\t- Digital pin 11");
    if (wakeSource == PIN_13)       port->println("[LLWU_SOURCE]\t- Digital pin 13");
    if (wakeSource == PIN_16)       port->println("[LLWU_SOURCE]\t- Digital pin 16");
    if (wakeSource == PIN_21)       port->println("[LLWU_SOURCE]\t- Digital pin 21");
    if (wakeSource == PIN_22)       port->println("[LLWU_SOURCE]\t- Digital pin 22");
    if (wakeSource == PIN_26)       port->println("[LLWU_SOURCE]\t- Digital pin 26");
    if (wakeSource == PIN_30)       port->println("[LLWU_SOURCE]\t- Digital pin 30");
    if (wakeSource == PIN_33)       port->println("[LLWU_SOURCE]\t- Digital pin 33");
    if (wakeSource == LPTMR_WAKE)   port->println("[LLWU_SOURCE]\t- LPTMR");
    if (wakeSource == TSI_WAKE)     port->println("[LLWU_SOURCE]\t- TSI");
    if (wakeSource == RTCA_WAKE)    port->println("[LLWU_SOURCE]\t- RTCA");
    port->println("------------------------------------------");
}

#ifdef __cplusplus
extern "C" {
#endif
    void startup_early_hook() __attribute__ ((weak));
    void startup_early_hook() {
        WDOG_STCTRLH = WDOG_STCTRLH_ALLOWUPDATE;
        if (PMC_REGSC & PMC_REGSC_ACKISO) TEENSY3_LP::wakeSource = (LLWU_F1 | LLWU_F2<<8 | LLWU_F3<<16);
    }
#ifdef __cplusplus
}
#endif

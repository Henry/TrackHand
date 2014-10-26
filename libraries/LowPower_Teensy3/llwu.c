/*******************************************************************************
 *  llwu.c
 *  Teensy3
 *
 * Purpose:     Provides routines for configuring low power wake sources.
 *
 * Notes:       Modules that can currently wake the teensy are LPTMR -
 *              (Low-Power Timer), RTC and TSI.
 *******************************************************************************/

#include "llwu.h"
#include "WProgram.h"
#include "bitband.h"

#define WUME0_BIT   0
#define WUME1_BIT   1
#define WUME2_BIT   2
#define WUME4_BIT   4
#define WUME5_BIT   5
#define WUME7_BIT   7

/*******************************************************************************
 *
 *       llwu_reset_enable -
 *
 *******************************************************************************/
// TODO: Assuming this will keep mcu in sleep mode after reset?
void llwu_reset_enable(void) {
    //LLWU_RST = LLWU_RST_LLRSTE_MASK;   //no reset filter for now
}


/*******************************************************************************
 *
 *       llwu_configure -
 *
 * description: Set up the LLWU for wakeup of the MCU from LLS and VLLSx modes
 * from the selected pin or module.
 *
 * inputs:  pin_en - indicates the wakeup pin is enabled.
 *          rise_fall - 0x00 = External input disabled as wakeup
 *          0x01 - External input enabled as rising edge detection
 *          0x02 - External input enabled as falling edge detection
 *          0x03 - External input enabled as any edge detection
 *          module_en - indicates the wakeup module is enabled.
 *******************************************************************************/
void llwu_configure(uint32_t pin_en, uint8_t rise_fall, uint32_t module_en ) {
    llwu_clear_flags();
    if (pin_en != 0) {
        //------------------------------------------------
        LLWU_PE1 = 0;
        if (pin_en & LLWU_PIN_26) {
            LLWU_PE1 |= LLWU_PE1_WUPE0(rise_fall);
        }
        if (pin_en & LLWU_PIN_33) {
            LLWU_PE1 |= LLWU_PE1_WUPE3(rise_fall);
        }
        //------------------------------------------------
        LLWU_PE2 = 0;
        if( pin_en & LLWU_PIN_4) {
            LLWU_PE2 |= LLWU_PE2_WUPE4(rise_fall);
        }
        if( pin_en & LLWU_PIN_16) {  //TSI enabled
            LLWU_PE2 |= LLWU_PE2_WUPE5(rise_fall);
        }
        if( pin_en & LLWU_PIN_22) {// TSI ensbled
            LLWU_PE2 |= LLWU_PE2_WUPE6(rise_fall);
        }
        if( pin_en & LLWU_PIN_9) {
            LLWU_PE2 |= LLWU_PE2_WUPE7(rise_fall);
        }
        //------------------------------------------------
        LLWU_PE3 = 0;
        if( pin_en & LLWU_PIN_10) {
            LLWU_PE3 |= LLWU_PE3_WUPE8(rise_fall);
        }
        if( pin_en & LLWU_PIN_13) {
            LLWU_PE3 |= LLWU_PE3_WUPE9(rise_fall);
        }
        if( pin_en & LLWU_PIN_11) {
            LLWU_PE3 |= LLWU_PE3_WUPE10(rise_fall);
        }
        if( pin_en & LLWU_PIN_30) {
            LLWU_PE3 |= LLWU_PE3_WUPE11(rise_fall);
        }
        //------------------------------------------------
        LLWU_PE4 = 0;
        if( pin_en & LLWU_PIN_2) {
            LLWU_PE4 |= LLWU_PE4_WUPE12(rise_fall);
        }
        if( pin_en & LLWU_PIN_7) {
            LLWU_PE4 |= LLWU_PE4_WUPE13(rise_fall);
        }
        if( pin_en & LLWU_PIN_6) {
            LLWU_PE4 |= LLWU_PE4_WUPE14(rise_fall);
        }
        if( pin_en & LLWU_PIN_21) {
            LLWU_PE4 |= LLWU_PE4_WUPE15(rise_fall);
        }
    }
    
    if (module_en != 0) {
        //------------------------------------------------
        LLWU_ME = 0;
        if( module_en & LLWU_LPTMR_MOD) {
            BITBAND_U8(LLWU_ME, WUME0_BIT) = 1;
            //LLWU_ME |= LLWU_ME_WUME0_MASK;
        }
        if( module_en & LLWU_CMP0_MOD) {
            BITBAND_U8(LLWU_ME, WUME1_BIT) = 1;
            //LLWU_ME |= LLWU_ME_WUME1_MASK;
        }
        if( module_en & LLWU_CMP1_MOD) {
            BITBAND_U8(LLWU_ME, WUME2_BIT) = 1;
            //LLWU_ME |= LLWU_ME_WUME2_MASK;
        }
        if( module_en & LLWU_TSI_MOD) {
            BITBAND_U8(LLWU_ME, WUME4_BIT) = 1;
            //LLWU_ME |= LLWU_ME_WUME4_MASK;
        }
        if( module_en & LLWU_RTCA_MOD) {
            BITBAND_U8(LLWU_ME, WUME5_BIT) = 1;
            //LLWU_ME |= LLWU_ME_WUME5_MASK;
        }
        if( module_en & LLWU_RTCS_MOD) {
            BITBAND_U8(LLWU_ME, WUME7_BIT) = 1;
            //LLWU_ME |= LLWU_ME_WUME7_MASK;
        }
        return;
    }
}


/*******************************************************************************
 *
 *       llwu_configure_filter -
 *
 *******************************************************************************/
void llwu_configure_filter(unsigned int wu_pin_num, unsigned char filter_en, unsigned char rise_fall ) {
    //wu_pin_num is the pin number to be written to FILTSEL.  wu_pin_num is not the same as pin_en.
    uint8_t temp;
    
    temp = 0;
    //first clear filter values and clear flag by writing a 1
    LLWU_FILT1 = LLWU_FILT1_FILTF_MASK;
    LLWU_FILT2 = LLWU_FILT2_FILTF_MASK;
    
    if(filter_en == 1) {
        //clear the flag bit and set the others
        temp |= (LLWU_FILT1_FILTF_MASK) | (LLWU_FILT1_FILTE(rise_fall) | LLWU_FILT1_FILTSEL(wu_pin_num));
        LLWU_FILT1 = temp;
    }else if (filter_en == 2) {
        //clear the flag bit and set the others
        temp |= (LLWU_FILT2_FILTF_MASK) | (LLWU_FILT2_FILTE(rise_fall) | LLWU_FILT2_FILTSEL(wu_pin_num));
        LLWU_FILT2 = temp;
    }else {
        
    }
}

/*******************************************************************************
 *
 *       llwu_clear_flags -
 *
 * description: Clear wakeup flags.
 *
 *******************************************************************************/
uint32_t llwu_clear_flags(void) {
    uint32_t flag = (LLWU_F1 | LLWU_F2<<8 | LLWU_F3<<16);
    LLWU_F1 = 0xFF;
    LLWU_F2 = 0xFF;
    LLWU_F3 = 0xFF;
    BITBAND_U8(LLWU_FILT1, 7) = 1;
    BITBAND_U8(LLWU_FILT2, 7) = 1;
    //LLWU_FILT1 = 0x80;
    //LLWU_FILT2 = 0x80;
    return flag;
}


/*******************************************************************************
 *  tsi.c
 *  Teensy3
 *
 * Purpose:     Provides routines for TSI (Touch Sense).
 *
 *******************************************************************************/
#include "tsi.h"

static const uint8_t tsi_pins[] = {
    //0    1    2    3    4    5    6    7    8    9
    9,  10, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255,  13,   0,   6,   8,   7,
    255, 255,  14,  15, 255,  12, 255, 255, 255, 255,
    255, 255,  11,   5
};

boolean tsi_init(uint8_t pin, uint16_t threshold) {
    boolean error = false;
    if ( tsi_pins[pin] == 255) { error = true; return error; }
    
    TSI0_THRESHOLD = threshold;
    
    TSI0_GENCS |= TSI_GENCS_EOSF;
    
    TSI0_GENCS |= ((TSI_GENCS_NSCN(9)) |
                   (TSI_GENCS_PS(2)) |
                   (TSI_GENCS_STPE) |
                   (TSI_GENCS_LPSCNITV(7)) |
                   (TSI_GENCS_TSIIE));
    
    TSI0_SCANC |= ((TSI_SCANC_EXTCHRG(2)) |
                   (TSI_SCANC_REFCHRG(3)) |
                   (TSI_SCANC_AMPSC(1)) |
                   (TSI_SCANC_AMCLKS(0)));
    
    TSI0_PEN |= TSI_PEN_LPSP(tsi_pins[pin]);
    
    TSI0_GENCS |= TSI_GENCS_TSIEN;  //Enables TSI
    
    // Wait for any in-progress scans to complete
    while (TSI0_GENCS & TSI_GENCS_SCNIP)
            yield();
    
    TSI0_GENCS |= TSI_GENCS_OUTRGF;   //Clear all pending flags
    TSI0_GENCS |= TSI_GENCS_EOSF;
    return false;
}

void tsi_stop(void) {
    TSI0_GENCS |= TSI_GENCS_OUTRGF;
    TSI0_GENCS |= TSI_GENCS_EOSF;
    TSI0_GENCS &= ~TSI_GENCS_TSIEN;
}

/*******************************************************************************
 *  cmp.c
 *  Teensy3
 *
 * Purpose:     Provides routines for Compare.
 *
 *******************************************************************************/

#include "cmp.h"
#include "bitband.h"

#define SIM_SCGC4_CMP_BIT       0x13

void cmp_init() {
    BITBAND_U32(SIM_SCGC4, SIM_SCGC4_CMP_BIT) = 1;
    //SIM_SCGC4 |= SIM_SCGC4_CMP;
    CMP0_CR0 = 0x73;//B00000011; //cmp control reg equals 0 at start, leave
    CMP0_CR1 = 0x09;//B00001001; //also 0 reset, set to CMP output pin and comp module enable
    CMP0_SCR = 0x08;//B00001000; // read LSB for analog comparator output state
    CMP0_FPR = 0x00;
    CMP0_MUXCR = 0x06;//B00000110; // Input pins select = IN0 = plus and IN1 = neg
    CMP0_DACCR = 0xBF;//B10111111;
}

void cmp_stop(void) {
    CMP0_CR1 = 0x00;
    BITBAND_U32(SIM_SCGC4, SIM_SCGC4_CMP_BIT) = 0;
}
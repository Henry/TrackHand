/*
 *  mcg.h
 *  Teensy3
 *
 */

#ifndef __MCG_H__
#define __MCG_H__
/********************************************************************/
#include "mk20dx128.h"
#include "mk20dx128_ext.h"
//#include "Arduino.h"
//#include "HardwareSerial.h"
/********************************************************************/
// mcg defines
#define BLPI 1
#define FBI  2
#define FEI  3
#define FEE  4
#define FBE  5
#define BLPE 6
#define PBE  7
#define PEE  8
/********************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
    // function prototypes
    /* transistion into different modes */
    void pee_blpi(void);
    void blpi_pee(void);
    void pee_blpe(void);
    void blpe_pee(void);
    void pbe_pee(void);
    unsigned char mcg_mode(void);
    void mcg_div(uint8_t cpu, uint8_t bus, uint8_t mem, uint32_t syst);
#ifdef __cplusplus
}
#endif
/********************************************************************/
#endif /* __MCG_H__ */

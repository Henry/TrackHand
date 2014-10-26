/*
 *  rtc.h
 *  Teensy3
 *
 */

#ifndef __TSI_H__
#define __TSI_H__
/********************************************************************/
#include "mk20dx128.h"
#include "mk20dx128_ext.h"
#include "Arduino.h"
//#include "WProgram.h"
/********************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
    boolean tsi_init(uint8_t pin, uint16_t threshold);
    void tsi_stop(void);
#ifdef __cplusplus
}
#endif
/********************************************************************/
#endif /* __TSI_H__ */

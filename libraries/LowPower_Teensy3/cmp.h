/*
 *  cmp.h
 *  Teensy3
 *
 */

#ifndef __CMP_H__
#define __CMP_H__
/********************************************************************/
#include "mk20dx128.h"
#include "mk20dx128_ext.h"
//#include "Arduino.h"
/********************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
    // function prototypes
    void cmp_init();
    void cmp_stop(void);
#ifdef __cplusplus
}
#endif
/********************************************************************/
#endif /* __CMP_H__ */
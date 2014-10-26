/*
 *  module.h
 *  Teensy3
 *
 */
#ifndef __MODULE_H__
#define __MODULE_H__
#include "mk20dx128.h"
#include "mk20dx128_ext.h"
#include <usb_dev.h>
/********************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
// prototypes
void adcEnable();
void adcDisable();
void cmp0Enable();
void cmp0Disable();
void cmp1Enable();
void cmp1Disable();
void dmaEnable();
void dmaDisable();
void i2cEnable();
void i2cDisable();
void i2sEnable();
void i2sDisable();
void lptmEnable();
void lptmDisable();
void pdbEnable();
void pdbDisable();
void spiEnable();
void spiDisable();
void rtcEnable();
void rtcDisable();
void tsiEnable();
void tsiDisable();
void uart1Enable();
void uart1Disable();
void uart2Enable();
void uart2Disable();
void uart3Enable();
void uart3Disable();
void usbEnable();
void usbDisable();
void vrefEnable();
void vrefDisable();
#ifdef __cplusplus
}
#endif
/********************************************************************/
#endif /* defined(__MODULE_H__) */

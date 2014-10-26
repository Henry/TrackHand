/*******************************************************************************
 *  lptmr.c
 *  Teensy3
 *
 * Purpose:     Provides routines for the Low-Power Timer.
 *
 * NOTE:        This Timer will work across all power modes. It can also
 *              be used as a wakeup source from low power modes. 
 *
 *              Updated Clock configuration, now sub millisecond intervals
 *              are supported. Credit: Kyle Matthews - kyle.matthews@duke.edu
 *******************************************************************************/
#include "lptmr.h"
/*
 *  t3core2lp.h
 *  Teensy3
 *
 */
#ifndef __T3CORE2LP_H__
#define __T3CORE2LP_H__

#include "Arduino.h"
#include <arm_math.h>

#define delayMicros_lp(n)  asm volatile("L_%=_delayMicroseconds_lp:"           "\n\t"\
                                        "sev"                                  "\n\t"\
                                        "wfe"                                  "\n\t"\
                                        "subs   %0, #1"                        "\n\t"\
                                        "bne    L_%=_delayMicroseconds_lp"	    "\n" \
                                        : "+r" (n) :                                 \
                                        );

#ifdef __cplusplus
extern "C" {
#endif
    uint32_t micros_lp(uint32_t f_cpu);
    
    void delay_lp(uint32_t msec, uint32_t f_cpu);
    
    static inline uint32_t millis_lp(void) __attribute__((always_inline, unused));
    static inline uint32_t millis_lp(void) {
        volatile uint32_t ret = systick_millis_count; // single aligned 32 bit is atomic;
        return ret;
    }
    
    static inline void delayMicroseconds96(uint32_t) __attribute__((always_inline, unused));
    static inline void delayMicroseconds96(uint32_t usec) {
        uint32_t n = (usec << 4);
        if (usec == 0) return;
        delayMicros_lp(n);
    }
    
    static inline void delayMicroseconds48(uint32_t) __attribute__((always_inline, unused));
    static inline void delayMicroseconds48(uint32_t usec) {
        uint32_t n = (usec << 3);
        if (usec == 0) return;
        delayMicros_lp(n);
    }
    
    static inline void delayMicroseconds24(uint32_t) __attribute__((always_inline, unused));
    static inline void delayMicroseconds24(uint32_t usec) {
        uint32_t n = (usec << 2);
        if (usec == 0) return;
        delayMicros_lp(n);
    }
    
    static inline void delayMicroseconds16(uint32_t) __attribute__((always_inline, unused));
    static inline void delayMicroseconds16(uint32_t usec) {
        uint32_t n = (usec * 2.66666666666667f);
        if (usec == 0) return;
        delayMicros_lp(n);
    }
    
    static inline void delayMicroseconds8(uint32_t) __attribute__((always_inline, unused));
    static inline void delayMicroseconds8(uint32_t usec) {
        uint32_t n = (usec * 1.33333333333333f);
        if (usec == 0) return;
        delayMicros_lp(n);
    }
    
    static inline void delayMicroseconds4(uint32_t) __attribute__((always_inline, unused));
    static inline void delayMicroseconds4(uint32_t usec) {
        uint32_t n = (usec * 0.66666666666667f);
        if (usec == 0) return;
        delayMicros_lp(n);
    }
    
    static inline void delayMicroseconds2(uint32_t) __attribute__((always_inline, unused));
    static inline void delayMicroseconds2(uint32_t usec) {
        uint32_t n = (usec * 0.33333333333333f);
        if (usec <= 2) return;
        delayMicros_lp(n);
    }
#ifdef __cplusplus
}
#endif
#endif

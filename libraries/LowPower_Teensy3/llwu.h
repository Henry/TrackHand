/*
 *  lptm.h
 *  Teensy3
 *
 */
#ifndef __LLWU_H__
#define __LLWU_H__
/********************************************************************/
#include "mk20dx128.h"
#include "mk20dx128_ext.h"

/********************************************************************/
// module_enable defines
#define LLWU_LPTMR_MOD      0x10000
#define LLWU_RTCA_MOD       0x200000
#define LLWU_RTCS_MOD       0x400000
#define LLWU_CMP0_MOD       0x20000
#define LLWU_CMP1_MOD       0x40000
#define LLWU_TSI_MOD        0x100000
// external pin interrupt type defines
#define LLWU_PIN_DIS        0x00
#define LLWU_PIN_RISING     0x01
#define LLWU_PIN_FALLING    0x02
#define LLWU_PIN_ANY        0x03
#define LLWU_PIN_DIS        0x00
// wakeup pin
#define LLWU_PIN_2          0x1000
#define LLWU_PIN_4          0x10
#define LLWU_PIN_6          0x4000
#define LLWU_PIN_7          0x2000
#define LLWU_PIN_9          0x80
#define LLWU_PIN_10         0x100
#define LLWU_PIN_11         0x200
#define LLWU_PIN_13         0x400
#define LLWU_PIN_16         0x20
#define LLWU_PIN_21         0x8000
#define LLWU_PIN_22         0x40
#define LLWU_PIN_26         0x01
#define LLWU_PIN_30         0x800
#define LLWU_PIN_33         0x02

/********************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
    // function prototypes
    /* Low-Leakage Wakeup Unit LLWU Functions */
    void llwu_reset_enable(void);
    
    //static inline void llwu_configure(uint32_t, uint8_t, uint32_t) __attribute__((always_inline, unused));
    void llwu_configure(uint32_t pin_en, uint8_t rise_fall, uint32_t module_en);
    
    void llwu_configure_filter(unsigned int wu_pin_num, unsigned char filter_en, unsigned char rise_fall );
    
    uint32_t llwu_clear_flags(void);
#ifdef __cplusplus
}
#endif
/********************************************************************/
#endif /* defined(__LLWU_H__) */

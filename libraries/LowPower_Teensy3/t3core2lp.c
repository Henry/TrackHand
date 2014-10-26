//
//  T3core2LP.c
//  teensy_18_RC1
//
//  Created by colin on 1/29/14.
//  Copyright (c) 2014 iwater. All rights reserved.
//

#include <t3core2lp.h>
#include "Arduino.h"

uint32_t micros_lp(uint32_t f_cpu) {
	uint32_t count, current, istatus;
	__disable_irq();
	current = SYST_CVR;
	count = systick_millis_count;
	istatus = SCB_ICSR;	// bit 26 indicates if systick exception pending
	__enable_irq();
	if ((istatus & SCB_ICSR_PENDSTSET) && current > 50) count++;
	current = ((f_cpu / 1000) - 1) - current;
	return count * 1000 + current / (f_cpu / 1000000);
}

void delay_lp(uint32_t msec, uint32_t f_cpu) {
	uint32_t start = micros_lp(f_cpu);
	if (msec > 0) {
		while (1) {
			if ((micros_lp(f_cpu) - start) >= 1000) {
				msec--;
                if (msec == 0) return;
				start += 1000;
			}
            /*********************************************
             *  asm "sev" sends event to "Wait-for-Event"
             *  asm "wfe" to wake up as fast as possible
             *  but in doing so results in power savings.
             *********************************************/
            asm volatile ("sev");
            asm volatile ("wfe"); // ~500uA reduction in current with minumal effect on time
			yield();
		}
	}
}


/*
 * SoftPWM.h
 *
 *  Created on: Oct 21, 2023
 *      Author: yvc
 */

#ifndef SRC_SOFTPWM_SOFTPWM_H_
#define SRC_SOFTPWM_SOFTPWM_H_

#include "../adapter.h"

#define IPCOUNT 15

typedef struct{
	uint8_t pin_use[IPCOUNT+1];
	uint8_t start[IPCOUNT+1];
	uint8_t one_time[IPCOUNT+1];
	uint32_t period[IPCOUNT+1];
	uint32_t period_counter[IPCOUNT+1];
	uint32_t init_tim_set_compare[IPCOUNT+1];
	uint32_t tim_set_compare[IPCOUNT+1];
	uint32_t new_tim_set_compare[IPCOUNT+1];
	uint32_t new_tim_set_compare_flag[IPCOUNT+1];
	uint32_t tim_set_compare_flag[IPCOUNT+1];
} SoftPWM_t;

SoftPWM_t softpwm;

extern uint8_t softpwminit;

void SoftPWMInit(uint8_t pin, uint8_t one_time, uint32_t period_us, uint32_t init_tim_set_compare_us);
void SoftPWMStart(uint8_t pin);
void SoftPWMStop(uint8_t pin);
void SoftPWMSet(uint8_t pin, int32_t tim_set_compare_us);
void SoftPWMUpdate();

#endif /* SRC_SOFTPWM_SOFTPWM_H_ */

/*
 * SoftPWM.c
 *
 *  Created on: Oct 21, 2023
 *      Author: yvc
 */

#include "SoftPWM.h"

uint32_t pwm_error_offset = 40;

uint8_t timerperiod_us = 50;

uint8_t softpwminit = 0;

/*
 * Function Name		: SoftPWMInit
 * Function Description : Init Software PWM to enable PWM on all digital pin
 * Function Remarks		: Using minimum of 10us timer interrupt to prevent lag
 * Function Arguments	: pin						Digital Pin Number on board
 * 						  period_us					PWM Period in micro second
 * 						  init_tim_set_compare_us	initial PWM value
 * Function Return		: None
 * Function Example		: SoftPWMInit(1, 20000, 1500);
 */
void SoftPWMInit(uint8_t pin, uint8_t one_time, uint32_t period_us, uint32_t init_tim_set_compare_us){
	softpwminit = 1;
	softpwm.one_time[pin] = one_time;
	softpwm.init_tim_set_compare[pin] = init_tim_set_compare_us;
	period_us = period_us / timerperiod_us * 0.98;
	init_tim_set_compare_us /= timerperiod_us;
	init_tim_set_compare_us = init_tim_set_compare_us>pwm_error_offset?(uint32_t)(init_tim_set_compare_us * 0.98):(uint32_t)(init_tim_set_compare_us);
	softpwm.pin_use[pin] = 1;
	softpwm.start[pin] = 1;
	softpwm.period[pin] = (uint32_t)(period_us);
	softpwm.period_counter[pin] = 0;
	softpwm.tim_set_compare[pin] = init_tim_set_compare_us;
	softpwm.new_tim_set_compare[pin] = init_tim_set_compare_us;
	softpwm.new_tim_set_compare_flag[pin] = 1;
	softpwm.tim_set_compare_flag[pin] = 0;
}

void SoftPWMStart(uint8_t pin){
	softpwm.period_counter[pin] = 0;
	softpwm.start[pin] = 1;
}

void SoftPWMStop(uint8_t pin){
	softpwm.start[pin] = 0;
	IP_OUT(pin, 0);
}

void SoftPWMReset(uint8_t pin){
	SoftPWMSet(pin, softpwm.init_tim_set_compare[pin]);
}

/*
 * Function Name		: SoftPWMSet
 * Function Description : Set new PWM value for specific digital pin
 * Function Remarks		: None
 * Function Arguments	: pin						Digital Pin Number on board
 * 						  tim_set_compare			new PWM value
 * Function Return		: None
 * Function Example		: SoftPWMSet(1, 1500);
 */
void SoftPWMSet(uint8_t pin, int32_t tim_set_compare_us){
	tim_set_compare_us += softpwm.init_tim_set_compare[pin];
	tim_set_compare_us /= timerperiod_us;
	tim_set_compare_us = tim_set_compare_us>pwm_error_offset?(int32_t)(tim_set_compare_us * 0.98):(int32_t)(tim_set_compare_us);
	if (softpwm.tim_set_compare[pin] != tim_set_compare_us){
		softpwm.new_tim_set_compare[pin] = tim_set_compare_us;
		softpwm.new_tim_set_compare_flag[pin] = 1;
	}
}

/*
 * Function Name		: SoftPWMUpdate
 * Function Description : Software PWM generation
 * Function Remarks		: Run inside timer interrupt function
 * Function Arguments	: None
 * Function Return		: None
 * Function Example		: SoftPWMUpdate();
 */
void SoftPWMUpdate(){
	if (softpwminit){
		for (uint8_t pin_num = 1; pin_num <= IPCOUNT; pin_num++){
			if (softpwm.start[pin_num] == 1){
				if (softpwm.tim_set_compare_flag[pin_num] == 1 && softpwm.one_time[pin_num] == 1){
				}
				else{
					if (softpwm.period_counter[pin_num] < softpwm.tim_set_compare[pin_num]){
						IP_OUT(pin_num, 1);
					}
					else{
						IP_OUT(pin_num, 0);
					}
				}
				softpwm.period_counter[pin_num]++;
				if (softpwm.period_counter[pin_num] >= softpwm.period[pin_num]){
					softpwm.period_counter[pin_num] = 0;
					softpwm.tim_set_compare_flag[pin_num] = 1;
					if (softpwm.new_tim_set_compare_flag[pin_num] == 1){
						softpwm.tim_set_compare[pin_num] = softpwm.new_tim_set_compare[pin_num];
						softpwm.new_tim_set_compare_flag[pin_num] = 0;
						softpwm.tim_set_compare_flag[pin_num] = 0;
					}
				}
			}
		}
	}
}

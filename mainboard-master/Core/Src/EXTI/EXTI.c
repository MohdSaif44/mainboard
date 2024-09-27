/*
 * EXTI.c
 *
 *  Created on: Apr 19, 2024
 *      Author: mohds
 */

#include "EXTI.h"

void ExtixInit(uint16_t GPIO_Pin, uint32_t PreemptPriority, uint32_t SubPriority, Exti_t *ExtiPin)
{
	IRQn_Type IRQn;

	ExtiPin->GPIO_EXTI_PIN = GPIO_Pin;

	if(GPIO_Pin == GPIO_PIN_0){

		IRQn = EXTI0_IRQn;

	}
	else if(GPIO_Pin == GPIO_PIN_1){

		IRQn = EXTI1_IRQn;

	}
	else if(GPIO_Pin == GPIO_PIN_2){

		IRQn = EXTI2_IRQn;

	}
	else if(GPIO_Pin == GPIO_PIN_3){

		IRQn = EXTI3_IRQn;

	}
	else if(GPIO_Pin == GPIO_PIN_4){

		IRQn = EXTI4_IRQn;

	}
	else if(GPIO_Pin == GPIO_PIN_5 || GPIO_Pin == GPIO_PIN_6 || GPIO_Pin == GPIO_PIN_7 || GPIO_Pin == GPIO_PIN_8 || GPIO_Pin == GPIO_PIN_9){

		IRQn = EXTI9_5_IRQn;

	}
	else if(GPIO_Pin == GPIO_PIN_10 || GPIO_Pin == GPIO_PIN_11 || GPIO_Pin == GPIO_PIN_12 || GPIO_Pin == GPIO_PIN_13 || GPIO_Pin == GPIO_PIN_14 || GPIO_Pin == GPIO_PIN_15){

		IRQn = EXTI15_10_IRQn;
	}

	HAL_NVIC_SetPriority(IRQn, PreemptPriority, SubPriority);

	HAL_NVIC_EnableIRQ(IRQn);

}

/*
 * EXTI.h
 *
 *  Created on: Apr 19, 2024
 *      Author: mohds
 */

#ifndef SRC_EXTI_EXTI_H_
#define SRC_EXTI_EXTI_H_

#include "../BIOS/bios.h"

typedef struct{

	uint16_t GPIO_EXTI_PIN;


}Exti_t;

void ExtixInit(uint16_t GPIO_Pin, uint32_t PreemptPriority, uint32_t SubPriority, Exti_t *ExtiPin);


#endif /* SRC_EXTI_EXTI_H_ */

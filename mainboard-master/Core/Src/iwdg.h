/*
 * iwdg.h
 *
 *  Created on: Aug 7, 2022
 *      Author: heheibhoi
 */

#ifndef SRC_IWDG_IWDG_H_
#define SRC_IWDG_IWDG_H_

#include "BIOS/bios.h"
#include "cmsis_os.h"

IWDG_HandleTypeDef hiwdg;

void IWDG_Init(int timeout_in_ms, int prescaler);

#endif /* SRC_IWDG_IWDG_H_ */

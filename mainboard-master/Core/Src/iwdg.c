/*
 * iwdg.c
 *
 *  Created on: Aug 7, 2022
 *      Author: heheibhoi
 */

#include "iwdg.h"

/*prescaler values 4,8,16,32,64,128,256*/

//reload counter must be smaller than 4095, if otherwise, increase prescaler value

//tested value: timeout = 20, prescalar = 8
//1s, prescalar = 64, timeout = 1000
//used to monitor software fault

void IWDG_Init(int timeout_in_ms, int prescaler)
{
	int reload_counter = 0;

	hiwdg.Instance = IWDG;
	reload_counter = ((timeout_in_ms * 40000) / (prescaler * 1000)) - 1;

	switch(prescaler){

	case 4:
		hiwdg.Init.Prescaler = IWDG_PRESCALER_4;
		break;

	case 8:
		hiwdg.Init.Prescaler = IWDG_PRESCALER_8;
		break;

	case 16:
		hiwdg.Init.Prescaler = IWDG_PRESCALER_16;
		break;

	case 32:
		hiwdg.Init.Prescaler = IWDG_PRESCALER_32;
		break;

	case 64:
		hiwdg.Init.Prescaler = IWDG_PRESCALER_64;
		break;

	case 128:
		hiwdg.Init.Prescaler = IWDG_PRESCALER_128;
		break;

	case 256:
		hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
		break;

	}

	hiwdg.Init.Reload = reload_counter;


	if(HAL_IWDG_Init(&hiwdg) != HAL_OK)
		Error_Handler();

}

/*
 * WDG.c
 *
 *  Created on: Aug 1, 2024
 *      Author: ahmed amir
 *
 */
#include "../WDG/WDG.h"


// Timeout = ((counter+1)/3200)*4000*2^pr ---> counter cant be more than 4096 12-bit (value calculated in ms)

// Prescaler 	dividerPR[2:0]bits	Mintimeout 		Maxtimeout
//   /4 			0				 0.125				512
//   /8				1				  0.25				1024
//   /16			2				  0.5				2048
//   /32			3				   1				4096
//   /64			4				   2				8192
//   /128			5				   4				16384
//   /256			6				   8				32768

void watchdoginit(uint32_t counter,uint32_t prescaler){


	  hiwdg.Instance = IWDG;
	  hiwdg.Init.Prescaler = prescaler;
	  hiwdg.Init.Reload = counter;

	  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
	  {
	    Error_Handler();
	  }

}



void watchdog(void){
	HAL_IWDG_Refresh(&hiwdg);
}

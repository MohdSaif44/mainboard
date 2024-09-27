/*
 * tfmini.h
 *
 *  Created on: 2 May 2022
 *      Author: wai
 */

#ifndef SRC_TFMINI_TFMINI_H_
#define SRC_TFMINI_TFMINI_H_

#include "../BIOS/bios.h"

enum {
	FIRMWARE_ver,
	TRIGGER_DETECTION,
	FRAME_RATE,
	DATA_FRAME,
	BAUD_RATE,
	OUTPUT_FORMAT,
	OUTPUT_EN,
	SYSTEM_RESET,
	FACTORY_SET,
	SAVE
};

//enum {PENDING_SYNC = 0, CONFIRMING_SYNC, IN_SYNC};

typedef struct {

	UART_HandleTypeDef* huartx;
	volatile uint8_t checksum;
	uint8_t state;
	unsigned char command;
	uint8_t buff[10];

	volatile float dist;	//in m
	volatile float str;
	volatile float temp;
	float dist_target;
	float dist_err;
	union{
		uint32_t setbuf;
		struct{
			char buf1;
			char buf2;
			char buf3;
			char buf4;
		};
	};

}tfmini_t;

void TFmini_Init(tfmini_t* tfmini, UART_HandleTypeDef* huartx);
void TFmini_set(tfmini_t* tfmini, unsigned char setting, ...);
void TFmini_Handler(tfmini_t* tfmini);

#endif /* SRC_TFMINI_TFMINI_H_ */

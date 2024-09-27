/*
 * PixyCam2_ESP32.c
 *
 *  Created on: May 28, 2024
 *      Author: kztam
 */

#include "PixyCam2_ESP32.h"

void ESP32_Init(esp32_t *Cam, UART_HandleTypeDef *huartx) {
	Cam->huartx = huartx;
	Cam->state = SYNCING_0xC1;

	memset(&Cam->data, 0, sizeof(Cam->data));
	memset(&Cam->x_pos, 0, sizeof(Cam->x_pos));
	memset(&Cam->buffer, 0, sizeof(Cam->buffer));

	HAL_UART_Receive_IT(Cam->huartx, Cam->buffer, 1);	//receive 0xc1 header
}

//send only 1 time
void ESP32_Pixy_Cam2_SendInstruction(esp32_t* Cam, uint8_t instruction){
	uint8_t send_data[] = {'c', instruction};
	if(HAL_UART_Transmit(Cam->huartx, send_data, 2, 10) != HAL_OK){
	}
}

void ESP32_Flag_SendInstruction(esp32_t* Cam, uint8_t instruction){
	uint8_t send_data[] = {'f', instruction};
	if(HAL_UART_Transmit(Cam->huartx, send_data, 2, 10) != HAL_OK){
	}
}


void ESP32_Handler(esp32_t* Cam){

	switch (Cam->state) {

	case SYNCING_0xC1:
		if(Cam->buffer[0] == 0xC1){
			Cam->state = SYNCING_0xAE;
			HAL_UART_Receive_IT(Cam->huartx, Cam->buffer+1, 2);
		}
		else{
			HAL_UART_Receive_IT(Cam->huartx,Cam->buffer, 1);
		}
		break;

	case SYNCING_0xAE:
		if(Cam->buffer[2] == 0xAE){
			Cam->x_pos = Cam->buffer[1];	//m_x value from cam
			Cam->state = SYNCING_0xC1;
			HAL_UART_Receive_IT(Cam->huartx, Cam->buffer, 1);
		}
		else{
			Cam->state = SYNCING_0xC1;
			HAL_UART_Receive_IT(Cam->huartx, Cam->buffer, 1);
		}
			break;

		break;

	}
}

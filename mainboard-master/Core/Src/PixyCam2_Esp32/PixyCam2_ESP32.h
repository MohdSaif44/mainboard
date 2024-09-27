/*
 * PixyCam2_ESP32.h
 *
 *  Created on: May 28, 2024
 *      Author: kztam
 */

#ifndef SRC_PIXYCAM2_ESP32_PIXYCAM2_ESP32_H_
#define SRC_PIXYCAM2_ESP32_PIXYCAM2_ESP32_H_

#include "../BIOS/bios.h"

enum pixy_cam2_instruction_list{  //copy this into mainboard code,
  PIXY_0x54_GET_VECTOR = 1,
  PIXY_0x55_GET_VECTOR,
  PIXY_0x56_GET_VECTOR,
  PIXY_0x57_GET_VECTOR,
  PIXY_ALL_GET_VECTOR,
  PIXY_STOP_GET_VECTOR = 6,

  PIXY_0x54_TOGGLE_LAMP,
  PIXY_0x55_TOGGLE_LAMP,
  PIXY_0x56_TOGGLE_LAMP,
  PIXY_0x57_TOGGLE_LAMP,
  PIXY_ALL_TOGGLE_LAMP,
};

typedef enum{
	SYNCING_0xC1,
	SYNCING_0xAE,
}esp32_state_t;

typedef struct{
	UART_HandleTypeDef *huartx;
	esp32_state_t state;
  uint8_t buffer[20];
	uint8_t data;
	uint8_t cam_address, x_pos;
}esp32_t;

void ESP32_Pixy_Cam2_Init(esp32_t* Cam, UART_HandleTypeDef *huartx);
void ESP32_Pixy_Cam2_SendInstruction(esp32_t* Cam, uint8_t instruction);

void ESP32_Flag_SendInstruction(esp32_t *Cam, uint8_t instruction);

void ESP32_Handler(esp32_t* Cam);

#endif /* SRC_PIXYCAM2_ESP32_PIXYCAM2_ESP32_H_ */

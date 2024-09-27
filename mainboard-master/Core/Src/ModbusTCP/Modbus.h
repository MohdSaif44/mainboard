/*
 * Modbus.h
 *
 *  Created on: May 29, 2024
 *      Author: yvc
 */

#ifndef SRC_MODBUSTCP_MODBUS_H_
#define SRC_MODBUSTCP_MODBUS_H_

#include "../BIOS/bios.h"

#define REQUEST_OFFSET 0xc8
#define REQUEST_REGISTERS 9

typedef union {
	uint8_t byte[9+REQUEST_REGISTERS*2];
	struct {
		uint8_t TID[2];
		uint8_t PID[2];
		uint8_t response_length[2];
		uint8_t device;
		uint8_t action;
		uint8_t info_length;
		uint8_t values[REQUEST_REGISTERS*2];
	};
}Modbus_rx_data;

typedef union {
	uint8_t byte[12];
	struct {
		uint8_t TID[2];
		uint8_t PID[2];
		uint8_t length[2];
		uint8_t device;
		uint8_t action;
		uint8_t offset[2];
		uint8_t registers[2];
	};
}Modbus_tx_data;

typedef struct {
	UART_HandleTypeDef *huartx;
	uint8_t success;
	uint8_t data[50];
	Modbus_rx_data rx_data;
	Modbus_tx_data tx_data;
	uint32_t time;
	uint16_t count;
	uint16_t hz;
	uint16_t values[3];
	float l, r, b;
	uint8_t available;
	uint8_t bypass;
}Modbus;

void Modbus_Init(Modbus *modbus, UART_HandleTypeDef *huartx);
void Modbus_request_new_data(Modbus *modbus);
void Modbus_Handler(Modbus *modbus);

#endif /* SRC_MODBUSTCP_MODBUS_H_ */

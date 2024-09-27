/*
 * Modbus.c
 *
 *  Created on: May 29, 2024
 *      Author: yvc
 */

#include "Modbus.h"

void Modbus_Init(Modbus *modbus, UART_HandleTypeDef *huartx){
	modbus->huartx = huartx;
	modbus->tx_data.PID[0] = 0;
	modbus->tx_data.PID[1] = 0;
	modbus->tx_data.length[0] = 0;
	modbus->tx_data.length[1] = 6;
	modbus->tx_data.device = 1;
	modbus->tx_data.action = 3;
	modbus->tx_data.offset[0] = 0;
	modbus->tx_data.offset[1] = REQUEST_OFFSET;
	modbus->tx_data.registers[0] = 0;
	modbus->tx_data.registers[1] = REQUEST_REGISTERS;
	modbus->available = 0;
}

void Modbus_request_new_data(Modbus *modbus){
	memset(modbus->rx_data.byte, 0, sizeof(Modbus_rx_data));
	if (modbus->success != 1){
		modbus->available = 0;
		GPIOC_OUT->bit15 = 1;
		HAL_UART_DeInit(modbus->huartx);
		UARTInit(modbus->huartx, 115200, ENABLE);
		GPIOC_OUT->bit15 = 0;
	}
	HAL_UART_Receive_IT(modbus->huartx, modbus->rx_data.byte, sizeof(Modbus_rx_data));
	HAL_UART_Transmit_IT(modbus->huartx, modbus->tx_data.byte, 12);
	modbus->time = HAL_GetTick();
	if (modbus->time % 1000 < 10 && modbus->count > 10) {
		modbus->hz = modbus->count;
		modbus->count = 0;
	}
	modbus->success = 0;
}

void Modbus_Handler(Modbus *modbus){
	if (
//			(modbus->rx_data.PID[1] == modbus->tx_data.PID[1]) &&
//			(modbus->rx_data.response_length[1] == 3+REQUEST_REGISTERS*2) &&
//			(modbus->rx_data.device == modbus->tx_data.device) &&
			(modbus->rx_data.info_length == REQUEST_REGISTERS*2)
		)
	{
		modbus->success = 1;
		modbus->values[0] = modbus->rx_data.values[0] << 8 | modbus->rx_data.values[1];
		modbus->values[1] = modbus->rx_data.values[8] << 8 | modbus->rx_data.values[9];
		modbus->values[2] = modbus->rx_data.values[16] << 8 | modbus->rx_data.values[17];
//		modbus->values[0] = modbus->rx_data.values[24] << 8 | modbus->rx_data.values[25];
		modbus->l = modbus->values[0] / 1000.0;
		modbus->r = modbus->values[1] / 1000.0;
		modbus->b = modbus->values[2] / 1000.0;
		modbus->count++;
		modbus->available = (modbus->l < 10.0 && modbus->r < 10.0 && modbus->b < 10.0);
	} else {
		modbus->success = 0;
	}
	Modbus_request_new_data(modbus);
}

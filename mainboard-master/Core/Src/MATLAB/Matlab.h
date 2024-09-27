/*
 * Matlab.h
 *
 *  Created on: Aug 4, 2022
 *      Author: Shaon
 */

#ifndef SRC_MATLAB_MATLAB_H_
#define SRC_MATLAB_MATLAB_H_

#include "../adapter.h"

typedef enum{
	Mat_sync,
	Mat_Head=3,
	Mat_End=8,
	Mat_type,
	Mat_tx,			//header, length, ins, data, ender
	Mat_rx,
	Mat_tx_single,	//header, length, ins, index, data, ender
	Mat_rx_single,
	Mat_ok,
	Mat_ftx,
	Mat_wf,
	Mat_plot,
	Mat_plotx,
	Mat_vnum,
	Mat_term,
	Mat_name,
	Mat_real,
	Mat_realx,

	Mat_min,
	i32,
	f32,
	u32,
	u16,
	u8,
	Mat_max,

}MDat;

union{
	uint8_t flag;
	struct{
		unsigned dataReady	:1;
		unsigned ok			:1;
		unsigned plot		:1;
		unsigned real		:1;
		unsigned flag4		:1;
		unsigned flag5		:1;
		unsigned flag6		:1;
		unsigned flag7		:1;
	};
}Mflag;


uint8_t MatSec;
uint32_t baseAdd;
uint32_t plotT;
uint8_t plotI[4];
uint16_t timeI;
uint8_t* pVar[50];
uint8_t varNum;
MDat varType[50];
char varName[200];
UART_HandleTypeDef* MatUart;
uint8_t rx_buff[20];
uint8_t datlen;


void MatInit(uint8_t flashSec, UART_HandleTypeDef* huartx);
void MatVarInit(uint8_t* add, MDat type, char name[]);
uint8_t Mat_FlashWrite();
void Mat_terminal(char* s, uint8_t len);
void Mat_clearTerminal();
void Mat_FlashRead();
void Mat_Handler();
void Mat_dataProc();
void Mat_send(uint8_t* ptr, uint8_t len, MDat ins);
void Mat_sendLong(uint8_t* ptr, uint8_t len, MDat ins);
void Mat_plotSend();

#endif /* SRC_MATLAB_MATLAB_H_ */

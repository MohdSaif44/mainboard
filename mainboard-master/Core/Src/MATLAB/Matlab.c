/*
 * Matlab.c
 *
 *  Created on: Aug 4, 2022
 *      Author: Shaon
 */

#include "Matlab.h"

/*
 * Function Name		: MatVarInit
 * Function Description : Initialize parameters for Matlab Tuning Interface.
 * Function Remarks		: NONE
 * Function Arguments	: *add 		, address of parameters
 * 						  type		, type of variable, refer to enumeration MDat
 * Function Return		: None
 * Function Example		: MatVarInit(&number, 5);
 */
void MatVarInit(uint8_t* add, MDat type, char name[]){
	static uint8_t varCnt=0;
	uint8_t len=strlen(varName);
	varType[varCnt]=type;
	pVar[varCnt]=add;
	varNum=++varCnt;
	memcpy(&varName[len], name, strlen(name));
	varName[len+strlen(name)]=5;
}

/*
 * Function Name		: Mat_terminal
 * Function Description : Sends strings to Matlab under Terminal Tab.
 * Function Remarks		: NONE
 * Function Arguments	: *char		, pointer to string
 * 						  len		, length of string
 * Function Return		: None
 * Function Example		: Mat_terminal(&buff, strlen(buff));
 */
void Mat_terminal(char* s, uint8_t len){
	Mat_send(s, len+1, Mat_term);
}

/*
 * Function Name		: Mat_clearTerminal
 * Function Description : clears the screen in Matlab Terminal Tab.
 * Function Remarks		: NONE
 * Function Arguments	: NONE
 * Function Return		: None
 * Function Example		: Mat_clearTerminal();
 */
void Mat_clearTerminal(){
	Mat_send(0, 1, Mat_term);
}

//*************************************************************************************************************************************

/*
 * Function Name		: MatInit
 * Function Description : Initialize the Matlab Tuning Interface.
 * Function Remarks		: NONE
 * Function Arguments	: rns 		, pointer to a RNS data structure with RNS_interface _t type
 * 						  flashSec	, Flash Sector Number
 * 						  *huartx	, Pointer to structure UART_HandleTypeDef
 * Function Return		: None
 * Function Example		: MatInit(5, &huart2);
 */
void MatInit(uint8_t flashSec, UART_HandleTypeDef* huartx){

	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP);
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPERR);
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_WRPERR);
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGAERR);
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGPERR);
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGSERR);

	MatSec=flashSec;
	MatUart=huartx;

	baseAdd=0x8000000;
	if(MatSec<=4)
		baseAdd+=(0x4000)*MatSec;
	else if(MatSec==5)
		baseAdd=0x8020000;
	else
		baseAdd=0x8040000 + (0x20000)*(MatSec-6);
	Mflag.plot=Mflag.real=0;

	HAL_UART_Receive_IT(MatUart, rx_buff, 1);

}

/*
 * Function Name		: Mat_FlashRead
 * Function Description : Reads data from flash and assign to Matlab Variables.
 * Function Remarks		: NONE
 * Function Arguments	: NONE
 * Function Return		: None
 * Function Example		: Mat_FlashRead();
 */
void Mat_FlashRead(){
	uint32_t cumAdd=baseAdd;
	for(uint8_t i=0; i<varNum; i++){
		switch(varType[i]){

		case u8:
			*pVar[i]=*(uint8_t*)cumAdd;
			break;

		case u16:
			*(uint16_t*)pVar[i]=*(uint16_t*)cumAdd;
			break;

		case u32:
			*(uint32_t*)pVar[i]=*(uint32_t*)cumAdd;
			break;

		case i32:
			*(int*)pVar[i]=*(int*)cumAdd;
			break;

		case f32:
			*(float*)pVar[i]=*(float*)cumAdd;
			break;

		default:
			break;

		}
		cumAdd+=4;

	}
}

/*
 * Function Name		: Mat_FlashWrite
 * Function Description : Writes values of Matlab Variables into Flash.
 * Function Remarks		: NONE
 * Function Arguments	: NONE
 * Function Return		: eFlag	, indicates whether writing to flash is successful
 * Function Example		: Mat_FlashWrite();
 */
uint8_t Mat_FlashWrite(){
	uint32_t cumAdd=baseAdd;
	uint64_t bufff=0;
	uint8_t eFlag=0;
	HAL_FLASH_Unlock();
	FLASH_Erase_Sector(MatSec, FLASH_VOLTAGE_RANGE_3);		//sector 5 because after building, will see
	//txt data bss dec hex filename, use text+data, that is flash memory used by program. for now, until sector 4 is used

	for(uint8_t i=0; i<varNum; i++){
		switch(varType[i]){

		case u8:
			*(uint8_t*)(&bufff)=*(uint8_t *)pVar[i];
			break;

		case u16:
			*(uint16_t*)(&bufff)=*(uint16_t *)pVar[i];
			break;

		case u32:
			*(uint32_t*)(&bufff)=*(uint32_t *)pVar[i];
			break;

		case i32:
			*(int*)(&bufff)=*(int *)pVar[i];
			break;

		case f32:
			*(float*)(&bufff)=*(float *)pVar[i];
			break;

		default:
			break;
		}

		if((HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, cumAdd, bufff))==HAL_ERROR){
			eFlag=1;
		}

		cumAdd+=4;
		bufff=0;
	}

	HAL_FLASH_Lock();
	return eFlag;

}

/*
 * Function Name		: Mat_Handler
 * Function Description : UART callback handler.
 * Function Remarks		: NONE
 * Function Arguments	: NONE
 * Function Return		: None
 * Function Example		: Mat_Handler();
 */
void Mat_Handler(){
	static uint8_t state=0;

	switch(state){
	case 0:
		if(rx_buff[0]==Mat_Head)
			state++;
		HAL_UART_Receive_IT(MatUart, rx_buff, 1);
		break;

	case 1:
		datlen=rx_buff[0];
		HAL_UART_Receive_IT(MatUart, rx_buff, datlen+1);
		state++;
		break;

	case 2:
		if(rx_buff[datlen]==Mat_End)
			Mflag.dataReady=1;
		HAL_UART_Receive_IT(MatUart, rx_buff, 1);
		state=0;
		break;
	}

}

/*
 * Function Name		: Mat_dataProc
 * Function Description : Process UART data when available.
 * Function Remarks		: NONE
 * Function Arguments	: NONE
 * Function Return		: None
 * Function Example		: Mat_dataProc();
 */
void Mat_dataProc(){
	uint8_t i=0, buff[8];
	uint16_t size;
	uint32_t cumAdd=baseAdd;
	switch(rx_buff[0]){
	case Mat_sync:
		Mat_send(0, 1, Mat_sync);
		Mflag.plot=0;
		Mflag.real=0;
		break;
	case Mat_tx:
		Mat_send(0, 1, Mat_ok); //add here for bug fix purpose.
		for(i=0; i<varNum; i++){

			switch(varType[i]){
			case i32:
				size=4;
				break;

			case f32:
				size=4;
				break;

			case u8:
				size=1;
				break;

			case u32:
				size=4;
				break;

			case u16:
				size=2;
				break;

			default:
				break;
			}

			Mat_send(pVar[i], size+1, Mat_tx);
		}
//		Mat_send(0, 1, Mat_ok);
		break;

	case Mat_ftx:

		for(i=0; i<varNum; i++){

			switch(varType[i]){
			case i32:
				size=4;
				break;

			case f32:
				size=4;
				break;

			case u8:
				size=1;
				break;

			case u32:
				size=4;
				break;

			case u16:
				size=2;
				break;

			default:
				break;
			}

			Mat_send(cumAdd, size+1, Mat_tx);
			cumAdd+=4;
		}
		Mat_send(0, 1, Mat_ok);
		break;

	case Mat_rx:

		break;

	case Mat_tx_single:

		break;

	case Mat_rx_single:
		switch (varType[rx_buff[1]]){
		case i32:
			*(int*)pVar[rx_buff[1]]=*(int*)&rx_buff[2];
			break;

		case f32:
			*(float*)pVar[rx_buff[1]]=*(float*)&rx_buff[2];
			break;

		case u8:
			*pVar[rx_buff[1]]=rx_buff[2];
			break;

		case u16:
			*(uint16_t*)pVar[rx_buff[1]]=*(uint16_t*)&rx_buff[2];
			break;

		case u32:
			*(uint32_t*)pVar[rx_buff[1]]=*(uint32_t*)&rx_buff[2];
			break;

		default:
			break;
		}
		Mat_send(0, 1, Mat_ok);
		break;

		case Mat_type:
			for(i=0; i<varNum; i++){
				buff[0]=varType[i];
				Mat_send(&buff, 2, Mat_type);
			}
			buff[0]=varNum;
			Mat_send(&buff, 2, Mat_vnum);
			size=0;
			while(size<strlen(varName)){
				if(strlen(varName)-size>20){
					Mat_send(&varName[size], 21, Mat_name);
					size+=20;
				}else{
					Mat_send(&varName[size], strlen(varName)-size+1, Mat_name);
					break;
				}
			}
			Mflag.real=0;
			Mat_send(0, 1, Mat_ok);
			break;

		case Mat_wf:
			if (!Mat_FlashWrite()){
				Mat_send(0, 1, Mat_ok);
				//led3 = 1;
			}
			break;

		case Mat_plot:
			Mat_send(0, 1, Mat_ok);
			for (i=0; i<=datlen-2; i++)
				plotI[i]=rx_buff[i+1];
			while(i<4){
				plotI[i]=0;
				i++;
			}
			timeI=1;
			Mflag.plot=1;
			plotT=HAL_GetTick();
			break;

		case Mat_plotx:
			Mflag.plot=0;
			break;

		case Mat_real:
			Mat_send(0, 1, Mat_ok);
			for (i=0; i<=datlen-2; i++)
				plotI[i]=rx_buff[i+1];
			while(i<4){
				plotI[i]=0;
				i++;
			}
			timeI=1;
			plotT=HAL_GetTick();
			Mflag.real=1;
			break;

		case Mat_realx:
			Mat_send(0, 1, Mat_ok);
			Mflag.real=0;
			break;
	}

	Mflag.dataReady=0;

}

/*
 * Function Name		: Mat_send
 * Function Description : Sends data to Matlab through UART.
 * Function Remarks		: NONE
 * Function Arguments	: *ptr	, pointer to data
 * 						  len	, length of data
 * 						  ins	, instruction of data
 * Function Return		: None
 * Function Example		: Mat_send(&buff, 2, Mat_vnum);
 */
void Mat_send(uint8_t* ptr, uint8_t len, MDat ins){
	uint8_t temp[24]={ Mat_Head, len, ins };
	if(ptr!=0)
		memcpy(&temp[3], ptr, len-1);
	temp[2+len]=Mat_End;
	HAL_UART_Transmit(MatUart, temp, 3+len, 20);

}

/*
 * Function Name		: Mat_sendIT
 * Function Description : Sends data to Matlab through UART in interrupt mode.
 * Function Remarks		: NONE
 * Function Arguments	: *ptr	, pointer to data
 * 						  len	, length of data
 * 						  ins	, instruction of data
 * Function Return		: None
 * Function Example		: Mat_sendIT(&buff, 2, Mat_vnum);
 */
void Mat_sendLong(uint8_t* ptr, uint8_t len, MDat ins){
	uint8_t temp[]={ Mat_Head, len, ins };
	if(ptr!=0)
		memcpy(&temp[3], ptr, len-1);
	temp[2+len]=Mat_End;
	HAL_UART_Transmit(MatUart, temp, 3+len, 500);

}

/*
 * Function Name		: Mat_plotSend
 * Function Description : Sends data to Matlab for plotting graph under Plot Tab.
 * Function Remarks		: NONE
 * Function Arguments	: NONE
 * Function Return		: None
 * Function Example		: Mat_plotSend();
 */
void Mat_plotSend(){
	uint8_t temp[24]={18, 19};
	uint8_t l=6;
	uint8_t d=0;
	*(uint32_t *)(&temp[2])=HAL_GetTick()-plotT;
	for (uint8_t i=0; i<4; i++){
		if(plotI[i]!=0){
			switch (varType[plotI[i]-1]){
			case i32:
				d=4;
				break;

			case f32:
				d=4;
				break;

			case u8:
				d=1;
				break;

			case u16:
				d=2;
				break;

			case u32:
				d=4;
				break;

			default:
				break;
			}
			memcpy(&temp[l], pVar[plotI[i]-1], d);
			l+=d;

		}else
			break;
	}
	temp[l]=20;
	temp[l+1]=21;
	HAL_UART_Transmit(MatUart, temp, l+2, 50);
}

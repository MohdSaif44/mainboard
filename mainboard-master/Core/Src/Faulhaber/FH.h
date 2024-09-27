/*
 * FB.h
 *
 *  Created on: Oct 7, 2022
 *      Author: Shaon
 */

#ifndef SRC_FAULHABER_FB_H_
#define SRC_FAULHABER_FB_H_

#include "../CAN/can.h"
#include "../BIOS/uart.h"
#include "cmsis_os.h"

enum{
	faulhaber1 = 0x30,
	faulhaber2,
	faulhaber3,
	faulhaber4,
	faulhaberlast = 0x3F,
};

typedef enum{
	NMT=0,
	SYNC,
	EMGY,
	PDO1Tx,		//control word
	PDO1Rx,		//status word
	PDO2Tx,		//actual position
	PDO2Rx,		//target position
	PDO3Tx,		//actual velocity
	PDO3Rx,		//target velocity
	PDO4Tx,		//actual torque
	PDO4Rx,		//target torque
	SDOTx,		//receive SDO from driver, only driver sends this function code
	SDORx		//send SDO to driver, only mainboard sends this function code
}FH_Fcode;

typedef enum{
	CNA=0,		//controller not activated
	PP,
	PV=3,
//	ATC					= -4,		//		Analog Torque Control Mode
//	AVC					= -3,		//	  Analog Velocity Control Mode
//	APC					= -2,		//	  Analog Position Control Mode
//	Voltage_mode		= -1,		//					  Voltage Mode
//	Not_activated		=  0,
//	PP					=  1,		//           Profile Position Mode
//	PV					=  3,		//           Profile Velocity Mode
//	Homing				=  6,		//					   Homing Mode
//	CSP					=  8,		//Cyclic Synchronous Position Mode
//	CSV					=  9,		//Cyclic Synchronous Velocity Mode
//	CST					= 10		//Cyclic Synchronous  Torque  Mode
}FH_OPmode;

typedef struct{

	uint32_t dist_offset;
	uint8_t nodeID;
	CAN_HandleTypeDef* hcanx;
	FH_OPmode mode;
	int offset;
	uint32_t maxV;
	int maxPos;
	int minPos;
	uint8_t conf[3];
	uint8_t uartrx;
	uint8_t FH_status[2];
	uint8_t SDOtx_dat[8];
	int statuswordtick;
	int SDOtick;
	int tempflag;
	union{
		int rx;
		uint8_t rx_buff[4];
	};
	union{
		uint8_t flag;
		struct{
			unsigned flag0		:1;
			unsigned waiting	:1;
			unsigned PDOInit	:1;
			unsigned brake		:1;			//1=manual, 0=auto
			unsigned target		:1;
			unsigned start		:1;
			unsigned PPflag		:1;
			unsigned reconnected:1;
		};
	};

}FaulHaber_t;

typedef struct {
	CAN_RxHeaderTypeDef Rxmsg;
	uint8_t Data[8];
} FaulHabermsg;

FaulHaber_t* pfh[5];	//max set as 4 motors for now

FaulHaber_t fh1, fh2;

FaulHabermsg FHmsg;

void FHInit(FaulHaber_t *fh, CAN_HandleTypeDef* hcanx, uint8_t ID, int of, uint32_t vel, int maxP, int minP);
void FH_WaitInit();
void FH_Config(FaulHaber_t *fh);
void FH_Start(FaulHaber_t *fh);
void FHEnq(FaulHaber_t *fh, FH_Fcode par);
void FHmode(FaulHaber_t *fh, FH_OPmode mode);
void FH_PosAbs(FaulHaber_t *fh, int pos);
void FH_PosRel(FaulHaber_t *fh, int pos);
void FH_Vel(FaulHaber_t *fh, int vel);
void FH_StopM(FaulHaber_t *fh, uint32_t dec);
void FH_StopA(FaulHaber_t *fh);
uint8_t FH_target(FaulHaber_t *fh);
void FH_Pvel(FaulHaber_t *fh, uint32_t vel);
void FH_Pos_Limit(FaulHaber_t *fh, int max, int min);
void FH_SDO(FaulHaber_t *fh, uint8_t buff[8]);
uint8_t FH_CheckCAN(FaulHaber_t *fh, uint8_t dat[8]);
void FaulHaber_Handler(uint8_t dat[8]);

void FH_WaitInit_KZ();
void FH_Node_ID(FaulHaber_t* fh);
void FH_COB_ID(FaulHaber_t* fh, uint8_t node_ID);
void FH_Reconnect(FaulHaber_t *fh);
void FH_StatusChecker(FaulHaber_t *fh);
void FH_Statusword_UART(UART_HandleTypeDef* huartx, FaulHaber_t *fh);
void FH_SDOTx_Enq(UART_HandleTypeDef* huartx, uint16_t index, uint8_t subindex, FaulHaber_t *fh);
void FH_Encoder_Reset(FaulHaber_t *fh);
void FH_Pdec(FaulHaber_t *fh, uint32_t dec);
#endif /* SRC_FAULHABER_FB_H_ */

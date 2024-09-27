
/*******************************************************************************
 * Title   : Laser (
 * Author  : LokCharming
 * Version : 1.00
 * Date    : Sept 2022
 *******************************************************************************
 * Description:
 * - Combined with ADC and kalman_filter(KF) to fully used it
 * -
 *
 * Version History:
 * 1.00 by Klok
 * - Basic function of laser calibration
 *
 * 1.1 by Anas
 * - Added Check for distance and way to manually tune
 *
 * Bugs:
 *
 ******************************************************************************/

#ifndef SRC_FAULHABER_INTERFACE_FAULHABER_INTERFACE_H_
#define SRC_FAULHABER_INTERFACE_FAULHABER_INTERFACE_H_

#include "../BIOS/bios.h"
#include "../CAN/can.h"

/**************************************************
 * 		Enumerator							  	  *
 *************************************************/
typedef enum{
	FAULHABER0,
	FAULHABER1,
	FAULHABER2,
	FAULHABER3,
	FAULHABER4,
	FAULHABER5,
	FAULHABER6,
	FAULHABER7
}MC5010S_CO_NODE_ID;

typedef enum{
	NMT 				= 0b0000,
	SYNC 				= 0b0001,
	EMERGENCY			= 0b0001,
	PDO1_TX 			= 0b0011,
	PDO1_RX 			= 0b0100,
	PDO2_TX				= 0b0101,
	PDO2_RX				= 0b0110,
	PDO3_TX				= 0b0111,
	PDO3_RX				= 0b1000,
	PDO4_TX				= 0b1001,
	PDO4_RX				= 0b1010,
	SDO_TX				= 0b1011,
	SDO_RX				= 0b1100,
	NMT_ERROR_CONTROL	= 0b1110
}FUNCTIONAL_CODE;

typedef enum{
	ATC					= -4,		//		Analog Torque Control Mode
	AVC					= -3,		//	  Analog Velocity Control Mode
	APC					= -2,		//	  Analog Position Control Mode
	Voltage_mode		= -1,		//					  Voltage Mode
	Not_activated		=  0,
	PP					=  1,		//           Profile Position Mode
	PV					=  3,		//           Profile Velocity Mode
	Homing				=  6,		//					   Homing Mode
	CSP					=  8,		//Cyclic Synchronous Position Mode
	CSV					=  9,		//Cyclic Synchronous Velocity Mode
	CST					= 10		//Cyclic Synchronous  Torque  Mode
}OPERATION_MODE;

typedef enum{
	PDO1				=  0,
	PDO2,
	PDO3,
	PDO4
}RECEIVE_PDO_PARAMETER;

typedef enum{
	CS_1_BYTE			= 0x2F,
	CS_2_BYTE			= 0x2B,
	CS_3_BYTE			= 0x27,
	CS_4_BYTE			= 0x23
}SDO_RX_CS;
/**************************************************
 * 		Structure							  	  *
 *************************************************/
typedef struct{
	CAN_HandleTypeDef* hcan;
	int actual_position;
	int actual_speed;
	int actual_torque;
	union{
		uint16_t Statusword;
		struct{		//Profile Position Mode
			uint16_t ReadyToSwitchOn	:1;
			uint16_t SwitchedOn			:1;
			uint16_t OperationEnabled	:1;
			uint16_t Fault				:1;
			uint16_t VoltageEnabled		:1;
			uint16_t QuickStop			:1;
			uint16_t SwitchOnDisabled	:1;
			uint16_t Warning			:1;
			uint16_t NotUsed			:1;
			uint16_t NotUsed1			:1;
			uint16_t PosTargetReached	:1;
			uint16_t InternalLimitActive:1;
			uint16_t SetPointAcknowledged:1;
			uint16_t FollowingError		:1;
			uint16_t Configurable1		:1;
		};
		struct{		//Profile Velocity Mode
			uint16_t same				:1;
			uint16_t same1				:1;
			uint16_t same2				:1;
			uint16_t same3				:1;
			uint16_t same4				:1;
			uint16_t same5				:1;
			uint16_t same6				:1;
			uint16_t same7				:1;
			uint16_t same8				:1;
			uint16_t same9				:1;
			uint16_t VelTargetReached	:1;
			uint16_t same10				:1;
			uint16_t DriveStandstill	:1;
			uint16_t MaxSlippageError	:1;
			uint16_t same11				:1;
		};
	};
	union{
		uint8_t masterFlag;
		struct{
			uint8_t waitingStatus		:1;
			uint8_t waitingPos			:1;
			uint8_t waitingSpeed		:1;
			uint8_t waitingTorque		:1;
			uint8_t flag4				:1;
			uint8_t flag5				:1;
			uint8_t flag6				:1;
			uint8_t flag7				:1;
		};
	};
}FAULHABER_INTERFACE_t;

FAULHABER_INTERFACE_t FaulHabers[8];


/**************************************************
 * 		Extern	variables					  	  *
 *************************************************/


/**************************************************
 * 		Function Prototype			  			  *
 *************************************************/
void FaulHaber_Init(CAN_HandleTypeDef* hcan, MC5010S_CO_NODE_ID id);
void FaulHaber_Change_Soft_Pos_Limit(int min, int max, MC5010S_CO_NODE_ID id);
void FaulHaber_Change_Operation_Mode(OPERATION_MODE mode, MC5010S_CO_NODE_ID id);
void FaulHaber_Move_Pos(int move, MC5010S_CO_NODE_ID id);
void FaulHaber_Velocity(int vel, MC5010S_CO_NODE_ID id);
void FaulHaber_Change_PDO_Transmission_Type
(RECEIVE_PDO_PARAMETER pdo, uint8_t PDO_Transfer_Type, MC5010S_CO_NODE_ID id);
void FaulHaber_Can_Handler(uint32_t stdId, uint8_t* aData);
void FaulHaber_Enquire_Pos(MC5010S_CO_NODE_ID id);
void FaulHaber_Init_Pos(int pos, MC5010S_CO_NODE_ID id);
void FaulHaber_Change_Profile_Velocity(uint32_t vel, MC5010S_CO_NODE_ID id);
#endif /* SRC_FAULHABER_INTERFACE_FAULHABER_INTERFACE_H_*/

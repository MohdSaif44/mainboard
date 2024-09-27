/*********************************************/
/*          Include Header                   */
/*********************************************/
#include "Faulhaber_Interface.h"
/*********************************************/

//References
//	https://www.faulhaber.com/fileadmin/Import/Media/EN_7000_05048.pdf  Drive_Functions.pdf
//	https://www.faulhaber.com/fileadmin/Import/Media/EN_7000_05050.pdf  CAN_Open.pdf

/*
 * 	WARNING !!!
 *  Don't SAVE Faulhaber parameters with Mainboard to avoid newbie save faulhaber
 *  parameters in infinite loops. Faulhaber have 30000 times to save only.
 *  Set the parameters(NODE ID, pid, ...) in Motion Manager before using it through mainboard
 */

/*
 * To write to Faulhaber Object dictionary(OD)
 * SDO_Rx format
 * [CS][Index LB][Index HB][Subindex][LLB][LHB][HLB][HHB]
 *  CS = 0x2F, 1 data byte in D0
 *  CS = 0x2B, 2 data bytes in D0 to D1
 *  CS = 0x27, 3 data bytes in D0 to D2
 *  CS = 0x23, 4 data bytes in D0 to D3
 *  Example
 *  uint8_t write[8]={0x2B, 0x17, 0x10, 0x00, 0xE8, 0x03, 0, 0};// write to OD 0x1017.00
 *  CAN_OPEN_TxMsg(&hcan1, SDO_RX, NodeId, write, 8);//SDO(RX)
*/

/*********************************************/
/*           Subroutine Function             */
/*********************************************/

/*
 * Function Name		: FaulHaber_Init
 * Function Description : This function is called to init FaulHaber to Operation Mode.
 * Function Remarks		: delay 200ms to avoid Motion Controller miss CAN message from mainboard
 * Function Arguments	: hcan 				pointer to CAN_HandleTypedef
 * 						  id				node id(should be set in software)
 * Function Return		: None
 * Function Example		: FaulHaber_Init(&hcan1, FAULHABER1);
 */

void FaulHaber_Init(CAN_HandleTypeDef* hcan, MC5010S_CO_NODE_ID id){
	FaulHabers[id].hcan = hcan;
	uint8_t go[2]={0x01, id};//start// NMT start remote node Enable transmission of PDO
	CAN_OPEN_TxMsg(hcan, NMT, 0x00, go, 2);
	HAL_Delay(200);


	uint8_t Controlword[2] = {0x06, 0x00};
	CAN_OPEN_TxMsg(hcan, PDO1_RX, id, Controlword, 2);
	Controlword[0] = 0x07;
	CAN_OPEN_TxMsg(hcan, PDO1_RX, id, Controlword, 2);
	Controlword[0] = 0x0F;
	CAN_OPEN_TxMsg(hcan, PDO1_RX, id, Controlword, 2);
}

/*
 * Function Name		: FaulHaber_Change_Soft_Pos_Limit
 * Function Description : This function is called to set min and max software position limit
 * Function Remarks		: The limit is only accurate when moving motor in slow speed, if move in
 * 						  high speed, it may over the software limit, but the overshoot is consistent
 * Function Arguments	: min				min encoder values(position)
 * 						: max 				max encoder values(position)
 * 						  id				node id(should be set in software)
 * Function Return		: None
 * Function Example		: FaulHaber_Change_Soft_Pos_Limit(-50361, 20000, FAULHABER1);
 */
void FaulHaber_Change_Soft_Pos_Limit(int min, int max, MC5010S_CO_NODE_ID id){
	uint8_t write[8] = {CS_4_BYTE, 0x7D, 0x60, 0x01, 0, 0, 0, 0};
	memcpy(&write[4], &min, 4);
	CAN_OPEN_TxMsg(FaulHabers[id].hcan, SDO_RX, id, write, 8);
	write[3] = 0x02;
	memcpy(&write[4], &max, 4);
	CAN_OPEN_TxMsg(FaulHabers[id].hcan, SDO_RX, id, write, 8);
}

/*
 * Function Name		: FaulHaber_Init_Pos
 * Function Description : This function is called to init FaulHaber to certain postion.
 * Function Remarks		: Toggle Led3 every 20ms if failed to init, check CAN wire
 * Function Arguments	: pos 				position in S32 datatype
 * 						  id				node id(should be set in software)
 * Function Return		: None
 * Function Example		: FaulHaber_Init_Pos(-20361, FAULHABER1);
 */
void FaulHaber_Init_Pos(int pos, MC5010S_CO_NODE_ID id){
	FaulHaber_Enquire_Pos(id);

	int move = pos - FaulHabers[id].actual_position;
	uint8_t PDO2[6] = {0x7F, 0x00, 0, 0, 0, 0};
	memcpy(&PDO2[2], &move, 4);
	CAN_OPEN_TxMsg(FaulHabers[id].hcan, PDO2_RX, id, PDO2, 6);
	uint32_t wait = HAL_GetTick();
	while(FaulHabers[id].PosTargetReached != 1){
		if(HAL_GetTick()-wait >= 20){
			GPIOC_OUT->bit15 = !GPIOC_OUT->bit15;
			CAN_TxRTR(FaulHabers[id].hcan, 640+FAULHABER1);
			wait = HAL_GetTick();
		}
	}
}

/*
 * Function Name		: FaulHaber_Change_Operation_Mode
 * Function Description : This function is called to change Operation Mode.
 * Function Remarks		: Default Operation Mode is Profile Position Mode
 * Function Arguments	: mode				Operation Mode
 * 						  id				node id(should be set in software)
 * Function Return		: None
 * Function Example		: FaulHaber_Change_Operation_Mode(PV, FAULHABER1);
 */
void FaulHaber_Change_Operation_Mode(OPERATION_MODE mode, MC5010S_CO_NODE_ID id){
	uint8_t write[8] = {CS_1_BYTE, 0x60, 0x60, 0x00, mode, 0, 0, 0};
	CAN_OPEN_TxMsg(FaulHabers[id].hcan, SDO_RX, id, write, 8);
}

/*
 * Function Name		: FaulHaber_Move_Pos
 * Function Description : This function is called to move Faulhaber in Profile Position Mode.
 * Function Remarks		: Change operation mode to Profile Position Mode first
 * Function Arguments	: move				steps to move in S32 datatype
 * 						  id				node id(should be set in software)
 * Function Return		: None
 * Function Example		: FaulHaber_Move_Pos(1000, FAULHABER1);
 */
void FaulHaber_Move_Pos(int move, MC5010S_CO_NODE_ID id){
	uint8_t Controlword[2] = {0x0F, 0x00};
	CAN_OPEN_TxMsg(FaulHabers[id].hcan, PDO1_RX, id, Controlword, 2);
	uint8_t PDO2[6] = {0x7F, 0x00, 0, 0, 0, 0};
	memcpy(&PDO2[2], &move, 4);
	CAN_OPEN_TxMsg(FaulHabers[id].hcan, PDO2_RX, id, PDO2, 6);
}

/*
 * Function Name		: FaulHaber_Velocity
 * Function Description : This function is called to move Faulhaber motor in specified rpm.
 * Function Remarks		: Default Operation Mode is Profile Position Mode
 * Function Arguments	: vel				velocity in rpm
 * 						  id				node id(should be set in software)
 * Function Return		: None
 * Function Example		: FaulHaber_Velocity(-2000, FAULHABER1);
 */
void FaulHaber_Velocity(int vel, MC5010S_CO_NODE_ID id){
	uint8_t PDO3[6] = {0x0F, 0x00, 0, 0, 0, 0};
	memcpy(&PDO3[2], &vel, 4);
	CAN_OPEN_TxMsg(FaulHabers[id].hcan, PDO3_RX, id, PDO3, 6);
}

/*
 * Function Name		: FaulHaber_Can_Handler
 * Function Description : This function is called in CAN callback to handle PDO objects
 * 						  sent from Motion Controller.
 * Function Remarks		: Can add code to handle SDO_TX Upload-Response sent from Faulhaber
 * Function Arguments	: stdId				CAN message Standard Id
 * 						  id				node id(should be set in software)
 * Function Return		: None
 * Function Example		: FaulHaber_Init_Pos(-20361, FAULHABER1);
 */
void FaulHaber_Can_Handler(uint32_t stdId, uint8_t* aData){
	if(stdId >= 384 && stdId <= 511){			//TXPDO1
		uint16_t id = stdId-384;
		FaulHabers[id].Statusword = *((uint16_t *)aData);
		FaulHabers[id].waitingStatus = 0;
	}else if(stdId >= 640 && stdId <=  767){	//TXPDO2
		uint16_t id = stdId-640;
		FaulHabers[id].Statusword = *((uint16_t *)aData);
		FaulHabers[id].actual_position = *((int *)&aData[2]);
		FaulHabers[id].waitingPos = 0;
	}else if(stdId >= 896 && stdId <= 1023){	//TXPDO3
		uint16_t id = stdId-896;
		FaulHabers[id].Statusword = *((uint16_t *)aData);
		FaulHabers[id].actual_speed = *((int *)&aData[2]);
		FaulHabers[id].waitingSpeed = 0;
	}else if(stdId >= 1152 && stdId <= 1278){	//TXPDO4
		uint16_t id = stdId-1152;
		FaulHabers[id].Statusword = *((uint16_t *)aData);
		FaulHabers[id].actual_torque = *((int *)&aData[2]);
		FaulHabers[id].waitingTorque = 0;
	}
}

/*
 * Function Name		: FaulHaber_Enquire_Pos
 * Function Description : This function is called to get FaulHaber motors encoder values
 * Function Remarks		: The PDO Transmission Type is currently set to 253(RTR) in software,
 * 						  can change based on application
 * Function Arguments	: id				node id(should be set in software)
 * Function Return		: None
 * Function Example		: FaulHaber_Enquire_Pos(FAULHABER1);
 */
void FaulHaber_Enquire_Pos(MC5010S_CO_NODE_ID id){
	FaulHabers[id].waitingPos = 1;
	CAN_TxRTR(FaulHabers[id].hcan, 640+id);
	while(FaulHabers[id].waitingPos == 1);
}

/*
 * Function Name		: FaulHaber_Change_Profile_Velocity
 * Function Description : This function is called to change profile velocity
 * Function Remarks		: In PV mode, the max speed is based on profile velocity,
 * 						  in PP mode, the speed to reach target pos is based on profile velocity
 * 						  Read the datasheet of motors to avoid operate motor over
 * 						  the specified speed limit
 * Function Arguments	: vel				profile velocity
 * 						  id				node id(should be set in software)
 * Function Return		: None
 * Function Example		: FaulHaber_Change_Profile_Velocity(1000, FAULHABER1);
 */
void FaulHaber_Change_Profile_Velocity(uint32_t vel, MC5010S_CO_NODE_ID id){
	uint8_t write[8] = {CS_4_BYTE, 0x81, 0x60, 0x00, 0, 0, 0, 0};
	memcpy(&write[4], &vel, 4);
	CAN_OPEN_TxMsg(FaulHabers[id].hcan, SDO_RX, id, write, 8);
}

/*
 * PDO Transfer Type
 * 0		synchronous, acyclical. A PDO is sent or executed once after a
 * 			SYNC object when the contents of the PDO have changed
 * 1-240	synchronous, cyclical A PDO is sent after every SYNC object.
 * 			The value is then equal to the number of SYNC objects that must
 * 			be received before the PDO is sent again (1 = PDO is sent for
 * 			every SYNC object)
 * 252		Only with TxPDOs: asynchronous. When a SYNC signal is received,
 * 			the content of the TxPDO is saved. When a request (RTR) is
 * 			received, the TxPDO is sent to the master
 * 253		Only with TxPDOs: asynchronous When a request (RTR) is received,
 * 			the TxPDO is sent to the master
 * 255		asynchronous (event-driven)
 */
void FaulHaber_Change_PDO_Transmission_Type
(RECEIVE_PDO_PARAMETER pdo, uint8_t PDO_Transfer_Type, MC5010S_CO_NODE_ID id){
	//	https://www.faulhaber.com/fileadmin/Import/Media/EN_7000_05048.pdf  Page 18
	uint8_t write[8] = {CS_1_BYTE, 0x14, pdo, 0x02, PDO_Transfer_Type, 0, 0, 0};
	CAN_OPEN_TxMsg(FaulHabers[id].hcan, SDO_RX, id, write, 8);
}

void FaulHaber_Quick_Stop(MC5010S_CO_NODE_ID id){
	uint8_t PDO1[2] = {0x0B, 0x00};
	CAN_OPEN_TxMsg(FaulHabers[id].hcan, PDO1_RX, id, PDO1, 2);
}

void FaulHaber_Reset(MC5010S_CO_NODE_ID id){
	uint8_t PDO1[2] = {0x80, 0x00};
	CAN_OPEN_TxMsg(FaulHabers[id].hcan, PDO1_RX, id, PDO1, 2);
	PDO1[0] = 0x06;
	CAN_OPEN_TxMsg(FaulHabers[id].hcan, PDO1_RX, id, PDO1, 2);
	PDO1[0] = 0x0F;
	CAN_OPEN_TxMsg(FaulHabers[id].hcan, PDO1_RX, id, PDO1, 2);
}

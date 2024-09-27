/*
 * FB.c
 *
 *  Created on: Oct 7, 2022
 *      Author: Shaon
 */

#include "FH.h"

/*
 * Function Name		: 	FHInit
 * Function Description : 	Called to init FaulHaber motion controller, will not cause delay
 * Function Remarks		: 	offset is only used to reduce small errors after you reset encoder,
 * 						  	if error is very big, just reset encoder
 * Function Arguments	: 	*fh		, pointer to structure FaulHaber_t
 * 							*hcanx	, pointer to structure CAN_HandleTypeDef
 * 							ID		, node ID of motion controller
 * 							of		, offset of absolute encoder
 * 							vel		, Profile Velocity, refer to documentation
 * 							maxP	, max position of absolute encoder (only when in PP mode)
 * 							minP	, min position of absolute encoder (only when in PP mode)
 * Function Return		: 	None
 * Function Example		: 	FHInit(&fh1, &hcan1, 1, 0, 3000, 2147483647, -2147483647);
 */
void FHInit(FaulHaber_t *fh, CAN_HandleTypeDef* hcanx, uint8_t ID, int of, uint32_t vel, int maxP, int minP){

	static uint8_t mtrCnt=0;
	fh->nodeID=ID;
	fh->hcanx=hcanx;
	fh->mode=CNA;
	fh->offset=of;
	fh->brake=1;
	fh->maxV=vel;
	fh->maxPos=maxP;
	fh->minPos=minP;
	pfh[mtrCnt++]=fh;

}

/*
 * Function Name		: 	FH_WaitInit
 * Function Description : 	Called to init FaulHaber motion controller, will wait for confirmation
 * 							from motion controller
 * Function Remarks		: 	will wait for motion controller to send boot up message first,
 * 							better call it after all initialization
 * Function Arguments	: 	None
 * Function Return		: 	None
 * Function Example		: 	FH_WaitInit();
 */
void FH_WaitInit(){
	uint8_t i=0;
	uint8_t temp[8]={ 0x01, 0, 0x60, 0, 0x06, 0, 0, 0};
	uint32_t timbuf;
//	while(pfh[i]!=0){
//		while(!pfh[i]->init);
//		temp[1]=pfh[i]->nodeID;
//		CAN_TxMsg(pfh[i]->hcanx, 0x00, temp, 2);	//activate PDO
//		i++;
//	}
//	HAL_Delay(1000);
//	temp[1]=pfh[i]->nodeID;
//	CAN_TxMsg(pfh[i]->hcanx, 0x00, temp, 2);	//activate PDO
//	i=1;
	while(pfh[i]!=0){		//have to wait for driver to enable PDO transmission first
		temp[1]=pfh[i]->nodeID;
		pfh[i]->PDOInit=0;
		timbuf=HAL_GetTick();
		CAN_TxMsg(pfh[i]->hcanx, 0x00, temp, 2);
		CAN_TxRTR(pfh[i]->hcanx, (PDO1Tx<<7) + pfh[i]->nodeID);
		while(!pfh[i]->PDOInit){

			if(HAL_GetTick()-timbuf>=5){
				CAN_TxMsg(pfh[i]->hcanx, 0x00, temp, 2);	//activate PDO
				CAN_TxRTR(pfh[i]->hcanx, (PDO1Tx<<7) + pfh[i]->nodeID);
				timbuf=HAL_GetTick();
			}
		}
		i++;
	}

	while(i>0){
		i--;
		FH_Start(pfh[i]);	//operation enable state
		FH_Config(pfh[i]);
		FH_Pvel(pfh[i], pfh[i]->maxV);	//set profile velocity
		pfh[i]->start=0;
		timbuf=HAL_GetTick();
		while(HAL_GetTick()-timbuf<3);
		pfh[i]->waiting=1;
		CAN_TxRTR(pfh[i]->hcanx, (PDO1Tx<<7) + pfh[i]->nodeID);
		while(pfh[i]->waiting);
		timbuf=HAL_GetTick();
		while(!pfh[i]->start){
			FH_Start(pfh[i]);
			while(HAL_GetTick()-timbuf<3);
			pfh[i]->waiting=1;
			CAN_TxRTR(pfh[i]->hcanx, (PDO1Tx<<7) + pfh[i]->nodeID);
			while(pfh[i]->waiting);
			timbuf=HAL_GetTick();
		}
	}
}

void FH_StatusChecker(FaulHaber_t *fh){
	CAN_TxRTR(fh->hcanx, (PDO1Tx<<7) + fh->nodeID);
}

/*
 * Function Name		: 	FH_WaitInit_KZ
 * Function Description : 	Called to init FaulHaber motion controller, will wait for confirmation
 * 							from motion controller
 * Function Remarks		:   written by KZTam.
 * Function Arguments	: 	None
 * Function Return		: 	None
 * Function Example		: 	FH_WaitInit_KZ();
 */
void FH_WaitInit_KZ(){
	uint8_t i=0;
	uint8_t temp[8]={0x01, 0, 0x60, 0, 0x06, 0, 0, 0};

	HAL_Delay(500); //Mainboard and Motion Manager switch on simultaneously,waiting motion manager ready
	while(pfh[i]!=0){
		FH_COB_ID(pfh[i],pfh[i]->nodeID);
		temp[1]=pfh[i]->nodeID;
		CAN_TxMsg(pfh[i]->hcanx, 0x00, temp, 2); // boot up
		temp[0]=0x00;
		CAN_OPEN_TxMsg(pfh[i]->hcanx, PDO1Rx, pfh[i]->nodeID, temp, 2); // disabled voltage.
		i++;
	}
	HAL_Delay(10); //if no delay here, motion controller will not execute shut down command.
	while(i>0){
		i--;
		FH_ShutDown(pfh[i]);
		pfh[i]->start = 0;
		while(!pfh[i]->start){
			temp[0]=0x0f;
			CAN_OPEN_TxMsg(pfh[i]->hcanx, PDO1Rx, pfh[i]->nodeID, temp, 2); //operation enable
			CAN_TxRTR(pfh[i]->hcanx, (PDO1Tx<<7) + pfh[i]->nodeID); //operation enable checker
			GPIOC_OUT->bit14 = 0;
		}
		FH_Config(pfh[i]);
		FH_Pvel(pfh[i], pfh[i]->maxV);
		//pfh[i]->statuswordtick = pfh[i]->SDOtick = HAL_GetTick();
		//FH_Encoder_Reset(pfh[i]);
	}
	//GPIOC_OUT->bit15 = 1;	//led3 = 1
}

//prototype waitinit function, trying to enable operation using different approach stated in drive function (no use)
void FH_Enable_TEST(){
	uint8_t i=0;
	//uint8_t temp[8]={0x01, 0, 0x60, 0, 0x06, 0, 0, 0};

	HAL_Delay(500);
	while(pfh[i]!=0){
		FH_COB_ID(pfh[i],pfh[i]->nodeID);
		i++;
	}
	HAL_Delay(10);
	while(i>0){
		i--;
		FH_Config(pfh[i]);
		FH_Pvel(pfh[i], pfh[i]->maxV);
		pfh[i]->statuswordtick = pfh[i]->SDOtick = HAL_GetTick();
		FH_Encoder_Reset(pfh[i]);
	}
	HAL_UART_Receive_IT(&huart2, &pfh[0]->uartrx, 1); //temporary
	GPIOC_OUT->bit15 = 1;
}

/*
 * Function Name		: 	FH_Config
 * Function Description : 	Called in FH_WaitInit() function
 * Function Remarks		: 	-
 * Function Arguments	: 	*fh	, pointer to structure FaulHaber_t
 * Function Return		: 	None
 * Function Example		: 	FH_Config(&fh1);
 */
void FH_Config(FaulHaber_t *fh){
	uint8_t temp[8]={0x2b, 0x5a, 0x60, 0x00, 0x06, 0, 0, 0};
	int num=2147483647;
	FH_SDO(fh, temp);	//quickStop uses Quicktop ramp
	temp[1]=0x3f;
	temp[2]=0x23;
	temp[4]=0x31;
	FH_SDO(fh, temp);	//PV mode not limited by position limit
	temp[0]=0x23;
	temp[1]=0x7b;
	temp[2]=0x60;
	temp[3]=0x02;
	memcpy(&temp[4], &num, 4);
	FH_SDO(fh, temp);	//max position limit
	temp[3]=0x01;
	num=-num;
	memcpy(&temp[4], &num, 4);
	FH_SDO(fh, temp);	//min position limit
	FH_Pos_Limit(fh, fh->maxPos, fh->minPos);
	temp[0]=0x23;
	temp[1]=0x65;
	temp[3]=0x00;
	temp[4]=temp[5]=temp[6]=temp[7]=0xff;
	FH_SDO(fh, temp);	//max following error
}

/*
 * Function Name		: 	FH_Node_ID
 * Function Description : 	Called to set motion controller node ID
 * Function Remarks		: 	Not recommend to change id while running
 * Function Arguments	: 	*fh	, pointer to structure FaulHaber_t
 * Function Return		: 	None
 * Function Example		: 	FH_Node_ID(&fh1);
 */
void FH_Node_ID(FaulHaber_t* fh){
	uint8_t temp[8] = {0x2f, 0x00, 0x24, 0x03, fh->nodeID, 0, 0, 0};
	FH_SDO(fh, temp);
}

/*
 * Function Name		: 	FH_Start
 * Function Description : 	Called to set motion controller into Operation Enable State
 * Function Remarks		: 	When the motion controller senses an error, it will go into Fault State,
 * 							call this function to set it back into Operational Enable State
 * Function Arguments	: 	*fh	, pointer to structure FaulHaber_t
 * Function Return		: 	None
 * Function Example		: 	FH_Start(&fh1);
 */
void FH_Start(FaulHaber_t *fh){
	uint8_t temp[2]={0x06, 0x00};
	CAN_OPEN_TxMsg(fh->hcanx, PDO1Rx, fh->nodeID, temp, 2);		//shut down
	temp[0]=0x07;
	CAN_OPEN_TxMsg(fh->hcanx, PDO1Rx, fh->nodeID, temp, 2);		//switch on
	temp[0]=0x0f;
	CAN_OPEN_TxMsg(fh->hcanx, PDO1Rx, fh->nodeID, temp, 2);		//enable operation
}

/*
 * Function Name		: 	FH_Reconnect
 * Function Description : 	Called to reconnect FH motor when its power accidentally disconnected
 * Function Remarks		: 	Call in while loop
 * 							Written by KZTam
 * Function Arguments	: 	*fh	, pointer to structure FaulHaber_t
 * Function Return		: 	None
 * Function Example		: 	FH_Reconnect(&fh1);
 */
void FH_Reconnect(FaulHaber_t *fh){
	uint32_t timbuf;
	uint8_t temp[2]={0x0f, 0x00};

	//CAN_TxRTR(fh->hcanx, (PDO1Tx<<7) + fh->nodeID); //status checker (if not Enq function then uncomment this)
	if(!fh->start){ //motor disconnected, start FH motor reconnect procedure
		if(fh->flag0 == 1){
			osDelay(1);
			FH_ShutDown(fh);
			timbuf = HAL_GetTick();
			fh->reconnected = 0;
			fh->flag0 = 0;
		}
		if(HAL_GetTick() - timbuf >=5 && !fh->start){
			CAN_OPEN_TxMsg(fh->hcanx, PDO1Rx, fh->nodeID, temp, 2); //operation enable
			timbuf = HAL_GetTick();
		}
		GPIOC_OUT->bit15 = 0;
	}
	else{ //when reconnect successful
		GPIOC_OUT->bit15 = 1;
		fh->flag0 = 1;
	}
}

/*
 * Function Name		: 	FH_Statusword_UART
 * Function Description : 	Called to print out 0-15 bits of statusword (PDOxTx) via UART
 * Function Remarks		: 	Call together with FHEnq(fh, <parameter>);
 * 							bit arrangement: MSB (bit-15) to LSB (bit-0)
 * 							Written by KZTam
 * Function Arguments	: 	*fh	, pointer to structure FaulHaber_t
 * 							*huartx , pointer to structure UART_HandleTypeDef
 * Function Return		: 	None
 * Function Example		: 	FH_Statusword_UART(&huart5, &fh1);
 */
void FH_Statusword_UART(UART_HandleTypeDef* huartx, FaulHaber_t *fh){
	char status[70];
	int status_bit[16], mask = 1 << 15, i = 0;
	uint16_t temp = (fh->FH_status[0] << 8) + fh->FH_status[1];

	while(i<16){
		status_bit[i] = (temp & mask)? 1:0;
		mask>>=1;
		i++;
	}

	if(HAL_GetTick() - fh->statuswordtick >= 50){
		sprintf(status, "statusword = %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
				status_bit[8],status_bit[9],status_bit[10],status_bit[11],status_bit[12],status_bit[13],status_bit[14],status_bit[15],
				status_bit[0],status_bit[1],status_bit[2],status_bit[3],status_bit[4],status_bit[5],status_bit[6],status_bit[7]);

		UARTPrintString(huartx, status);
		fh->statuswordtick = HAL_GetTick();
	}
}

/*
 * Function Name		: 	FH_SDOTx_Enq
 * Function Description : 	Called to enquire SDOTx
 * Function Remarks		: 	Not suitable to be used with RNS_Enquire. Refer to documentation for solution (from KZ: not sure abt this as RNS board long time no use alr)
 * 							Refer to Drive Function to get index and subindex of the enquire parameter
 * 							Written by KZTam
 * Function Arguments	: 	*fh	, pointer to structure FaulHaber_t
 * 							index , index of parameter
 * 							subindex , subindex of parameter
 * 							*huartx , pointer to structure UART_HandleTypeDef
 * Function Return		: 	None
 * Function Example		: 	FH_SDOTx_Enq(&huart5, 0x6060, 0, &fh1);
 */
void FH_SDOTx_Enq(UART_HandleTypeDef* huartx, uint16_t index, uint8_t subindex, FaulHaber_t *fh){
	uint8_t temp[2];
	char msg[70];
	temp[1] = index >> 8;
	temp[0] = index & 0x00ff;

	uint8_t data[8] = {0x40, temp[0], temp[1], subindex, 0, 0, 0, 0};

	FH_SDO(fh, data);

	if(HAL_GetTick() - fh->SDOtick >= 50){
		sprintf(msg, "SDOTx dataframe (HEX): %x %x %x %x %x %x %x %x\n",
				fh->SDOtx_dat[0],fh->SDOtx_dat[1],fh->SDOtx_dat[2],fh->SDOtx_dat[3],
				fh->SDOtx_dat[4],fh->SDOtx_dat[5],fh->SDOtx_dat[6],fh->SDOtx_dat[7]);

		UARTPrintString(huartx, msg);
		fh->SDOtick = HAL_GetTick();
	}
}

/*
 * Function Name		: 	FH_ShutDown
 * Function Description : 	Turn Motion Controller to "ready to switch on" state.
 * Function Remarks		: 	Written by KZTam
 * Function Arguments	: 	*fh	, pointer to structure FaulHaber_t
 * Function Return		: 	None
 * Function Example		: 	FH_ShutDown(&fh1);
 */
void FH_ShutDown(FaulHaber_t *fh){
	uint8_t temp[2]={0x06, 0x00};
	CAN_OPEN_TxMsg(fh->hcanx, PDO1Rx, fh->nodeID, temp, 2);
}

/*
 * Function Name		: 	FHEnq
 * Function Description : 	Called to enquire PDOs
 * Function Remarks		: 	Not suitable to be used with RNS_Enquire. Refer to documentation for solution
 * Function Arguments	: 	*fh	, pointer to structure FaulHaber_t
 * 							par	, PDO to be enquired
 * Function Return		: 	None
 * Function Example		: 	FHEnq(&fh1, PDO2Tx);
 */
void FHEnq(FaulHaber_t *fh, FH_Fcode par){
	fh->waiting=1;
	CAN_TxRTR(fh->hcanx, (par<<7)+fh->nodeID);
	//while(fh->waiting);
}

/*
 * Function Name		: 	FH_PosAbs
 * Function Description : 	Called to set absolute target position
 * Function Remarks		: 	unlike relative position, it overides the target position completely
 * Function Arguments	: 	*fh	, pointer to structure FaulHaber_t
 * 							pos	, value of absolute target position
 * Function Return		: 	None
 * Function Example		: 	FH_PosAbs(&fh1, 500000);
 */
void FH_PosAbs(FaulHaber_t *fh, int pos){
	uint8_t temp[6]={0x0f, 0x00, 0, 0, 0, 0};
	FHmode(fh, PP);
	FH_Pvel(fh, fh->maxV);

	if(fh->PPflag){
		CAN_OPEN_TxMsg(fh->hcanx, PDO2Rx, fh->nodeID, temp, 6);		//set bit 4 to LOW (enable operation)
	}
	fh->PPflag = 1;

	int posi=pos+fh->offset;
	temp[0]=0x3f;
	memcpy(&temp[2], &posi, 4);
	CAN_OPEN_TxMsg(fh->hcanx, PDO2Rx, fh->nodeID, temp, 6);
}

/*
 * Function Name		: 	FH_PosRel
 * Function Description : 	Called to set relative absolute position
 * Function Remarks		: 	This function adds/substracts the value of the target position,
 * 							Eg. If initial position = 0;
 * 							FH_PosRel(1000); FH_PosRel(-800)	=> target position=200
 * 							FH_PosAbs(1000); FH_PosRel(200)		=> target position=1200
 * 							FH_PosRel(200); FH_PosAbs(1000)		=> target position=1000
 * 							*The motion of motor is only based on the target position
 * Function Arguments	: 	*fh	, pointer to structure FaulHaber_t
 * 							pos	, value of relative target position
 * Function Return		: 	None
 * Function Example		: 	FH_PosRel(&fh1, 20000);
 */
void FH_PosRel(FaulHaber_t *fh, int pos){
	uint8_t temp[6]={0x0f, 0x00, 0, 0, 0, 0};
	FHmode(fh, PP);
	FH_Pvel(fh, fh->maxV);

	if(fh->PPflag){
		CAN_OPEN_TxMsg(fh->hcanx, PDO2Rx, fh->nodeID, temp, 6);		//set bit 4 to LOW (enable operation)
	}
	fh->PPflag = 1;
	temp[0]=0x7f;
	memcpy(&temp[2], &pos, 4);
	CAN_OPEN_TxMsg(fh->hcanx, PDO2Rx, fh->nodeID, temp, 6);
}

/*
 * Function Name		: 	FH_Vel
 * Function Description : 	Called to set target velocity
 * Function Remarks		: 	-
 * Function Arguments	: 	*fh	, pointer to structure FaulHaber_t
 * 							vel	, value of target velocity
 * Function Return		: 	None
 * Function Example		: 	FH_Vel(&fh1, 2000);
 */
void FH_Vel(FaulHaber_t *fh, int vel){
	FHmode(fh, PV);
	uint8_t temp[8]={0x0f, 0x00, 0, 0, 0, 0};
	memcpy(&temp[2], &vel, 4);
	CAN_OPEN_TxMsg(fh->hcanx, PDO3Rx, fh->nodeID, temp, 6);
}

/*
 * Function Name		: 	FH_StopM
 * Function Description : 	Stops the motor with specified deceleration (manual)
 * Function Remarks		: 	Sets motion controller into Quick Stop Active State
 * Function Arguments	: 	*fh	, pointer to structure FaulHaber_t
 * 							dec	, value of deceleration
 * Function Return		: 	None
 * Function Example		: 	FH_StopM(&fh1, 25);
 */
void FH_StopM(FaulHaber_t *fh, uint32_t dec){

	uint8_t temp[8]={ 0x2b, 0x5a, 0x60, 0, 0x06, 0, 0, 0 };
	if(!fh->brake)
		FH_SDO(fh, temp);	//set to user-set brake deceleration
	fh->brake=1;
	temp[0]=0x23;
	temp[1]=0x85;
	memcpy(&temp[4], &dec, 4);
	FH_SDO(fh, temp);		//set brake deceleration
	temp[0]=0x0b;
	temp[1]=0;
	CAN_OPEN_TxMsg(fh->hcanx, PDO1Rx, fh->nodeID, temp, 2);		//quick stop active state

}

/*
 * Function Name		: 	FH_StopA
 * Function Description : 	Stops the motor naturally (Voltage = 0)  (auto)
 * Function Remarks		: 	-
 * Function Arguments	: 	*fh	, pointer to structure FaulHaber_t
 * Function Return		: 	None
 * Function Example		: 	FH_StopM(&fh1);
 */
void FH_StopA(FaulHaber_t *fh){
	//this function was never tested. Please delete this comment if tested
	uint8_t temp[8]={ 0x2b, 0x5a, 0x60, 0, 0x08, 0, 0, 0 };
	if(fh->brake)
		FH_SDO(fh, temp);	//set to natural brake deceleration
	fh->brake=0;
	temp[0]=0x0b;
	temp[1]=0;
	CAN_OPEN_TxMsg(fh->hcanx, PDO1Rx, fh->nodeID, temp, 2);		//quick stop active state

}
/*
 * Function Name		: 	FH_target
 * Function Description : 	Called to check if target position is reached in PP mode
 * Function Remarks		: 	If you call FH_target() immediately after FH_PosRel/Abs(),
 * 							it will return 1 (True), have to wait for a little only
 * 							will it return 0 (False)
 * Function Arguments	: 	*fh	, pointer to structure FaulHaber_t
 * Function Return		: 	1(True) or 0(False)
 * Function Example		: 	FH_target(&fh1);
 */
uint8_t FH_target(FaulHaber_t *fh){
	FHEnq(fh, PDO1Tx);
	if(fh->target)
		return 1;
	else
		return 0;
}

/*
 * Function Name		: 	FH_Pvel
 * Function Description : 	Called to set Profile Velocity.
 * 							PP mode: runs motor at speed = Profile Velocity
 * 							PV mode: max motor speed = Profile Velocity
 * Function Remarks		: 	The value has to be positive value, or the command will be ignored
 * Function Arguments	: 	*fh	, pointer to structure FaulHaber_t
 * 							vel	, value of Profile Velocity
 * Function Return		: 	None
 * Function Example		: 	FH_Pvel(&fh1, 3000);
 */
void FH_Pvel(FaulHaber_t *fh, uint32_t vel){
	fh->maxV=vel;
	uint8_t temp[8]={ 0x23, 0x81, 0x60, 0x00, 0, 0, 0, 0 };
	memcpy(&temp[4], &vel, 4);
	FH_SDO(fh, temp);
}

void FH_acc(FaulHaber_t *fh, uint32_t acc){

	uint8_t temp[8]={ 0x23, 0x83, 0x60, 0, 0, 0, 0, 0 };
	memcpy(&temp[4], &acc, 4);
	FH_SDO(fh, temp);
}

/*
 * Function Name		: 	FH_Pdec
 * Function Description : 	Called to set Profile Deceleration.
 * Function Remarks		: 	The value has to be positive value, or the command will be ignored
 * Function Arguments	: 	*fh	, pointer to structure FaulHaber_t
 * 							dec	, value of Profile Deceleration (default value: 30000)
 * Function Return		: 	None
 * Function Example		: 	FH_Pdec(&fh1, 10000);
 */
void FH_Pdec(FaulHaber_t *fh, uint32_t dec){
	uint8_t temp[8]={ 0x23, 0x84, 0x60, 0, 0, 0, 0, 0 };
	memcpy(&temp[4], &dec, 4);
	FH_SDO(fh, temp);
}

/*
 * Function Name		: 	FH_Encoder_Reset
 * Function Description : 	Called to set FH absolute encoder to 0
 * Function Remarks		: 	The reset is temporary only as not safe configuration in controller. If restart controller, the value will change back to the value before reseting
 * 							Written by KZTam
 * Function Arguments	: 	*fh	, pointer to structure FaulHaber_t
 * Function Return		: 	None
 * Function Example		: 	FH_Encoder_Reset(&fh1);
 */
void FH_Encoder_Reset(FaulHaber_t *fh){
	uint8_t temp[8] = {0x23, 0x15, 0x23, 0x05, 0, 0, 0, 0};
	FH_SDO(fh, temp);
}

//function below not to be called by user.
/**********************************************************************************************************************/

/*
 * Function Name		: 	FH_COB_ID
 * Function Description : 	Set COB-ID for all PDOtx and PDOrx based on node ID, instead of wasting
 * 							5 minutes to set up those parameters one-by-one
 * 							But still need to set CAN rate in Motion Manager app
 * Function Remarks		: 	Written by KZTam
 * Function Arguments	: 	*fh	, pointer to structure FaulHaber_t
 * 							node_ID , node_ID of FaulHaber motor (node_ID should set in motion manager app)
 * Function Return		: 	None
 * Function Example		: 	FH_COB_ID(&fh1,49);
 */
void FH_COB_ID(FaulHaber_t* fh, uint8_t node_ID){
	uint8_t temp[8] = {0x23, 0x00, 0x14, 0x01, node_ID, 0x02, 0, 0};
	uint8_t i = 0;

	while(i<4){ //set COB-ID of PDO1rx to PDO4rx
		FH_SDO(fh, temp);
		temp[1]++;
		temp[5]++;
		i++;
	}
	temp[1] = 0x00;
	temp[2] = 0x18;
	temp[4] = 0x80 + node_ID;
	temp[5] = 0x01;
	for(i = 0;i<4; i++){ //set COB-ID of PDO1tx to PDO4tx
		FH_SDO(fh, temp);
		temp[1]++;
		temp[5]++;
	}
}

/*
 * Function Name		: 	FH_Pos_Limit
 * Function Description : 	Sets min and max Software Position Limit
 * Function Remarks		: 	-
 * Function Arguments	: 	*fh	, pointer to structure FaulHaber_t
 * 							max	, value of maximum Software Position Limit
 * 							min	, value of minimum Software Position Limit
 * Function Return		: 	None
 * Function Example		: 	FH_Pos_Limit(&fh1, 200000000, -200000000);
 */
void FH_Pos_Limit(FaulHaber_t *fh, int max, int min){
	fh->maxPos=max;
	fh->minPos=min;
	uint8_t temp[8]={0x23, 0x7d, 0x60, 0x02, 0, 0, 0, 0};
	int set = max + fh->offset;
	memcpy(&temp[4], &set, 4);
	FH_SDO(fh, temp);
	temp[3]=0x01;
	set = min + fh->offset;
	memcpy(&temp[4], &set, 4);
	FH_SDO(fh, temp);
}

/*
 * Function Name		: 	FHmode
 * Function Description : 	Called to set motion controller into specific mode
 * Function Remarks		: 	-
 * Function Arguments	: 	*fh	, pointer to structure FaulHaber_t
 * 							mode, mode of motion controller
 * Function Return		: 	None
 * Function Example		: 	FHmode(&fh1, PV);
 */
void FHmode(FaulHaber_t *fh, FH_OPmode mode){
	if(fh->mode != mode){
		uint8_t temp[8]={ 0x2f, 0x60, 0x60, 0x00, mode, 0x00, 0x00, 0x00 };
		FH_SDO(fh, temp);
	}

}

/*
 * Function Name		: 	FH_SDO
 * Function Description : 	Called to write SDO request
 * Function Remarks		: 	-
 * Function Arguments	: 	*fh		, pointer to structure FaulHaber_t
 * 							buff	, data to be written
 * Function Return		: 	None
 * Function Example		: 	FH_SDO(&fh1, temp);
 */
void FH_SDO(FaulHaber_t *fh, uint8_t buff[8]){
	fh->waiting=1;
	memcpy(fh->conf, &buff[1], 3);
	CAN_OPEN_TxMsg(fh->hcanx, SDORx, fh->nodeID, buff, 8);
	uint32_t ti=HAL_GetTick();
	while(fh->waiting){
		if(HAL_GetTick()-ti >= 25){
			CAN_OPEN_TxMsg(fh->hcanx, SDORx, fh->nodeID, buff, 8);
			ti=HAL_GetTick();
		}
	}
	fh->conf[0]=fh->conf[1]=fh->conf[2]=0;
}

/*
 * Function Name		: 	FH_CheckCAN
 * Function Description : 	Check if CAN Id received is one of the FaulHaber instructions
 * Function Remarks		: 	-
 * Function Arguments	: 	*fh	, pointer to structure FaulHaber_t
 * 							dat	, data received by CAN
 * Function Return		: 	0(FaulHaber ID found) or 1(not from FaulHaber)
 * Function Example		: 	FH_CheckCAN(&fh1, temp);
 */
uint8_t FH_CheckCAN(FaulHaber_t *fh, uint8_t dat[8]){
	uint32_t id;
	if(fh->hcanx==&hcan1){
		id=CAN1RxMessage.StdId;
	}else{
		id=CAN2RxMessage.StdId;
	}

	if((id - fh->nodeID) & 0b00001111111)
		return 1;
	switch((id)>>7){

		case 0b1110:		//boot up messege
			return 0;
			break;

		case 0x80:		//error messege
			return 0;
			break;

		case PDO1Tx:		//PDO1/statusword
			fh->target=(dat[1]>>2) & 0b01;		//target reached bit in PP mode
			memcpy(fh->FH_status,&dat[0],2);
			if(dat[0] == 0b01000000){				//switch on disabled bit
				fh->PDOInit=1;
				fh->start = 0;
			}
			else if(dat[0] == 0b00100111)
				fh->start=1;
			fh->waiting=0;
			return 0;
			break;

		case PDO2Tx:		//PDO2/position
			memcpy(fh->FH_status,&dat[0],2);
			memcpy(fh->rx_buff, &dat[2], 4);
			fh->rx-=fh->offset;
			fh->waiting=0;
			return 0;
			break;

		case PDO3Tx:		//PDO3/velocity
			memcpy(fh->FH_status,&dat[0],2);
			memcpy(fh->rx_buff, &dat[2], 4);
			fh->waiting=0;
			return 0;
			break;

		case PDO4Tx:		//PDO4/torque
			memcpy(fh->rx_buff, &dat[2], 4);//not sure if need to convert to int16, because datasheet says type is S16, not S32
			fh->waiting=0;
			return 0;
			break;

		case SDOTx:		//SDO
			memcpy(fh->SDOtx_dat,&dat[0], 8);
			if((dat[0]=0x60)&&(dat[1]==fh->conf[0])&&(dat[2]==fh->conf[1])&&(dat[3]==fh->conf[2]))
				fh->waiting=0;
			return 0;
			break;

		default :
			return 1;
			break;
	}
}

/*
 * Function Name		: 	FaulHaber_Handler
 * Function Description : 	Called to handle the received CAN data from FaulHaber.
 * Function Remarks		: 	Can be configured to return boolean value to indicate whether
 * 							the CAN data is from FaulHaber or not
 * Function Arguments	: 	dat	, data received by CAN
 * Function Return		: 	None
 * Function Example		: 	FaulHaber_Handler(aData);
 */
void FaulHaber_Handler(uint8_t dat[8]){

	uint8_t bflag=1, ad=0;
	while(bflag && pfh[ad]!=0)
		bflag=FH_CheckCAN(pfh[ad++], dat);

}

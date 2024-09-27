
/************************************************/
/*		 	 	Include Header	       		  	*/
/************************************************/
#include "PMW3901.h"

/************************************************/
/*		 	 	Variables	      	 		  	*/
/************************************************/


/************************************************/
/*		 	 	Functions		       		  	*/
/************************************************/

/*
 * Function Name		: PMW3901_SlaveInit
 * Function Description : This function is called to initialize PSx.
 * Function Arguments	: *psxbt	, Pointer to structure PSxBT_t
 * 						   hi2cx    , Pointer to I2C handle
 * Function Return		: None
 * Function Example		: PMW3901_SlaveInit(&ps4,&hi2c1);
 */
void PMW3901_SlaveInit(PMW3901_t *PMW3901,I2C_HandleTypeDef* hi2cx){
	PMW3901->hi2c = hi2cx;
	PMW3901->slave=1;
	I2CxInit(PMW3901->hi2c, main_board_1, CLOCK_SPEED_400KHz,ENABLE);
	HAL_I2C_Slave_Receive_IT(PMW3901->hi2c ,PMW3901->ReceiveBuffer, 6);
}



/*
 * Function Name		: PMW3901_SlaveHandler
 * Function Description : This function is called to handle the received PSx data from ESP32.
 * Function Remarks		: None
 * Function Arguments	: *psxbt	,pointer to structure PSxBT_t
 * Function Return		: None
 * Function Example		: PSx_SlaveHandler(&ps4);
 */

void PMW3901_SlaveHandler(PMW3901_t *PMW3901){
	PMW3901->disconnected=0;
	if(PMW3901->ReceiveBuffer[0]=='E' && PMW3901->ReceiveBuffer[5]=='@'){
		PMW3901->input_x = (int16_t)((PMW3901->ReceiveBuffer[2] << 8) | PMW3901->ReceiveBuffer[1]);  // Combine the upper and lower bytes of deltaX
		PMW3901->input_y = (int16_t)((PMW3901->ReceiveBuffer[4] << 8) | PMW3901->ReceiveBuffer[3]);  // Combine the upper and lower bytes of deltaY
		PMW3901->x +=PMW3901->input_x/1000.0*30.0;
		PMW3901->y +=PMW3901->input_y/1000.0*30.0;
	}
	HAL_I2C_Slave_Receive_IT(PMW3901->hi2c ,PMW3901->ReceiveBuffer, 6);
}

/*
 * Function Name		: PSxConnectionHandler
 * Function Description : This function is called to handle disconnection.
 * Function Remarks		: Called in 20ms timer interrupt
 * Function Arguments	: *psxbt	,pointer to structure PSxBT_t
 * Function Return		: None
 * Function Example		: PSxConnectionHandler(&ps4);
 */
void PMW3901_ConnectionHandler(PMW3901_t *PMW3901) {
	if (PMW3901->slave) {
		if (PMW3901->disconnected) {
			PMW3901->input_x = 0;
			PMW3901->input_y = 0;
			HAL_I2C_DeInit(PMW3901->hi2c);
			I2CxInit(PMW3901->hi2c, main_board_1, CLOCK_SPEED_400KHz, ENABLE);
			HAL_I2C_Slave_Receive_IT(PMW3901->hi2c,PMW3901->ReceiveBuffer, 6);
		}
		PMW3901->disconnected = 1;
	}
}



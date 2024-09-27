/*********************************************/
/*          Include Header                   */
/*********************************************/

#include "common.h"


uint8_t blockturn = 0;

void set(void) {

	sys.flag16=0;
	KalmanFilter_Init(&enc1.Angle, &Filtered_Angle, 1.0, 1.0, 0.001, 1.0, 1.0, &KF);
//	watchdoginit(4, 4);   // use the watchdog refresh inside 5ms loop
	Initialize();
	PSxSlaveInit(&ps4, &hi2c1);
//	PMW3901_SlaveInit(&enc, &hi2c2);
//	Modbus_Init(&modbus, &huart4);
//	TIMxInit(&htim6, 50, 84);			// 50us use for SoftPWM
	TIMxInit(&htim7, 5000, 84);			// 5ms
	TIMxInit(&htim9, 65535, 74);
	ExtixInit(GPIO_PIN_0, 9, 0,&ExtiPin);


	/***NAVI***/

	//	rns.init = 0;
	void RNS_config(CAN_HandleTypeDef* hcanx) {
		//RNSInit(hcanx, &rns);
		//Encoder dcba(0-swap, 1-keep)  BDC dcba(0-keep, 1-swap) //0x00 0x00 0x
		RNSSet(&rns, RNS_DEVICE_CONFIG, (float) 0b00100101, (float) fwd_omni, (float) roboconPID);
		RNSSet(&rns, RNS_X_Y_ENC_CONFIG, 0.125 / 4000 * 3.142, 1.0, 0.125 / 4000 * 3.142, 1.0); //1.0 for nonswap , 2.0 for swap
		RNSSet(&rns, RNS_F_KCD_PTD, 203.20885/ 204.50492, (float)(0.125 * 3.142 / 203.20885));
		RNSSet(&rns, RNS_B_KCD_PTD, 203.56232/ 203.60160, (float)(0.125 * 3.142 / 203.56232));

		RNSSet(&rns, RNS_F_LEFT_VEL_SATEU, 1.0, 1.0 / 17.9120, 19999.0);
		RNSSet(&rns, RNS_F_RIGHT_VEL_SATEU, 1.0, 1.0 / 20.7897, 19999.0);
		RNSSet(&rns, RNS_B_LEFT_VEL_SATEU, 1.0, 1.0 / 18.3077, 19999.0);
		RNSSet(&rns, RNS_B_RIGHT_VEL_SATEU, 1.0, 1.0 / 18.7605, 19999.0);

		RNSSet(&rns, RNS_F_LEFT_VEL_PID,  5.0, 3.86, 0.0);
		RNSSet(&rns, RNS_F_RIGHT_VEL_PID, 4.6, 3.13, 0.0);
		RNSSet(&rns, RNS_B_LEFT_VEL_PID,  4.78, 3.32, 0.0);
		RNSSet(&rns, RNS_B_RIGHT_VEL_PID, 5.03, 3.50, 0.0);

		RNSSet(&rns, RNS_F_LEFT_VEL_FUZZY_PID_BASE, 0.2, 0.2, 0.2);
		RNSSet(&rns, RNS_F_LEFT_VEL_FUZZY_PID_PARAM, 0.02, 0.02, 0.02);

		RNSSet(&rns, RNS_PPInit); //Path Planning
		RNSSet(&rns, RNS_PPPathPID, 1.0, 0.5, 0.5);
		RNSSet(&rns, RNS_PPEndPID, 0.5, 0.1, 0.7);
		RNSSet(&rns, RNS_PPZPID, 1.0, 0.05, 0.2, 5.5);
		RNSSet(&rns, RNS_PPSetCRV_PTS, 10.0);         // Change No. of Points in the Curved Path
	}

//	MODNRobotBaseVelInit(MODN_FWD_OMNI, 0.47, 0.47, &modn);
//	MODNRobotConInit(&x_vel, &y_vel, &w_vel, &modn);
//	MODNWheelVelInit(&v2, &v1, &v3, &v4, &modn);
//	MODNWheelDirInit(&d2, &d1, &d3, &d4, &modn);


}



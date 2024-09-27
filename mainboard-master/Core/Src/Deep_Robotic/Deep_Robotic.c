/*
 * Deep_Robotic.c
 *
 *  Created on: Sep 15, 2024
 *      Author: clement
 */


#include "Deep_Robotic.h"

#define PI 3.1415926

void J60_init(J60_t *j60, CAN_HandleTypeDef *hcan, uint8_t *JointIDs, int JointNumber){
	j60->hcan = hcan;
	j60->totalJoint = JointNumber;
	for (int i=0; i<j60->totalJoint; i++){
		j60->J60_motor[i].motor_id_ = JointIDs[i];
	}
}

void J60_disable(J60_t *j60, int JointIndex){
	CAN_ID = (0x01<<5) + j60->J60_motor[JointIndex].motor_id_;
	CAN_TxRTR(j60->hcan, CAN_ID);
}

void J60_enable(J60_t *j60, int JointIndex){
	CAN_ID = (0x02<<5) + j60->J60_motor[JointIndex].motor_id_;
	CAN_TxRTR(j60->hcan, CAN_ID);
}

uint32_t FloatToUint(const float x, const float x_min, const float x_max, const uint8_t bits){
    /// Converts a float to an unsigned int, given range and number of bits ///
    float span = x_max - x_min;
    float offset = x_min;
    return (uint32_t)((x-offset)*((float)((1<<bits)-1))/span);
}
float UintToFloat(const int x_int, const float x_min, const float x_max, const uint8_t bits){
    /// converts unsigned int to float, given range and number of bits ///
    float span = x_max - x_min;
    float offset = x_min;
    return ((float)x_int)*span/((float)((1<<bits)-1)) + offset;
}


void J60_motion_control(J60_t *j60, int JointIndex, float pos, float vel, float torq, int Kp, float Kd){
	CAN_ID = (0x04<<5) + j60->J60_motor[JointIndex].motor_id_;

	j60->J60_motor[JointIndex].position_ = pos;
	j60->J60_motor[JointIndex].velocity_ = vel;
	j60->J60_motor[JointIndex].torque_ = torq;
	j60->J60_motor[JointIndex].kd_ = Kd;
	j60->J60_motor[JointIndex].kp_ = Kp;

    uint16_t _position = FloatToUint(j60->J60_motor[JointIndex].position_ , POSITION_MIN, POSITION_MAX, SEND_POSITION_LENGTH);
    uint16_t _velocity = FloatToUint(j60->J60_motor[JointIndex].velocity_, VELOCITY_MIN, VELOCITY_MAX, SEND_VELOCITY_LENGTH);
    uint16_t _torque = FloatToUint(j60->J60_motor[JointIndex].torque_ , TORQUE_MIN, TORQUE_MAX, SEND_TORQUE_LENGTH);
    uint16_t _kp = FloatToUint(j60->J60_motor[JointIndex].kp_, J60KP_MIN, J60KP_MAX, SEND_KP_LENGTH);
    uint16_t _kd = FloatToUint(j60->J60_motor[JointIndex].kd_ , J60KD_MIN, J60KD_MAX, SEND_KD_LENGTH);
    j60->CANtxdata[0] = _position;
    j60->CANtxdata[1] = _position >> 8;
    j60->CANtxdata[2] = _velocity;
    j60->CANtxdata[3] = ((_velocity >> 8) & 0x3f)| ((_kp & 0x03) << 6);
    j60->CANtxdata[4] = _kp >> 2;
    j60->CANtxdata[5] = _kd;
    j60->CANtxdata[6] = _torque;
    j60->CANtxdata[7] = _torque >> 8;

	CAN_TxMsg(&hcan1, CAN_ID, j60->CANtxdata, 8);
}

uint8_t J60_CANHandler(CAN_RxHeaderTypeDef *pRxMsg, uint8_t data[8]){

	uint8_t idbuff = (pRxMsg->StdId >> 5) & 0x3f;
	uint8_t canid = pRxMsg->StdId & 0x0f;

	switch (idbuff) {

	case ENABLE_MOTOR:
		for (int i = 0; i < j60.totalJoint; i++) {
			if (j60.J60_motor[i].motor_id_ == canid) {
				if (data[0] == 0x01){
					j60.J60_status[i].enable = 1;
				}else{
					j60.J60_status[i].enable = 0;
				}
			}
		}
		break;

	case DISABLE_MOTOR:
		for (int i = 0; i < j60.totalJoint; i++) {
			if (j60.J60_motor[i].motor_id_ == canid) {
				if (data[0] == 0x01) {
					j60.J60_status[i].enable = 0;
				}else{
					j60.J60_status[i].enable = 1;
				}
			}
		}
		break;

	case SET_HOME:

		break;

	case ERROR_RESET:

		break;

	case CONTROL_MOTOR:

		for (int i = 0; i < j60.totalJoint; i++) {
			if (j60.J60_motor[i].motor_id_ == canid) {
			    const ReceivedMotionData *pcan_data = (const ReceivedMotionData*)data;
				j60.J60_status[i].position_ = UintToFloat(pcan_data->position, POSITION_MIN, POSITION_MAX, RECEIVE_POSITION_LENGTH);
				j60.J60_status[i].velocity_ = UintToFloat(pcan_data->velocity, VELOCITY_MIN, VELOCITY_MAX, RECEIVE_VELOCITY_LENGTH);
				j60.J60_status[i].torque_ = UintToFloat(pcan_data->torque, TORQUE_MIN, TORQUE_MAX, RECEIVE_TORQUE_LENGTH);
				j60.J60_status[i].flag_ = (bool)pcan_data->temp_flag;
				if (j60.J60_status[i].flag_ == kMotorTempFlag) {
					j60.J60_status[i].temp_ = UintToFloat(pcan_data->temperature, MOTOR_TEMP_MIN, MOTOR_TEMP_MAX, RECEIVE_TEMP_LENGTH);
				} else {
					j60.J60_status[i].temp_ = UintToFloat(pcan_data->temperature, DRIVER_TEMP_MIN, DRIVER_TEMP_MAX, RECEIVE_TEMP_LENGTH);
				}
			}
		}
		break;

	case GET_STATUS_WORD:
		break;

	default:
		return 0;
	}

	return 1;
}


void J60_rotation_motion_ctrl(J60_t *j60, int JointIndex, float pos, float vel, float torq, int Kp, float Kd){
	pos = pos * (2.0 * PI);
	vel = vel * PI / 30.0;
	J60_motion_control(j60, JointIndex, pos, vel, torq, Kp, Kd);
}

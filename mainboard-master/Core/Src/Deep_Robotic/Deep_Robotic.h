/*
 * Deep_Robotic.h
 *
 *  Created on: Sep 15, 2024
 *      Author: clement
 */

#ifndef SRC_DEEP_ROBOTIC_DEEP_ROBOTIC_H_
#define SRC_DEEP_ROBOTIC_DEEP_ROBOTIC_H_

#include "../BIOS/system.h"
#include "../CAN/can.h"
#include "can_protocol.h"


typedef struct
{
    uint8_t motor_id_;
    uint8_t cmd_;
    float position_;
    float velocity_;
    float torque_;
    bool flag_;
    float temp_;
    uint16_t error_;
    uint8_t enable;
}MotorDATA;

typedef struct
{
    uint8_t motor_id_;
    uint8_t cmd_;
    float position_;
    float velocity_;
    float torque_;
    float kp_;
    float kd_;
}MotorCMD;

typedef struct{
	CAN_RxHeaderTypeDef *pRxMsg;
	CAN_HandleTypeDef *hcan;
	int totalJoint;
	MotorCMD J60_motor[15]; //15 MAX
	MotorDATA J60_status[15];
	uint8_t CANtxdata[8];
	uint64_t CANbuff;
}J60_t;

J60_t j60;

uint16_t CAN_ID;

void J60_init(J60_t *j60, CAN_HandleTypeDef *hcan, uint8_t *JointIDs, int JointNumber);
void J60_disable(J60_t *j60, int JointIndex);
uint8_t J60_CANHandler(CAN_RxHeaderTypeDef *pRxMsg, uint8_t data[8]);
void J60_enable(J60_t *j60, int JointIndex);
void J60_motion_control(J60_t *j60, int JointIndex, float pos, float vel, float torq, int Kp, float Kd);
uint32_t FloatToUint(const float x, const float x_min, const float x_max, const uint8_t bits);
float UintToFloat(const int x_int, const float x_min, const float x_max, const uint8_t bits);
void J60_rotation_motion_ctrl(J60_t *j60, int JointIndex, float pos, float vel, float torq, int Kp, float Kd);

#endif /* SRC_DEEP_ROBOTIC_DEEP_ROBOTIC_H_ */

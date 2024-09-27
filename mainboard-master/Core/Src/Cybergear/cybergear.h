/*
 * cybergear_driver.h
 *
 *  Created on: May 1, 2024
 *      Author: yvc
 */

#ifndef SRC_CYBERGEAR_CYBERGEAR_H_
#define SRC_CYBERGEAR_CYBERGEAR_H_

#include "../BIOS/system.h"
#include "../CAN/can.h"
#include "../Cybergear/cybergear_defs.h"

typedef struct {
    uint8_t encoder_not_calibrated;
    uint8_t over_current_phase_a;
    uint8_t over_current_phase_b;
    uint8_t over_voltage;
    uint8_t under_voltage;
    uint8_t driver_chip;
    uint8_t motor_over_tempareture;
} MotorFault;

typedef struct {
    uint16_t raw_position;
    uint16_t raw_velocity;
    uint16_t raw_torque;
    uint16_t raw_temperature;
    float position;
    float velocity;
    float torque;
    float temperature;
    uint8_t motor_id;
    uint32_t stamp_usec;
    uint8_t motor_update_flag;
} MotorStatus;

typedef struct {
    uint8_t run_mode;
    float iq_ref;
    float spd_ref;
    float limit_torque;
    float cur_kp;
    float cur_ki;
    float cur_filt_gain;
    float loc_ref;
    float limit_spd;
    float limit_cur;
    float mech_pos;
    float iqf;
    float mech_vel;
    float vbus;
    int16_t rotation;
    float loc_kp;
    float spd_kp;
    float spd_ki;
    uint32_t stamp_usec;
} MotorParam;

typedef struct {
	CAN_HandleTypeDef *hcan;
    uint8_t master_can_id;
    uint8_t target_can_id;
    uint8_t run_mode;
    uint32_t send_count;
    MotorStatus motor_status;
    MotorParam motor_param;
    MotorFault motor_fault;
    CAN_RxHeaderTypeDef pRxMsg;
	CAN_TxHeaderTypeDef pTxMsg;
    uint8_t rxdata[8];
    uint8_t txdata[8];
    uint8_t disconnected;
    uint8_t enable;
    float pos_offset;
} CybergearDriver;

#define MAX_MOTORS 8

typedef struct {
	uint8_t masterID;
    CybergearDriver drivers[MAX_MOTORS];
    uint8_t numMotors;
} Cybergear;

Cybergear cybergear;

uint8_t Cybergear_init(Cybergear* cybergear, CAN_HandleTypeDef *hcan, uint8_t masterID, uint8_t* motorIDs, uint8_t numMotors);
uint8_t Cybergear_enable(Cybergear* cybergear);
void CybergearDriver_enable_motor(CybergearDriver *driver);
uint8_t Cybergear_disable(Cybergear* cybergear);
void CybergearDriver_disable_motor(CybergearDriver *driver);
void CybergearDriver_clear_error(CybergearDriver *driver);
uint8_t Cybergear_set_mech_pos_zaro_all(Cybergear* cybergear);
uint8_t Cybergear_set_mech_pos_zaro(Cybergear* cybergear, uint8_t motorIndex);
void CybergearDriver_set_mech_pos_zaro(CybergearDriver *driver);
uint8_t Cybergear_set_run_mode(Cybergear* cybergear, uint8_t motorIndex, uint8_t mode);
uint8_t Cybergear_set_run_mode_all(Cybergear* cybergear, uint8_t mode);
//uint8_t Cybergear_setSpeed(Cybergear* cybergear, uint8_t motorIndex, float speed);
//uint8_t Cybergear_setPosition(Cybergear* cybergear, uint8_t motorIndex, float position);
uint8_t Cybergear_send_motion_command(Cybergear* cybergear, uint8_t motorIndex, float pos, float vel, float torque, float kp, float kd);

void Cybergear_set_limit_torque(Cybergear *cybergear, uint8_t motorindex, float torque);
void Cybergear_set_current_kp(Cybergear *cybergear, uint8_t motorindex, float cur_kp);
void Cybergear_set_current_ki(Cybergear *cybergear, uint8_t motorindex, float cur_ki);
void Cybergear_set_current_filter_gain(Cybergear *cybergear, uint8_t motorindex, float cur_filter_gain);
void Cybergear_set_position_ref(Cybergear *cybergear, uint8_t motorindex, float pos_ref);
void Cybergear_set_limit_speed(Cybergear *cybergear, uint8_t motorindex, float speed);
void Cybergear_set_limit_current(Cybergear *cybergear, uint8_t motorindex, float current);
void Cybergear_set_loc_kp(Cybergear *cybergear, uint8_t motorindex, float loc_kp);
void Cybergear_set_spd_kp(Cybergear *cybergear, uint8_t motorindex, float spd_kp);
void Cybergear_set_spd_ki(Cybergear *cybergear, uint8_t motorindex, float spd_ki);
uint8_t Cybergear_write_param(Cybergear* cybergear, uint8_t motorIndex, uint16_t addr, float value);

void CybergearDriver_init(CybergearDriver *driver, CAN_HandleTypeDef *can, uint8_t master_can_id, uint8_t target_can_id);
void CybergearDriver_set_run_mode(CybergearDriver* driver, uint8_t runMode);
void CybergearDriver_set_control(CybergearDriver* driver, float position, float speed, float torque, float kp, float kd);

//void CybergearDriver_set_limit_speed(CybergearDriver *driver, float speed);
//void CybergearDriver_set_limit_current(CybergearDriver *driver, float current);
//void CybergearDriver_set_current_kp(CybergearDriver *driver, float kp);
//void CybergearDriver_set_current_ki(CybergearDriver *driver, float ki);
//void CybergearDriver_set_current_filter_gain(CybergearDriver *driver, float gain);
//void CybergearDriver_set_limit_torque(CybergearDriver *driver, float torque);
//void CybergearDriver_set_position_kp(CybergearDriver *driver, float kp);
//void CybergearDriver_set_velocity_kp(CybergearDriver *driver, float kp);
//void CybergearDriver_set_velocity_ki(CybergearDriver *driver, float ki);
//void CybergearDriver_get_mech_position(CybergearDriver *driver);
//void CybergearDriver_get_mech_velocity(CybergearDriver *driver);
//void CybergearDriver_get_vbus(CybergearDriver *driver);
//void CybergearDriver_get_rotation(CybergearDriver *driver);
//void CybergearDriver_dump_motor_param(CybergearDriver *driver);
//void CybergearDriver_set_position_ref(CybergearDriver *driver, float position);
//void CybergearDriver_set_speed_ref(CybergearDriver *driver, float speed);
//void CybergearDriver_set_current_ref(CybergearDriver *driver, float current);
//void CybergearDriver_set_mech_position_to_zero(CybergearDriver *driver);
//void CybergearDriver_change_motor_can_id(CybergearDriver *driver, uint8_t can_id);
//void CybergearDriver_read_ram_data(CybergearDriver *driver, uint16_t index);
//uint8_t CybergearDriver_get_run_mode(CybergearDriver *driver);
//uint8_t CybergearDriver_get_motor_id(CybergearDriver *driver);

void CybergearDriver_send_command(CybergearDriver *driver, uint8_t cmd_id, uint16_t option);
uint8_t Cybergear_CAN_Handler(CAN_RxHeaderTypeDef *pRxMsg, uint8_t rxdata[]);
uint8_t CybergearDriver_update_motor_status(CybergearDriver *driver);
uint8_t CybergearDriver_connection_check(CybergearDriver *driver);
uint8_t CybergearDriver_process_motor_packet(CybergearDriver *driver);
void CybergearDriver_read_param(CybergearDriver *driver, uint16_t addr);
void CybergearDriver_process_read_parameter_packet(CybergearDriver *driver);
void CybergearDriver_write_param_float(CybergearDriver *driver, uint16_t addr, float value);

//unsigned long CybergearDriver_get_send_count(CybergearDriver *driver);

#endif /* SRC_CYBERGEAR_CYBERGEAR_H_ */

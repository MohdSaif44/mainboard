/*
 * cybergear_driver.c
 *
 *  Created on: May 1, 2024
 *      Author: yvc
 */


#include "../Cybergear/cybergear.h"
#include "../adapter.h"

static int float_to_uint(float x, float min, float max, int bits)
{
    float span = max - min;
    x = (x < min) ? min : ((x > max) ? max : x);
    return (int)((x - min) * ((1 << bits) - 1) / span);
}

static float uint_to_float(uint16_t x, float min, float max)
{
    return min + (float)x * (max - min) / 0xFFFF;
}
uint8_t Cybergear_init(Cybergear* cybergear, CAN_HandleTypeDef *hcan, uint8_t masterID, uint8_t* motorIDs, uint8_t numMotors) {
    if (numMotors > MAX_MOTORS) {
        return false; // Maximum of MAX_MOTORS motors supported
    }

    cybergear->masterID = masterID;
    cybergear->numMotors = numMotors;

    for (int i = 0; i < numMotors; i++) {
        CybergearDriver_init(&cybergear->drivers[i], hcan, masterID, motorIDs[i]);
    }

    return true;
}

uint8_t Cybergear_enable(Cybergear* cybergear) {
    // Enable the master and all associated motors
    for (int i = 0; i < cybergear->numMotors; i++) {
        CybergearDriver_enable_motor(&cybergear->drivers[i]);
    }
    return true;
}

void CybergearDriver_enable_motor(CybergearDriver *driver)
{
	driver->enable = 1;
	memset(&driver->txdata, 0, 8);
    CybergearDriver_send_command(driver, CMD_ENABLE, driver->master_can_id);
}

uint8_t Cybergear_disable(Cybergear* cybergear) {
    // Disable the master and all associated motors
    for (int i = 0; i < cybergear->numMotors; i++) {
    	CybergearDriver_disable_motor(&cybergear->drivers[i]);
    	CybergearDriver_clear_error(&cybergear->drivers[i]);
    }
    return true;
}

void CybergearDriver_disable_motor(CybergearDriver *driver)
{
	driver->enable = 0;
	memset(&driver->txdata, 0, 8);
    CybergearDriver_send_command(driver, CMD_DISABLE, driver->master_can_id);
}

void CybergearDriver_clear_error(CybergearDriver *driver)
{
	memset(&driver->txdata, 0, 8);
	driver->txdata[0] = 1;
    CybergearDriver_send_command(driver, CMD_DISABLE, driver->master_can_id);
}

uint8_t Cybergear_set_mech_pos_zaro_all(Cybergear* cybergear){
	for (int i = 0; i < cybergear->numMotors; i++) {
		CybergearDriver_set_mech_pos_zaro(&cybergear->drivers[i]);
	}
    return true;
}

uint8_t Cybergear_set_mech_pos_zaro(Cybergear* cybergear, uint8_t motorIndex){
	if (motorIndex < 0 || motorIndex >= cybergear->numMotors) {
		return false; // Invalid motor index
	}
	CybergearDriver_set_mech_pos_zaro(&cybergear->drivers[motorIndex]);
    return true;
}

void CybergearDriver_set_mech_pos_zaro(CybergearDriver *driver)
{
	memset(&driver->txdata, 0, 8);
	driver->txdata[0] = 1;
    CybergearDriver_send_command(driver, CMD_SET_MECH_POSITION_TO_ZERO, driver->master_can_id);
}

uint8_t Cybergear_set_run_mode(Cybergear* cybergear, uint8_t motorIndex, uint8_t mode) {
	if (motorIndex < 0 || motorIndex >= cybergear->numMotors) {
		return false; // Invalid motor index
	}
	CybergearDriver_set_run_mode(&cybergear->drivers[motorIndex], mode);
    return true;
}

uint8_t Cybergear_set_run_mode_all(Cybergear* cybergear, uint8_t mode) {
    for (uint8_t idx = 0; idx < cybergear->numMotors; ++idx) {
    	CybergearDriver_set_run_mode(&cybergear->drivers[idx], mode);
    }
    return true;
}

uint8_t Cybergear_send_motion_command(Cybergear* cybergear, uint8_t motorIndex, float pos, float vel, float torque, float kp, float kd) {
    if (motorIndex < 0 || motorIndex >= cybergear->numMotors) {
        return false; // Invalid motor index
    }
	CybergearDriver_set_control(&cybergear->drivers[motorIndex], pos, vel, torque, kp, kd);
	return true;
}

uint8_t Cybergear_write_param(Cybergear* cybergear, uint8_t motorIndex, uint16_t addr, float value) {
	if (motorIndex < 0 || motorIndex >= cybergear->numMotors) {
		return false; // Invalid motor index
	}
	CybergearDriver_write_param_float(&cybergear->drivers[motorIndex], addr, value);
	return true;
}

//uint8_t Cybergear_setSpeed(Cybergear* cybergear, int motorIndex, int speed) {
//    if (motorIndex < 0 || motorIndex >= cybergear->numMotors) {
//        return false; // Invalid motor index
//    }
//
//    return CybergearDriver_setSpeed(&cybergear->drivers[motorIndex], speed);
//}
//
//uint8_t Cybergear_setPosition(Cybergear* cybergear, int motorIndex, int position) {
//    if (motorIndex < 0 || motorIndex >= cybergear->numMotors) {
//        return false; // Invalid motor index
//    }
//
//    return CybergearDriver_setPosition(&cybergear->drivers[motorIndex], position);
//}

void CybergearDriver_init(CybergearDriver *driver, CAN_HandleTypeDef *hcan, uint8_t master_can_id, uint8_t target_can_id) {
    driver->hcan = hcan;
    driver->master_can_id = master_can_id;
    driver->target_can_id = target_can_id;
    driver->send_count = 0;
    memset(&driver->txdata, 0, 8);
    memset(&driver->motor_status, 0, sizeof(MotorStatus));
    memset(&driver->motor_param, 0, sizeof(MotorParam));
    memset(&driver->motor_fault, 0, sizeof(MotorFault));
}

void CybergearDriver_set_run_mode(CybergearDriver* driver, uint8_t runMode) {
    // Set the class variable
    driver->run_mode = runMode;

    uint8_t data[8] = {0x00};
    data[0] = ADDR_RUN_MODE & 0x00FF;
    data[1] = ADDR_RUN_MODE >> 8;
    data[4] = runMode;
    memcpy(&driver->txdata, data, 8);
    CybergearDriver_send_command(driver, CMD_RAM_WRITE, driver->master_can_id);
}

void CybergearDriver_set_control(CybergearDriver* driver, float position, float speed, float torque, float kp, float kd)
{
	if (!driver->enable) {
		return;
	}
	position -= driver->pos_offset;
	position *= 2*PI; // rot to rad
	speed *= 2*PI/60.0; // rpm to rad/s
	uint8_t data[8] = {0x00};
	data[0] = float_to_uint(position, P_MIN, P_MAX, 16) >> 8;
	data[1] = float_to_uint(position, P_MIN, P_MAX, 16);
	data[2] = float_to_uint(speed, V_MIN, V_MAX, 16) >> 8;
	data[3] = float_to_uint(speed, V_MIN, V_MAX, 16);
	data[4] = float_to_uint(kp, KP_MIN, KP_MAX, 16) >> 8;
	data[5] = float_to_uint(kp, KP_MIN, KP_MAX, 16);
	data[6] = float_to_uint(kd, KD_MIN, KD_MAX, 16) >> 8;
	data[7] = float_to_uint(kd, KD_MIN, KD_MAX, 16);
	memcpy(&driver->txdata, data, 8);
	uint16_t data_torque = float_to_uint(torque, T_MIN, T_MAX, 16);
	CybergearDriver_send_command(driver, CMD_CONTROL, data_torque);
}

void CybergearDriver_send_command(CybergearDriver *driver, uint8_t cmd_id, uint16_t option) {
    uint32_t id = (cmd_id << 24) | (option << 8) | driver->target_can_id;
    driver->motor_status.motor_update_flag = 0;
    driver->pTxMsg.ExtId = id;
    CAN_TxMsgEID(driver->hcan, id, driver->txdata, 8);
    driver->send_count++;
//    osDelay(1);
    uint32_t time_sent = HAL_GetTick();
    while(!driver->motor_status.motor_update_flag && (HAL_GetTick()-time_sent <= 3)){
//    	if (HAL_GetTick()-time_sent > 5){
//    	    CAN_TxMsgEID(driver->hcan, id, driver->txdata, 8);
//    	    time_sent = HAL_GetTick();
//    	}
    }
//	CAN_TxHeaderTypeDef CAN_TxHeader_MI;
//    CAN_TxHeader_MI.DLC = 8;
//    CAN_TxHeader_MI.IDE = CAN_ID_EXT;
//    CAN_TxHeader_MI.RTR = CAN_RTR_DATA;
//    CAN_TxHeader_MI.ExtId = id;
//    uint32_t mailbox;
//    /* Start the Transmission process */
//    uint32_t ret = HAL_CAN_AddTxMessage(driver->hcan, &CAN_TxHeader_MI, driver->txdata, &mailbox);
//    if (ret != HAL_OK) {
//        /* Transmission request Error */
//        while(1);
//    }
}

uint8_t Cybergear_CAN_Handler(CAN_RxHeaderTypeDef *pRxMsg, uint8_t rxdata[]) {
    uint32_t id = pRxMsg->ExtId;
//    uint8_t receive_can_id = id & 0xFF;
//    if (receive_can_id == cybergear.masterID) {

    	uint8_t motor_can_id = (id & 0xFF00) >> 8;
		for (uint8_t i = 0; i < cybergear.numMotors; i++) {
			if (motor_can_id == cybergear.drivers[i].target_can_id){
//				led3 = !led3;
				cybergear.drivers[i].motor_status.motor_update_flag = 1;
				memcpy(&cybergear.drivers[i].pRxMsg, pRxMsg, sizeof(CAN_RxHeaderTypeDef));
				memcpy(&cybergear.drivers[i].rxdata, rxdata, 8);
				return CybergearDriver_update_motor_status(&cybergear.drivers[i]);
			}
		}
		int message_len = 0;
		sprintf(data, "Cybergear Invalid target can id. Expected=[");
		for (int i = 0; i < cybergear.numMotors; i++) {
		    message_len += sprintf(data + message_len, "0x%02x", cybergear.drivers[i].target_can_id);
		    if (i < cybergear.numMotors - 1) {
		        message_len += sprintf(data + message_len, ", ");
		    }
		}
		sprintf(data + message_len, "] Actual=[0x%02x] Raw=[%lx]\n", motor_can_id, id);
//    }
	return false;
}

uint8_t CybergearDriver_update_motor_status(CybergearDriver *driver) {
	uint32_t id = driver->pRxMsg.ExtId;
//    uint8_t receive_can_id = id & 0xff;
//    if (receive_can_id != driver->master_can_id) {
//        return false;
//    }

    uint8_t motor_can_id = (id & 0xff00) >> 8;
    if (motor_can_id != driver->target_can_id) {
        return false;
    }
    // check packet type
    uint8_t packet_type = id >> 24;
    if (packet_type == CMD_RESPONSE) {
    	CybergearDriver_process_motor_packet(driver);
    } else if (packet_type == CMD_RAM_READ) {
    	CybergearDriver_process_read_parameter_packet(driver);
    } else if (packet_type == CMD_GET_MOTOR_FAIL) {
        // NOT IMPLEMENTED
    	sprintf(data, "CMD_GET_MOTOR_FAIL\n");
    } else {
        sprintf(data, "Cybergear invalid command response [0x%x]\n", packet_type);
        return false;
    }

    return true;
}

uint8_t CybergearDriver_connection_check(CybergearDriver *driver){
	if (!driver->motor_status.motor_update_flag){
		if (HAL_GetTick() - driver->motor_status.stamp_usec > 100){
			return true;
		}
	}
	return false;
}

uint8_t CybergearDriver_process_motor_packet(CybergearDriver *driver) {
	if (driver->pRxMsg.DLC != 8)
		return false;

	uint8_t data[8];
	memcpy(&data, driver->rxdata, 8);

    driver->motor_status.raw_position = (data[1] << 8) | data[0];
    driver->motor_status.raw_velocity = (data[3] << 8) | data[2];
    driver->motor_status.raw_torque = (data[5] << 8) | data[4];
    driver->motor_status.raw_temperature = (data[7] << 8) | data[6];

    driver->motor_status.stamp_usec = HAL_GetTick();
    driver->motor_status.motor_id = driver->target_can_id;
    driver->motor_status.position = uint_to_float(driver->motor_status.raw_position, P_MIN, P_MAX);
    driver->motor_status.velocity = uint_to_float(driver->motor_status.raw_velocity, V_MIN, V_MAX);
    driver->motor_status.torque = uint_to_float(driver->motor_status.raw_torque, T_MIN, T_MAX);
    driver->motor_status.temperature = driver->motor_status.raw_temperature ;

    return true;
}

void CybergearDriver_read_param(CybergearDriver *driver, uint16_t addr) {

	memset(&driver->txdata, 0, 8);
	driver->txdata[0] = addr & 0x00FF;
	driver->txdata[1] = addr >> 8;

	CybergearDriver_send_command(driver, CMD_RAM_READ, driver->master_can_id);
}

void CybergearDriver_process_read_parameter_packet(CybergearDriver *driver) {
    uint16_t index = (driver->rxdata[1] << 8) | driver->rxdata[0];
    uint8_t is_updated = true;
//    led5 = !led5;
    uint8_t rxdata[8];
//    uint8_t data[50];
	memcpy(&rxdata, driver->rxdata, 8);
    switch (index) {
        case ADDR_RUN_MODE:
            driver->motor_param.run_mode = rxdata[4];
            sprintf(data, "Receive ADDR_RUN_MODE = [0x%02x]\n", rxdata[4]);
            break;
        case ADDR_IQ_REF:
            memcpy(&driver->motor_param.iq_ref, &rxdata[4], sizeof(float));
            sprintf(data, "Receive ADDR_IQ_REF = [%f]\n", driver->motor_param.iq_ref);
            break;
        case ADDR_SPEED_REF:
            memcpy(&driver->motor_param.spd_ref, &rxdata[4], sizeof(float));
            sprintf(data, "Receive ADDR_SPEED_REF = [%f]\n", driver->motor_param.spd_ref);
            break;
        case ADDR_LIMIT_TORQUE:
            memcpy(&driver->motor_param.limit_torque, &rxdata[4], sizeof(float));
            sprintf(data, "Receive ADDR_LIMIT_TORQUE = [%f]\n", driver->motor_param.limit_torque);
            break;
        case ADDR_CURRENT_KP:
            memcpy(&driver->motor_param.cur_kp, &rxdata[4], sizeof(float));
            sprintf(data, "Receive ADDR_CURRENT_KP = [%f]\n", driver->motor_param.cur_kp);
            break;
        case ADDR_CURRENT_KI:
            memcpy(&driver->motor_param.cur_ki, &rxdata[4], sizeof(float));
            sprintf(data, "Receive ADDR_CURRENT_KI = [%f]\n", driver->motor_param.cur_ki);
            break;
        case ADDR_CURRENT_FILTER_GAIN:
            memcpy(&driver->motor_param.cur_filt_gain, &rxdata[4], sizeof(float));
            sprintf(data, "Receive ADDR_CURRENT_FILTER_GAIN = [%f]\n", driver->motor_param.cur_filt_gain);
            break;
        case ADDR_LOC_REF:
            memcpy(&driver->motor_param.loc_ref, &rxdata[4], sizeof(float));
            sprintf(data, "Receive ADDR_LOC_REF = [%f]\n", driver->motor_param.loc_ref);
            break;
        case ADDR_LIMIT_SPEED:
            memcpy(&driver->motor_param.limit_spd, &rxdata[4], sizeof(float));
            sprintf(data, "Receive ADDR_LIMIT_SPEED = [%f]\n", driver->motor_param.limit_spd);
            break;
        case ADDR_LIMIT_CURRENT:
            memcpy(&driver->motor_param.limit_cur, &rxdata[4], sizeof(float));
            sprintf(data, "Receive ADDR_LIMIT_CURRENT = [%f]\n", driver->motor_param.limit_cur);
            break;
        case ADDR_MECH_POS:
            memcpy(&driver->motor_param.mech_pos, &rxdata[4], sizeof(float));
            sprintf(data, "Receive ADDR_MECH_POS = [%f]\n", driver->motor_param.mech_pos);
            break;
        case ADDR_IQF:
            memcpy(&driver->motor_param.iqf, &rxdata[4], sizeof(float));
            sprintf(data, "Receive ADDR_IQF = [%f]\n", driver->motor_param.iqf);
            break;
        case ADDR_MECH_VEL:
            memcpy(&driver->motor_param.mech_vel, &rxdata[4], sizeof(float));
            sprintf(data, "Receive ADDR_MECH_VEL = [%f]\n", driver->motor_param.mech_vel);
            break;
        case ADDR_VBUS:
            memcpy(&driver->motor_param.vbus, &rxdata[4], sizeof(float));
            sprintf(data, "Receive ADDR_VBUS = [%f]\n", driver->motor_param.vbus);
            break;
        case ADDR_ROTATION:
            memcpy(&driver->motor_param.rotation, &rxdata[4], sizeof(int16_t));
            sprintf(data, "Receive ADDR_ROTATION = [%d]\n", driver->motor_param.rotation);
            break;
        case ADDR_LOC_KP:
            memcpy(&driver->motor_param.loc_kp, &rxdata[4], sizeof(float));
            sprintf(data, "Receive ADDR_LOC_KP = [%f]\n", driver->motor_param.loc_kp);
            break;
        case ADDR_SPD_KP:
            memcpy(&driver->motor_param.spd_kp, &rxdata[4], sizeof(float));
            sprintf(data, "Receive ADDR_SPD_KP = [%f]\n", driver->motor_param.spd_kp);
            break;
        case ADDR_SPD_KI:
            memcpy(&driver->motor_param.spd_ki, &rxdata[4], sizeof(float));
            sprintf(data, "Receive ADDR_SPD_KI = [%f]\n", driver->motor_param.spd_ki);
            break;
        default:
            sprintf(data, "Unknown parameter value index=[0x%04x]\n", index);
            is_updated = false;
            break;
    }
//    float float_data;
//    memcpy(&float_data, &rxdata[4], sizeof(float));
//    sprintf(data, "%x %x %x %x %x %x %x %x %f\n", rxdata[0], rxdata[1], rxdata[2], rxdata[3], rxdata[4], rxdata[5], rxdata[6], rxdata[7], float_data);
//    UARTSend
    if (is_updated) {
        driver->motor_param.stamp_usec = HAL_GetTick();
    }
}

void Cybergear_set_limit_torque(Cybergear *cybergear, uint8_t motorindex, float torque){
	CybergearDriver_write_param_float(&cybergear->drivers[motorindex], ADDR_LIMIT_TORQUE, torque);
}

void Cybergear_set_current_kp(Cybergear *cybergear, uint8_t motorindex, float cur_kp){
	CybergearDriver_write_param_float(&cybergear->drivers[motorindex], ADDR_CURRENT_KP, cur_kp);
}

void Cybergear_set_current_ki(Cybergear *cybergear, uint8_t motorindex, float cur_ki){
	CybergearDriver_write_param_float(&cybergear->drivers[motorindex], ADDR_CURRENT_KI, cur_ki);
}

void Cybergear_set_current_filter_gain(Cybergear *cybergear, uint8_t motorindex, float cur_filter_gain){
	CybergearDriver_write_param_float(&cybergear->drivers[motorindex], ADDR_CURRENT_FILTER_GAIN, cur_filter_gain);
}

void Cybergear_set_position_ref(Cybergear *cybergear, uint8_t motorindex, float pos_ref){
//	if (!cybergear->drivers[motorindex].enable) {
//		return;
//	}
	pos_ref *= 2*PI;
	CybergearDriver_write_param_float(&cybergear->drivers[motorindex], ADDR_LOC_REF, pos_ref);
}

void Cybergear_set_limit_speed(Cybergear *cybergear, uint8_t motorindex, float speed){
	speed *= 2*PI/60.0;
	CybergearDriver_write_param_float(&cybergear->drivers[motorindex], ADDR_LIMIT_SPEED, speed);
}

void Cybergear_set_limit_current(Cybergear *cybergear, uint8_t motorindex, float current){
	CybergearDriver_write_param_float(&cybergear->drivers[motorindex], ADDR_LIMIT_CURRENT, current);
}

void Cybergear_set_loc_kp(Cybergear *cybergear, uint8_t motorindex, float loc_kp){
	CybergearDriver_write_param_float(&cybergear->drivers[motorindex], ADDR_LOC_KP, loc_kp);
}

void Cybergear_set_spd_kp(Cybergear *cybergear, uint8_t motorindex, float spd_kp){
	CybergearDriver_write_param_float(&cybergear->drivers[motorindex], ADDR_SPD_KP, spd_kp);
}

void Cybergear_set_spd_ki(Cybergear *cybergear, uint8_t motorindex, float spd_ki){
	CybergearDriver_write_param_float(&cybergear->drivers[motorindex], ADDR_SPD_KI, spd_ki);
}

void CybergearDriver_write_param_float(CybergearDriver *driver, uint16_t addr, float value)
{
	memset(&driver->txdata, 0, 8);
	driver->txdata[0] = addr & 0xFF;
	driver->txdata[1] = (addr >> 8) & 0xFF;
//	uint16_t senddata;

	switch (addr){
	case ADDR_IQ_REF:
//		senddata = float_to_uint(value, IQ_REF_MIN, IQ_REF_MAX, 16);
		Ulim(value, IQ_REF_MAX);
		Llim(value, -IQ_REF_MIN);
		break;
	case ADDR_SPEED_REF:
//		senddata = float_to_uint(value, SPD_REF_MIN, SPD_REF_MAX, 16);
		Ulim(value, SPD_REF_MAX);
		Llim(value, -SPD_REF_MIN);
		break;
	case ADDR_LIMIT_TORQUE:
//		senddata = float_to_uint(value, LIMIT_TORQUE_MIN, LIMIT_TORQUE_MAX, 16);
		Ulim(value, LIMIT_TORQUE_MAX);
		Llim(value, -LIMIT_TORQUE_MIN);
		break;
	case ADDR_CURRENT_KP:
//		senddata = float_to_uint(value, CUR_KP_MIN, CUR_KP_MAX, 16);
		Ulim(value, CUR_KP_MAX);
		Llim(value, -CUR_KP_MIN);
		break;
	case ADDR_CURRENT_KI:
//		senddata = float_to_uint(value, CUR_KI_MIN, CUR_KI_MAX, 16);
		Ulim(value, CUR_KI_MAX);
		Llim(value, -CUR_KI_MIN);
		break;
	case ADDR_CURRENT_FILTER_GAIN:
//		senddata = float_to_uint(value, CURRENT_FILTER_GAIN_MIN, CURRENT_FILTER_GAIN_MAX, 16);
		Ulim(value, CURRENT_FILTER_GAIN_MAX);
		Llim(value, -CURRENT_FILTER_GAIN_MIN);
		break;
	case ADDR_LIMIT_SPEED:
//		senddata = float_to_uint(value, LIMIT_SPD_MIN, LIMIT_SPD_MAX, 16);
		Ulim(value, LIMIT_SPD_MAX);
		Llim(value, -LIMIT_SPD_MIN);
		break;
	case ADDR_LIMIT_CURRENT:
//		senddata = float_to_uint(value, LIMIT_CUR_MIN, LIMIT_CUR_MAX, 16);
		Ulim(value, LIMIT_CUR_MAX);
		Llim(value, -LIMIT_CUR_MIN);
		break;
	default:
		break;
	}
//	senddata = float_to_uint(value, min, max, 16);
    memcpy(&driver->txdata[4], &value, sizeof(float));
//	driver->txdata[7] = 0x40;
//	float float_data;
//	memcpy(&float_data, &driver->txdata[4], sizeof(float));
////    float float_data = *(float*)&uint32_data;
////    uint16_t data16 = rxdata[4] | (rxdata[5] << 8);
//	sprintf(data, "									send %x %x %x %x %x %x %x %x %f\n", driver->txdata[0], driver->txdata[1], driver->txdata[2], driver->txdata[3], driver->txdata[4], driver->txdata[5], driver->txdata[6], driver->txdata[7], value);
//	UARTSend

    CybergearDriver_send_command(driver, CMD_RAM_WRITE, driver->master_can_id);
}




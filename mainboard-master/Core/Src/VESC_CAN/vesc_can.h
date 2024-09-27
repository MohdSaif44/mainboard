/*
 * vesc_can.h
 *
 *  Created on: Dec 23, 2020
 *      Author: root
 */

#ifndef VESC_CAN_VESC_CAN_H_
#define VESC_CAN_VESC_CAN_H_

#include "bldc_interface.h"
#include "buffer.h"
#include "crc.h"
#include "datatypes.h"
#include "../CAN/can.h"

#define RX_FRAMES_SIZE	100
#define RX_BUFFER_SIZE	512

enum{
	//0x70
	VESC1 = 112,
	VESC2,
	VESC3,
	VESC4,
	VESCPOSLAST,
};

typedef struct {
	CAN_RxHeaderTypeDef Rxmsg;
	uint8_t Data[8];
} Vescmsg;

enum{
	mainboard_TO_VESC = 29,
		RNS_TO_VESC
};

//edit or write flash
enum{
	Edit,
	Write
};


#define	CAN_STATUS_DISABLED 	0
#define	CAN_STATUS_1			0x01
#define	CAN_STATUS_2			0x02
#define	CAN_STATUS_3			0x04
#define	CAN_STATUS_4			0x08
#define	CAN_STATUS_5			0x10
#define	CAN_STATUS_6			0x20
#define	CAN_STATUS_7			0x40
#define	CAN_STATUS_8			0x80
#define	CAN_STATUS_9			0x100
#define	CAN_STATUS_10			0x200
#define	CAN_STATUS_11			0x400
#define	CAN_STATUS_12			0x800
#define	CAN_STATUS_13			0x1000
#define	CAN_STATUS_14			0x2000
#define	CAN_STATUS_15			0x4000

#define	CAN_STATUS_ALL			0xFFFF


const char* mc_interface_fault_to_string(mc_fault_code fault);
char vescerror [100];

typedef struct{
	unsigned char CANid;

	float rpm;					//0dp, rpm
	float current;				//1dp, A, filtered motor current
	float duty_cycle;			//3dp, %
	float amp_hours;			//1dp, mAh, miliamp hours drawn from battery
	float amp_hours_charged;	//1dp, mAh, miliamp hours fed back into battery
	float watt_hours;			//1dp, mWh, miliwatt hours drawn from battery
	float watt_hours_charged;	//1dp, mWh, miliwatt hours fed back into battery
	float temp_fet;				//1d.p, 0C
	float temp_motor;			//1d.p, 0C
	float current_in;			//1dp, A, filtered battery input current to the motor controller
	float pos;					//2dp, deg
	float tacho_value;			//0dp
	float v_in;					//1dp, V  filtered battery input voltage
	int Fault;

	float adc_1;
	float adc_2;
	float adc_3;
	float ppm;					//pwm
	float speed;				//4dp, m/s
	float dist;					//4dp, m

	float Id;					//4dp, A
	float Iq;					//4dp, A, current that produce torque
	float Vd;					//4dp, V
	float Vq;					//4dp, V

	float KV;
//	float id_filter;
//	float iq_filter;

	float enc_pos;
	float testdata;
	float tol_angle;

	float pos_kp;
	float pos_ki;
	float pos_kd;
	float pos_kdfilter;
	float rpm_kp;
	float rpm_ki;
	float rpm_kd;
	float rpm_kdfilter;
}vesc_enq;

volatile vesc_enq infolist[VESCPOSLAST - VESC1];

extern VESC_t vesc;
volatile float moved_pos[VESCPOSLAST - VESC1];
volatile uint8_t pos_overflow[VESCPOSLAST - VESC1];

void comm_can_send_buffer(uint8_t controller_id, uint8_t *data, unsigned int len, uint8_t send);
void comm_can_set_duty(uint8_t controller_id, float duty);
void comm_can_set_current(uint8_t controller_id, float current);
void comm_can_set_current_off_delay(uint8_t controller_id, float current, float off_delay);
void comm_can_set_rpm(uint8_t controller_id, float rpm);
void comm_can_move_pos(uint8_t controller_id, float pos);
void comm_can_set_pos(uint8_t controller_id, float pos);
void comm_can_set_current_rel(uint8_t controller_id, float current_rel);
void comm_can_set_current_brake(uint8_t controller_id, float current);
void comm_can_set_current_brake_rel(uint8_t controller_id, float current_rel);
void comm_can_set_handbrake(uint8_t controller_id, float current);
void comm_can_set_handbrake_rel(uint8_t controller_id, float current_rel);

//set the motor parameter
void comm_can_conf_current_limits(uint8_t controller_id, unsigned char flash, float min, float max);
void comm_can_conf_current_limits_in(uint8_t controller_id, unsigned char flash, float min, float max);
void comm_can_conf_foc_erpms(uint8_t controller_id, unsigned char flash, float foc_openloop_rpm, float foc_sl_erpm);
void comm_can_conf_battery_cut(uint8_t controller_id, unsigned char flash, float start, float end);
void comm_can_set_pos_pid(uint8_t controller_id, float rpm_kp, float rpm_ki, float rpm_kd);
void comm_can_set_rpm_pid(uint8_t controller_id, float rpm_kp, float rpm_ki, float rpm_kd);

uint8_t rx_buffer[RX_BUFFER_SIZE];
io_board_adc_values io_board_adc_1_4[10];
io_board_adc_values io_board_adc_5_8[10];
//edit
void comm_can_packet_status(uint8_t controller_id, uint32_t status);
void comm_can_packet_ADC14(uint8_t controller_id);
void comm_can_packet_ADC58(uint8_t controller_id);
void comm_can_set_pos_offset(uint8_t controller_id, float pos, unsigned char flash);
void comm_can_set_tachometer(uint8_t controller_id, float techometer);
void comm_can_brake(uint8_t controller_id);
void comm_can_release_motor(uint8_t controller_id);
//void comm_can_get_status(uint8_t controller_id, unsigned char parameter, VESC_infolist_t* infolist);

void decode_VESC(void);
Vescmsg *get_rx_frame(void);
uint8_t set_rx_frames(Vescmsg* CANRxFrame);
Vescmsg vescmsg;
#endif /* VESC_CAN_VESC_CAN_H_ */

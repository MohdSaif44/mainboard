/*******************************************************************************
 * Title   : adapter.h
 * Author  : Anas Amer
 * Version : 1.0
 * Date    : 12/10
 *******************************************************************************
 * Description: includes all the important includes and pin definitions
 *
 * Version History:
 *  1.0 - converted to HAL library
 *
 * Bugs:
 *
 ******************************************************************************/

#ifndef SRC_ADAPTER_H_
#define SRC_ADAPTER_H_

//#define mainboard3_3

/* Private variables ---------------------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "BIOS/bios.h"
#include <math.h>
#include "CAN/can.h"
#include "MODN/MODN.h"
#include "PID/PID.h"
#include "ABT/ABT.h"
#include "I2C/i2c.h"
#include "PSx_Interface/PSx_Interface.h"
#include "RNS_interface/RNS_interface.h"
#include "ADC/adc.h"
#include "SERVO/servo.h"
#include "KF/KF.h"
#include "LASER/laser.h"
#include "Moving_Average/mov_ave.h"
#include "VESC_CAN/vesc_interface.h"
#include "IMU/r6091u.h"
#include "Odrive/odriver.h"
#include "Robomaster/Robomaster.h"
#include "PathProfile/PathProfile.h"
#include "SoftPWM/SoftPWM.h"
#include "usb.h"
#include "PWMEncoder/PWMEncoder.h"
#include "Cybergear/cybergear.h"
#include "ModbusTCP/Modbus.h"
#include "PMW3901/PMW3901.h"
#include "EXTI/EXTI.h"
#include "KalmanFilter/KalmanFilter.h"
#include "Hipnuc/ch0x0.h"
#include "Deep_Robotic/Deep_Robotic.h"
// #include "MATLAB/Matlab.h"
// #include "SPI/SPI.h"
// #include "STEPPER/stepper.h"
// #include "Eeprom/eeprom.h"
// #include "ILI9341/ILI9341_Driver.h"
// #include "Dongbei/dongbei.h"
// #include "RGB/rgb.h"
// #include "TFmini/tfmini.h"
#include "Faulhaber/FH.h"
#include "trajectory_control/traj.h"
// #include "PixyCam2_Esp32/PixyCam2_ESP32.h"

/* Exported functions prototypes ---------------------------------------------*/

/* Private defines -----------------------------------------------------------*/
#ifdef mainboard3_3
//#define LED4_PIN			GPIOC, GPIO_PIN_13
//#define LED5_PIN			GPIOC, GPIO_PIN_14
//#define LED6_PIN			GPIOC, GPIO_PIN_15
//#define LED8_PIN			GPIOD, GPIO_PIN_7
//#define LED9_PIN			GPIOA, GPIO_PIN_8
//#define LED10_PIN			GPIOC, GPIO_PIN_9
//#define LED11_PIN			GPIOB, GPIO_PIN_6
//#define LED12_PIN			GPIOB, GPIO_PIN_7
#else
#define LED1_PIN			GPIOC, GPIO_PIN_15
#define LED2_PIN			GPIOC, GPIO_PIN_14
#define LED3_PIN			GPIOC, GPIO_PIN_13
#endif

#ifdef newpin
#define PB1_PIN				GPIOE, GPIO_PIN_3
#define PB2_PIN				GPIOE, GPIO_PIN_4

#define IP1_PIN				GPIOB, GPIO_PIN_9				//TIM11_CH1
#define IP2_PIN				GPIOB, GPIO_PIN_5               // TIM3_CH2
#define IP3_PIN				GPIOD, GPIO_PIN_15              // TIM4_CH4
#define IP4_PIN				GPIOD, GPIO_PIN_14				// TIM4_CH3
#define IP5_PIN				GPIOB, GPIO_PIN_15				//TIM12_CH2
#define IP6_PIN				GPIOB, GPIO_PIN_14              //TIM12_CH1

#define IP7_Analog1_PIN		GPIOC, GPIO_PIN_0
#define IP8_Analog2_PIN		GPIOC, GPIO_PIN_1
#else
#ifdef mainboard3_3
#define PB1_PIN				GPIOB, GPIO_PIN_5			// TIM4_CH2
#define PB2_PIN				GPIOE, GPIO_PIN_0
#define PB3_PIN				GPIOE, GPIO_PIN_4

#define IP1_PIN				GPIOE, GPIO_PIN_12          // TIM1_CH3N
#define IP2_PIN				GPIOE, GPIO_PIN_13          // TIM1_CH3
#define IP3_PIN				GPIOE, GPIO_PIN_14          // TIM1_CH4
#define IP4_PIN				GPIOE, GPIO_PIN_15          // TIM1_BKN
#define IP5_PIN				GPIOB, GPIO_PIN_14          // TIM12_CH1
#define IP6_PIN				GPIOB, GPIO_PIN_15          // TIM12_CH2
#define IP7_PIN				GPIOD, GPIO_PIN_10
#define IP8_PIN				GPIOD, GPIO_PIN_11
#define IP9_PIN				GPIOC, GPIO_PIN_8           // TIM8_CH3, TIM3_CH3
#define IP10_PIN			GPIOA, GPIO_PIN_10          // TIM1_CH3
#define IP11_PIN			GPIOD, GPIO_PIN_3
#define IP12_PIN			GPIOD, GPIO_PIN_4
#else
#define PB1_PIN				GPIOB, GPIO_PIN_7			// TIM4_CH2
#define PB2_PIN				GPIOE, GPIO_PIN_0

#define IP1_PIN				GPIOE, GPIO_PIN_12          // TIM1_CH3N
#define IP2_PIN				GPIOE, GPIO_PIN_13          // TIM1_CH3
#define IP3_PIN				GPIOE, GPIO_PIN_14          // TIM1_CH4
#define IP4_PIN				GPIOE, GPIO_PIN_15          // TIM1_BKN
#define IP5_PIN				GPIOB, GPIO_PIN_14          // TIM12_CH1
#define IP6_PIN				GPIOB, GPIO_PIN_15          // TIM12_CH2
#define IP7_PIN				GPIOD, GPIO_PIN_10
#define IP8_PIN				GPIOD, GPIO_PIN_11
#define IP9_PIN				GPIOC, GPIO_PIN_8           // TIM8_CH3, TIM3_CH3
#define IP10_PIN			GPIOA, GPIO_PIN_10          // TIM1_CH3
#define IP11_PIN			GPIOD, GPIO_PIN_3
#define IP12_PIN			GPIOD, GPIO_PIN_4
#define IP13_PIN			GPIOD, GPIO_PIN_7
#define IP14_PIN			GPIOB, GPIO_PIN_5           // TIM3_CH2
#define IP15_PIN			GPIOB, GPIO_PIN_6           // TIM4_CH1
#endif

#define IP16_Analog1_PIN	GPIOC, GPIO_PIN_0
#define IP17_Analog2_PIN	GPIOC, GPIO_PIN_1
#define IP18_Analog3_PIN	GPIOC, GPIO_PIN_2			//SPI2_MISO
#define IP19_Analog4_PIN	GPIOC, GPIO_PIN_3			//SPI2_MOSI
#define IP20_Analog5_PIN	GPIOC, GPIO_PIN_4
#define IP21_Analog6_PIN	GPIOC, GPIO_PIN_5
#endif

#define QEI1_PLUSEA_PIN		GPIOE, GPIO_PIN_9			//TIM1_CH1
#define QEI1_PLUSEB_PIN		GPIOE, GPIO_PIN_11			//TIM1_CH2
#define TIM1_CHANNEL1_PIN	GPIOE, GPIO_PIN_9
#define TIM1_CHANNEL2_PIN	GPIOE, GPIO_PIN_11
#define TIM1_CHANNEL3_PIN	GPIOE, GPIO_PIN_13
#define TIM1_CHANNEL4_PIN	GPIOE, GPIO_PIN_14

#define QEI4_PLUSEA_PIN		GPIOD, GPIO_PIN_12			//TIM4_CH1
#define QEI4_PLUSEB_PIN		GPIOD, GPIO_PIN_13			//TIM4_CH2
#define TIM4_CHANNEL1_PIN	GPIOD, GPIO_PIN_12
#define TIM4_CHANNEL2_PIN	GPIOD, GPIO_PIN_13
#define TIM4_CHANNEL3_PIN	GPIOD, GPIO_PIN_14
#define TIM4_CHANNEL4_PIN	GPIOD, GPIO_PIN_15

#define QEI6_PLUSEA_PIN		GPIOC, GPIO_PIN_6			//TIM8_CH1
#define QEI6_PLUSEB_PIN		GPIOC, GPIO_PIN_7			//TIM8_CH2
#define TIM8_CHANNEL1_PIN	GPIOC, GPIO_PIN_6
#define TIM8_CHANNEL2_PIN	GPIOC, GPIO_PIN_7
#define TIM8_CHANNEL3_PIN	GPIOC, GPIO_PIN_8
#define TIM8_CHANNEL4_PIN	GPIOC, GPIO_PIN_9

#define TIM5_CHANNEL1_PIN	GPIOA, GPIO_PIN_0			//hspm8
#define TIM5_CHANNEL2_PIN	GPIOA, GPIO_PIN_1			//hspm7
#define TIM5_CHANNEL3_PIN	GPIOA, GPIO_PIN_2			//hspm6
#define TIM5_CHANNEL4_PIN	GPIOA, GPIO_PIN_3			//hspm5

#define TIM9_CHANNEL1_PIN	GPIOE, GPIO_PIN_5			//hspm4
#define TIM9_CHANNEL2_PIN	GPIOE, GPIO_PIN_6			//hspm3

#define TIM3_CHANNEL1_PIN	GPIOB, GPIO_PIN_4			//switch debug,
//#define TIM3_CHANNEL1_PIN	GPIOA, GPIO_PIN_6			//SPI1_MISO, TIM13_CH1
#define TIM3_CHANNEL2_PIN	GPIOA, GPIO_PIN_7			//SPI1_MOSI, TIM14_CH1
#define TIM3_CHANNEL3_PIN	GPIOB, GPIO_PIN_0			//hspm2
#define TIM3_CHANNEL4_PIN	GPIOB, GPIO_PIN_1			//hspm1

#define TIM11_CHANNEL1_PIN	GPIOB, GPIO_PIN_9			//SPI2_NSS, I2C1_SDA

#define TIM12_CHANNEL1_PIN	GPIOB, GPIO_PIN_14			//SPI2_MISO
#define TIM12_CHANNEL2_PIN	GPIOB, GPIO_PIN_15			//SPI2_MOSI

#define TIM13_CHANNEL1_PIN	GPIOA, GPIO_PIN_6			//SPI1_MISO, TIM13_CH1

#define TIM14_CHANNEL1_PIN	GPIOA, GPIO_PIN_7			//SPI1_MOSI, TIM3_CH2

#define MUX1_INPUT_PIN 		GPIOE, GPIO_PIN_1
#define MUX1_S0_PIN 		GPIOE, GPIO_PIN_2
#define MUX1_S1_PIN 		GPIOE, GPIO_PIN_3
#define MUX1_S2_PIN 		GPIOE, GPIO_PIN_4

#define SR_SCK_PIN			GPIOE, GPIO_PIN_7
#define SR_RCK_PIN			GPIOE, GPIO_PIN_8
#define SR_SI_PIN			GPIOE, GPIO_PIN_10

#define SPI1_NSS_PIN		GPIOA, GPIO_PIN_4
#define SPI1_SCK_PIN		GPIOA, GPIO_PIN_5
#define SPI1_MISO_PIN		GPIOA, GPIO_PIN_6			//TIM13_CH1, TIM3_CH1
#define SPI1_MOSI_PIN		GPIOA, GPIO_PIN_7			//TIM14_CH1, TIM3_CH2

#define SPI2_NSS_PIN		GPIOB, GPIO_PIN_12
#define SPI2_SCK_PIN		GPIOB, GPIO_PIN_13
#define SPI2_MISO_PIN		GPIOB, GPIO_PIN_14			//TIM12_CH1
#define SPI2_MOSI_PIN		GPIOB, GPIO_PIN_15			//TIM12_CH2

#define UART1_Tx			GPIOA, GPIO_PIN_9
#define UART1_Rx			GPIOA, GPIO_PIN_10

#define UART2_Tx			GPIOD, GPIO_PIN_5
#define UART2_Rx			GPIOD, GPIO_PIN_6

#define UART3_Tx			GPIOD, GPIO_PIN_8
#define UART3_Rx			GPIOD, GPIO_PIN_9

#define UART4_Tx			GPIOC, GPIO_PIN_10			//SPI3_SCK
#define UART4_Rx			GPIOC, GPIO_PIN_11			//SPI3_MISO

#define UART5_Tx			GPIOC, GPIO_PIN_12			//SPI3_MOSI
#define UART5_Rx			GPIOD, GPIO_PIN_2

#ifdef newpin
#define CAN1_Tx				GPIOA, GPIO_PIN_11
#define CAN1_Rx				GPIOA, GPIO_PIN_12
#else
#define CAN1_Tx				GPIOD, GPIO_PIN_1
#define CAN1_Rx				GPIOD, GPIO_PIN_0
#endif

//#define CAN2_Tx				GPIOB, GPIO_PIN_6
//#define CAN2_Rx				GPIOB, GPIO_PIN_5

#define CAN2_Tx				GPIOB, GPIO_PIN_13
#define CAN2_Rx				GPIOB, GPIO_PIN_12

#ifdef newpin
#define I2C1_SDA_PIN		GPIOB, GPIO_PIN_7
#else
#define I2C1_SDA_PIN		GPIOB, GPIO_PIN_9
#endif
#define I2C1_SCL_PIN		GPIOB, GPIO_PIN_8

#define I2C2_SDA_PIN		GPIOB, GPIO_PIN_11
#define I2C2_SCL_PIN		GPIOB, GPIO_PIN_10		//SPI2_SCK

#define I2C3_SDA_PIN		GPIOC, GPIO_Pin_9		//TIM8_CH4
#define I2C3_SCL_PIN		GPIOA, GPIO_Pin_8

//from system.h
//#define QEI1_PLUSEA_PIN		GPIOE, GPIO_PIN_9
//#define QEI1_PLUSEB_PIN		GPIOE, GPIO_PIN_11
//
//#define QEI2_PLUSEA_PIN		GPIOA, GPIO_PIN_5
//#define QEI2_PLUSEB_PIN		GPIOB, GPIO_PIN_3
//
//#define QEI3_PLUSEA_PIN		GPIOA, GPIO_PIN_6
//#define QEI3_PLUSEB_PIN		GPIOA, GPIO_PIN_7
//
//#define QEI4_PLUSEA_PIN		GPIOD, GPIO_PIN_12
//#define QEI4_PLUSEB_PIN		GPIOD, GPIO_PIN_13
//
//#define QEI5_PLUSEA_PIN		GPIOA, GPIO_PIN_0
//#define QEI5_PLUSEB_PIN		GPIOA, GPIO_PIN_1
//
//#define QEI6_PLUSEA_PIN		GPIOC, GPIO_PIN_6
//#define QEI6_PLUSEB_PIN		GPIOC, GPIO_PIN_7

MUX_t MUX;
shiftreg_t SR;
RNS_interface_t rns;

BDC_t BDC1, BDC2, BDC3, BDC4, BDC5, BDC6, BDC7, BDC8;
uint8_t insData_receive[2];
PSxBT_t ps4;
ABT_t filter;
ADC_t adc;
Laser_t X_laser, Y_laser;
KALMANFILTER_t kf_adc_x, kf_adc_y, kf_pres;
Mov_Ave_t move_aveX, move_aveY;
PID_t pid_pos_x,pid_pos_y,pid_pres,pid_z,imu_rotate;
R6091U_t IMU;
Mov_Ave_t mov_l_r,mov_l_l;
VESC_t vesc,vesc1;
SERVO_t servo, servo1;
PWMEnc_t enc1,enc2,enc3,enc4;
Modbus modbus;
PMW3901_t enc;
Exti_t ExtiPin;
KalmanFilter_t KF;
//esp32_t ESP32;

int counter_rns;
int counter;

#ifdef newpin
#define PB1 		GPIOE_IN->bit3
#define PB2 		GPIOE_IN->bit4

#define led1		GPIOC_OUT->bit15
#define led2		GPIOC_OUT->bit14
#define led3		GPIOC_OUT->bit13
#endif

#ifdef mainboard3_3
#define PB1			GPIOB_IN->bit5
#define PB2			GPIOE_IN->bit0
#define PB3			GPIOE_IN->bit4

#define LED1_PIN			GPIOB, GPIO_PIN_7
#define LED2_PIN			GPIOC, GPIO_PIN_15
#define LED3_PIN			GPIOA, GPIO_PIN_8
#define LED4_PIN			GPIOC, GPIO_PIN_9
#define LED5_PIN			GPIOC, GPIO_PIN_14
#define LED6_PIN			GPIOC, GPIO_PIN_13
#define LED7_PIN			GPIOD, GPIO_PIN_7
#define LED8_PIN			GPIOB, GPIO_PIN_6

#define led1		GPIOB_OUT->bit7  //12
#define led2		GPIOC_OUT->bit15 //6
#define led3		GPIOA_OUT->bit8  //9
#define led4		GPIOC_OUT->bit9  //10
#define led5		GPIOC_OUT->bit14 //5
#define led6		GPIOC_OUT->bit13 //4
#define led7		GPIOD_OUT->bit7  //8
#define led8		GPIOB_OUT->bit6  //11
#else
#define PB1 		GPIOB_IN->bit7
#define PB2 		GPIOE_IN->bit0

#define led1		GPIOC_OUT->bit13
#define led2		GPIOC_OUT->bit14
#define led3		GPIOC_OUT->bit15
#endif

#define IP1_IN				GPIOE_IN->bit12          // TIM1_CH3N
#define IP2_IN				GPIOE_IN->bit13          // TIM1_CH3
#define IP3_IN				GPIOE_IN->bit14          // TIM1_CH4
#define IP4_IN				GPIOE_IN->bit15          // TIM1_BKN
#define IP5_IN				GPIOB_IN->bit14          // TIM12_CH1
#define IP6_IN				GPIOB_IN->bit15          // TIM12_CH2
#define IP7_IN				GPIOD_IN->bit10
#define IP8_IN				GPIOD_IN->bit11
#define IP9_IN				GPIOC_IN->bit8           // TIM8_CH3, TIM3_CH3
#define IP10_IN				GPIOA_IN->bit10          // TIM1_CH3
#define IP11_IN				GPIOD_IN->bit3
#define IP12_IN				GPIOD_IN->bit4
#define IP13_IN				GPIOD_IN->bit7
#define IP14_IN				GPIOB_IN->bit5           // TIM3_CH2
#define IP15_IN				GPIOB_IN->bit6           // TIM4_CH1

#define IP1_OUT				GPIOE_OUT->bit12          // TIM1_CH3N
#define IP2_OUT				GPIOE_OUT->bit13          // TIM1_CH3
#define IP3_OUT				GPIOE_OUT->bit14          // TIM1_CH4
#define IP4_OUT				GPIOE_OUT->bit15          // TIM1_BKN
#define IP5_OUT				GPIOB_OUT->bit14          // TIM12_CH1
#define IP6_OUT				GPIOB_OUT->bit15          // TIM12_CH2
#define IP7_OUT				GPIOD_OUT->bit10
#define IP8_OUT				GPIOD_OUT->bit11
#define IP9_OUT				GPIOC_OUT->bit8           // TIM8_CH3, TIM3_CH3
#define IP10_OUT			GPIOA_OUT->bit10          // TIM1_CH3
#define IP11_OUT			GPIOD_OUT->bit3
#define IP12_OUT			GPIOD_OUT->bit4
#define IP13_OUT			GPIOD_OUT->bit7
#define IP14_OUT			GPIOB_OUT->bit5           // TIM3_CH2
#define IP15_OUT			GPIOB_OUT->bit6           // TIM4_CH1
#define HSPM_3				GPIOE_OUT->bit5
#define HSPM_4				GPIOE_OUT->bit6

#define AN1_IN				GPIOC_IN->bit0
#define AN2_IN				GPIOC_IN->bit1
#define AN3_IN				GPIOC_IN->bit2
#define AN4_IN				GPIOC_IN->bit3
#define AN5_IN				GPIOC_IN->bit4
#define AN6_IN				GPIOC_IN->bit5

//#define Side	IP9_IN
#define QEI1_PLUSEA_OUT		GPIOE_OUT->bit9			// TIM1_CH2
#define QEI1_PLUSEB_OUT		GPIOE_OUT->bit11			// TIM1_CH2

#define value_in_range_eq(data, low, high)   (data>=low && data<=high)?1:0
#define value_in_range(data, low, high)   (data>low && data<high)?1:0
#define value_in_pm(data, target, pm)	(data>target-pm && data<target+pm)?1:0
#define MIN(a, b)  a < b ? a : b
#define MAX(a, b)  a > b ? a : b


//Global Declarations
float x_vel, y_vel, w_vel, v1, v2, v3, v4, d1, d2, d3, d4;         //MODN variables
float pid_angle_error,pid_angle_output;
float Filtered_Angle;


//#define TOPSHOOT(duty)		RBMS_Set_Target_Velocity(&rbms2, TOP_SHOOT, duty);

typedef enum{
	RNS_PACKET,
	ODRIVE_PACKET,
	VESC_PACKET,
	FAULHABER_PACKET,
	RBMS_PACKET,
	CYBERGEAR_PACKET,
	DEEP_PACKET,
}PACKET_t;

uint32_t faul_counter;
struct{
	uint16_t statusword;
	int pos_act;
	int vel_act;
	int16_t tor_act;
	uint16_t ODindex;
	uint8_t ODsubindex;
	uint8_t num_valid;
	uint8_t buffer[4];
	int HomeOffset;
	uint32_t velKp;
	int8_t modeOperation;
	union{
		uint16_t flags;
		struct{
			unsigned can    	    :1;
			unsigned flag1          :1;
			unsigned flag2    		:1;
			unsigned flag3          :1;
			unsigned flag4          :1;
			unsigned flag5          :1;
			unsigned flag6          :1;
			unsigned flag7          :1;
			unsigned flag8          :1;
			unsigned flag9          :1;
			unsigned flag10         :1;
			unsigned flag11         :1;
			unsigned flag12         :1;
			unsigned flag13 	    :1;
			unsigned flag14         :1;
			unsigned flag15        	:1;
		};
	};
}Faul_t;

union{
	float data;
	struct{
		char byte1;
		char byte2;
		char byte3;
		char byte4;
	};
}buf1_receive[2];
union{
	float data;
	struct{
		char byte1;
		char byte2;
		char byte3;
		char byte4;
	};
}buf2_receive[2];
union{
	float data;
	struct{
		char byte1;
		char byte2;
		char byte3;
		char byte4;
	};
}buf3_receive[2];
union{
	float data;
	struct{
		char byte1;
		char byte2;
		char byte3;
		char byte4;
	};
}buf4_receive[2];



void Initialize (void);
void CAN_PROCESS(PACKET_t packet_src);

void IP_OUT(uint8_t pin, uint8_t value);

#ifdef __cplusplus
}
#endif
#endif /* SRC_ADAPTER_H_ */

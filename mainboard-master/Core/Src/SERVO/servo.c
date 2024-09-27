
/************************************************/
/*		 	 	Include Header	       		  	*/
/************************************************/

#include"servo.h"

/************************************************/
/*		 	 	Variables	      	 		  	*/
/************************************************/

/************************************************/
/*		 	 	Functions		       		  	*/
/************************************************/

/*
 * Function Name		: ServoInit
 * Function Description : This function is called to initialize servo.
 * Function Remarks		:
 * Function Arguments	: servo				structure to store data
 * 						  htimx				can be htim1 to htim14 except htim6 and htim7
 * 						  SERVO_GPIOx		GPIOx where x can be A to I.
 * 						  SERVO_GPIO_Pin	The desired pin that you want to configure in group GPIOx. This parameter
 * 						  					can be combination of GPIO_Pin_x where x can be (0..15)
 * 						  channel			Channel of PWM:
 * 						  					TIM_CHANNEL_1
 * 						  					TIM_CHANNEL_2
 * 						  					TIM_CHANNEL_3
 * 						  					TIM_CHANNEL_4
 * Function Return		: None
 * Function Example		: ServoInit(&SERVO1, &htim3, GPIOA,GPIO_Pin_6, TIM_CHANNEL_1);
 */
void ServoxInit(SERVO_t* servo, TIM_HandleTypeDef* htimx, GPIO_TypeDef * SERVO_GPIOx, uint16_t SERVO_GPIO_Pin, uint32_t channel){

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
		TIM_MasterConfigTypeDef sMasterConfig = {0};


		if(htimx == &htim1){
			htimx->Instance = TIM1;
			htimx->Init.Prescaler = 167;
		}
		else if(htimx == &htim2)	{
			htimx->Instance = TIM2;
			htimx->Init.Prescaler = 83;
		}
		else if(htimx == &htim3)	{
			htimx->Instance = TIM3;
			htimx->Init.Prescaler = 83;
		}
		else if(htimx == &htim4)	{
			htimx->Instance = TIM4;
			htimx->Init.Prescaler = 83;
		}
		else if(htimx == &htim5)	{
			htimx->Instance = TIM5;
			htimx->Init.Prescaler = 83;
		}
		else if(htimx == &htim8)	{
			htimx->Instance = TIM8;
			htimx->Init.Prescaler = 167;
		}
		else if(htimx == &htim9)	{
			htimx->Instance = TIM9;
			htimx->Init.Prescaler = 167;
		}
		else if(htimx == &htim10)	{
			htimx->Instance = TIM10;
			htimx->Init.Prescaler = 167;
		}
		else if(htimx == &htim11)	{
			htimx->Instance = TIM11;
			htimx->Init.Prescaler = 167;
		}
		else if(htimx == &htim12)	{
			htimx->Instance = TIM12;
			htimx->Init.Prescaler = 83;
		}
		else if(htimx == &htim13)	{
			htimx->Instance = TIM13;
			htimx->Init.Prescaler = 83;
			}
		else if(htimx == &htim14)	{
			htimx->Instance = TIM14;
			htimx->Init.Prescaler = 83;
			}


		htimx->Init.CounterMode = TIM_COUNTERMODE_UP;
		htimx->Init.Period = 19999;
		htimx->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		htimx->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
		 if (HAL_TIM_Base_Init(htimx) != HAL_OK)
		  {
		    Error_Handler();
		  }
		  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
		  if (HAL_TIM_ConfigClockSource(htimx, &sClockSourceConfig) != HAL_OK)
		  {
		    Error_Handler();
		  }
		if (HAL_TIM_PWM_Init(htimx) != HAL_OK)
			{
				Error_Handler();
			}

		sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
		sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
		if (HAL_TIMEx_MasterConfigSynchronization(htimx, &sMasterConfig) != HAL_OK)
		{
			Error_Handler();
		}


	    HAL_TIM_Base_Start(htimx);

	GPIOPinsInit(SERVO_GPIOx, SERVO_GPIO_Pin, GPIO_MODE_AF_PP, GPIO_SPEED_FREQ_HIGH, GPIO_PULLUP);

	servo->htimx = htimx;
	servo->htimx_Channel = channel;

	PWMChannelConfig(htimx, channel , SERVO_GPIOx,SERVO_GPIO_Pin);

	servo->currentPulse = 0;
}

/*
 * Function Name		: ServoSetPulse
 * Function Description : Sets the TIMx Capture Compare1,2,3,4 Register value
 * Function Remarks		: None
 * Function Arguments	: servo		structure to store data
 * 						  pulse		output compare pulse to be set
 * Function Return		: None
 * Function Example		: ServoSetPulse(&SERVO1, 1000);
 */

void ServoSetPulse(SERVO_t* servo, uint32_t pulse){
	if(pulse < servo->SERVO_pulseMinLimit){
		servo->currentPulse = servo->SERVO_pulseMinLimit;
	}else if(pulse > servo->SERVO_pulseMaxLimit){
		servo->currentPulse = servo->SERVO_pulseMaxLimit;
	}else{
		servo->currentPulse = pulse;
	}
	__HAL_TIM_SET_COMPARE(servo->htimx, servo->htimx_Channel, servo->currentPulse);

}

/*
 * Function Name		: ServoInitAngle
 * Function Description : Initialize the angle of servo.
 * Function Remarks		: Support servo with angle 0-180 degree
 * Function Arguments	: servo				structure to store data
 * 						  pulse0degree		value of output compare pulse when servo is at 0 degree
 * 						  pulse180degree	value of output compare pulse when servo is at 180 degree
 * Function Return		: None
 * Function Example		: ServoInitAngle(&SERVO1, 800 , 2600);
 */

void ServoInitAngle(SERVO_t* servo, uint32_t degree){
	servo->SERVO_pulse0degree = servo->SERVO_pulseMinLimit;
	servo->SERVO_pulseMaxDegree = servo->SERVO_pulseMaxLimit;
	servo->SERVO_pulse1degree = (uint32_t)((servo->SERVO_pulseMaxDegree - servo->SERVO_pulse0degree)/degree);
	servo->SERVO_pulse36second = servo->SERVO_pulse1degree / 100;	//0.01 degree
	servo->SERVO_MaxAngle = degree;
}

/*
 * Function Name		: ServoSetAngle
 * Function Description : This function set the servo to desired angle.
 * Function Remarks		: User need to initialize the angle of servo by calling the function ServoInitAngle
 * 						  before using this function.
 * Function Arguments	: servo		structure to store data
 * 						  angle		angle of servo to be set
 * Function Return		: None
 * Function Example		: ServoSetAngle(&SERVO1, 90);
 */

void ServoSetAngle(SERVO_t* servo, uint8_t angle){
	servo->TIMx_Compare = servo->SERVO_pulse0degree + (servo->SERVO_pulse1degree)*angle;
	if(servo->TIMx_Compare > servo->SERVO_pulseMaxDegree)
		servo->TIMx_Compare = servo->SERVO_pulseMaxDegree;
	if(servo->TIMx_Compare < servo->SERVO_pulse0degree)
		servo->TIMx_Compare = servo->SERVO_pulse0degree;
	servo->currentPulse = servo->TIMx_Compare;
	__HAL_TIM_SET_COMPARE(servo->htimx, servo->htimx_Channel, servo->TIMx_Compare);
}

/*
 * Function Name		: ServoSetAngle_Sec
 * Function Description : This function set the servo to desired angle in second or 0.01 degree.
 * Function Remarks		: User need to initialize the angle of servo by calling the function ServoInitAngle
 * 						  before using this function.
 * Function Arguments	: servo		structure to store data
 * 						  angle		angle of servo to be set
 * Function Return		: None
 * Function Example		: ServoSetAngle_Sec(&SERVO1, 90.06);
 */

void ServoSetAngle_Sec(SERVO_t* servo, float angle){
	servo->TIMx_Compare = servo->SERVO_pulse0degree + (servo->SERVO_pulse36second) * angle * 100;
	if(servo->TIMx_Compare > servo->SERVO_pulseMaxDegree)
		servo->TIMx_Compare = servo->SERVO_pulseMaxDegree;
	if(servo->TIMx_Compare < servo->SERVO_pulse0degree)
		servo->TIMx_Compare = servo->SERVO_pulse0degree;
	servo->currentPulse = servo->TIMx_Compare;
	__HAL_TIM_SET_COMPARE(servo->htimx, servo->htimx_Channel, servo->TIMx_Compare);
}

/*
 * Function Name		: ServoInitPulseLimit
 * Function Description : This function set the pulse limit servo.
 * Function Remarks		: User need to initialize the timer by calling the function ServoxInit
 * 						  before using this function.
 * Function Arguments	: servo		structure to store data
 * 						  MinPulse	minimum pulse of servo can reach
 * 						  MaxPulse	maximum pulse of servo can reach
 * Function Return		: None
 * Function Example		: ServoInitPulseLimit(&SERVO1, 330, 2700);
 */

void ServoInitPulseLimit(SERVO_t* servo, uint32_t MinPulse, uint32_t MaxPulse){
	servo->SERVO_pulseMinLimit = MinPulse;
	servo->SERVO_pulseMaxLimit = MaxPulse;
}

/*
 * Function Name		: ServoMovePulse
 * Function Description : This function to increase or decrease pulse of servo from current state.
 * Function Remarks		: User need to initialize the timer by calling the function ServoxInit
 * 						  before using this function.
 * Function Arguments	: servo		structure to store data
 * 						  pulse		pulse changes of servo
 * Function Return		: None
 * Function Example		: ServoInitPulseLimit(&SERVO1, 50);
 */

void ServoMovePulse(SERVO_t* servo, int pulse){
	servo->currentPulse = servo->currentPulse + pulse;

	if(servo->currentPulse < servo->SERVO_pulseMinLimit){
		servo->currentPulse = servo->SERVO_pulseMinLimit;
	}
	if(servo->currentPulse > servo->SERVO_pulseMaxLimit){
		servo->currentPulse = servo->SERVO_pulseMaxLimit;
	}
	__HAL_TIM_SET_COMPARE(servo->htimx, servo->htimx_Channel, servo->currentPulse);
}

/*
 * Function Name		: ServoMoveAngle
 * Function Description : This function to increase or decrease angle of servo from current angle.
 * Function Remarks		: User need to initialize the angle of servo by calling the function ServoInitAngle
 * 						  before using this function.
 * Function Arguments	: servo		structure to store data
 * 						  angle		angle changes of servo
 * Function Return		: None
 * Function Example		: ServoInitPulseLimit(&SERVO1, 5);
 */

void ServoMoveAngle(SERVO_t* servo, int angle){
	servo->TIMx_Compare = servo->SERVO_pulse0degree + (servo->SERVO_pulse36second) * angle * 100;
	servo->currentPulse = servo->currentPulse + servo->TIMx_Compare;

	if(servo->currentPulse < servo->SERVO_pulseMinLimit){
		servo->currentPulse = servo->SERVO_pulseMinLimit;
	}
	if(servo->currentPulse > servo->SERVO_pulseMaxLimit){
		servo->currentPulse = servo->SERVO_pulseMaxLimit;
	}
	__HAL_TIM_SET_COMPARE(servo->htimx, servo->htimx_Channel, servo->currentPulse);
}

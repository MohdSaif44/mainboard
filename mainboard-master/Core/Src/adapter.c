/*********************************************/
/*          Include Header                   */
/*********************************************/
#include "adapter.h"
uint8_t buf2_flag = 0;

/************************************************/
/*		 	 	Functions		       		  	*/
/************************************************/
void Initialize() {
	/* MCU Configuration--------------------------------------------------------*/
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
#ifdef mainboard3_3
	GPIOPinsInit(LED1_PIN, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_HIGH, GPIO_PULLUP);
	GPIOPinsInit(LED2_PIN, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_HIGH, GPIO_PULLUP);
	GPIOPinsInit(LED3_PIN, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_HIGH, GPIO_PULLUP);
	GPIOPinsInit(LED4_PIN, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_HIGH, GPIO_PULLUP);
	GPIOPinsInit(LED5_PIN, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_HIGH, GPIO_PULLUP);
	GPIOPinsInit(LED6_PIN, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_HIGH, GPIO_PULLUP);
	GPIOPinsInit(LED7_PIN, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_HIGH, GPIO_PULLUP);
	GPIOPinsInit(LED8_PIN, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_HIGH, GPIO_PULLUP);

	GPIOPinsInit(PB1_PIN, GPIO_MODE_INPUT, GPIO_SPEED_FREQ_HIGH, GPIO_PULLUP);
	GPIOPinsInit(PB2_PIN, GPIO_MODE_INPUT, GPIO_SPEED_FREQ_HIGH, GPIO_PULLUP);
	GPIOPinsInit(PB3_PIN, GPIO_MODE_INPUT, GPIO_SPEED_FREQ_HIGH, GPIO_PULLUP);
#else
	GPIOPinsInit(LED1_PIN, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_HIGH, GPIO_PULLUP);
	GPIOPinsInit(LED2_PIN, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_HIGH, GPIO_PULLUP);
	GPIOPinsInit(LED3_PIN, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_HIGH, GPIO_PULLUP);

	GPIOPinsInit(PB1_PIN, GPIO_MODE_INPUT, GPIO_SPEED_FREQ_HIGH, GPIO_PULLUP);
	GPIOPinsInit(PB2_PIN, GPIO_MODE_INPUT, GPIO_SPEED_FREQ_HIGH, GPIO_PULLUP);
#endif

/***debug start***/
	/*Normal IOs*/
	GPIOPinsInit(IP1_PIN,  GPIO_MODE_INPUT, GPIO_SPEED_FREQ_MEDIUM, GPIO_PULLUP);
	GPIOPinsInit(IP2_PIN,  GPIO_MODE_INPUT, GPIO_SPEED_FREQ_MEDIUM, GPIO_PULLUP);
	GPIOPinsInit(IP3_PIN,  GPIO_MODE_INPUT, GPIO_SPEED_FREQ_MEDIUM, GPIO_PULLUP);
	GPIOPinsInit(IP4_PIN,  GPIO_MODE_INPUT, GPIO_SPEED_FREQ_MEDIUM, GPIO_PULLUP);
	GPIOPinsInit(IP5_PIN,  GPIO_MODE_INPUT, GPIO_SPEED_FREQ_MEDIUM, GPIO_PULLUP);
	GPIOPinsInit(IP6_PIN,  GPIO_MODE_INPUT, GPIO_SPEED_FREQ_MEDIUM, GPIO_PULLUP);
	GPIOPinsInit(IP7_PIN,  GPIO_MODE_INPUT, GPIO_SPEED_FREQ_MEDIUM, GPIO_PULLUP);
	GPIOPinsInit(IP8_PIN,  GPIO_MODE_INPUT, GPIO_SPEED_FREQ_MEDIUM, GPIO_PULLUP);
	GPIOPinsInit(IP9_PIN,  GPIO_MODE_INPUT, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_PULLUP);
	GPIOPinsInit(IP10_PIN, GPIO_MODE_INPUT, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_PULLUP);
	GPIOPinsInit(IP11_PIN, GPIO_MODE_INPUT, GPIO_SPEED_FREQ_MEDIUM,GPIO_PULLUP);
	GPIOPinsInit(IP12_PIN, GPIO_MODE_INPUT, GPIO_SPEED_FREQ_MEDIUM,GPIO_PULLUP);
//	GPIOPinsInit(IP13_PIN, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_MEDIUM,GPIO_PULLUP);
//	GPIOPinsInit(IP14_PIN, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_VERY_HIGH,GPIO_PULLUP);
//	GPIOPinsInit(IP15_PIN, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_VERY_HIGH,GPIO_PULLUP);

#ifndef mainboard3_3
	GPIOPinsInit(IP13_PIN, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_MEDIUM,GPIO_PULLUP);
	GPIOPinsInit(IP14_PIN, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_VERY_HIGH,GPIO_PULLUP);
	GPIOPinsInit(IP15_PIN, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_VERY_HIGH,GPIO_PULLUP);
//	GPIOPinsInit(IP13_PIN, GPIO_MODE_INPUT, GPIO_SPEED_FREQ_MEDIUM,GPIO_PULLUP);
//	GPIOPinsInit(IP13_PIN, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_PULLUP);
//	GPIOPinsInit(IP14_PIN, GPIO_MODE_INPUT, GPIO_SPEED_FREQ_MEDIUM,GPIO_PULLUP);
//	GPIOPinsInit(QEI6_PLUSEA_PIN, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_PULLUP);
//	GPIOPinsInit(IP15_PIN, GPIO_MODE_INPUT, GPIO_SPEED_FREQ_MEDIUM,GPIO_PULLUP);
//	GPIOPinsInit(QEI6_PLUSEB_PIN, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_PULLUP);
#endif

//	GPIOPinsInit(QEI1_PLUSEB_PIN, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_PULLUP);
//
//	GPIOPinsInit(QEI4_PLUSEA_PIN, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_PULLUP);
//	GPIOPinsInit(QEI4_PLUSEB_PIN, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_PULLUP);
//	GPIOPinsInit(QEI6_PLUSEA_PIN, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_PULLUP);
//	GPIOPinsInit(QEI6_PLUSEB_PIN, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_PULLUP);
#ifndef newpin
	GPIOPinsInit(IP16_Analog1_PIN, GPIO_MODE_IT_RISING_FALLING, GPIO_SPEED_FREQ_MEDIUM,GPIO_NOPULL);
	GPIOPinsInit(IP17_Analog2_PIN, GPIO_MODE_IT_RISING_FALLING, GPIO_SPEED_FREQ_MEDIUM,GPIO_NOPULL);
	GPIOPinsInit(IP17_Analog2_PIN, GPIO_MODE_IT_RISING_FALLING, GPIO_SPEED_FREQ_MEDIUM,GPIO_NOPULL);
	GPIOPinsInit(IP18_Analog3_PIN, GPIO_MODE_IT_RISING_FALLING, GPIO_SPEED_FREQ_MEDIUM,GPIO_NOPULL);
	GPIOPinsInit(IP19_Analog4_PIN, GPIO_MODE_IT_RISING_FALLING, GPIO_SPEED_FREQ_MEDIUM,GPIO_NOPULL);
	GPIOPinsInit(IP20_Analog5_PIN, GPIO_MODE_IT_RISING_FALLING, GPIO_SPEED_FREQ_MEDIUM,GPIO_NOPULL);
	GPIOPinsInit(IP21_Analog6_PIN, GPIO_MODE_IT_RISING_FALLING, GPIO_SPEED_FREQ_MEDIUM,GPIO_NOPULL);
#else
//	GPIOPinsInit (IP7_Analog1_PIN, GPIO_MODE_INPUT,GPIO_SPEED_FREQ_MEDIUM, GPIO_PULLUP);
//	GPIOPinsInit (IP8_Analog2_PIN, GPIO_MODE_INPUT,GPIO_SPEED_FREQ_MEDIUM, GPIO_PULLUP);
#endif
//	MUXInit(&MUX, MUX1_INPUT_PIN, MUX1_S0_PIN, MUX1_S1_PIN, MUX1_S2_PIN);
	SHIFTREGInit(&SR, CASCADE_1, SR_SCK_PIN, SR_RCK_PIN, SR_SI_PIN);
//	GPIOPinsInit(IP16_Analog1_PIN, GPIO_MODE_IT_RISING_FALLING, GPIO_SPEED_FREQ_HIGH,GPIO_NOPULL);
//	GPIOPinsInit(IP17_Analog2_PIN, GPIO_MODE_IT_RISING_FALLING, GPIO_SPEED_FREQ_HIGH,GPIO_NOPULL);
//	GPIOPinsInit(IP18_Analog3_PIN, GPIO_MODE_IT_RISING_FALLING, GPIO_SPEED_FREQ_HIGH,GPIO_NOPULL);
//	GPIOPinsInit(IP19_Analog4_PIN, GPIO_MODE_IT_RISING_FALLING, GPIO_SPEED_FREQ_HIGH,GPIO_NOPULL);
//	GPIOPinsInit(IP20_Analog5_PIN, GPIO_MODE_IT_RISING_FALLING, GPIO_SPEED_FREQ_HIGH,GPIO_NOPULL);
//	GPIOPinsInit(IP21_Analog6_PIN, GPIO_MODE_IT_RISING_FALLING, GPIO_SPEED_FREQ_HIGH,GPIO_NOPULL);

	//https://stackoverflow.com/questions/50243996/what-are-valid-values-of-hal-nvic-setpriority-when-using-stm32-and-freertos
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	/*************** Set Interrupt Priorities in BIOS/priorities.h ***************/

//	I2CxInit (&hi2c1, main_board_1, CLOCK_SPEED_100KHz, ENABLE);
//	I2CxInit (&hi2c2, main_board_2, CLOCK_SPEED_100KHz, ENABLE);
//	I2CxInit (&hi2c3, main_board_3, CLOCK_SPEED_100KHz, ENABLE);
//	I2CX_DMA_RX_Init(&hi2c1, &hi2c1_rx_dma, main_board_1, CLOCK_SPEED_100KHz);
//	I2CX_DMA_RX_Init(&hi2c2, &hi2c2_rx_dma, main_board_2, CLOCK_SPEED_100KHz);
//	Servo Driver - recommended to use 100KHz I2C as 400KHz hang frequently
//	ServoDriverInit(&srv_drv,&hi2c3,0x40);
//	UARTx_DMA_Rx_Init(&huart4, &hdma_uart4_rx, 115200);
//	UARTx_DMA_Rx_Init(&huart2, &hdma_usart2_rx, 115200);//Bluebee Tuning
//	UARTInit(&huart1, 115200, ENABLE);
	UARTInit(&huart2, 115200, ENABLE);					//imu
	UARTInit(&huart3, 115200, ENABLE);					//
	UARTInit(&huart4, 115200, ENABLE);					//tfmini
	UARTInit(&huart5, 115200, ENABLE);					//bluebee

//	QEIInit(&htim1);	//X
//	QEIInit(&htim4);	//Y
//	QEIInit(&htim8);

	CANxInit(&hcan1, CAN_FILTER_FIFO0, CAN_FILTERSCALE_32BIT, 0, 0, 10, CAN_500KHz);	// RNS & VESC
	CANxInit(&hcan2, CAN_FILTER_FIFO1, CAN_FILTERSCALE_32BIT, 0, 0, 14, CAN_1MHz);

//	PWMTimeBaseInit(&htim1, 65355, 75); // allignment enc
//	PWMChannelConfig(&htim1, TIM_CHANNEL_1, TIM1_CHANNEL1_PIN);
//	PWMChannelConfig(&htim1, TIM_CHANNEL_2, TIM1_CHANNEL2_PIN);
//	PWMChannelConfig(&htim1, TIM_CHANNEL_3, IP2_PIN);
//	PWMChannelConfig(&htim1, TIM_CHANNEL_4, IP3_PIN);
//
	PWMTimeBaseInit(&htim3, 20000, 84);
//	PWMChannelConfig(&htim3, TIM_CHANNEL_2, IP14_PIN); // IP14
	PWMChannelConfig(&htim3, TIM_CHANNEL_3, TIM3_CHANNEL3_PIN); // hspm2
	PWMChannelConfig(&htim3, TIM_CHANNEL_4, TIM3_CHANNEL4_PIN); // hspm1

	PWMTimeBaseInit(&htim5, 20000, 84);
	PWMChannelConfig(&htim5, TIM_CHANNEL_1, TIM5_CHANNEL1_PIN); // hspm8
	PWMChannelConfig(&htim5, TIM_CHANNEL_2, TIM5_CHANNEL2_PIN); // hspm7
	PWMChannelConfig(&htim5, TIM_CHANNEL_3, TIM5_CHANNEL3_PIN); // hspm6
	PWMChannelConfig(&htim5, TIM_CHANNEL_4, TIM5_CHANNEL4_PIN); // hspm5

	PWMTimeBaseInit(&htim9, 20000, 168);
	PWMChannelConfig(&htim9, TIM_CHANNEL_1, TIM9_CHANNEL1_PIN); // hspm4
	PWMChannelConfig(&htim9, TIM_CHANNEL_2, TIM9_CHANNEL2_PIN); // hspm3

//	PWMTimeBaseInit(&htim12, 20000, 84);
//	PWMChannelConfig(&htim12, TIM_CHANNEL_1, IP5_PIN);
//	PWMChannelConfig(&htim12, TIM_CHANNEL_2, IP6_PIN);


//	PWMTimeBaseInit(&htim4, 20000, 84); // qei 4

//	PWMChannelConfig(&htim4, TIM_CHANNEL_1, IP15_PIN); // IP15
//	PWMChannelConfig(&htim4, TIM_CHANNEL_2, TIM4_CHANNEL2_PIN);

//	PWMTimeBaseInit(&htim8, 20000, 168); // qei 6
//	PWMChannelConfig(&htim8, TIM_CHANNEL_1, QEI6_PLUSEA_PIN);
//	PWMChannelConfig(&htim8, TIM_CHANNEL_2, QEI6_PLUSEB_PIN);
//	PWMChannelConfig(&htim8, TIM_CHANNEL_3, IP9_PIN);

	BDCInit(&BDC1, &htim3, TIM_CHANNEL_4, SHIFTREG, &(SR.cast[1]), Bit6, Bit7);
	BDCInit(&BDC2, &htim3, TIM_CHANNEL_3, SHIFTREG, &(SR.cast[1]), Bit4, Bit5);
#ifdef newpin
	BDCInit(&BDC3, &htim9, TIM_CHANNEL_2, SHIFTREG, &(SR.cast[1]), Bit2, Bit3);
	BDCInit(&BDC4, &htim9, TIM_CHANNEL_1, SHIFTREG, &(SR.cast[1]), Bit0, Bit1);
#else
	BDCInit(&BDC3, &htim9, TIM_CHANNEL_1, SHIFTREG, &(SR.cast[1]), Bit2, Bit3);
	BDCInit(&BDC4, &htim9, TIM_CHANNEL_2, SHIFTREG, &(SR.cast[1]), Bit0, Bit1);
#endif
	BDCInit(&BDC5, &htim5, TIM_CHANNEL_4, SHIFTREG, &(SR.cast[0]), Bit6, Bit7);
	BDCInit(&BDC6, &htim5, TIM_CHANNEL_3, SHIFTREG, &(SR.cast[0]), Bit4, Bit5);
	BDCInit(&BDC7, &htim5, TIM_CHANNEL_2, SHIFTREG, &(SR.cast[0]), Bit2, Bit3);
	BDCInit(&BDC8, &htim5, TIM_CHANNEL_1, SHIFTREG, &(SR.cast[0]), Bit0, Bit1);
//	IP2_OUT = 0;

	QEIInit(&htim1);	//X
	QEIInit(&htim4);	//Y
	QEIInit(&htim8);

	/******SPI******/
//	SPIx_DMA_TX_Init(&hspi1, &hdma_spi1_tx, SPI1_NSS_PIN, SPI_MODE_MASTER);
//	SPIxInit(&hspi1, SPI1_NSS_PIN, SPI_MODE_MASTER, ENABLE);
//	GPIOPinsInit(SPI1_MISO_PIN, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_HIGH,
//	GPIO_PULLUP);

	/*****Motor*****/
//	IMU_Init(&IMU, &huart2);
//	ch0x0Init(&ch010, &huart3);
//	PWMEncoder_Angle(&htim1, GPIOC, GPIO_PIN_0, &enc1);
//	VESCInit(BOTTOMSHOOT, LOADING, 0, 0, &vesc);
//
//	RBMS_Init(&rbms1, &hcan2, RBMS_1234);
//	RBMS_Init(&rbms2, &hcan2, RBMS_5678);
//
//


}

void CAN1_RX0_IRQHandler() {
	HAL_CAN_IRQHandler(&hcan1);
}

void CAN1_RX1_IRQHandler() {
	HAL_CAN_IRQHandler(&hcan1);
}

void CAN2_RX0_IRQHandler() {
	HAL_CAN_IRQHandler(&hcan2);
}

void CAN2_RX1_IRQHandler() {
	HAL_CAN_IRQHandler(&hcan2);
}

void CAN_PROCESS(PACKET_t packet_src) {

	switch (packet_src) {
	case VESC_PACKET:
		decode_VESC();
		if (vesc.error_flag) {
			vesc.error_flag = 0;
////			strcpy(data, vescerror);
////			UARTSend
		}
		break;

	case ODRIVE_PACKET:
//		SwerveCANHandler();

		break;

	case RNS_PACKET:
		if (insData_receive[0] == 1) {
			rns.RNS_data.common_instruction = insData_receive[1];
			insData_receive[0] = 2;
		}
		if (insData_receive[0] == 17) {
			if (buf2_flag == 1) {
				rns.RNS_data.common_instruction = insData_receive[1];
				rns.RNS_data.common_buffer[0].data = buf1_receive[0].data;
				rns.RNS_data.common_buffer[1].data = buf1_receive[1].data;
				rns.RNS_data.common_buffer[2].data = buf2_receive[0].data;
				rns.RNS_data.common_buffer[3].data = buf2_receive[1].data;
				insData_receive[0] = 3;
			}
		}

		break;

	case RBMS_PACKET:
		break;
	case CYBERGEAR_PACKET:

		break;
	}
}

//uint8_t aData[8] = {0};


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	uint8_t aData[8] = {0};
	PACKET_t source = 0;
	if (hcan == &hcan1) {
		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &CAN1RxMessage, aData);


		if (CAN1RxMessage.IDE == CAN_ID_EXT) {
			if ((CAN1RxMessage.ExtId & 0xFF) >= 112
					&& (CAN1RxMessage.ExtId & 0xFF) < 127) {
				vescmsg.Rxmsg = CAN1RxMessage;
				memcpy(vescmsg.Data, aData, 8);
				set_rx_frames(&vescmsg);
				source = VESC_PACKET;
			} else {
				Cybergear_CAN_Handler(&CAN1RxMessage, aData);
				source = CYBERGEAR_PACKET;
			}

		} else if (CAN1RxMessage.IDE == CAN_ID_STD) {
			uint16_t command_mask = 0x7E0; // mask first 7 bits, get only the id
			uint16_t id = CAN1RxMessage.StdId & command_mask;
			id = id >> 5;
//			sprintf(data, "id is %d \n", id);
//			UARTPrintString(&huart2, data);
//			led2 = !led2;
			int i = 0;
			for (i = 0; i < number_of_odrive; i++) {
				if (id == P_to_Odrive[i]->Instance) {
					source = ODRIVE_PACKET;
					Odrvmsg.RXmsg = CAN1RxMessage;
					memcpy(Odrvmsg.Data, aData, 8);
					decode_Odrive(P_to_Odrive[i]);
					break;
				}
			}

			if((CAN1RxMessage.StdId & 0x10) == 0x10){
//				led5 = !led5;
				if (J60_CANHandler(&CAN1RxMessage, aData)) {
					source = DEEP_PACKET;
				}

			}


			if (CAN1RxMessage.StdId >= 0x201 && CAN1RxMessage.StdId <= 0x208){
				RBMS_CAN_Handler(&CAN1RxMessage, aData);
				source = RBMS_PACKET;
			}
			if (source != ODRIVE_PACKET && source != RBMS_PACKET) {
				source = RNS_PACKET;
				switch (CAN1RxMessage.StdId) {
				case RNS_TO_mainboard:
					memcpy(&insData_receive, &aData, CAN1RxMessage.DLC);
					buf2_flag = 0;
					break;
				case RNS_TO_mainboard_buf1:
					memcpy(&buf1_receive, &aData, CAN1RxMessage.DLC);
					break;
				case RNS_TO_mainboard_buf2:
					memcpy(&buf2_receive, &aData, CAN1RxMessage.DLC);
					buf2_flag = 1;
					break;
				default:
					FHmsg.Rxmsg = CAN1RxMessage;
					memcpy(FHmsg.Data, aData, 8);
					source = FAULHABER_PACKET;
					break;
				}
			}
		}
		CAN_PROCESS(source);
		HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
	} else {
		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &CAN2RxMessage, aData);

		if (CAN2RxMessage.IDE == CAN_ID_EXT) {

			if ((CAN1RxMessage.ExtId & 0xFF) >= 112
					&& (CAN1RxMessage.ExtId & 0xFF) < 127) {
				vescmsg.Rxmsg = CAN1RxMessage;
				memcpy(vescmsg.Data, aData, 8);
				set_rx_frames(&vescmsg);
				source = VESC_PACKET;
			} else {
				Cybergear_CAN_Handler(&CAN1RxMessage, aData);
				source = CYBERGEAR_PACKET;
			}

		} else {

			if (CAN2RxMessage.StdId >= 0x201 && CAN2RxMessage.StdId <= 0x208) {
				RBMS_CAN_Handler(&CAN2RxMessage, aData);
				source = RBMS_PACKET;
			}
			if(source != ODRIVE_PACKET && source != RBMS_PACKET){
				source = RNS_PACKET;
				switch (CAN2RxMessage.StdId) {
				case RNS_TO_mainboard:
					memcpy(&insData_receive, &aData, CAN2RxMessage.DLC);
					buf2_flag = 0;
					break;
				case RNS_TO_mainboard_buf1:
					memcpy(&buf1_receive, &aData, CAN2RxMessage.DLC);
					break;
				case RNS_TO_mainboard_buf2:
					memcpy(&buf2_receive, &aData, CAN2RxMessage.DLC);
					buf2_flag = 1;
					break;
				default:
					FHmsg.Rxmsg = CAN2RxMessage;
					memcpy(FHmsg.Data, aData, 8);
					source = FAULHABER_PACKET;
					break;
				}
			}

		}
		CAN_PROCESS(source);
		HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
	}
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	uint8_t aData[8] = {0};
	PACKET_t source = 0;
	if (hcan == &hcan1) {

		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &CAN1RxMessage, aData);

		if (CAN1RxMessage.IDE == CAN_ID_EXT) {

			if ((CAN1RxMessage.ExtId & 0xFF) >= 112
					&& (CAN1RxMessage.ExtId & 0xFF) < 127) {
				vescmsg.Rxmsg = CAN1RxMessage;
				memcpy(vescmsg.Data, aData, 8);
				set_rx_frames(&vescmsg);
				source = VESC_PACKET;
			} else {
				Cybergear_CAN_Handler(&CAN1RxMessage, aData);
				source = CYBERGEAR_PACKET;
			}

		} else {
			if (CAN1RxMessage.StdId >= 0x201 && CAN1RxMessage.StdId <= 0x208){
				RBMS_CAN_Handler(&CAN1RxMessage, aData);
				source = RBMS_PACKET;
			}
			if (source != ODRIVE_PACKET && source != RBMS_PACKET){
			source = RNS_PACKET;
				switch (CAN1RxMessage.StdId) {
				led2 = !led2;
				case RNS_TO_mainboard:
					memcpy(&insData_receive, &aData, CAN1RxMessage.DLC);
					buf2_flag = 0;
					break;
				case RNS_TO_mainboard_buf1:
					memcpy(&buf1_receive, &aData, CAN1RxMessage.DLC);
					break;
				case RNS_TO_mainboard_buf2:
					memcpy(&buf2_receive, &aData, CAN1RxMessage.DLC);
					buf2_flag = 1;
					break;
				default:
					FHmsg.Rxmsg = CAN1RxMessage;
					memcpy(FHmsg.Data, aData, 8);
					source = FAULHABER_PACKET;
					break;
				}
			}
		}
		CAN_PROCESS(source);
		HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO1_MSG_PENDING);

	} else {

		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &CAN2RxMessage, aData);

		if (CAN2RxMessage.IDE == CAN_ID_EXT) {
			if ((CAN1RxMessage.ExtId & 0xFF) >= 112
					&& (CAN1RxMessage.ExtId & 0xFF) < 127) {
				vescmsg.Rxmsg = CAN1RxMessage;
				memcpy(vescmsg.Data, aData, 8);
				set_rx_frames(&vescmsg);
				source = VESC_PACKET;
			} else {
				Cybergear_CAN_Handler(&CAN1RxMessage, aData);
				source = CYBERGEAR_PACKET;
			}

		} else if (CAN2RxMessage.IDE == CAN_ID_STD) {
//			sprintf(data, "can2 %d \n", CAN2RxMessage.StdId);
//			HAL_UART_Transmit(&huart2, data, strlen(data), 100);
			uint16_t command_mask = 0x7E0; // mask first 7 bits, get only the id
			uint16_t id = CAN2RxMessage.StdId & command_mask;
			id = id >> 5;
//			sprintf(data, "id is %d \n", id);
//			UARTPrintString(&huart2, data);
//			led2 = !led2;
			int i = 0;
			for (i = 0; i < number_of_odrive; i++) {
				if (id == P_to_Odrive[i]->Instance) {
					source = ODRIVE_PACKET;
					Odrvmsg.RXmsg = CAN2RxMessage;
					memcpy(Odrvmsg.Data, aData, 8);
					decode_Odrive(P_to_Odrive[i]);
					break;
				}
			}
			if (CAN2RxMessage.StdId >= 0x201 && CAN2RxMessage.StdId <= 0x208){
				RBMS_CAN_Handler(&CAN2RxMessage, aData);
//				led6 = !led6;
				source = RBMS_PACKET;
			}
			if (source != ODRIVE_PACKET && source != RBMS_PACKET) {
				source = RNS_PACKET;
				switch (CAN2RxMessage.StdId) {
				led2 = !led2;
				case RNS_TO_mainboard:
					memcpy(&insData_receive, &aData, CAN2RxMessage.DLC);
					buf2_flag = 0;
					break;
				case RNS_TO_mainboard_buf1:
					memcpy(&buf1_receive, &aData, CAN2RxMessage.DLC);
					break;
				case RNS_TO_mainboard_buf2:
					memcpy(&buf2_receive, &aData, CAN2RxMessage.DLC);
					buf2_flag = 1;
					break;
				default:
					FHmsg.Rxmsg = CAN2RxMessage;
					memcpy(FHmsg.Data, aData, 8);
					source = FAULHABER_PACKET;
					break;
				}
			}
		}
		CAN_PROCESS(source);
		HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO1_MSG_PENDING);
	}
}

void IP_OUT(uint8_t pin, uint8_t value){
	switch(pin){
	case 1:
		IP1_OUT = value;
		break;
	case 2:
		IP2_OUT = value;
		break;
	case 3:
		IP3_OUT = value;
		break;
	case 4:
		IP4_OUT = value;
		break;
	case 5:
		IP5_OUT = value;
		break;
	case 6:
		IP6_OUT = value;
		break;
	case 7:
		IP7_OUT = value;
		break;
	case 8:
		IP8_OUT = value;
		break;
	case 9:
		IP9_OUT = value;
		break;
	case 10:
		IP10_OUT = value;
		break;
	case 11:
		IP11_OUT = value;
		break;
	case 12:
		IP12_OUT = value;
		break;
	case 13:
		IP13_OUT = value;
		break;
	case 14:
		IP14_OUT = value;
		break;
	case 15:
		IP15_OUT = value;
		break;
	default:
		break;
	}
}


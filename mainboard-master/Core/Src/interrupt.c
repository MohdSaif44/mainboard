
/*********************************************/
/*          Include Header                   */
/*********************************************/
#include "interrupt.h"
#include "common.h"
/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/

#define USED_QEI1
#define USED_QEI4
#define USED_QEI6


/**
 * * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void)
{

}

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void)
{

}

/**
 * @brief This function handles Memory management fault.
 */
void MemManage_Handler(void)
{

	while(1){

	}
}

/**
 * @brief This function handles Pre-fetch fault, memory access fault.
 */
void BusFault_Handler(void)
{

}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler(void)
{

}

/**
 * @brief This function handles System service call via SWI instruction.
 */
//void SVC_Handler(void)
//{
//
//}

/**
 * @brief This function handles Debug monitor.
 */
void DebugMon_Handler(void)
{

}

/**
 * @brief This function handles Pendable request for system service.
 */
//void PendSV_Handler(void)
//{
//
//}

/**
 * @brief This function handles System tick timer.
 */
//void SysTick_Handler(void)
//{
//
//	HAL_IncTick();
//
//}


void TIM1_UP_TIM10_IRQHandler(void)
{
#ifdef USED_QEI1
	if (htim1.Instance -> CR1 == 129)
	{
		BIOS_QEI1.signbit += 1;
	}
	else if (htim1.Instance ->CR1 == 145)
	{
		BIOS_QEI1.signbit -= 1;
	}
	htim1.Instance -> SR = 0;
	QEIDelay(200);
#else
	HAL_TIM_IRQHandler(&htim1);
#endif
//	HAL_TIM_IRQHandler(&htim10);
	return;

}



void TIM4_IRQHandler(void)
{

#ifdef USED_QEI4
	if (htim4.Instance -> CR1 == 129)
	{
		BIOS_QEI4.signbit += 1;
	}
	else if (htim4.Instance ->CR1 == 145)
	{
		BIOS_QEI4.signbit -= 1;
	}
	htim4.Instance -> SR = 0;
	QEIDelay(100);

#else
	HAL_TIM_IRQHandler(&htim4);

	return;
#endif

}



void TIM8_UP_TIM13_IRQHandler(void)
{
#ifdef USED_QEI6
	if (htim8.Instance -> CR1 == 129)
	{
		BIOS_QEI6.signbit += 1;
	}
	else if (htim8.Instance ->CR1 == 145)
	{
		BIOS_QEI6.signbit -= 1;
	}
	htim8.Instance -> SR = 0;
	QEIDelay(200);
#else
	HAL_TIM_IRQHandler(&htim8);
#endif
//	HAL_TIM_IRQHandler(&htim13);
	return;
}

void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim2);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM2) {
//		if (htim->Instance == TIM2) {
		HAL_IncTick();
//		MUXUpdate(&MUX);
		SHIFTREGShift(&SR);
		counter++;
		if(counter >= 300){
			counter = 0;
		}
//		}
		HAL_IncTick();
//		MUXUpdate(&MUX);
		SHIFTREGShift(&SR);
//		Path_Profile_Inc();
//		counter ++;
//		smttime ++;
	}

}

int ERflag = 0;
//Callback for I2C RXBuffer
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c) {
//	sprintf(data,"L2: %f R2: %f \n",ps4.joyL_2,ps4.joyR_2);
//	UARTPrintString(&huart5, data);
//	led2 = !led2;
	if(hi2c == ps4.hi2cps4){
		PSx_SlaveHandler(&ps4);
	}
	else if(hi2c==enc.hi2c){
		PMW3901_SlaveHandler(&enc);
	}

}
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c){
//	sprintf(data,"L2: %f R2: %f \n",ps4.joyL_2,ps4.joyR_2);
//	UARTPrintString(&huart5, data);
//	PSx_MasterHandler(&ps4);

}
//{
/*
 * multiple slave
 * need to remove the interrupt calling function from the file and put here
 * 1 interrupt is triggered at a time only
 */

//	static int device = 0;
//	if(device){
//		device = 0;
//		HAL_I2C_Master_Receive_DMA(IMU.hi2cimu, 0x35<<1, (uint8_t*)&IMU.Buffer, 20);//RECEIVE FROM R6091U
//	}else{
//		device++;
//		HAL_I2C_Master_Receive_DMA(ps4.hi2cps4, 0x44 << 1 ,(unsigned char *)ps4.ReceiveBuffer, 11);
//	}
//
//	if(hi2c->Devaddress == 0x44 << 1){
//		PSxConnectDMA(&ps4);
//	}void PSxBTReceiveData(PSxBT_t *psxbt, PSx psx, UART_HandleTypeDef* huartx);
//	void PSx_HandlerUART_Legacy(PSxBT_t *psxbt);
//	void PSx_HandlerUART(PSxBT_t *psxbt);
//	void PSx_HandlerI2C(PSxBT_t *psxbt);
//	void PSxBTGetXY(PSxBT_t *psxbt);
//	void PSxConnect(PSxBT_t *psxbt);
//	void PSxConnectUART(PSxBT_t *psxbt);
//	void PSxConnectDMA(PSxBT_t* psxbt);
//	void PSxInit(PSxBT_t *psxbt,I2C_HandleTypeDef* hi2cx);
//	void PSxInitDMA(PSxBT_t *psxbt,I2C_HandleTypeDef* hi2cx);
//	void PSxInitUART(PSxBT_t *psxbt,UART_HandleTypeDef* huartx);
//	void PSxReconnect(PSxBT_t* psxbt);
//	void PSxReconnectDMA(PSxBT_t* psxbt);
//	if(hi2c->Devaddress == 0x35 << 1){
//		IMU_DMAHandle(&IMU);
//	}
//	if(hi2c->Devaddress == 0x01 << 1){
//
//		swerve.angle[0] = *((float *)&tleBuffer[0][1]);
//		swerve.realAngle[0] = swerve.readAngle[0]*180.0/236.0;
//		HAL_I2C_Master_Receive_IT(hi2c, 0x01 << 1, (uint8_t *)&tleBuffer[0], 6);
//	}
//
//	ERflag = 1;
//}

void I2C1_ER_IRQHandler(void){
	if(ps4.master){
			HAL_DMA_DeInit(&hi2c1_rx_dma);
			HAL_I2C_DeInit(&hi2c1);
			I2CX_DMA_RX_Init(&hi2c1, &hi2c1_rx_dma, main_board_1, CLOCK_SPEED_400KHz);
			ps4.disconnected=1;
		}
		HAL_I2C_ER_IRQHandler(&hi2c1);
}

void I2C2_ER_IRQHandler(void){
//	HAL_I2C_ER_IRQHandler(&hi2c2);
//
//	if(ERflag){
//
//		HAL_DMA_DeInit(&hi2c2_rx_dma);
//		HAL_I2C_DeInit(&hi2c2);
//
//		I2CX_DMA_RX_Init(&hi2c2, &hi2c2_rx_dma, main_board_2, CLOCK_SPEED_100KHz);
//
//		if(IMU.hi2cimu == &hi2c2){
//			HAL_I2C_DeInit(&hi2c2);
//			I2CxInit (&hi2c2, main_board_2, CLOCK_SPEED_100KHz, ENABLE);
//			HAL_I2C_Master_Receive_IT(IMU.hi2cimu, 0x35 << 1, (uint8_t*)IMU.Buffer, 20);
//
//			HAL_DMA_DeInit(&hi2c2_rx_dma);
//			I2CX_DMA_RX_Init(&hi2c2, &hi2c2_rx_dma, main_board_2, CLOCK_SPEED_100KHz);
//			HAL_I2C_Master_Receive_DMA(&hi2c2, 0x35 << 1, (uint8_t*)IMU.Buffer, 20);
//		}else if(hi2c2.Devaddress == 0x44 << 1){
//			HAL_DMA_DeInit(&hi2c2_rx_dma);
//			HAL_I2C_DeInit(&hi2c2);
//			I2CX_DMA_RX_Init(&hi2c2, &hi2c2_rx_dma, main_board_2, CLOCK_SPEED_100KHz);
//			PSxInitDMA(&ps4, &hi2c2);
//		}else if(hi2c2.Devaddress == 0x01 << 1){
//
//			HAL_I2C_DeInit(&hi2c2);
//			I2CxInit (&hi2c2, main_board_2, CLOCK_SPEED_100KHz, ENABLE);
//			HAL_I2C_Master_Receive_IT(&hi2c2, 0x01 << 1, (uint8_t *)&tleBuffer[0], 6);
//			HAL_I2C_DeInit(&hi2c2);
//			I2CxInit (&hi2c2, main_board_2, CLOCK_SPEED_400KHz, ENABLE);
//			HAL_I2C_Slave_Receive_IT(&hi2c2, (uint8_t *)&tleBuffer[0], 6);
//			while(HAL_I2C_Master_Receive_IT(&hi2c2, 0x01 << 1, (uint8_t *)&tleBuffer[0], 6) != HAL_OK);
//			HAL_I2C_Master_Seq_Receive_IT(&hi2c2, 0x01 << 1, (uint8_t *)&tleBuffer[0], 6, I2C_LAST_FRAME);
//		}
//		if(HAL_GetTick() - tick >= 333){
//			tick = HAL_GetTick();
//			led3 = !led3;
//		}
//		ERflag = 0;
//	}
}

/*
 * Function Name		: I2C3_ER_IRQHandler
 * Function Description : I2C3 Error interrupt handler.
 * Function Remarks		: This interrupt handle the error event of I2C3.
 * Function Arguments	: None
 * Function Return		: None
 * Function Example		: None
 */
void I2C3_ER_IRQHandler(void){
	HAL_I2C_ER_IRQHandler(&hi2c3);

	if(ERflag){

//		HAL_DMA_DeInit(&hi2c3_rx_dma);
		HAL_I2C_DeInit(&hi2c3);

//		I2CX_DMA_RX_Init(&hi2c3, &hi2c3_rx_dma, main_board_3, CLOCK_SPEED_100KHz);
		I2CxInit (&hi2c3, main_board_3, CLOCK_SPEED_100KHz, ENABLE);

		if(IMU.hi2cimu == &hi2c3){
			HAL_I2C_Master_Receive_IT(IMU.hi2cimu, 0x35 << 1, (uint8_t*)IMU.Buffer, 20);
		}else if(hi2c2.Devaddress == 0x01 << 1){
//			HAL_I2C_Master_Receive_IT(&hi2c3, 0x01 << 1, (uint8_t *)&tleBuffer[0], 6);
		}

//		if(HAL_GetTick() - tick >= 333){
//			tick = HAL_GetTick();
//			led3 = !led3;
//		}
		ERflag = 0;
	}
}
//void I2C3_ER_IRQHandler(void){
//
//	HAL_I2C_DeInit(&hi2c3);
//
//	I2CxInit (&hi2c3,main_board_1, CLOCK_SPEED_100KHz,DISABLE);
//
//	HAL_I2C_ER_IRQHandler(&hi2c3);
//
//}
//void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi){
//
//	HAL_SPI_Transmit_IT(&hspi1, (uint8_t *)&command, 1);
//
//}

void  USART1_IRQHandler(void){

	HAL_UART_IRQHandler(&huart1);
}

void  USART2_IRQHandler(void){

	HAL_UART_IRQHandler(&huart2);
}

void  USART3_IRQHandler(void){

	HAL_UART_IRQHandler(&huart3);
}

void UART4_IRQHandler(void){

	HAL_UART_IRQHandler(&huart4);
}

void UART5_IRQHandler(void){

	HAL_UART_IRQHandler(&huart5);
}

void  USART6_IRQHandler(void){

	HAL_UART_IRQHandler(&huart6);
}

/**
 * @brief This function handles USB On The Go FS global interrupt.
 */
void OTG_FS_IRQHandler(void) {

//	led3 = !led3;
	HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);

}
void EXTI0_IRQHandler(void) {

	PWMEncoder_Angle(&htim9, GPIOC, GPIO_PIN_0, &enc1);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);

}
void EXTI1_IRQHandler(void) {

//	PWMEncoder_Angle(&htim1, GPIOC, GPIO_PIN_1, &enc2);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);

}

void EXTI2_IRQHandler(void) {

//	PWMEncoder_Angle(&htim1, GPIOC, GPIO_PIN_3, &enc3);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);

}

void EXTI3_IRQHandler(void) {

//	PWMEncoder_Angle(&htim1, GPIOC, GPIO_PIN_3, &enc4);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);

}

void EXTI4_IRQHandler(void) {

}

void EXTI9_5_IRQHandler(void) {

}

void EXTI15_10_IRQHandler(void) {

}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

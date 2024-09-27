/*
 * ch010.c
 *
 *  Created on: Sep 2, 2024
 *      Author: clement
 */


#include "ch0x0.h"

void ch0x0Init(ch0x0_t *ch0x0, UART_HandleTypeDef* huartx){
	ch0x0->crc = 0;
	ch0x0->huartx = huartx;
	ch0x0->ch0x0rxstate = CH0X0_HEADER_PENDING;
	HAL_UART_Receive_DMA(ch0x0->huartx, ch0x0->dma_rx_buf, 1);
}

void ch0x0Handler(ch0x0_t *ch0x0){
	switch(ch0x0->ch0x0rxstate){
	case CH0X0_HEADER_PENDING:
		ch0x0->crc = 0;
		if(ch0x0->dma_rx_buf[0] == 0x5A){
			ch0x0_crc16(&ch0x0->crc, ch0x0->dma_rx_buf, 1);
			ch0x0->ch0x0rxstate = CH0X0_FTYPE_PENDING;
			HAL_UART_Receive_DMA(ch0x0->huartx, ch0x0->dma_rx_buf, 1);
		}else{
			ch0x0->ch0x0rxstate = CH0X0_HEADER_PENDING;
			HAL_UART_Receive_DMA(ch0x0->huartx, ch0x0->dma_rx_buf, 1);
		}
		break;

	case CH0X0_FTYPE_PENDING:
		if(ch0x0->dma_rx_buf[0] == 0xA5){
			ch0x0_crc16(&ch0x0->crc, ch0x0->dma_rx_buf, 1);
			ch0x0->ch0x0rxstate = CH0X0_DATAL_PENDING;
			HAL_UART_Receive_DMA(ch0x0->huartx, ch0x0->dma_rx_buf, 2);
		}else{
			ch0x0->ch0x0rxstate = CH0X0_HEADER_PENDING;
			HAL_UART_Receive_DMA(ch0x0->huartx, ch0x0->dma_rx_buf, 1);
		}
		break;

	case CH0X0_DATAL_PENDING:
		ch0x0->data_length = (ch0x0->dma_rx_buf[1]<<8) + ch0x0->dma_rx_buf[0];
		ch0x0_crc16(&ch0x0->crc, ch0x0->dma_rx_buf, 2);
		ch0x0->ch0x0rxstate = CH0X0_CHECKSUM_PENDING;
		HAL_UART_Receive_DMA(ch0x0->huartx, ch0x0->dma_rx_buf, 2);
		break;

	case CH0X0_CHECKSUM_PENDING:
		ch0x0->checksum = (ch0x0->dma_rx_buf[1] << 8) + ch0x0->dma_rx_buf[0];
		ch0x0->ch0x0rxstate = CH0X0_READY;
		HAL_UART_Receive_DMA(ch0x0->huartx, ch0x0->dma_rx_buf, ch0x0->data_length);
		break;

	case CH0X0_READY:
		ch0x0->hi91.tag = ch0x0->dma_rx_buf[0];
		ch0x0->hi91.temp = ch0x0->dma_rx_buf[3];
		ch0x0->hi91.prs = (ch0x0->dma_rx_buf[7] << 24) + (ch0x0->dma_rx_buf[6] << 16) + (ch0x0->dma_rx_buf[5] << 8) + ch0x0->dma_rx_buf[4];
		ch0x0->hi91.ts = (ch0x0->dma_rx_buf[11] << 24) + (ch0x0->dma_rx_buf[10] << 16) + (ch0x0->dma_rx_buf[9] << 8) + ch0x0->dma_rx_buf[8];
		ch0x0->buffer = (ch0x0->dma_rx_buf[15] << 24) + (ch0x0->dma_rx_buf[14] << 16) + (ch0x0->dma_rx_buf[13] << 8) + ch0x0->dma_rx_buf[12];
		ch0x0->hi91.acc[0] =  *((float *)&ch0x0->buffer);
		ch0x0->buffer = (ch0x0->dma_rx_buf[19] << 24) + (ch0x0->dma_rx_buf[18] << 16) + (ch0x0->dma_rx_buf[17] << 8) + ch0x0->dma_rx_buf[16];
		ch0x0->hi91.acc[1] =  *((float *)&ch0x0->buffer);
		ch0x0->buffer = (ch0x0->dma_rx_buf[23] << 24) + (ch0x0->dma_rx_buf[22] << 16) + (ch0x0->dma_rx_buf[21] << 8) + ch0x0->dma_rx_buf[20];
		ch0x0->hi91.acc[2] =  *((float *)&ch0x0->buffer);
		ch0x0->buffer = (ch0x0->dma_rx_buf[27] << 24) + (ch0x0->dma_rx_buf[26] << 16) + (ch0x0->dma_rx_buf[25] << 8) + ch0x0->dma_rx_buf[24];
		ch0x0->hi91.gyr[0] =  *((float *)&ch0x0->buffer);
		ch0x0->buffer = (ch0x0->dma_rx_buf[31] << 24) + (ch0x0->dma_rx_buf[30] << 16) + (ch0x0->dma_rx_buf[29] << 8) + ch0x0->dma_rx_buf[28];
		ch0x0->hi91.gyr[1] =  *((float *)&ch0x0->buffer);
		ch0x0->buffer = (ch0x0->dma_rx_buf[35] << 24) + (ch0x0->dma_rx_buf[34] << 16) + (ch0x0->dma_rx_buf[33] << 8) + ch0x0->dma_rx_buf[32];
		ch0x0->hi91.gyr[2] =  *((float *)&ch0x0->buffer);
		ch0x0->buffer = (ch0x0->dma_rx_buf[39] << 24) + (ch0x0->dma_rx_buf[38] << 16) + (ch0x0->dma_rx_buf[37] << 8) + ch0x0->dma_rx_buf[36];
		ch0x0->hi91.mag[0] =  *((float *)&ch0x0->buffer);
		ch0x0->buffer = (ch0x0->dma_rx_buf[43] << 24) + (ch0x0->dma_rx_buf[42] << 16) + (ch0x0->dma_rx_buf[41] << 8) + ch0x0->dma_rx_buf[40];
		ch0x0->hi91.mag[1] =  *((float *)&ch0x0->buffer);
		ch0x0->buffer = (ch0x0->dma_rx_buf[47] << 24) + (ch0x0->dma_rx_buf[46] << 16) + (ch0x0->dma_rx_buf[45] << 8) + ch0x0->dma_rx_buf[44];
		ch0x0->hi91.mag[2] =  *((float *)&ch0x0->buffer);
		ch0x0->buffer = (ch0x0->dma_rx_buf[51] << 24) + (ch0x0->dma_rx_buf[50] << 16) + (ch0x0->dma_rx_buf[49] << 8) + ch0x0->dma_rx_buf[48];
		ch0x0->hi91.roll =  *((float *)&ch0x0->buffer);
		ch0x0->buffer = (ch0x0->dma_rx_buf[55] << 24) + (ch0x0->dma_rx_buf[54] << 16) + (ch0x0->dma_rx_buf[53] << 8) + ch0x0->dma_rx_buf[52];
		ch0x0->hi91.pitch = *((float *)&ch0x0->buffer);
		ch0x0->buffer = (ch0x0->dma_rx_buf[59] << 24) + (ch0x0->dma_rx_buf[58] << 16) + (ch0x0->dma_rx_buf[57] << 8) + ch0x0->dma_rx_buf[56];
		ch0x0->hi91.yaw = *((float *)&ch0x0->buffer);
		ch0x0->buffer = (ch0x0->dma_rx_buf[63] << 24) + (ch0x0->dma_rx_buf[62] << 16) + (ch0x0->dma_rx_buf[61] << 8) + ch0x0->dma_rx_buf[60];
		ch0x0->hi91.quat[0] = *((float *)&ch0x0->buffer);
		ch0x0->buffer = (ch0x0->dma_rx_buf[67] << 24) + (ch0x0->dma_rx_buf[66] << 16) + (ch0x0->dma_rx_buf[65] << 8) + ch0x0->dma_rx_buf[64];
		ch0x0->hi91.quat[1] = *((float *)&ch0x0->buffer);
		ch0x0->buffer = (ch0x0->dma_rx_buf[71] << 24) + (ch0x0->dma_rx_buf[70] << 16) + (ch0x0->dma_rx_buf[69] << 8) + ch0x0->dma_rx_buf[68];
		ch0x0->hi91.quat[2] = *((float *)&ch0x0->buffer);
		ch0x0->buffer = (ch0x0->dma_rx_buf[75] << 24) + (ch0x0->dma_rx_buf[74] << 16) + (ch0x0->dma_rx_buf[73] << 8) + ch0x0->dma_rx_buf[72];
		ch0x0->hi91.quat[3] = *((float *)&ch0x0->buffer);
		ch0x0_crc16(&ch0x0->crc, ch0x0->dma_rx_buf, ch0x0->data_length);
		if(ch0x0->crc == ch0x0->checksum){
			ch0x0->data = ch0x0->hi91;
		}
		ch0x0->ch0x0rxstate = CH0X0_HEADER_PENDING;
		HAL_UART_Receive_DMA(ch0x0->huartx, ch0x0->dma_rx_buf, 1);
		break;
	}
}

void ch0x0_crc16(uint16_t *inital, const uint8_t *buf, uint32_t len)
{
    uint32_t crc = *inital;
    uint32_t j;
    for (j=0; j < len; ++j)
    {
        uint32_t i;
        uint32_t byte = buf[j];
        crc ^= byte << 8;
        for (i = 0; i < 8; ++i)
        {
            uint32_t temp = crc << 1;
            if (crc & 0x8000)
            {
                temp ^= 0x1021;
            }
            crc = temp;
        }
    }
    *inital = crc;
}

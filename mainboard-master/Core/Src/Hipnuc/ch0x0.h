/*
 * ch010.h
 *
 *  Created on: Sep 2, 2024
 *      Author: clement
 */

#ifndef SRC_HIPNUC_CH0X0_H_
#define SRC_HIPNUC_CH0X0_H_

#define UART_RX_BUF_SIZE        (1024)
#define LOG_STRING_SIZE         (512)
#define CH_HDR_SIZE             (0x06)          /* CHAOHE protocol header size */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../BIOS/bios.h"

enum{
	CH0X0_HEADER_PENDING,
	CH0X0_FTYPE_PENDING,
	CH0X0_DATAL_PENDING,
	CH0X0_CHECKSUM_PENDING,
	CH0X0_READY,
	CH0X0_CHECK_PENDING,
};

typedef struct __attribute__((__packed__))
{
    uint8_t         tag; /* data packet tag, if tag = 0x00, means that this packet is null */
    int8_t          temp;
    float           prs;
    uint32_t        ts; /* timestamp */
    float           acc[3];
    float           gyr[3];
    float           mag[3];
    float           roll;
    float           pitch;
    float           yaw;
    float           quat[4];
} hi91_t;


typedef struct{
	uint8_t dma_rx_buf[100];
    hi91_t hi91, data;
    UART_HandleTypeDef* huartx;
    int ch0x0rxstate;
    uint16_t data_length;
    uint16_t checksum;
    uint32_t buffer;
    uint16_t crc;
}ch0x0_t;


ch0x0_t ch010;

void ch0x0Init(ch0x0_t *ch0x0, UART_HandleTypeDef* huartx);
void ch0x0Handler(ch0x0_t *ch0x0);
void ch0x0_crc16(uint16_t *inital, const uint8_t *buf, uint32_t len);

#endif /* SRC_HIPNUC_CH0X0_H_ */

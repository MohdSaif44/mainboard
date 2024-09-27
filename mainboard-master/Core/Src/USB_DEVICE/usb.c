/*
 * usb.c
 *
 *  Created on: Sep 9, 2023
 *      Author: Adam Amer
 */
#include "usb.h"
#include "../adapter.h"
#include "usbd_cdc_if.h"


/*
 * Function Name		: 	usbParserinit
 * Function Description : 	Called to init usb out-going msg
 * Function Remarks		: 	called once before start to send
 * Function Arguments	: 	None
 * Function Return		: 	None
 * Function Example		: 	usbParserinit);
 */
void usbParserinit(void) {
//	out_usbmsg.header = 0x9524;
//	for (size_t i = 0; i < 14; i++) {
//		out_usbmsg.payload[i] = 0.0000;
//	}
//	out_usbmsg.flags = 0x00000000;
//	out_usbmsg.footer = 0x2004;
}

/*
 * Function Name		: 	usbHandler
 * Function Description : 	called to copy received buffer into in_usbmsg and then send the out msg
 * Function Remarks		: 	preferably called inside its own task and receive
 * 							the data from the callback function through a freeRtos buffer
 * Function Arguments	: 	Buf			array buffer received from the callback
 * Function Return		: 	None
 * Function Example		: 	usbHandler
 */
void usbHandler(uint8_t *Buf) {
//	in_usbmsg.header = (Buf[0] << 8) + Buf[1];
//	memcpy(in_usbmsg.buffer, &Buf[2], sizeof(in_usbmsg.buffer));
//	in_usbmsg.flags = (Buf[61] << 24) | (Buf[60] << 16) | (Buf[59] << 8)
//			| Buf[58];
//	in_usbmsg.footer = (Buf[62] << 8) + Buf[63];
//
//	usbSend(out_usbmsg);
////	usbSend(in_usbmsg);

}


/*
 * Function Name		: 	usbSend
 * Function Description : 	called to send a usb msg
 * Function Arguments	: 	Buf			array buffer received from the callback
 * Function Return		: 	None
 * Function Example		: 	usbHandler
 */
void usbSend(usb_msg_t out_msg) {
	usbBuf[0] = (uint8_t) ((out_msg.header >> 8) & 0xFF);
	usbBuf[1] = (uint8_t) (out_msg.header & 0xFF);
	memcpy(&usbBuf[2], out_msg.buffer, sizeof(out_msg.buffer));

	usbBuf[58] = (uint8_t) (out_msg.flags & 0xFF);
	usbBuf[59] = (uint8_t) ((out_msg.flags >> 8) & 0xFF);
	usbBuf[60] = (uint8_t) ((out_msg.flags >> 16) & 0xFF);
	usbBuf[61] = (uint8_t) ((out_msg.flags >> 24) & 0xFF);

	usbBuf[62] = (uint8_t) ((out_msg.footer >> 8) & 0xFF);
	usbBuf[63] = (uint8_t) (out_msg.footer & 0xFF);

	CDC_Transmit_FS(usbBuf, sizeof(usbBuf));
}

char format[] = "H: %X m1: %.3f m2: %.3f m3: %.3f flags: %X f: %X\r\n";
//char format[] =
//		"m1: %.3f m2: %.3f m3: %.3f m4: %.3f m5: %.3f m6: %.3f m7: %.3f flags: %X\r\n";

void usbPrint(void) {

//	sprintf(usbdata, format, in_usbmsg.payload[motor1], in_usbmsg.payload[motor2],
//			in_usbmsg.payload[motor3], in_usbmsg.payload[motor4],
//			in_usbmsg.payload[motor5], in_usbmsg.payload[motor6],
//			in_usbmsg.payload[motor7], in_usbmsg.flags);

//	sprintf(usbdata, format,
//				out_usbmsg.motors[motor1], out_usbmsg.motors[motor2],
//				out_usbmsg.motors[motor3], out_usbmsg.motors[motor4],
//				fxPos, fyPos,fyaw,frad);
//	HAL_UART_Transmit_IT(&huart5, (uint8_t*) usbdata, strlen(usbdata));

}

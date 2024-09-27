/*
 * usb.h
 *
 *  Created on: Sep 9, 2023
 *      Author: Adam Amer
 */

#ifndef INC_USB_H_
#define INC_USB_H_

#include "../Src/BIOS/system.h"
#include "FreeRTOS.h"
#include "message_buffer.h"

/*********************************************************************
 * incoming usb flags
 *********************************************************************/
//#define USB_FLAG0 in_usbmsg.flags & (1 << 0)
//#define USB_FLAG1 in_usbmsg.flags & (1 << 1)
//#define USB_FOUND_BALL_FLAG (in_usbmsg.flags & (1 << 2))
//#define USB_NEAR_TARGET in_usbmsg.flags & (1 << 3)
//#define USB_SHOOTING_ENDED in_usbmsg.flags & (1 << 4)
//#define USB_PATHPLAN_ENDED in_usbmsg.flags & (1 << 5)
//#define USB_FLAG6 in_usbmsg.flags & (1 << 6)
//#define USB_FLAG7 in_usbmsg.flags & (1 << 7)
//#define USB_FLAG8 in_usbmsg.flags & (1 << 8)
//#define USB_FLAG9 in_usbmsg.flags & (1 << 9)
//#define USB_FLAG10 in_usbmsg.flags & (1 << 10)
//#define USB_FLAG11 in_usbmsg.flags & (1 << 11)
//#define USB_FLAG12 in_usbmsg.flags & (1 << 12)
//#define USB_FLAG13 in_usbmsg.flags & (1 << 13)
//#define USB_FLAG14 in_usbmsg.flags & (1 << 14)
//#define USB_FLAG15 in_usbmsg.flags & (1 << 15)
//#define USB_FLAG16 in_usbmsg.flags & (1 << 16)
//#define JOY_A in_usbmsg.flags & (1 << 17)
//#define JOY_B in_usbmsg.flags & (1 << 18)
//#define JOY_X in_usbmsg.flags & (1 << 19)
//#define JOY_Y in_usbmsg.flags & (1 << 20)
//#define JOY_LB (in_usbmsg.flags & (1 << 21))
//#define JOY_RB in_usbmsg.flags & (1 << 22)
//#define JOY_SELECT in_usbmsg.flags & (1 << 23)
//#define JOY_START in_usbmsg.flags & (1 << 24)
//#define JOY_POWER in_usbmsg.flags & (1 << 25)
//#define JOY_L3 in_usbmsg.flags & (1 << 26)
//#define JOY_R3 in_usbmsg.flags & (1 << 27)
//#define JOY_DPAD_UP in_usbmsg.flags & (1 << 28)
//#define JOY_DPAD_DOWN in_usbmsg.flags & (1 << 29)
//#define JOY_DPAD_RIGHT in_usbmsg.flags & (1 << 30)
//#define JOY_DPAD_LEFT in_usbmsg.flags & (1 << 31)

/*********************************************************************
 * outgoing usb flags
 *********************************************************************/
//#define USB_START_PICKING 	out_usbmsg.flags |=  (1<<(11))
//#define USB_STOP_PICKING 	out_usbmsg.flags &= ~(1 << (11))
//#define USB_START_SHOOTING	out_usbmsg.flags |=  (1<<(12))
//#define USB_STOP_SHOOTING 	out_usbmsg.flags &= ~(1 << (12))
//#define USB_START_PP		out_usbmsg.flags |=  (1<<(13))
//#define USB_STOP_PP			out_usbmsg.flags &= ~(1 << (13))



typedef
	 struct {
		uint16_t header;
		union {
		  float payload[14];
		  uint8_t buffer[56];
		};
		uint32_t flags;
		uint16_t footer;
}usb_msg_t;

enum{
	USB_ENC_X,
	USB_ENC_Y,
	USB_OUT_X,
	USB_OUT_Y,
	USB_SERVO,
	USB_LSR_XL,
	USB_LSR_Y,
	USB_LSR_XR,
	JOY_LEFT_X,
	JOY_LEFT_Y,
	JOY_LT,
	JOY_RIGHT_X,
	JOY_RIGHT_Y,
	JOY_RT
};


char usbdata[64];
uint8_t usbBuf[64];
uint8_t ack;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

MessageBufferHandle_t usbrxBuffer;
MessageBufferHandle_t usbflagsBuffer;

void usbParserinit(void);
void usbHandler(uint8_t *Buf);
void usbSend(usb_msg_t out_msg);
void usbPrint(void);

#endif /* INC_USB_H_ */

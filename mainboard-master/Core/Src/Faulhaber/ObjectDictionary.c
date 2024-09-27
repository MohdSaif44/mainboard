///*
//************************************************************************************************************************
//*                                                   CONVERTED CODE
//*
//*                                This code is converted from uCOS-III template code
//*
//************************************************************************************************************************
//*/
///*
//************************************************************************************************************************
//*
//*                                               MODULE - Data Logger
//*
//*                                                   STM32F407VGT6
//*                                                        on
//*                                              UTMRBC Mainboard v2.1.1
//*
//* Filename      : dtlog.c
//* Version       : V1.00
//* Programmer(s) : HW
//* Note(s)       : (1) The format of the data packet:
//*
//************************************************************************************************************************
//*/
//
///*
//************************************************************************************************************************
//*                                                   INCLUDE FILES
//************************************************************************************************************************
//*/
//#include "ObjectDictionary.h"
//
///*
//************************************************************************************************************************
//*                                                       DEFINES
//************************************************************************************************************************
//*/
//
///*
//************************************************************************************************************************
//*                                                   LOCAL CONSTANTS
//************************************************************************************************************************
//*/
//                                                            /* pass buff length to bsp uart                           */
//
///*
//************************************************************************************************************************
//*                                                      CONSTANTS
//************************************************************************************************************************
//*/
//
//
//
///*
//************************************************************************************************************************
//*                                                      DATA TYPES
//*
//* Note(s)     : (1) There is no support for string, as it should stay in msglog
//************************************************************************************************************************
//*/
//
///*
//************************************************************************************************************************
//*                                                     CUSTOM AREA
//*                               (TODO: Please edit here, mod_dtlog.h, and also client side)
//*
//* Note(s)     : (1) The name is best to be less than 10 words, excessive character will be ignored.
//************************************************************************************************************************
//*/
//
//OD_t  READ_WRITE_OBJECT_DICITONARY_LIST [] =
//{
//   /* INDEX   SUBINDEX   NAME    			 	   TYPE   */
//    { 0X1005, 	0X00, 	"COB-ID-SYNC",    		   U32 },
//    { 0X100C, 	0X00, 	"GUARD-TIME" ,    		   U16 },      /* Monitoring time for node guarding in ms. The value 0 switches node guarding off*/
//    { 0X100D, 	0X00, 	"LIFE-TIME-FACTOR",        U08 },      /* Time factor for node guarding                       */
//    { 0X1010,   0X01,   "SAVE-ALL-PARAM"  ,        U32 },
//    { 0X1010,   0X02,   "SAVE-COM-PARAM"  ,        U32 },      /*                                                        */
//    { 0X1010,   0X03,   "SAVE-APP-PARAM"  ,        U32 },      /*                                                        */
//    { 0X1016,   0X01,   "CONSUMER-HEARTBEAT",      U32 },      /*                                                        */
//    { 0X1017,   0X00,   "PRODUCER-HEARBEAT-TIME",  U16 },      /*                                                        */
//    { 0X1029,   0X01,   "COMMUNICATION-ERROR",     U08 },      /*                                                        */
//    { 0X1400,   0X01,   "COB-ID-RXPDO1",           U32 },      /* CAN OBJECT IDENTIFIER FOR RXPDO1 SERVERS               */
//    { 0X1400,   0X02,   "RXPDO1-TX-TYPE",          U08 },      /* PDO TRANSFER TYPE                                      */
//    { 0X1401,   0X01,   "COB-ID-RXPDO2",           U32 },      /* CAN OBJECT IDENTIFIER FOR RXPDO2 SERVERS               */
//    { 0X1401,   0X02,   "RXPDO2-TX-TYPE",          U08 },      /* PDO TRANSFER TYPE                                      */
//    { 0X1402,   0X01,   "COB-ID-RXPDO3",           U32 },      /* CAN OBJECT IDENTIFIER FOR RXPDO3 SERVERS               */
//    { 0X1402,   0X02,   "RXPDO3-TX-TYPE",          U08 },      /* PDO TRANSFER TYPE                                      */
//    { 0X1403,   0X01,   "COB-ID-RXPDO4",           U32 },      /* CAN OBJECT IDENTIFIER FOR RXPDO4 SERVERS               */
//    { 0X1403,   0X02,   "RXPDO4-TX-TYPE",          U08 },      /* PDO TRANSFER TYPE                                      */
//    { 0X1600,   0X01,   "RXPDO1-MAPPING-ENTRY1",   U32 },      /* DEFAULT: POINTER TO 16 BIT CONTROLWORD 0X6040          */
//    { 0X1600,   0X02,   "RXPDO1-MAPPING-ENTRY2",   U32 },      /*                                                        */
//    { 0X1600,   0X03,   "RXPDO1-MAPPING-ENTRY3",   U32 },      /*                                                        */
//    { 0X1600,   0X04,   "RXPDO1-MAPPING-ENTRY4",   U32 },      /*                                                        */
//    { 0X1601,   0X01,   "RXPDO2-MAPPING-ENTRY1",   U32 },      /* DEFAULT: POINTER TO 16 BIT CONTROLWORD 0X6040          */
//    { 0X1601,   0X02,   "RXPDO2-MAPPING-ENTRY2",   U32 },      /* DEFAULT: POINTER TO 16 BIT CONTROLWORD 0X607A          */
//    { 0X1601,   0X03,   "RXPDO2-MAPPING-ENTRY3",   U32 },      /*                                                        */
//    { 0X1601,   0X04,   "RXPDO2-MAPPING-ENTRY4",   U32 },      /*                                                        */
//    { 0X1602,   0X01,   "RXPDO3-MAPPING-ENTRY1",   U32 },      /* DEFAULT: POINTER TO 16 BIT CONTROLWORD 0X6040          */
//    { 0X1602,   0X02,   "RXPDO3-MAPPING-ENTRY2",   U32 },      /* DEFAULT: POINTER TO 16 BIT CONTROLWORD 0X60FF          */
//    { 0X1602,   0X03,   "RXPDO3-MAPPING-ENTRY3",   U32 },      /*                                                        */
//    { 0X1602,   0X04,   "RXPDO3-MAPPING-ENTRY4",   U32 },      /*                                                        */
//    { 0X1603,   0X01,   "RXPDO4-MAPPING-ENTRY1",   U32 },      /* DEFAULT: POINTER TO 16 BIT CONTROLWORD 0X6040          */
//    { 0X1603,   0X02,   "RXPDO4-MAPPING-ENTRY2",   U32 },      /* DEFAULT: POINTER TO 32 BIT 0x257A                      */
//    { 0X1603,   0X03,   "RXPDO4-MAPPING-ENTRY3",   U32 },      /*                                                        */
//    { 0X1603,   0X04,   "RXPDO4-MAPPING-ENTRY4",   U32 },      /*                                                        */
//    { 0X1800,   0X01,   "COB-ID-TXPDO1",           U32 },      /* CAN OBJECT ID FOR TXPDO1                               */
//    { 0X1800,   0X02,   "TXPDO1-TX-TYPE",          U08 },      /* PDO TRANSFER TYPE                                      */
//    { 0X1800,   0X05,   "TXPDO1-TX-EVENT-TIMER",   U16 },      /* MINIMUM TIME IN MS BETWEEN TXPDO TRANSMISSION          */
//    { 0X1801,   0X01,   "COB-ID-TXPDO2",           U32 },      /* CAN OBJECT ID FOR TXPDO2                               */
//    { 0X1801,   0X02,   "TXPDO2-TX-TYPE",          U08 },      /* PDO TRANSFER TYPE                                      */
//    { 0X1801,   0X05,   "TXPDO2-TX-EVENT-TIMER",   U16 },      /* MINIMUM TIME IN MS BETWEEN TXPDO TRANSMISSION          */
//    { 0X1802,   0X01,   "COB-ID-TXPDO3",           U32 },      /* CAN OBJECT ID FOR TXPDO3                               */
//    { 0X1802,   0X02,   "TXPDO3-TX-TYPE",          U08 },      /* PDO TRANSFER TYPE                                      */
//    { 0X1802,   0X05,   "TXPDO3-TX-EVENT-TIMER",   U16 },      /* MINIMUM TIME IN MS BETWEEN TXPDO TRANSMISSION          */
//    { 0X1803,   0X01,   "COB-ID-TXPDO4",           U32 },      /* CAN OBJECT ID FOR TXPDO4                               */
//    { 0X1803,   0X02,   "TXPDO4-TX-TYPE",          U08 },      /* PDO TRANSFER TYPE                                      */
//    { 0X1803,   0X05,   "TXPDO4-TX-EVENT-TIMER",   U16 },      /* MINIMUM TIME IN MS BETWEEN TXPDO TRANSMISSION          */
//    { 0X1A00,   0X01,   "TXPDO1-MAPPING-ENTRY1",   U32 },      /* DEFAULT: POINTER TO 16 BIT CONTROLWORD 0X6040          */
//    { 0X1A00,   0X02,   "TXPDO1-MAPPING-ENTRY2",   U32 },      /*                                                        */
//    { 0X1A00,   0X03,   "TXPDO1-MAPPING-ENTRY3",   U32 },      /*                                                        */
//    { 0X1A00,   0X04,   "TXPDO1-MAPPING-ENTRY4",   U32 },      /*                                                        */
//    { 0X1A01,   0X01,   "TXPDO2-MAPPING-ENTRY1",   U32 },      /* DEFAULT: POINTER TO 16 BIT CONTROLWORD 0X6040          */
//    { 0X1A01,   0X02,   "TXPDO2-MAPPING-ENTRY2",   U32 },      /* POINTER TO 32 BIT POS ACTUAL VALUE 0X6064              */
//    { 0X1A01,   0X03,   "TXPDO2-MAPPING-ENTRY3",   U32 },      /*                                                        */
//    { 0X1A01,   0X04,   "TXPDO2-MAPPING-ENTRY4",   U32 },      /*                                                        */
//    { 0X1A02,   0X01,   "TXPDO3-MAPPING-ENTRY1",   U32 },      /* DEFAULT: POINTER TO 16 BIT CONTROLWORD 0X6040          */
//    { 0X1A02,   0X02,   "TXPDO3-MAPPING-ENTRY2",   U32 },      /* POINTER TO 32 BIT VEL ACTUAL VALUE 0X606C              */
//    { 0X1A02,   0X03,   "TXPDO3-MAPPING-ENTRY3",   U32 },      /*                                                        */
//    { 0X1A02,   0X04,   "TXPDO3-MAPPING-ENTRY4",   U32 },      /*                                                        */
//    { 0X1A03,   0X01,   "TXPDO4-MAPPING-ENTRY1",   U32 },      /* DEFAULT: POINTER TO 16 BIT CONTROLWORD 0X6040          */
//    { 0X1A03,   0X02,   "TXPDO4-MAPPING-ENTRY2",   U32 },      /* POINTER TO 32 BIT VEL ACTUAL VALUE 0X7077              */
//    { 0X1A03,   0X03,   "TXPDO4-MAPPING-ENTRY3",   U32 },      /*                                                        */
//    { 0X1A03,   0X04,   "TXPDO4-MAPPING-ENTRY4",   U32 },      /*                                                        */
//    { 0X2330,   0X01,   "SAMPLING-RATE",           U16 },      /* SAMPLING RATE FACTOR                                   */
//    { 0X2330,   0X02,   "GAIN-SCHEDULING",         U16 },      /* 1:REDUCED CONTROLLER GAIN IN TARGET CORRIDOR DURING POSITIONING*/
//    { 0X2331,   0X01,   "PROPORTIONAL-TERM-POR",   U16 },      /* PROPORTIONAL GAIN OF VELOCITY CONTROLLER               */
//    { 0X2331,   0X02,   "INTEGRAL TERM I",         U16 },      /* INTEGRAL TERM OF VELOCITY CONTROLLER                   */
//    { 0X2332,   0X01,   "PROPORTIONAL TERM PP",    U16 },      /* PROPORTIONAL GAIN OF POSITION CONTROLLER               */
//    { 0X2332,   0X02,   "DERIVATIVE TERM PD",      U16 },      /* DIFFEREINTIAL TERM OF POSITION CONTROLLER              */
//    { 0X2333,   0X01,   "CONTINUOUS I LIMIT",      U16 },      /* CONTINUOUS CURRENT LIMIT[mA]                           */
//    { 0X2333,   0X02,   "PEAK I LIMIT",            U16 },      /* PEAK CURRENT LIMIT[mA]                                 */
//    { 0X2333,   0X03,   "INTEGRAL TERM CI",        U16 },      /* INTEGRAL TERM OF CURRENT CONTROLLER                    */
//    { 0X2338,   0X01,   "PURE SINUS COMMUTATION",  U16 },      /* 0:FULL MODULATION   1:LIMITATION TO SINUS FORM         */
//    { 0X2338,   0X02,   "ACT POS LIM IN VEL MODE", U16 },      /* 1:ALSO USE POSITIONING LIMIT IN VELOCITY MODE          */
//    { 0X2338,   0X03,   "ACT POS LIM IN POS MODE", U16 },      /* 0:NO RANGE LIMITS IN POSITIONING MODE                  */
//    { 0X2350,   0X01,   "SPEED CONSTANT KN",       U16 },      /* SPEED CONSTANT                                         */
//    { 0X2350,   0X02,   "TERMINAL RESISTANCE RM",  U16 },      /* CONNECTION RESISTANCE                                  */
//    { 0X2350,   0X03,   "POLE NUMBER",             U16 },      /* MAGNETIC POLAR PITCH OF FORCER ROD                     */
//    { 0X2350,   0X05,   "THERMAL TIME CONSTANT",   U16 },      /* THERMAL TIME CONSTANT 1                                */
//    { 0X2351,   0X01,   "SENSOR TYPE",             U08 },      /* 0:ANALOG HALL 1:INCREMENTAL ENC 10:INCREMENTAL+HALL    */
//    { 0X2351,   0X02,   "RESOLUTION EXT ENC",      U32 },      /* 4-EDGE RESOLUTION OF AN EXTERNALLY CONNECTED INCREMENTAL ENC PER MM*/
//    { 0X2562,   0X00,   "POSITION DEMAND INTE         },      /*                                                        */
//
//
//
//};
//OD_t  READ_ONLY_OBJECT_DICITONARY_LIST [] =
//{
//   /* INDEX   SUBINDEX   NAME    			 	   TYPE   */
//    { 0X2351, 	0X03, 	"RESOLUTION INT ENC",      U32 },      /* RESOLUTION OF INTERNAL HALL SENSOR ENCODER PER TM      */
//    { 0X2361,   0X00,   "VEL ACT VAL UNFILTERED",  I16 },      /* UNFILTERED ACTUAL VELOCITY                             */
//    {                                                  },      /*                                                        */
//}
///*
//************************************************************************************************************************
//*                                                   LOCAL VARIABLES
//************************************************************************************************************************
//*/
//
///*
//************************************************************************************************************************
//*                                                   LOCAL FUNCTION
//************************************************************************************************************************
//*/
//
//
///*
//************************************************************************************************************************
//*                                                    DTlog Tx Task
//*
//* Description : Send a frame of variables raw data whenever data is ready
//* Argument(s) : p_arg   arg of a OSTask, not used
//* Return(s)   : none.
//* Caller(s)   : APP.
//************************************************************************************************************************
//*/
//

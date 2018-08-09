/*! @file radio.h
 * @brief This file is contains the public radio interface functions.
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2012 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */

#ifndef RADIO_H_
#define RADIO_H_

#include "main.h"
/*****************************************************************************
 *  Global Macros & Definitions
 *****************************************************************************/
/*! Maximal packet length definition (FIFO size) */
#define RADIO_MAX_PACKET_LENGTH     64u

/*****************************************************************************
 *  Global Typedefs & Enums
 *****************************************************************************/
typedef struct
{
    const U8   *Radio_ConfigurationArray;

    U8   Radio_ChannelNumber;
    U8   Radio_PacketLength;
    U8   Radio_State_After_Power_Up;

    U16  Radio_Delay_Cnt_After_Reset;
} tRadioConfiguration;

/*****************************************************************************
 *  Global Variable Declarations
 *****************************************************************************/
//extern const SEGMENT_VARIABLE_SEGMENT_POINTER(pRadioConfiguration, tRadioConfiguration, SEG_CODE, SEG_CODE);
//extern SEGMENT_VARIABLE(fixRadioPacket[RADIO_MAX_PACKET_LENGTH], U8, SEG_XDATA);
extern U8 fixRadioPacket[RADIO_MAX_PACKET_LENGTH];
extern const tRadioConfiguration *pRadioConfiguration;

/*! Si446x configuration array */
//extern const SEGMENT_VARIABLE(Radio_Configuration_Data_Array[], U8, SEG_CODE);
extern const U8 Radio_Configuration_Data_Array[];

/*****************************************************************************
 *  Global Function Declarations
 *****************************************************************************/
void  vRadio_Init(void);
BIT   gRadio_CheckReceived(void);
void  vRadio_StartRX(U8);
U8    bRadio_Check_Ezconfig(U16);

#endif /* RADIO_H_ */

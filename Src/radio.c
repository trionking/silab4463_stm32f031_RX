/*! @file radio.c
 * @brief This file contains functions to interface with the radio chip.
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2012 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */

#include "main.h"
#include "stm32f0xx_hal.h"
#include "radio.h"
#include "radio_config.h"
#include "radio_hal.h"
#include "radio_comm.h"
#include "si446x_api_lib.h"
#include "si446x_cmd.h"

/*****************************************************************************
 *  Local Macros & Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Global Variables
 *****************************************************************************/
const uint8_t Radio_Configuration_Data_Array[] = RADIO_CONFIGURATION_DATA_ARRAY;
const tRadioConfiguration RadioConfiguration = RADIO_CONFIGURATION_DATA;
const tRadioConfiguration *pRadioConfiguration = &RadioConfiguration;

U8 fixRadioPacket[RADIO_MAX_PACKET_LENGTH];

/*****************************************************************************
 *  Local Function Declarations
 *****************************************************************************/
void vRadio_PowerUp(void);

/*!
 *  Power up the Radio.
 *
 *  @note
 *
 */
void vRadio_PowerUp(void)
{
  //SEGMENT_VARIABLE(wDelay,  U16, SEG_XDATA) = 0u;
//  U16 wDelay = 0u;

  /* Hardware reset the chip */
  si446x_reset();

  /* Wait until reset timeout or Reset IT signal */
	HAL_Delay(500);
  //for (; wDelay < pRadioConfiguration->Radio_Delay_Cnt_After_Reset; wDelay++);
}

/*!
 *  Radio Initialization.
 *
 *  @author Sz. Papp
 *
 *  @note
 *
 */
void vRadio_Init(void)
{
//  U16 wDelay;
	U8 ST_Rtn;

  /* Power Up the radio chip */
  vRadio_PowerUp();
	si446x_get_int_status(0, 0, 0);

  /* Load radio configuration */
  while (1)
  {
		ST_Rtn = si446x_configuration_init(pRadioConfiguration->Radio_ConfigurationArray);
		if (ST_Rtn == SI446X_SUCCESS)
			break;
    /* Error hook */
		HAL_GPIO_TogglePin(LD_OUT_GPIO_Port,LD_OUT_Pin);
		HAL_Delay(500);

    /* Power Up the radio chip */
    vRadio_PowerUp();
  }

  // Read ITs, clear pending ones
  si446x_get_int_status(0u, 0u, 0u);
}

/*!
 *  Check if Packet received IT flag is pending.
 *
 *  @return   TRUE - Packet successfully received / FALSE - No packet pending.
 *
 *  @note
 *
 */
BIT gRadio_CheckReceived(void)
{
  U8 lCnt;
  if (radio_hal_NirqLevel() == GPIO_PIN_RESET)
  {
    /* Read ITs, clear pending ones */
    si446x_get_int_status(0u, 0u, 0u);

    /* check the reason for the IT */
    if (Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PH_PEND_PACKET_RX_PEND_BIT)
    {
      /* Packet RX */
      si446x_read_rx_fifo(RadioConfiguration.Radio_PacketLength, (U8 *) &fixRadioPacket[0u]);

#ifdef UART_LOGGING_SUPPORT
      /* Send it to UART */
      for (lCnt = 0u; lCnt < RadioConfiguration.Radio_PacketLength; lCnt++)
      {
        printf("%c",(*((U8 *) &fixRadioPacket[0u] + lCnt)));
      }
      printf("\n");
#endif

      return TRUE;
    }

    if (Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PH_STATUS_CRC_ERROR_BIT)
    {
      /* Reset FIFO */
      si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_FIFO_RX_BIT);
    }
  }

  return FALSE;
}

/*!
 *  Set Radio to RX mode, fixed packet length.
 *
 *  @param channel Freq. Channel
 *
 *  @note
 *
 */
void vRadio_StartRX(U8 channel)
{
  // Read ITs, clear pending ones
  si446x_get_int_status(0u, 0u, 0u);

  /* Start Receiving packet, channel 0, START immediately, Packet n bytes long */
  si446x_start_rx(channel, 0u, RadioConfiguration.Radio_PacketLength,
                  SI446X_CMD_START_RX_ARG_NEXT_STATE1_RXTIMEOUT_STATE_ENUM_NOCHANGE,
                  SI446X_CMD_START_RX_ARG_NEXT_STATE2_RXVALID_STATE_ENUM_RX,
                  SI446X_CMD_START_RX_ARG_NEXT_STATE3_RXINVALID_STATE_ENUM_RX );
                  
  /* Switch on LED1 to show RX state */
  HAL_GPIO_WritePin(LD_OUT_GPIO_Port,LD_OUT_Pin,GPIO_PIN_SET);
}

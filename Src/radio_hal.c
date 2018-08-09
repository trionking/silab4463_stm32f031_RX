/*!
 * File:
 *  radio_hal.c
 *
 * Description:
 *  This file contains RADIO HAL.
 *
 * Silicon Laboratories Confidential
 * Copyright 2011 Silicon Laboratories, Inc.
 */

                /* ======================================= *
                 *              I N C L U D E              *
                 * ======================================= */

#include "main.h"
#include "stm32f0xx_hal.h"
#include "radio_config.h"
#include "radio_hal.h"
#include "si446x_cmd.h"
#include "string.h"

extern SPI_HandleTypeDef hspi1;

                /* ======================================= *
                 *          D E F I N I T I O N S          *
                 * ======================================= */

                /* ======================================= *
                 *     G L O B A L   V A R I A B L E S     *
                 * ======================================= */

                /* ======================================= *
                 *      L O C A L   F U N C T I O N S      *
                 * ======================================= */

                /* ======================================= *
                 *     P U B L I C   F U N C T I O N S     *
                 * ======================================= */

void radio_hal_AssertShutdown(void)
{
	HAL_GPIO_WritePin(SI446x_SDN_GPIO_Port,SI446x_SDN_Pin,GPIO_PIN_SET);
}

void radio_hal_DeassertShutdown(void)
{
	HAL_GPIO_WritePin(SI446x_SDN_GPIO_Port,SI446x_SDN_Pin,GPIO_PIN_RESET);
}

void radio_hal_ClearNsel(void)
{
	HAL_GPIO_WritePin(nSPI1_CS_GPIO_Port,nSPI1_CS_Pin,GPIO_PIN_RESET);			// CS LOW
}

void radio_hal_SetNsel(void)
{
	HAL_GPIO_WritePin(nSPI1_CS_GPIO_Port,nSPI1_CS_Pin,GPIO_PIN_SET);			// CS LOW
}

BIT radio_hal_NirqLevel(void)
{
  return HAL_GPIO_ReadPin(SI446x_IRQ_GPIO_Port,SI446x_IRQ_Pin);
}

void radio_hal_SpiWriteByte(U8 byteToWrite)
{
	U8 rx_dat;
  //if (HAL_SPI_Transmit(&hspi1,&byteToWrite,1,10) != HAL_OK)
	if (HAL_SPI_TransmitReceive(&hspi1,&byteToWrite,&rx_dat,1,10) != HAL_OK)
	{
		Error_Handler();
	}
}

U8 radio_hal_SpiReadByte(void)
{
	U8 rx_dat;
	//U8 tx_dat=0xFF;
	U8 tx_dat=0;
	
	//if (HAL_SPI_Receive(&hspi1,&rx_dat,1,10) != HAL_OK)
	if (HAL_SPI_TransmitReceive(&hspi1,&tx_dat,&rx_dat,1,10) != HAL_OK)
	{
		Error_Handler();
	}
	return rx_dat;
}

void radio_hal_SpiWriteData(U8 byteCount, U8* pData)
{
	U8 rx_dat[100];
  //if (HAL_SPI_Transmit(&hspi1,pData,byteCount,100) != HAL_OK)
	if (HAL_SPI_TransmitReceive(&hspi1,pData,rx_dat,byteCount,10) != HAL_OK)
	{
		Error_Handler();
	}
}

void radio_hal_SpiReadData(U8 byteCount, U8* pData)
{
	U8 tx_dat[100];
	//memset(tx_dat,0xFF,20);
	memset(tx_dat,0,20);
	//if (HAL_SPI_Receive(&hspi1,pData,byteCount,100) != HAL_OK)
	if (HAL_SPI_TransmitReceive(&hspi1,tx_dat,pData,byteCount,10) != HAL_OK)
	{
		Error_Handler();
	}
}

#ifdef RADIO_DRIVER_EXTENDED_SUPPORT
BIT radio_hal_Gpio0Level(void)
{
  GPIO_PinState retVal = GPIO_PIN_RESET;

  return retVal;
}

BIT radio_hal_Gpio1Level(void)
{
  GPIO_PinState retVal = GPIO_PIN_RESET;

	retVal = HAL_GPIO_ReadPin(SI446x_CTS_GPIO_Port,SI446x_CTS_Pin);
  return retVal;
}

BIT radio_hal_Gpio2Level(void)
{
  GPIO_PinState retVal = GPIO_PIN_RESET;

  return retVal;
}

BIT radio_hal_Gpio3Level(void)
{
  GPIO_PinState retVal = GPIO_PIN_RESET;

  return retVal;
}

#endif

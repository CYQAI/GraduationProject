/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "tim.h"
#include "LED.h"
#include "stdio.h"
/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

/* USER CODE BEGIN Private defines */

/**
  * @brief  Uart buf Status structures definition
  */
typedef enum
{
  UART_buf_OK  = 0x00U,   /*读取UART的接收缓存成�??*/
  UART_buf_no  = 0x01U,   /*读取UART的接收缓存失败，UART的接收缓存还没有*/
  UART_Rxbuf_small  = 0x02U    /*接收数组小于UART的接收数�??*/
} UART_StatusTypeDef;


#define UART2_MAX_RECV_LEN 100

extern uint8_t UART2_RX_FinishFlag;
extern uint8_t UART2_RX_BUF[UART2_MAX_RECV_LEN];
extern uint8_t UART2_RX_Cnt;

/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);

/* USER CODE BEGIN Prototypes */
UART_StatusTypeDef Read_Uart2Buf(uint8_t *pData, uint16_t Size);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */


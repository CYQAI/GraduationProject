/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

typedef enum 
{
  run_door_task,
  run_ui_task,
  NO_run_task,
}TASK_status;

typedef struct {
uint32_t ID;
uint32_t identity;
}People;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern uint32_t password[8];
extern TASK_status task_status;
extern People people[10];
extern int people_ID_now;

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
#define FLASH_INIT 0
#define PEOPLE_MAX_COUNT  10

#define  DEBUG   1  

#if DEBUG
  #define DBG_PRINTF(fmt, ...)  \
  do\
  {\
      printf("<<File:%s  Line:%d  Function:%s>> \r\n", __FILE__, __LINE__, __FUNCTION__);\
      printf(fmt, ##__VA_ARGS__);\
  }while(0)
#else
  #define DBG_PRINTF(fmt, args...)   
#endif

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void string_Printf(uint32_t *string, uint8_t num);
void AS608_ShowErrMessage(uint8_t ensure);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY_Row1_Pin GPIO_PIN_13
#define KEY_Row1_GPIO_Port GPIOC
#define KEY_Row2_Pin GPIO_PIN_14
#define KEY_Row2_GPIO_Port GPIOC
#define KEY_Row3_Pin GPIO_PIN_15
#define KEY_Row3_GPIO_Port GPIOC
#define TFTLCD_BLK_Pin GPIO_PIN_0
#define TFTLCD_BLK_GPIO_Port GPIOA
#define TFTLCD_CS_Pin GPIO_PIN_1
#define TFTLCD_CS_GPIO_Port GPIOA
#define TFTLCD_DC_Pin GPIO_PIN_4
#define TFTLCD_DC_GPIO_Port GPIOA
#define TFTLCD_RES_Pin GPIO_PIN_5
#define TFTLCD_RES_GPIO_Port GPIOA
#define TFTLCD_SDA_Pin GPIO_PIN_6
#define TFTLCD_SDA_GPIO_Port GPIOA
#define TFTLCD_SCL_Pin GPIO_PIN_7
#define TFTLCD_SCL_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_0
#define LED2_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_1
#define LED1_GPIO_Port GPIOB
#define AS608_WAK_Pin GPIO_PIN_10
#define AS608_WAK_GPIO_Port GPIOB
#define BEEP_Pin GPIO_PIN_13
#define BEEP_GPIO_Port GPIOB
#define RELAY_Pin GPIO_PIN_14
#define RELAY_GPIO_Port GPIOB
#define KEY_Column4_Pin GPIO_PIN_5
#define KEY_Column4_GPIO_Port GPIOB
#define KEY_Column3_Pin GPIO_PIN_6
#define KEY_Column3_GPIO_Port GPIOB
#define KEY_Column2_Pin GPIO_PIN_7
#define KEY_Column2_GPIO_Port GPIOB
#define KEY_Column1_Pin GPIO_PIN_8
#define KEY_Column1_GPIO_Port GPIOB
#define KEY_Row4_Pin GPIO_PIN_9
#define KEY_Row4_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

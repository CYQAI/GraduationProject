/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED_H__
#define __LED_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

typedef enum 
{
    LED1 = 0,
    LED2 = 1,
}led_typedef_enum;

#define LEDn                             2U

void LED_Toggle(led_typedef_enum lednum);
void LED_On(led_typedef_enum lednum);
void LED_Off(led_typedef_enum lednum);

#ifdef __cplusplus
}
#endif

#endif /* __LED_H__ */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __KEY_H
#define __KEY_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "main.h"

/* Private defines -----------------------------------------------------------*/
#define KEY_RowN 4
#define KEY_ColumnN 4

typedef enum {unexist = 0, exist = !unexist} keyStatus;

typedef struct key
{
  uint8_t num;
  uint8_t flag;
}key_struct;


extern key_struct key;



uint8_t Key_Column_Scan(uint8_t row);
uint8_t Key_Scan(void);
void get_key(void);
#ifdef __cplusplus
}
#endif

#endif /* __KEY_H */

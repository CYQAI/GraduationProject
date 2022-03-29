
/* Includes ------------------------------------------------------------------*/
#include "LED.h"

/* private variables */
/*LED*/
static  GPIO_TypeDef * GPIO_PORT[LEDn]       = {LED1_GPIO_Port,
                                               LED2_GPIO_Port,
                                               };

static  uint16_t GPIO_PIN[LEDn]        = {LED1_Pin,
                                          LED2_Pin,
                                          };
/*!
    \brief      toggle selected led
    \param[in]  lednum: specify the led to be toggled
      \arg        LED0
      \arg        LED1
      \arg        LED2
    \param[out] none
    \retval     none
*/
void LED_Toggle(led_typedef_enum lednum)
{
    HAL_GPIO_TogglePin(GPIO_PORT[lednum],GPIO_PIN[lednum]);
}

void LED_On(led_typedef_enum lednum)
{
  HAL_GPIO_WritePin(GPIO_PORT[lednum], GPIO_PIN[lednum], GPIO_PIN_RESET);
}

void LED_Off(led_typedef_enum lednum)
{
  HAL_GPIO_WritePin(GPIO_PORT[lednum], GPIO_PIN[lednum], GPIO_PIN_SET);
}



/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "LED.h"
#include "lcd.h"
#include "lcd_init.h"
#include "as608.h"
#include "AS608_Func.h"
#include "KEY.h"
#include "user_interface.h"
#include "flash.h"
#include "func.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#ifdef __GNUC __
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/*前六位密码，第7位是否已初始化标志*/
uint32_t password[8] = {'1','2','3','4','5','6',0,0};
People people[PEOPLE_MAX_COUNT];
int people_ID_now = 0xaaaa;  /*0xaaaaa是没有身份的标志*/
uint32_t test[8];
TASK_status task_status;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  // char *chinese = "中景";
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  flash_init();
  MX_USART1_UART_Init();
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  LCD_Init();
	// LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	// LCD_ShowChinese(0,0,"中景",RED,WHITE,24,0);
	// LCD_ShowString(24,30,"LCD_W:",RED,WHITE,16,0);
  AS608_Init();
  init_ui();

  LCD_Fill(0,0,LCD_W,LCD_H,WHITE);

  task_status = run_door_task;  /*默认先启动这个任务*/
 

#if FLASH_INIT

  people_init();

  flash_write_password( ADDR_FLASH_PAGE_61,  ADDR_FLASH_PAGE_62,  password, COUNTOF(password) );
  HAL_Delay(200);
  flash_write_people( ADDR_FLASH_PAGE_62,  ADDR_FLASH_PAGE_63, people , COUNTOF(people));
  HAL_Delay(200);
  flash_read_password(ADDR_FLASH_PAGE_61, password, COUNTOF(password));
  HAL_Delay(200);
  flash_read_people( ADDR_FLASH_PAGE_62, people, COUNTOF(people) );
  HAL_Delay(200);

  printf("密码：");
  for (uint8_t i = 0; i < COUNTOF(password); i++)
  {
    printf("|%d",password[i]);
  }
  printf("\r\n");

  printf("people：");
  for (uint8_t i = 0; i < COUNTOF(people); i++)
  {
    printf("|ID:%d,identity:%d",people[i].ID,people[i].identity);
  }
    printf("\r\n");

  while (1)
  {
    // printf("FLASH_INIT 成功\r\n");
  }
  
#else
  
  flash_read_password(ADDR_FLASH_PAGE_61, password, COUNTOF(password));
  HAL_Delay(200);
  flash_read_people( ADDR_FLASH_PAGE_62, people, COUNTOF(people) );
  HAL_Delay(200);

  printf("密码：");
  for (uint8_t i = 0; i < COUNTOF(password); i++)
  {
    printf("|%d",password[i]);
  }
  printf("\r\n");

  printf("people：");
  for (uint8_t i = 0; i < COUNTOF(people); i++)
  {
    printf("|ID:%d,identity:%d",people[i].ID,people[i].identity);
  }
    printf("\r\n");

#endif


  first_use();

  // Add_FR();


/* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    // if(UART2_RX_FinishFlag == 1)
    // {
    //   Read_Uart2Buf((uint8_t*)test,COUNTOF(test));
    //   HAL_UART_Transmit(&huart2, (uint8_t*)test, COUNTOF(test), 0xFFFF);
    // }

    // HAL_UART_Transmit(&huart1, test, 8, 0XFFFF);
    // key_num = Key_Scan();
    get_key();
    LED_Toggle(LED1);
    HAL_Delay(200);
    printf("个数\r\n" );
    door_task();
    UI_TASK();
    
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}

void string_Printf(uint32_t *string, uint8_t num)
{
  printf("count = %d", num);

  while (num--)
  {
    printf(" |%c",*string++);
  }

  printf("\r\n");
}







/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
    // LED_Off(LED0);
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */


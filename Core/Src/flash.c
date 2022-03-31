
#include "flash.h"
#include "stdio.h"
#include "AS608_Func.h"
/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
/* Private define ------------------------------------------------------------*/

/* Uncomment this line to Enable Write Protection */
//#define WRITE_PROTECTION_ENABLE

/* Uncomment this line to Disable Write Protection */
#define WRITE_PROTECTION_DISABLE

#define FLASH_USER_START_ADDR       ADDR_FLASH_PAGE_61   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR         ADDR_FLASH_PAGE_63   /* End @ of user Flash area */
#define FLASH_PAGE_TO_BE_PROTECTED (OB_WRP_PAGES60TO63)  

#define DATA_32                     ((uint32_t)0x12345678)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t PageError = 0;

__IO TestStatus MemoryProgramStatus = PASSED;
/*Variable used to handle the Options Bytes*/
static FLASH_OBProgramInitTypeDef OptionsBytesStruct;
/*Variable used for Erase procedure*/
static FLASH_EraseInitTypeDef EraseInitStruct;
// union DATA_change_tag
// {
//   uint32_t DATA_32;
//   uint8_t DATA_8[4];
// }data;




/*
 *写flash过程：解锁→擦除→写入→读取
 *但是为了程序更稳健，在init这里加了是否有写保护的检测，如有写保护就取消
 */
void flash_init(void)
{
     
  /* Initialize test status */
  MemoryProgramStatus = PASSED;

  /* Unlock the Flash to enable the flash control register access *************/ 
  HAL_FLASH_Unlock();

  /* Unlock the Options Bytes *************************************************/
  HAL_FLASH_OB_Unlock();

  /* Get pages write protection status ****************************************/
  HAL_FLASHEx_OBGetConfig(&OptionsBytesStruct);

  #ifdef WRITE_PROTECTION_DISABLE
  /* Check if desired pages are already write protected ***********************/
  if((OptionsBytesStruct.WRPPage & FLASH_PAGE_TO_BE_PROTECTED) != FLASH_PAGE_TO_BE_PROTECTED)
  {
    /* Restore write protected pages */
    OptionsBytesStruct.OptionType   = OPTIONBYTE_WRP;
    OptionsBytesStruct.WRPState     = OB_WRPSTATE_DISABLE;
    OptionsBytesStruct.WRPPage = FLASH_PAGE_TO_BE_PROTECTED;
    if(HAL_FLASHEx_OBProgram(&OptionsBytesStruct) != HAL_OK)
    {
      /* Error occurred while options bytes programming. **********************/
      while (1)
      {
        /* Make LED2 blink (100ms on, 2s off) to indicate error in OB programming operation */
        printf("error in OB programming operation");
      }
    }

    /* Generate System Reset to load the new option byte values ***************/

    HAL_FLASH_OB_Launch();
  }
  #elif defined WRITE_PROTECTION_ENABLE
  /* Check if desired pages are not yet write protected ***********************/
  if(((~OptionsBytesStruct.WRPPage) & FLASH_PAGE_TO_BE_PROTECTED )!= FLASH_PAGE_TO_BE_PROTECTED)
  {
    /* Enable the pages write protection **************************************/
    OptionsBytesStruct.OptionType = OPTIONBYTE_WRP;
    OptionsBytesStruct.WRPState   = OB_WRPSTATE_ENABLE;
    OptionsBytesStruct.WRPPage    = FLASH_PAGE_TO_BE_PROTECTED;
    if(HAL_FLASHEx_OBProgram(&OptionsBytesStruct) != HAL_OK)
    {
      /* Error occurred while options bytes programming. **********************/
      while (1)
      {
        /* Make LED2 blink (100ms on, 2s off) to indicate error in OB programming operation */
        BSP_LED_On(LED2);
        HAL_Delay(100);
        BSP_LED_Off(LED2);
        HAL_Delay(2000);
      }
    }

    /* Generate System Reset to load the new option byte values ***************/
    HAL_FLASH_OB_Launch();
  }
  #endif /* WRITE_PROTECTION_DISABLE */


  /* Lock the Options Bytes *************************************************/
  HAL_FLASH_OB_Lock();


}

void flash_write_people(uint32_t start_address, uint32_t end_adress, People from[], uint8_t size)
{
  // uint32_t Address = 0;

    /*size数值不是4的倍数返回*/
  HAL_FLASH_Unlock();

  /* The selected pages are not write protected *******************************/
  if ((OptionsBytesStruct.WRPPage & FLASH_PAGE_TO_BE_PROTECTED) != 0x00)
  {
    /* Fill EraseInit structure************************************************/
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = start_address;
    EraseInitStruct.NbPages     = (end_adress - start_address)/FLASH_PAGE_SIZE;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
    {

      while (1)
      {
        printf("error in Erase operation"); 
      }
    }


    for(int i=0; i < size ; ++i)
    {
      if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, start_address, from[i].ID) == HAL_OK){
        start_address += 4;
      }else{
        while (1)
        {
           printf("error in Write operation");
        }
      } 
      if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, start_address, from[i].identity) == HAL_OK){
        start_address += 4;
      }else{
        while (1)
        {
           printf("error in Write operation");
        }
      }

    }

  }
  else
  { 
    printf("The selected pages are write protected");
  }

  HAL_FLASH_Lock();
}

void flash_write_password(uint32_t start_address, uint32_t end_adress, uint32_t from[], uint8_t size)
{
  // uint32_t Address = 0;

    /*size数值不是4的倍数返回*/
    if((size%4)!= 0 && sizeof(from[0])!=4)
    {
      return;
    }
    
      HAL_FLASH_Unlock();

  /* The selected pages are not write protected *******************************/
  if ((OptionsBytesStruct.WRPPage & FLASH_PAGE_TO_BE_PROTECTED) != 0x00)
  {
    /* Fill EraseInit structure************************************************/
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = start_address;
    EraseInitStruct.NbPages     = (end_adress - start_address)/FLASH_PAGE_SIZE;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
    {

      while (1)
      {
        printf("error in Erase operation"); 
      }
    }


    for(int i=0; i < size ; )
    {
      if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, start_address, from[i]) == HAL_OK)
      {
        ++i;
        start_address += 4;
      }
      else
      {
        while (1)
        {
           printf("error in Write operation");
        }
      }      
    }

  }
  else
  { 
    printf("The selected pages are write protected");
  }

  HAL_FLASH_Lock();
}


void flash_read_password(uint32_t address, uint32_t to[], uint8_t size)
{
  uint8_t i=0;
  
  for(i=0;i<size;i++,address += 4)
  {
    to[i]=(*(__IO uint32_t*) address);
    // printf("to[%d]=%d\r\n", i, to[i]);
  }  
   
}

void flash_read_people(uint32_t address, People to[], uint8_t size )
{
  uint8_t i=0;

  for(i=0;i<size;i++,address += 4)
  {
    to[i].ID=(*(__IO uint32_t*) address);
    // printf("to[%d]=%d\r\n", i, to[i]);
    address += 4;
    to[i].identity=(*(__IO uint32_t*) address);
  }
}





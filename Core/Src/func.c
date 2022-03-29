
#include "func.h"
#include "lcd.h"
#include "KEY.h"
#include "flash.h"
#include "gpio.h"
#include "lcd_init.h"
#include "LED.h"
#include "stdio.h"

Lock_Behavior_STATUS lock_status = lock_unkown;


void my_StrCopy(uint32_t *s1, uint8_t *s2, uint8_t size)
{
  for (uint8_t i = 0; i < size; i++)
  {
    *s1++ = *s2++;
  }
  
}

uint8_t password_cmp(uint32_t *s1, uint8_t *s2, uint8_t size)
{
    for (int i = 0; i < size; i++)
    {
        if(*s1++ != *s2++)
          goto error;
    }
    return 0;

error:
    return 1;    
}

void password_ChangeOrRead(PASSWORD_STATUS password_status)
{
  uint8_t i = 0;
  uint8_t password_form_key[6];


  LCD_ShowChinese(1*16, 4*16, "密码",RED,WHITE,16,0);
  LCD_ShowString(3*16, 4*16, ":", RED,WHITE,16,0);
  /*必须获取6位数密码*/
  while (1)
  {

    printf("key.num=%d,i=%d\r\n", key.num, i);
    
    /*获取按键输入的密码*/
    if(key.num>='0'&&key.num<='9')
    {
      if (i < 6)
      { 
        password_form_key[i]=key.num;     
        LCD_ShowString(3*16+8+i*8, 4*16,&password_form_key[i], RED,WHITE,16,0);
        ++i; 
      }
    }
    else if (key.num == 'C')
    {
      /*确定键*/
      if (!(i < 6))
      {
        printf("password_form_key:%d,%d,%d,%d,%d,%d",password_form_key[0],password_form_key[1],\
                password_form_key[2],password_form_key[3],password_form_key[4],password_form_key[5]);
        if(password_status == PasswordCHANGE)
        {
          my_StrCopy(password, password_form_key, 6);
          flash_write(ADDR_FLASH_PAGE_60, ADDR_FLASH_PAGE_61, password, COUNTOF(password));
        }
        else if (password_status == PasswordREAD)
        {

          if(password_cmp(password, password_form_key, 6) == 0)
          {
            lock_status = password_success;
            LCD_ShowChinese(1.5*16, 5*16, "密码正确",RED,WHITE,16,0);
            HAL_Delay(500);
            LCD_Fill(0,5*16,LCD_W,6*16,WHITE);
          }
          else
          {
            lock_status = password_fail;
            LCD_ShowChinese(1.5*16, 5*16, "密码错误",RED,WHITE,16,0);
            HAL_Delay(500);
            LCD_Fill(0,5*16,LCD_W,6*16,WHITE);
          }
        }
          LCD_Fill(0,4*16,LCD_W,5*16,WHITE);
          break;             
      }
      else{
        LCD_ShowChinese(3*16, 5*16, "错误",RED,WHITE,16,0);
        HAL_Delay(500);
        LCD_Fill(0,5*16,LCD_W,6*16,WHITE);
      }

    }
    else if (key.num == 'D')
    {
      /*取消键*/
      /*边界情况*/
      if (i == 0)   i = 1;

      password_form_key[--i]=' '; /*将一个数用空格覆盖*/ 
      LCD_ShowString(3*16+8+i*8, 4*16,&password_form_key[i], RED,WHITE,16,0);
    }   
    
     get_key();
     HAL_Delay(150);
  }
}

void password_form_key(void)
{

}



void first_use(void)
{
  if (password[6] == 0)  return;

  password[6] = 0;

  LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
  LCD_ShowChinese(1.5*16, 2*16, "第一次使用",RED,WHITE,16,0);
  LCD_ShowChinese(2*16, 3*16, "设置密码",RED,WHITE,16,0);

  password_ChangeOrRead(PasswordCHANGE);
}



void open_door(void)
{
  relay_on();
}

void close_door(void)
{
  relay_off();
}

void door_task(void)
{
  uint8_t pass_fail_num=0;

  if (task_status !=  run_door_task)
    return;


  LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
  while (1)
  {

    
    LCD_ShowChinese(2*16, 2*16, "欢迎使用",RED,WHITE,16,0);
    LCD_ShowChinese(1.5*16, 3*16, "指纹密码锁",RED,WHITE,16,0);


    get_key();

    printf("key.num=%d,pass_fail_num=%d\r\n", key.num,pass_fail_num);

    /*切换到不同的状态*/
    if (key.num>='0'&&key.num<='9')
    {
      password_ChangeOrRead(PasswordREAD);
    }
    else if (key.num  =='*' )
    {
      close_door();
    }
    else if (key.num  =='#')
    {
      task_status = run_ui_task;
      break;
    }
    
    /*对应不同开锁方式的状态*/
    switch (lock_status)
    {
    case password_success:
      open_door();

      lock_status = lock_unkown;
      break;
    case password_fail:
      if ( ++pass_fail_num == 3)
      {
        for (uint8_t i = 0; i < 6; i++)
        {
          beep_toggle();
          LED_Toggle(LED2);
          HAL_Delay(200);
        }
      }
      lock_status = lock_unkown;
      break;
      
    default:
      break;
    }


  }


}

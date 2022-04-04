
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

    get_key(); /*在开锁的过程中，按下一个按键0-9就进入这个状态，所以按下那个的键值没有去丢弃*/

    // printf("key.num=%d,i=%d\r\n", key.num, i);
    
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
        printf("password_form_key:%c,%c,%c,%c,%c,%c",password_form_key[0],password_form_key[1],\
        password_form_key[2],password_form_key[3],password_form_key[4],password_form_key[5]);
        if(password_status == PasswordCHANGE)
        {
          my_StrCopy(password, password_form_key, 6);
          flash_write_password(ADDR_FLASH_PAGE_61, ADDR_FLASH_PAGE_62, password, COUNTOF(password));
          LCD_ShowChinese(1*16, 5*16, "密码设置成功",RED,WHITE,16,0);
          HAL_Delay(1000);
          LCD_Fill(0,5*16,LCD_W,6*16,WHITE);       
        }
        else if (password_status == PasswordREAD)
        {

          if(password_cmp(password, password_form_key, 6) == 0)
          {
            lock_status = password_success;
            LCD_ShowChinese(1.5*16, 5*16, "密码正确",RED,WHITE,16,0);
            HAL_Delay(1000);
            LCD_Fill(0,5*16,LCD_W,6*16,WHITE);
          }
          else
          {
            lock_status = password_fail;
            LCD_ShowChinese(1.5*16, 5*16, "密码错误",RED,WHITE,16,0);
            HAL_Delay(1000);
            LCD_Fill(0,5*16,LCD_W,6*16,WHITE);
          }
        }
          LCD_Fill(0,4*16,LCD_W,5*16,WHITE);
          break;             
      }
      else{
        LCD_ShowChinese(1*16, 5*16, "密码格式错误",RED,WHITE,16,0);
        HAL_Delay(1000);
        LCD_Fill(0,5*16,LCD_W,6*16,WHITE);
      }

    }
    else if (key.num == 'D')
    {
      /*取消键*/
      /*边界情况*/
      if (i == 0 && password[6] == 1)      /*第一次初始化，不能退出*/
      {
        i = 1;
      }
      else if (i == 0 && password[6] == 0)  /*其余情况退出*/
      {
        break;
      }
      
      password_form_key[--i]=' '; /*将一个数用空格覆盖*/ 
      LCD_ShowString(3*16+8+i*8, 4*16,&password_form_key[i], RED,WHITE,16,0);
    }   
    
  }
}

void password_form_key(void)
{

}



void first_use(void)
{
  if (password[6] == 0)  return;


  LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
  LCD_ShowChinese(1.5*16, 2*16, "第一次使用",RED,WHITE,16,0);
  LCD_ShowChinese(2*16, 3*16, "设置密码",RED,WHITE,16,0);

  password_ChangeOrRead(PasswordCHANGE);

   password[6] = 0;
  flash_write_password(ADDR_FLASH_PAGE_61, ADDR_FLASH_PAGE_62, password, COUNTOF(password));

  //擦除设置密码
  //录入管理员指纹
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
  uint8_t pass_fail_num=0;  /*密码错误的次数*/
  int fr_ID=-1;             /*刷指纹时搜索到的用户ID（0-299）*/
 
  if (task_status !=  run_door_task)
    return;



  while (1)
  {
  
    LCD_ShowChinese(2*16, 2*16, "欢迎使用",RED,WHITE,16,0);
    LCD_ShowChinese(1.5*16, 3*16, "指纹密码锁",RED,WHITE,16,0);
    LCD_ShowChinese(0.5*16, 7*16, "密码或指纹开锁",RED,WHITE,16,0);

    get_key();

    // printf("key.num=%d,pass_fail_num=%d\r\n", key.num,pass_fail_num);

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
      LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
      break;
    }

    /*感应到有指纹*/
    if (AS608_WAK_status == GPIO_PIN_SET)
    {
      printf("刷指纹中：");
      fr_ID = press_FR();
      if (fr_ID != -1)
      {
        lock_status = as608_success;
      }else
      {
        lock_status = as608_fail;
      }
      
    }

    
    /*对应不同锁的状态*/
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
        pass_fail_num=0;
      }
      lock_status = lock_unkown;
      break;

    case as608_success:
      open_door();
      lock_status = lock_unkown;
      break; 

    case as608_fail:
      // LCD_ShowChinese(0*16, 4*16, "没有该用户",RED,WHITE,16,0);	
      // HAL_Delay(900);
      // LCD_Fill(0,4*16,LCD_W,5*16,WHITE);
      lock_status = lock_unkown;
      break; 
      
    default:
      break;
    }


  }


}

/**
 * @brief 更改密码，用在非初始化的时候
 * 
 */
void password_change(void)
{
  LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
  LCD_ShowChinese(1.5*16, 3*16, "设置新密码",RED,WHITE,16,0);

  password_ChangeOrRead(PasswordCHANGE);

}





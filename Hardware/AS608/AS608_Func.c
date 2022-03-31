
#include "AS608_Func.h"
#include "as608.h"
#include "stdio.h"
#include "lcd.h"
#include "lcd_init.h"
#include "main.h"


/* Private variables ---------------------------------------------------------*/
uint8_t ensure;
uint16_t ValidN;
SysPara AS608Para;//指纹模块AS608参数

//通过串口显示错误码信息
void ShowErrMessage(uint8_t ensure)
{
  printf("AS608:%s\r\n", EnsureMessage(ensure));
}


void AS608_Init(void)
{
    while(PS_HandShake(&AS608Addr))//与AS608模块握手
    {
        HAL_Delay(500);
    }
    printf("ok!!!!");


    ensure=PS_ValidTempleteNum(&ValidN);//读库指纹个数
    if(ensure!=0x00)
        ShowErrMessage(ensure);//显示确认码错误信息	

	ensure=PS_ReadSysPara(&AS608Para);  //读参数 
	if(ensure==0x00)
	{
	}
	else
		ShowErrMessage(ensure);	
}


void Add_FR(void)
{
	u8 i=0,ensure=0 ,processnum=0;
	u16 ID;
	while(1)
	{
		switch (processnum)
		{
			case 0:
				i++;
        LCD_ShowChinese(0*16, 4*16, "请按指纹",RED,WHITE,16,0);
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer1);//生成特征
					if(ensure==0x00)
					{
						LCD_Fill(0,4*16,LCD_W,5*16,WHITE);
            LCD_ShowChinese(0*16, 4*16, "指纹正常",RED,WHITE,16,0);
						i=0;
						processnum=1;//跳到第二步						
					}else ShowErrMessage(ensure);				
				}else ShowErrMessage(ensure);						
				break;
			
			case 1:
				i++;
				LCD_Fill(0,4*16,LCD_W,5*16,WHITE);
        LCD_ShowChinese(0*16, 4*16, "请按再按一次指纹",RED,WHITE,16,0);
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer2);//生成特征
					if(ensure==0x00)
					{
						LCD_Fill(0,4*16,LCD_W,5*16,WHITE);
            LCD_ShowChinese(0*16, 4*16, "指纹正常",RED,WHITE,16,0);
						i=0;
						processnum=2;//跳到第三步
					}else ShowErrMessage(ensure);	
				}else ShowErrMessage(ensure);		
				break;

			case 2:
				LCD_Fill(0,4*16,LCD_W,5*16,WHITE);
        LCD_ShowChinese(0*16, 4*16, "对比两次指纹",RED,WHITE,16,0);
				ensure=PS_Match();
				if(ensure==0x00) 
				{
					LCD_Fill(0,4*16,LCD_W,5*16,WHITE);
          LCD_ShowChinese(0*16, 4*16, "对比成功,两次指纹一样",RED,WHITE,16,0);
					processnum=3;//跳到第四步
				}
				else 
				{
					LCD_Fill(0,4*16,LCD_W,5*16,WHITE);
					LCD_ShowChinese(0*16, 4*16, "对比失败，请重新录入指纹",RED,WHITE,16,0);
					ShowErrMessage(ensure);
					i=0;
					processnum=0;//跳回第一步		
				}
				HAL_Delay(1200);
				break;

			case 3:
				LCD_Fill(0,4*16,LCD_W,5*16,WHITE);
        LCD_ShowChinese(0*16, 4*16, "生成指纹模板",RED,WHITE,16,0);
				ensure=PS_RegModel();
				if(ensure==0x00) 
				{
					LCD_Fill(0,4*16,LCD_W,5*16,WHITE);
          LCD_ShowChinese(0*16, 4*16, "生成指纹模板成功",RED,WHITE,16,0);
					processnum=4;//跳到第五步
				}else {processnum=0;ShowErrMessage(ensure);}
				HAL_Delay(1200);
				break;
				
			case 4:	

				do
					ID=1;
				while(!(ID<AS608Para.PS_max));//输入ID必须小于模块容量最大的数值
				ensure=PS_StoreChar(CharBuffer2,ID);//储存模板
				if(ensure==0x00) 
				{			
					LCD_Fill(0,4*16,LCD_W,5*16,WHITE);					
          LCD_ShowChinese(0*16, 4*16, "录入指纹成功",RED,WHITE,16,0);
					PS_ValidTempleteNum(&ValidN);//读库指纹个数
					HAL_Delay(1500);
					LCD_Fill(0,100,240,160,WHITE); 
					return ;
				}else {processnum=0;ShowErrMessage(ensure);}					
				break;				
		}
		HAL_Delay(400);
		if(i==5)//超过5次没有按手指则退出
		{
			LCD_Fill(0,4*16,LCD_W,5*16,WHITE);	
			break;	
		}				
	}
}

/**
  * @brief  刷指纹
  * 
  * @retval 返回搜索的指纹的序号（0-299）
  */
int press_FR(void)
{
	SearchResult seach;
	u8 ensure;
	ensure=PS_GetImage();
	if(ensure==0x00)//获取图像成功 
	{	
		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //生成特征成功
		{		
			ensure=PS_HighSpeedSearch(CharBuffer1,0,AS608Para.PS_max,&seach);
			if(ensure==0x00)//搜索成功
			{				
				LCD_Fill(0,4*16,LCD_W,5*16,WHITE);
				LCD_ShowChinese(0*16, 4*16, "刷指纹成功",RED,WHITE,16,0);			
				printf("序号：%d\r\n",seach.pageID);
				return seach.pageID;	
			}
			else
			{ 
				HAL_Delay(800);
				LCD_Fill(0,4*16,LCD_W,5*16,WHITE);
				ShowErrMessage(ensure);		
				return -1;			
			}
    	}
		else
		{
			HAL_Delay(800);
			LCD_Fill(0,4*16,LCD_W,5*16,WHITE);
			ShowErrMessage(ensure);
			return -1;
		}

	}		
}

//删除指纹
void Del_FR(void)
{
	u8  ensure;
	u16 num;

	if(num==0xFFFF)
		 return; //返回主页面
	else if(num==0xFF00)
		ensure=PS_Empty();//清空指纹库
	else 
		ensure=PS_DeletChar(num,1);//删除单个指纹
	if(ensure==0)
	{

    LCD_Fill(0,4*16,LCD_W,5*16,WHITE);
    LCD_ShowChinese(0*16, 4*16, "删除指纹成功",RED,WHITE,16,0);				
	}
  else
		ShowErrMessage(ensure);	
	PS_ValidTempleteNum(&ValidN);//读库指纹个数

}

/**
 * @brief 判断是不是管理员
 *
 * 
 * @retval 0则是管理员，1不是管理员
 *  
 */

uint8_t is_people_admin(int fr_id)
{
	for (uint8_t i = 0; i < people_num; i++)
	{
		if (fr_id == people[i].ID)
		{
			if (people[i].identity == ADMIN)
			{
				return 0;
			}	
		}
	}
	return 1;
}


void people_init(void)
{
	uint8_t i =0;

	people[0].ID = 0xaaaa;
	people[0].identity = ID_UNKOWN;

	people[1].ID = 1;
	people[1].identity = ADMIN;

	for ( i = 2; i < 8; i++)
	{
		people[i].ID = 0xaaaa;
		people[i].identity = ID_UNKOWN;
	}
	


}






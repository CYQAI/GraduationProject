
#include "AS608_Func.h"
#include "as608.h"
#include "stdio.h"
#include "lcd.h"
#include "lcd_init.h"
#include "main.h"
#include "KEY.h"
#include "math.h"

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


int Add_FR(uint8_t ID)
{
	u8 i=0,ensure=0 ,processnum=0;
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
        		LCD_ShowChinese(0*16, 4*16, "请再按一次指纹",RED,WHITE,16,0);
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
          			LCD_ShowChinese(0*16, 4*16, "成功两次指纹一样",RED,WHITE,16,0);
					processnum=3;//跳到第四步
				}
				else 
				{
					LCD_Fill(0,4*16,LCD_W,5*16,WHITE);
					LCD_ShowChinese(0*16, 4*16, "失败重新录入指纹",RED,WHITE,16,0);
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

				if (ID>AS608Para.PS_max)//输入ID必须小于模块容量最大的数值
					break;

				ensure=PS_StoreChar(CharBuffer2,ID);//储存模板
				if(ensure==0x00) 
				{			
					LCD_Fill(0,4*16,LCD_W,5*16,WHITE);					
          			LCD_ShowChinese(0*16, 4*16, "录入指纹成功",RED,WHITE,16,0);
					PS_ValidTempleteNum(&ValidN);//读库指纹个数
					HAL_Delay(1500);
					LCD_Fill(0,4*16,LCD_W,5*16,WHITE);					
					return 0;
				}else {processnum=0;ShowErrMessage(ensure);}					
				break;				
		}
		HAL_Delay(400);
		if(i==5)//超过5次没有按手指则退出
		{
			LCD_Fill(0,4*16,LCD_W,5*16,WHITE);	
			LCD_ShowChinese(0*16, 4*16, "录入指纹成功",RED,WHITE,16,0);
			HAL_Delay(800);
			LCD_Fill(0,4*16,LCD_W,5*16,WHITE);	
			return -1;
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
				LCD_Fill(0,5*16,LCD_W,6*16,WHITE);
				LCD_ShowChinese(0*16, 5*16, "刷指纹成功",RED,WHITE,16,0);			
				HAL_Delay(1000);
				LCD_Fill(0,5*16,LCD_W,6*16,WHITE);
				printf("序号：%d\r\n",seach.pageID);
				return seach.pageID;	
			}
			else
			{ 
				LCD_Fill(0,5*16,LCD_W,6*16,WHITE);
				LCD_ShowChinese(0*16, 5*16, "刷指纹失败",RED,WHITE,16,0);			
				HAL_Delay(1000);
				LCD_Fill(0,5*16,LCD_W,6*16,WHITE);
				ShowErrMessage(ensure);		
				return -1;			
			}
    	}
		else
		{
			LCD_Fill(0,5*16,LCD_W,6*16,WHITE);
			LCD_ShowChinese(0*16, 5*16, "刷指纹失败",RED,WHITE,16,0);			
			HAL_Delay(1000);
			LCD_Fill(0,5*16,LCD_W,6*16,WHITE);
			ShowErrMessage(ensure);
			return -1;
		}

	}		
}

//删除指纹
void del_FR(void)
{
	uint8_t  ensure,t=0,flag=0;
	uint16_t num=0,i=0;

	LCD_Fill(0,0*16,LCD_W,LCD_H,WHITE);
	LCD_ShowChinese(0*16, 2*16, "输入要删除的用户",RED,WHITE,16,0);				
	LCD_ShowString(2.5*16, 3*16, "ID:", RED,WHITE,16,0);

	while (1)
	{
		get_key();

		if (key.num == 'D' && flag == 0)	/*退出这状态*/
		{
			LCD_Fill(0,0*16,LCD_W,LCD_H,WHITE);
			break;
		}
	
		if (key.num == 'D' && flag == 1)	/**/
		{
			LCD_Fill(4*16, 3*16, LCD_W, 4*16, WHITE);
			t=0;
			num = 0;
			flag = 0;
		}

		/*获取按键输入的密码*/
		if(key.num>='0'&& key.num<='9')
		{
			if (t < 3)
			{
				key.num = key.num - '0';
				num = num*10 + key.num;
				++t;
				key.num = 0;		/*为了保险*/
				flag = 1;
				LCD_ShowIntNum(4*16, 3*16, num, 3, RED, WHITE, 16);
				printf("num:%d\r\n", num);
			}			
		}


		if (key.num == 'C')	/**/
		{

			if (num == people_ID_now)
			{
				LCD_ShowChinese(0*16, 5*16, "无法删除自己",RED,WHITE,16,0);								
				HAL_Delay(1000);
				LCD_Fill(0*16, 5*16, LCD_W, 6*16,WHITE);
				goto point1;
			}

			if (!(num<=299))
			{
				LCD_ShowChinese(0*16, 5*16, "用户",RED,WHITE,16,0);
				LCD_ShowString(2*16, 5*16, "ID:", RED,WHITE,16,0);
				LCD_ShowChinese(3.5*16, 5*16, "超过范围",RED,WHITE,16,0);				
				HAL_Delay(1000);
				LCD_Fill(0*16, 5*16, LCD_W, 6*16,WHITE);
				goto point1;				
			}
			
			for ( i = 0; i < PEOPLE_MAX_COUNT; i++)
			{
				if (num == people[i].ID)
					break;
			}
			
			if (i == PEOPLE_MAX_COUNT)
			{
				LCD_ShowChinese(0*16, 5*16, "用户不存在",RED,WHITE,16,0);								
				HAL_Delay(1000);
				LCD_Fill(0*16, 5*16, LCD_W, 6*16,WHITE);
				goto point1;
			}
			
			ensure=PS_DeletChar(num,1);//删除单个指纹
			if(ensure==0)
			{
				LCD_ShowChinese(0*16, 5*16, "删除指纹成功",RED,WHITE,16,0);				
				HAL_Delay(1000);
				LCD_Fill(0*16, 5*16, LCD_W, 6*16,WHITE);			
			}
			else{
				LCD_ShowChinese(0*16, 5*16, "删除指纹失败",RED,WHITE,16,0);
				HAL_Delay(1000);
				LCD_Fill(0*16, 5*16, LCD_W, 6*16,WHITE);
				ShowErrMessage(ensure);	
			}
point1:
			PS_ValidTempleteNum(&ValidN);//读库指纹个数

		}		


	}
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
	for (uint8_t i = 0; i < PEOPLE_MAX_COUNT; i++)
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

/**
 * @brief people 初始化
 * 
 * @note ID=0xaaaa 是无效的意思，是该位没有被占领
 */
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


void add_people(PEOPLE_IDENTITY people_identity)
{
	uint8_t i=0;

	for (i = 0; i < PEOPLE_MAX_COUNT; i++)
	{
		if (people[i].ID == 0xaaaa)	/*找到没有占领的位置*/
			break;
	}

	if (i == PEOPLE_MAX_COUNT)
	{							
		LCD_ShowChinese(0*16, 4*16, "人员已满",RED,WHITE,16,0);
		HAL_Delay(800);
		LCD_Fill(0,4*16,LCD_W,5*16,WHITE);
		return;
	}
	

	if (people_identity == ADMIN)
	{
		if (Add_FR(i) == 0)	/*成功添加指纹*/
		{
			people[i].ID = i;
			people[i].identity = ADMIN;
		}		 		
	}
	else if (people_identity == COMMON)
	{
		if (Add_FR(i) == 0)	/*成功添加指纹*/
		{
			people[i].ID = i;
			people[i].identity = COMMON;
		}	
	}
	


}

void add_admin_people(void)
{
	add_people(ADMIN);
}

void add_common_people(void)
{
	add_people(COMMON);
}


void show_people(PEOPLE_IDENTITY people_identity)
{
	uint8_t i=0,t=0,flag=1;	/*t拿来排序,flag控制只进行一次计算*/
	LCD_Fill(0,0*16,LCD_W,LCD_H,WHITE);

	while (1)
	{
		get_key();
		HAL_Delay(150);
		
		if (key.num == 'D')	/*退出这状态*/
		{
			LCD_Fill(0,0*16,LCD_W,LCD_H,WHITE);
			break;
		}
		

		if (people_identity == ADMIN && flag == 1)
		{
			LCD_ShowChinese(0*16, 0*16, "管理员",RED,WHITE,16,0);
			for (i = 0; i < PEOPLE_MAX_COUNT; i++)
			{
				if (people[i].identity == ADMIN)
				{
					++t;
					LCD_ShowIntNum(0*16, t*16, people[i].ID, 3, RED, WHITE, 16);
				}	

			}
			flag = 0;
		}
		else if (people_identity == COMMON && flag ==1 ) 
		{
			LCD_ShowChinese(0*16, 0*16, "普通用户",RED,WHITE,16,0);
			for (i = 0; i < PEOPLE_MAX_COUNT; i++)
			{
				if (people[i].identity == COMMON)
				{
					++t;
					LCD_ShowIntNum(0*16, t*16, people[i].ID, 3, RED, WHITE, 16);
				}	

			}
			flag = 0;		
		}
	}
}

void show_admin_people(void)
{
	show_people(ADMIN);
}

void show_common_people(void)
{
	show_people(COMMON);
}







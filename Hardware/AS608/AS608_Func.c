
#include "AS608_Func.h"
#include "as608.h"
#include "stdio.h"
/* Private variables ---------------------------------------------------------*/
uint8_t ensure;
uint16_t ValidN;
SysPara AS608Para;//指纹模块AS608参数

void AS608_Init(void)
{
    while(PS_HandShake(&AS608Addr))//与AS608模块握手
    {
        HAL_Delay(500);
    }
    printf("ok!!!!");


    ensure=PS_ValidTempleteNum(&ValidN);//读库指纹个数
    if(ensure!=0x00)
        AS608_ShowErrMessage(ensure);//显示确认码错误信息	

	ensure=PS_ReadSysPara(&AS608Para);  //读参数 
	if(ensure==0x00)
	{
	}
	else
		AS608_ShowErrMessage(ensure);	
}



//通过串口显示错误码信息
void AS608_ShowErrMessage(uint8_t ensure)
{
  printf("AS088:%s\r\n", EnsureMessage(ensure));
}

// void Add_FR(void)
// {
// 	u8 i,ensure ,processnum=0;
// 	u16 ID;
// 	while(1)
// 	{
// 		switch (processnum)
// 		{
// 			case 0:
// 				i++;
// 				Show_Str_Mid(0,100,"请按指纹",16,240);
// 				ensure=PS_GetImage();
// 				if(ensure==0x00) 
// 				{
// 					BEEP=1;
// 					ensure=PS_GenChar(CharBuffer1);//生成特征
// 					BEEP=0;
// 					if(ensure==0x00)
// 					{
// 						LCD_Fill(0,120,lcddev.width,160,WHITE);
// 						Show_Str_Mid(0,120,"指纹正常",16,240);
// 						i=0;
// 						processnum=1;//跳到第二步						
// 					}else ShowErrMessage(ensure);				
// 				}else ShowErrMessage(ensure);						
// 				break;
			
// 			case 1:
// 				i++;
// 				LCD_Fill(0,100,lcddev.width,160,WHITE);
// 				Show_Str_Mid(0,100,"请按再按一次指纹",16,240);
// 				ensure=PS_GetImage();
// 				if(ensure==0x00) 
// 				{
// 					BEEP=1;
// 					ensure=PS_GenChar(CharBuffer2);//生成特征
// 					BEEP=0;
// 					if(ensure==0x00)
// 					{
// 						LCD_Fill(0,120,lcddev.width,160,WHITE);
// 						Show_Str_Mid(0,120,"指纹正常",16,240);
// 						i=0;
// 						processnum=2;//跳到第三步
// 					}else ShowErrMessage(ensure);	
// 				}else ShowErrMessage(ensure);		
// 				break;

// 			case 2:
// 				LCD_Fill(0,100,lcddev.width,160,WHITE);
// 				Show_Str_Mid(0,100,"对比两次指纹",16,240);
// 				ensure=PS_Match();
// 				if(ensure==0x00) 
// 				{
// 					LCD_Fill(0,120,lcddev.width,160,WHITE);
// 					Show_Str_Mid(0,120,"对比成功,两次指纹一样",16,240);
// 					processnum=3;//跳到第四步
// 				}
// 				else 
// 				{
// 					LCD_Fill(0,100,lcddev.width,160,WHITE);
// 					Show_Str_Mid(0,100,"对比失败，请重新录入指纹",16,240);
// 					ShowErrMessage(ensure);
// 					i=0;
// 					processnum=0;//跳回第一步		
// 				}
// 				delay_ms(1200);
// 				break;

// 			case 3:
// 				LCD_Fill(0,100,lcddev.width,160,WHITE);
// 				Show_Str_Mid(0,100,"生成指纹模板",16,240);
// 				ensure=PS_RegModel();
// 				if(ensure==0x00) 
// 				{
// 					LCD_Fill(0,120,lcddev.width,160,WHITE);
// 					Show_Str_Mid(0,120,"生成指纹模板成功",16,240);
// 					processnum=4;//跳到第五步
// 				}else {processnum=0;ShowErrMessage(ensure);}
// 				delay_ms(1200);
// 				break;
				
// 			case 4:	
// 				LCD_Fill(0,100,lcddev.width,160,WHITE);
// 				Show_Str_Mid(0,100,"请输入储存ID,按Enter保存",16,240);
// 				Show_Str_Mid(0,120,"0=< ID <=299",16,240);
// 				do
// 					ID=GET_NUM();
// 				while(!(ID<AS608Para.PS_max));//输入ID必须小于模块容量最大的数值
// 				ensure=PS_StoreChar(CharBuffer2,ID);//储存模板
// 				if(ensure==0x00) 
// 				{			
// 					LCD_Fill(0,100,lcddev.width,160,WHITE);					
// 					Show_Str_Mid(0,120,"录入指纹成功",16,240);
// 					PS_ValidTempleteNum(&ValidN);//读库指纹个数
// 					LCD_ShowNum(56,80,AS608Para.PS_max-ValidN,3,16);//显示剩余指纹容量
// 					delay_ms(1500);
// 					LCD_Fill(0,100,240,160,WHITE);
// 					return ;
// 				}else {processnum=0;ShowErrMessage(ensure);}					
// 				break;				
// 		}
// 		delay_ms(400);
// 		if(i==5)//超过5次没有按手指则退出
// 		{
// 			LCD_Fill(0,100,lcddev.width,160,WHITE);
// 			break;	
// 		}				
// 	}
// }


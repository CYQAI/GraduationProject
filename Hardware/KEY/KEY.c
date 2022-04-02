
/* Includes ------------------------------------------------------------------*/
#include "KEY.h"
#include "stdio.h"

/*KEY*/
static  GPIO_TypeDef *KEY_Row_Port[KEY_RowN]       = { KEY_Row1_GPIO_Port,
                                                            KEY_Row2_GPIO_Port,
                                                            KEY_Row3_GPIO_Port,
                                                            KEY_Row4_GPIO_Port
                                                          };

static  uint16_t KEY_Row_PIN[KEY_RowN]         = { KEY_Row1_Pin, 
                                                        KEY_Row2_Pin,
                                                        KEY_Row3_Pin,
                                                        KEY_Row4_Pin,
                                                      };

static  GPIO_TypeDef *KEY_Column_Port[KEY_ColumnN]        = { KEY_Column1_GPIO_Port,
                                                                KEY_Column2_GPIO_Port,
                                                                KEY_Column3_GPIO_Port,
                                                                KEY_Column4_GPIO_Port
                                                              };

static  uint16_t KEY_Column_PIN[KEY_ColumnN]           = {KEY_Column1_Pin, 
                                                            KEY_Column2_Pin,
                                                            KEY_Column3_Pin,
                                                            KEY_Column4_Pin,
                                                           };


key_struct key;



uint8_t Key_Column_Scan(uint8_t row)
{
    uint8_t Key_Column_Num = 0x00;
    for(int i=0; i<KEY_ColumnN; i++)
    {
        Key_Column_Num |=  HAL_GPIO_ReadPin(KEY_Column_Port[i], KEY_Column_PIN[i]) << (3-i);
        if(Key_Column_Num != 0)
            break;
    }

    if(Key_Column_Num != 0X00)
    {
        HAL_Delay(30);
        for(int i=0; i<KEY_ColumnN; i++)
        {
            Key_Column_Num |= HAL_GPIO_ReadPin(KEY_Column_Port[i], KEY_Column_PIN[i]) << (3-i);
            if(Key_Column_Num != 0)
            break;
        }

        if(Key_Column_Num != 0X00)
        {
            if(row == 0)
            {
                switch (Key_Column_Num)
                {
                case 0x08:
                     return '1';
                case 0x04:
                     return '2';
                case 0x02:
                     return '3';
                case 0x01:
                     return 'A';
                default:
                    break;
                }
            }
            else if(row == 1)
            {
                switch (Key_Column_Num)
                {
                case 0x08:
                     return '4';
                case 0x04:
                     return '5';
                case 0x02:
                     return '6';
                case 0x01:
                     return 'B';
                default:
                    break;
                }              
            }
            else if(row == 2)
            {
                 switch (Key_Column_Num)
                {
                case 0x08:
                     return '7';
                case 0x04:
                     return '8';
                case 0x02:
                     return '9';
                case 0x01:
                     return 'C';
                default:
                    break;
                }               
            }
            else if(row == 3)
            {
                switch (Key_Column_Num)
                {
                case 0x08:
                     return '*';
                case 0x04:
                     return '0';
                case 0x02:
                     return '#';
                case 0x01:
                     return 'D';
                default:
                    break;
                }
            }
        }


        return 0;
    }

    return 0;
}


uint8_t Key_Scan(void)
{
    uint8_t key_num = 0;

    for(int i = 0; i < KEY_RowN; i++)
    {
        HAL_GPIO_WritePin(KEY_Row_Port[i], KEY_Row_PIN[i], GPIO_PIN_SET);

        key_num = Key_Column_Scan(i); 

     //    printf("key_num=%c\r\n", key_num);
        HAL_GPIO_WritePin(KEY_Row_Port[i], KEY_Row_PIN[i], GPIO_PIN_RESET);
    
        if(key_num != 0)
            break;
    }

    return key_num;
 
}

void get_key(void)
{
     uint8_t i = 0;
     while (i++<2)
     {
          key.num = Key_Scan();
          HAL_Delay(120);          
     }
     

}



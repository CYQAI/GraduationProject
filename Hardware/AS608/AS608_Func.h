#ifndef __AS608_Func_H
#define __AS608_Func_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"



typedef enum {
    ID_UNKOWN,
    COMMON, 
    ADMIN,
}PEOPLE_IDENTITY;

#define people_num  10

void AS608_Init(void);
void Add_FR(void);
int press_FR(void);
void Del_FR(void);
uint8_t is_people_admin(int fr_id);
void people_init(void);


#ifdef __cplusplus
}
#endif

#endif 


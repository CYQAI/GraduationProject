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


void AS608_Init(void);
int Add_FR(uint8_t ID);
int press_FR(void);
void del_FR(void);
uint8_t is_people_admin(int fr_id);
void people_init(void);
void add_people(PEOPLE_IDENTITY people_identity);
void add_admin_people(void);
void add_common_people(void);
void show_admin_people(void);
void show_common_people(void);


#ifdef __cplusplus
}
#endif

#endif 


#ifndef __FUNC_H__
#define __FUNC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"


typedef enum {PasswordREAD = 0, PasswordCHANGE = !PasswordREAD} PASSWORD_STATUS;

typedef enum 
{
    lock_unkown,
    password_success, 
    password_fail,
    as608_success,
    as608_fail,

}  Lock_Behavior_STATUS;


void password_ChangeOrRead(PASSWORD_STATUS password_status);
void first_use(void);
void open_door(void);
void close_door(void);
void door_task(void);

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */


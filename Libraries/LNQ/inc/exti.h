#ifndef __EXTI_H
#define __EXTI_H
#include "common.h"
//#define KEY_RAM                (*((volatile unsigned short *) 0x60000006))     //外部地址是从0x60000000开始的
#define KEY_RAM                (*((volatile unsigned short *) 0x6000000c))     //外部地址是从0x60000000开始的

typedef struct _key_type
{
  char keysign;
  u8 keycode;
}key_type;
extern volatile key_type MAIN_KEY;
extern void EXTI_init(void);
extern void EXTI1_IRQHandler(void);
#define Wait_For_Any_Key() while (not MAIN_KEY.keysign);MAIN_KEY.keysign = 0
#endif

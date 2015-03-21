#ifndef __EXTI_H
#define __EXIT_H
#include "sys.h"

#define KEY_RAM                (*((volatile unsigned short *) 0x60000006))     //外部地址是从0x60000000开始的

void EXTI_init(void);

#endif

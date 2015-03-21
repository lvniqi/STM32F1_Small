/*
 * Common.h
 *
 *  Created on: 2013-12-2
 *      Author: lvniqi
 *      Email: lvniqi@gmail.com
 */

#ifndef COMMON_H_
#define COMMON_H_

#include "stm32f10x_it.h"
#include "stm32f10x.h"
#include "USART.h"
#include "GPIO.h"
#include <stdio.h>

#define pass asm("nop")
#define True 1
#define False 0
#define not !
#define or ||
#define and &&
#define elif else if
/*Êý¾Ý¼Ä´æÆ÷µØÖ·*/
#define DRIVE_DR_BASE(DRIVE) ((uint32_t)&((DRIVE->DR)))

extern int fputc(int ch, FILE *f);
extern void delay_ms(volatile u16 time);
extern u16 Num_Len(u32 num);

#endif /* COMMON_H_ */

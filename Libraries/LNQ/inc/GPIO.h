#ifndef __GPIO_H
#define __GPIO_H	
#include "common.h"

//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
/*系统定义*/ 
/*警告！非用户使用*/
#define _BIT_BAND(add, channel) ((add & 0xF0000000)+0x2000000+((add &0xFFFFF)<<5)+(channel<<2)) 
/*警告！非用户使用*/
#define _MEM_Add(add)  *((volatile unsigned long  *)(add)) 
/*警告！非用户使用*/
#define _BIT_Add(add, channel)   _MEM_Add(_BIT_BAND(add, channel)) 


//IO口地址映射
#define GPIOA_OUT_ADDRESS    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_OUT_ADDRESS    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_OUT_ADDRESS    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_OUT_ADDRESS    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_OUT_ADDRESS    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_OUT_ADDRESS    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_OUT_ADDRESS    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IN_ADDRESS    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IN_ADDRESS    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IN_ADDRESS    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IN_ADDRESS    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IN_ADDRESS    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IN_ADDRESS    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IN_ADDRESS    (GPIOG_BASE+8) //0x40011E08 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   _BIT_Add(GPIOA_OUT_ADDRESS,n)  //输出 
#define PAin(n)    _BIT_Add(GPIOA_IN_ADDRESS,n)  //输入 

#define PBout(n)   _BIT_Add(GPIOB_OUT_ADDRESS,n)  //输出 
#define PBin(n)    _BIT_Add(GPIOB_IN_ADDRESS,n)  //输入 

#define PCout(n)   _BIT_Add(GPIOC_OUT_ADDRESS,n)  //输出 
#define PCin(n)    _BIT_Add(GPIOC_IN_ADDRESS,n)  //输入 

#define PDout(n)   _BIT_Add(GPIOD_OUT_ADDRESS,n)  //输出 
#define PDin(n)    _BIT_Add(GPIOD_IN_ADDRESS,n)  //输入 

#define PEout(n)   _BIT_Add(GPIOE_OUT_ADDRESS,n)  //输出 
#define PEin(n)    _BIT_Add(GPIOE_IN_ADDRESS,n)  //输入

#define PFout(n)   _BIT_Add(GPIOF_OUT_ADDRESS,n)  //输出 
#define PFin(n)    _BIT_Add(GPIOF_IN_ADDRESS,n)  //输入

#define PGout(n)   _BIT_Add(GPIOG_OUT_ADDRESS,n)  //输出 
#define PGin(n)    _BIT_Add(GPIOG_IN_ADDRESS,n)  //输入

#endif

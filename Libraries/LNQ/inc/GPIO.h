#ifndef __GPIO_H
#define __GPIO_H	
#include "common.h"

//λ������,ʵ��51���Ƶ�GPIO���ƹ���
//����ʵ��˼��,�ο�<<CM3Ȩ��ָ��>>������(87ҳ~92ҳ).
/*ϵͳ����*/ 
/*���棡���û�ʹ��*/
#define _BIT_BAND(add, channel) ((add & 0xF0000000)+0x2000000+((add &0xFFFFF)<<5)+(channel<<2)) 
/*���棡���û�ʹ��*/
#define _MEM_Add(add)  *((volatile unsigned long  *)(add)) 
/*���棡���û�ʹ��*/
#define _BIT_Add(add, channel)   _MEM_Add(_BIT_BAND(add, channel)) 


//IO�ڵ�ַӳ��
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
 
//IO�ڲ���,ֻ�Ե�һ��IO��!
//ȷ��n��ֵС��16!
#define PAout(n)   _BIT_Add(GPIOA_OUT_ADDRESS,n)  //��� 
#define PAin(n)    _BIT_Add(GPIOA_IN_ADDRESS,n)  //���� 

#define PBout(n)   _BIT_Add(GPIOB_OUT_ADDRESS,n)  //��� 
#define PBin(n)    _BIT_Add(GPIOB_IN_ADDRESS,n)  //���� 

#define PCout(n)   _BIT_Add(GPIOC_OUT_ADDRESS,n)  //��� 
#define PCin(n)    _BIT_Add(GPIOC_IN_ADDRESS,n)  //���� 

#define PDout(n)   _BIT_Add(GPIOD_OUT_ADDRESS,n)  //��� 
#define PDin(n)    _BIT_Add(GPIOD_IN_ADDRESS,n)  //���� 

#define PEout(n)   _BIT_Add(GPIOE_OUT_ADDRESS,n)  //��� 
#define PEin(n)    _BIT_Add(GPIOE_IN_ADDRESS,n)  //����

#define PFout(n)   _BIT_Add(GPIOF_OUT_ADDRESS,n)  //��� 
#define PFin(n)    _BIT_Add(GPIOF_IN_ADDRESS,n)  //����

#define PGout(n)   _BIT_Add(GPIOG_OUT_ADDRESS,n)  //��� 
#define PGin(n)    _BIT_Add(GPIOG_IN_ADDRESS,n)  //����

#endif

#include "sys.h"

void EXTI_init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);

		EXTI_InitStructure.EXTI_Line=EXTI_Line1;        
		EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	
		EXTI_Init(&EXTI_InitStructure);
	
		EXTI_ClearFlag(EXTI_Line1);
	
	    NVIC_InitStructure.NVIC_IRQChannel=EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init (&NVIC_InitStructure);
}


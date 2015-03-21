#include "exti.h"
volatile key_type MAIN_KEY;
void EXTI_init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//ʹ�ܸ��ù���ʱ��

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);

	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;

	EXTI_Init(&EXTI_InitStructure);

	EXTI_ClearFlag(EXTI_Line1);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
        MAIN_KEY.keycode = 0;
        MAIN_KEY.keysign = 0;
}

void EXTI1_IRQHandler(void)	//�����ж�
{
	MAIN_KEY.keycode = KEY_RAM;
	MAIN_KEY.keycode &= 0x0f;
	MAIN_KEY.keysign = 1;
	EXTI_ClearITPendingBit(EXTI_Line1);
}

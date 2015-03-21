/*
 * Timer.c
 *
 *  Created on: 2014年7月30日
 *      Author: lvniqi
 */
#include "Timer.h"
void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = 20; //设置0.05ms一次TIM2比较的周期
	TIM_TimeBaseStructure.TIM_Prescaler = 71; //系统主频72M，这里分频72，相当于1M的定时器2时钟
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update); //发生时间 产生DAC
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //下面详细说明
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //TIM_OutputState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 10;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //如果是PWM1要为Low，PWM2则为High
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);

	TIM_Cmd(TIM2, ENABLE);
	TIM_CtrlPWMOutputs(TIM2, ENABLE);

}


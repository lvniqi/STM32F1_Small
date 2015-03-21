#include "stm32f10x.h"
#include <stdio.h>
#include "common.h"
#include "USART.h"
#include "Lcd.h"
#include "exti.h"
#include "DA.h"
#include "AD.h"
#include "SPI_FLASH.h"
void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = 8; //设置0.05ms一次TIM2比较的周期
	TIM_TimeBaseStructure.TIM_Prescaler = 7; //系统主频72M，这里分频72，相当于1M的定时器2时钟
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update); //发生时间 产生DAC
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //下面详细说明
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //TIM_OutputState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 4;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //如果是PWM1要为Low，PWM2则为High
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);

	TIM_Cmd(TIM2, ENABLE);
	TIM_CtrlPWMOutputs(TIM2, ENABLE);

}
int main(void)
{
	LCD_Init();
	EXTI_init();
	Dac_Init(DAC_Channel_1, DAC_Trigger_T2_TRGO);
	TIM2_Configuration();
	ADC1_GPIO_Config();
	ADC1_Mode_Config();
	Spi_Flash_Init();
	int i = 0;
        LCD_Curve_Init(&Lcd_Curve1,5,5,BLUE);
	//LCD_ShowString(140, 250, 18, "Dispaly Test ^_^",YELLOW);
	int temp;
        /*等待注满缓冲区*/
        while(Sequeue_Getlen(&ADC_BUFFER) < Sequeue_GetlenMax(&ADC_BUFFER)/2 )
        {
          Ad_Get_Service();
        }
	while (1)
	{
		sprintf(LCD_BUFFER,"AD_Get:%d",temp);
		//LCD_ShowString(140,200,18,LCD_BUFFER);
		Ad_Get_Service();
		while (Sequeue_Getlen(&ADC_BUFFER) and (Sequeue_Getlen(&(DAC1_BUFFER.TxBuffer))
			< Sequeue_GetlenMax(&(DAC1_BUFFER.TxBuffer))-20))
		{
			temp = Sequeue_Get_Front(&ADC_BUFFER);
			Sequeue_In_Queue(&(DAC1_BUFFER.TxBuffer), temp & 0xfff);
			Sequeue_Out_Queue(&ADC_BUFFER);
			//DAC_SetChannel1Data(DAC_Align_12b_R, temp);    //设置数据右对齐，转换值设置为102
			//DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);    //软件触发通道1 开始转换

		}
		Dac_Send_Service();
                

	}

}

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
	TIM_TimeBaseStructure.TIM_Period = 8; //����0.05msһ��TIM2�Ƚϵ�����
	TIM_TimeBaseStructure.TIM_Prescaler = 7; //ϵͳ��Ƶ72M�������Ƶ72���൱��1M�Ķ�ʱ��2ʱ��
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update); //����ʱ�� ����DAC
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //������ϸ˵��
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //TIM_OutputState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 4;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�����PWM1ҪΪLow��PWM2��ΪHigh
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
        /*�ȴ�ע��������*/
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
			//DAC_SetChannel1Data(DAC_Align_12b_R, temp);    //���������Ҷ��룬ת��ֵ����Ϊ102
			//DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);    //�������ͨ��1 ��ʼת��

		}
		Dac_Send_Service();
                

	}

}

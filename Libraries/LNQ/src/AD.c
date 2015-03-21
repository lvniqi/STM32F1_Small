/*
 * AD.c
 *
 *  Created on: 2014��6��9��
 *      Author: lvniqi
 */
#include "AD.h"
u16 _ADC_BUFFER[ADC_BUFFER_LEN];
int_sequeue ADC_BUFFER;
/*���ò���ͨ���˿� ʹ��GPIOʱ��	  ����ADC����PA0�˿��ź�*/
void ADC1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		    //GPIO����Ϊģ������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*����ADC1�Ĺ���ģʽΪDMAģʽ  */
void ADC1_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	/*��ʱ���� ��Ҫ�޸�*/
	Sequeue_Init(&ADC_BUFFER, _ADC_BUFFER,ADC_BUFFER_LEN);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //ʹ��MDA1ʱ��
	//DMA channel1 configuration
	DMA_DeInit(DMA1_Channel1);  //ָ��DMAͨ�� ����
	DMA_InitStructure.DMA_PeripheralBaseAddr = DRIVE_DR_BASE(ADC1);  //����DMA�����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32) ADC_BUFFER.dataspace; //����DMA�ڴ��ַ��ADCת�����ֱ�ӷ���õ�ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //����Ϊ����Ϊ���ݴ������Դ
	DMA_InitStructure.DMA_BufferSize = ADC_BUFFER_LEN;	//DMA����������ΪADC_BUFFER_LEN��
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);


	//Enable DMA channel1
	DMA_Cmd(DMA1_Channel1, ENABLE);  //ʹ��DMAͨ��

	/* ADC1 configuration */
	ADC_DeInit(ADC1);
	ADC_InitTypeDef ADC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	//ʹ��ADC1ʱ��
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ʹ�ö���ģʽ��ɨ��ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; //��ͨ�� ����ɨ��
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //����ת�� �ر�
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ʹ�������Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2; //����TIM2_CC2Ϊ����Դ
	ADC_InitStructure.ADC_NbrOfChannel = 1;  // ֻ��1��ת��ͨ��
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC1 regular channel11 configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_1Cycles5); //ͨ��1��������55.5��ʱ������
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE); //ʹ��ADC1
	Adc_Calibaration(ADC1);
	/* Start ADC1 Software Conversion */
	ADC_ExternalTrigConvCmd(ADC1, ENABLE); //ʹ���ⲿ����
	ADC_DMACmd(ADC1, ENABLE);
}
void ADC1_Init(void)
{
    ADC1_GPIO_Config();
    ADC1_Mode_Config();
}
/********************************************************************
 * ���� : Ad_Get_Service
 * ���� : AD ������ �õ�������
 * ���� : ��
 * ��� : ��
 ***********************************************************************/
void Ad_Get_Service()
{
	u16 len;
	len = (ADC_BUFFER).len_max - (u32) DMA1_Channel1->CNDTR;
	if (len != 0 and len != (ADC_BUFFER).len_max)
	{
		(ADC_BUFFER).rear = len;
		if ((ADC_BUFFER).rear == (ADC_BUFFER).front)
		{
			if ((ADC_BUFFER).len != 0)
			{
				(ADC_BUFFER).len = (ADC_BUFFER).len_max;
			}
		}
		else
		{

			(ADC_BUFFER).len = ((ADC_BUFFER).rear + (ADC_BUFFER).len_max
					- (ADC_BUFFER).front) % ((ADC_BUFFER).len_max);
		}
	}
	else
	{
		if ((ADC_BUFFER).rear != (len % (ADC_BUFFER).len_max))
		{
			(ADC_BUFFER).len = (ADC_BUFFER).len_max - (ADC_BUFFER).front;
			(ADC_BUFFER).rear = 0;
		}
	}
}

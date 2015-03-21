/*
 * AD.c
 *
 *  Created on: 2014年6月9日
 *      Author: lvniqi
 */
#include "AD.h"
u16 _ADC_BUFFER[ADC_BUFFER_LEN];
int_sequeue ADC_BUFFER;
/*配置采样通道端口 使能GPIO时钟	  设置ADC采样PA0端口信号*/
void ADC1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		    //GPIO设置为模拟输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*配置ADC1的工作模式为DMA模式  */
void ADC1_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	/*暂时用用 需要修改*/
	Sequeue_Init(&ADC_BUFFER, _ADC_BUFFER,ADC_BUFFER_LEN);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //使能MDA1时钟
	//DMA channel1 configuration
	DMA_DeInit(DMA1_Channel1);  //指定DMA通道 禁用
	DMA_InitStructure.DMA_PeripheralBaseAddr = DRIVE_DR_BASE(ADC1);  //设置DMA外设地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32) ADC_BUFFER.dataspace; //设置DMA内存地址，ADC转换结果直接放入该地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //外设为设置为数据传输的来源
	DMA_InitStructure.DMA_BufferSize = ADC_BUFFER_LEN;	//DMA缓冲区设置为ADC_BUFFER_LEN；
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);


	//Enable DMA channel1
	DMA_Cmd(DMA1_Channel1, ENABLE);  //使能DMA通道

	/* ADC1 configuration */
	ADC_DeInit(ADC1);
	ADC_InitTypeDef ADC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	//使能ADC1时钟
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //使用独立模式，扫描模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; //单通道 无需扫描
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //连续转换 关闭
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //使用数据右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2; //配置TIM2_CC2为触发源
	ADC_InitStructure.ADC_NbrOfChannel = 1;  // 只有1个转换通道
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC1 regular channel11 configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_1Cycles5); //通道1采样周期55.5个时钟周期
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE); //使能ADC1
	Adc_Calibaration(ADC1);
	/* Start ADC1 Software Conversion */
	ADC_ExternalTrigConvCmd(ADC1, ENABLE); //使能外部触发
	ADC_DMACmd(ADC1, ENABLE);
}
void ADC1_Init(void)
{
    ADC1_GPIO_Config();
    ADC1_Mode_Config();
}
/********************************************************************
 * 名称 : Ad_Get_Service
 * 功能 : AD 缓冲区 得到服务函数
 * 输入 : 无
 * 输出 : 无
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

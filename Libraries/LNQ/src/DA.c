/*
 * DA.c
 *
 *  Created on: 2014年7月27日
 *      Author: lvniqi
 */
#include "DA.h"
u16 _DAC_1_Buffer[DAC_Buffer_LEN];
DAC_BUFFER DAC1_BUFFER;
int Dac_Init(uint32_t DAC_Channel, uint32_t DAC_TRIGGER)
{
	if (not(IS_DAC_CHANNEL(DAC_Channel) and IS_DAC_TRIGGER(DAC_TRIGGER)))
	{
		return 0;
	}
	else if (DAC_Channel_1 == DAC_Channel)
	{
		/*GPIO初始化*/
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		/*设置引脚*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;    //A4
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;    //DAC设置为IN模式
		GPIO_Init(GPIOA, &GPIO_InitStructure);    // 初始化 PA4
		/*DAC初始化*/
		DAC_InitTypeDef DAC_InitStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
		DAC_InitStructure.DAC_Trigger = DAC_TRIGGER;    //不使用触发功能
		DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None; //不使用波形发生功能
		DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude =
				DAC_LFSRUnmask_Bit0;    //不屏蔽幅值选择
		DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;   //使用缓存输出
		DAC_Init(DAC_Channel_1, &DAC_InitStructure);

		/*DMA设置*/
		DMA_InitTypeDef DMA_InitStructure;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
		DAC1_BUFFER.TxBuffer.dataspace = _DAC_1_Buffer;
		DAC1_BUFFER.TxBuffer.len_should_cut = 0;
		DAC1_BUFFER.TxFinishedFlag = 1;
		Sequeue_Set_Null(&(DAC1_BUFFER.TxBuffer), DAC_Buffer_LEN);
		DMA_DeInit(DMA2_Channel3);
		DMA_InitStructure.DMA_PeripheralBaseAddr = DAC1_DR_BASE;
		DMA_InitStructure.DMA_MemoryBaseAddr =
				(uint32_t) DAC1_BUFFER.TxBuffer.dataspace;
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
		DMA_InitStructure.DMA_BufferSize = 32;
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize =
				DMA_PeripheralDataSize_HalfWord;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

		DMA_Init(DMA2_Channel3, &DMA_InitStructure);
		DAC_Cmd(DAC_Channel_1, ENABLE);
		DAC_DMACmd(DAC_Channel_1, ENABLE);

		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		return 1;
	}
	return 0;
}

/********************************************************************
 * 名称 :Dac_Send_Service
 * 功能 : DAC发送服务函数
 * 输入 : 无
 * 输出 : 无
 ***********************************************************************/
void Dac_Send_Service()
{
	if ((DAC1_BUFFER.TxFinishedFlag))
	{
		DMA_Cmd(DMA2_Channel3, DISABLE);
		DMA_ITConfig(DMA2_Channel3, DMA_IT_TC, DISABLE);
		(DAC1_BUFFER.TxBuffer).len -= (DAC1_BUFFER.TxBuffer).len_should_cut;
		(DAC1_BUFFER.TxBuffer).len_should_cut = 0;
		if ((DAC1_BUFFER.TxBuffer).len)
		{
			if ((DAC1_BUFFER.TxBuffer).rear >= (DAC1_BUFFER.TxBuffer).front)
			{
				DMA2_Channel3->CNDTR = (DAC1_BUFFER.TxBuffer).len;
				DMA2_Channel3->CMAR =
						(uint32_t) ((DAC1_BUFFER.TxBuffer).dataspace
								+ (DAC1_BUFFER.TxBuffer).front);
				(DAC1_BUFFER.TxBuffer).len_should_cut =
						(DAC1_BUFFER.TxBuffer).len;
				(DAC1_BUFFER.TxBuffer).front = (DAC1_BUFFER.TxBuffer).rear;
			}
			else
			{
				DMA2_Channel3->CNDTR = (DAC1_BUFFER.TxBuffer).len_max
						- (DAC1_BUFFER.TxBuffer).front;
				DMA2_Channel3->CMAR =
						(uint32_t) ((DAC1_BUFFER.TxBuffer).dataspace
								+ (DAC1_BUFFER.TxBuffer).front);
				(DAC1_BUFFER.TxBuffer).front = 0;
				(DAC1_BUFFER.TxBuffer).len_should_cut =
						(DAC1_BUFFER.TxBuffer).len
								- (DAC1_BUFFER.TxBuffer).rear;
			}
			(DAC1_BUFFER.TxFinishedFlag) = False;
			DMA_ITConfig(DMA2_Channel3, DMA_IT_TC, ENABLE);
			DMA_Cmd(DMA2_Channel3, ENABLE);
		}
	}
}

 
void DAC_Task(u16 i)
{
    DAC_SetChannel1Data(DAC_Align_12b_R,i);    //设置数据右对齐，转换值设置为1028
    DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);    //软件触发通道1 开始转换
}
 
void DAC_Config(void)
{
    DAC_InitTypeDef DAC_InitStructure;                  //DAC库函数结构体
    GPIO_InitTypeDef GPIO_InitStructure;            //GPIO结构体
 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC , ENABLE);   //DAC 时钟使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);   //GPIO 时钟使能
 
    //将GPIO配置为DAC的模拟复用功能
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA,&GPIO_InitStructure);  //A4为通道1 A5为通道2
 
    //DAC通道1、通道2配置
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_Software;       //设置触发方式为软件触发
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;     //无波形产生
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;    //DAC输出缓冲使能
    DAC_Init(DAC_Channel_1 ,&DAC_InitStructure); //使能以上设置到DAC1      
 
    DAC_Cmd(DAC_Channel_1 ,ENABLE);      //最后的开关
 
    DAC_Task(0);
                   
}
/********************************************************************
 * 名称 : DMA2_Channel4_IRQHandler
 * 功能 : DMA 3 通道中断
 * 输入 : 无
 * 输出 : 无
 ***********************************************************************/
void DMA2_Channel3_IRQHandler(void)
{
	if (DMA_GetFlagStatus(DMA2_FLAG_TC3) != RESET)
	{
		DMA_ClearITPendingBit(DMA2_IT_TC3);
		(DAC1_BUFFER.TxFinishedFlag) = True;
                Dac_Send_Service();
	}
}

/*
 * main.cpp
 *
 *  Created on: 2014-4-28
 *      Author: lvniqi
 */
#include "USART.h"
#ifdef USART1_USED_
/* Public Variable ------------------------------------------------------- */
char _USART1_RX_BUFFER[USART1_RX_BUFFER_LEN];
char _USART1_TX_BUFFER[USART1_TX_BUFFER_LEN];
USART_BUFFER USART1_BUFFER;
/* public functions ------------------------------------------------------- */
/********************************************************************
 * ���� : NVIC_Configuration_USART1
 * ���� : ��ʼ������1�жϣ�������ʹ�ã�
 * ���� : ��
 * ��� : ��
 ***********************************************************************/
void NVIC_Configuration_USART1(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); //ѡ����鷽ʽ0

	/* ʹ�� USART1 �ж� */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/********************************************************************
 * ���� : USART1_IRQHandler
 * ���� : ����1�жϷ�������������ʹ�ã�
 * ���� : ��
 * ��� : ��
 ***********************************************************************/
void USART1_IRQHandler(void)
{

	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //����Ĵ�����������
	{
		char a;
		a = USART_ReceiveData(USART1);
		Sequeue_In_Queue(&(USART1_BUFFER.RxBuffer), a);
	}
}

/********************************************************************
 * ���� : DMA_Configuration_Usart_1
 * ���� : USART1��DMA����(�Ƽ�ʹ��)
 * ���� : ��
 * ��� : ��
 ***********************************************************************/
void DMA_Configuration_Usart_1(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	/* DMA clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/*Buffer init*/
	USART_Buffer_Init(&USART1_BUFFER, _USART1_RX_BUFFER, _USART1_TX_BUFFER,
	USART1_RX_BUFFER_LEN, USART1_TX_BUFFER_LEN);
	/*uart1 TX Config*/
	DMA_DeInit(USART1_DMA_CHANNEL_TX);
	DMA_InitStructure.DMA_PeripheralBaseAddr = DRIVE_DR_BASE(USART1); //Դ��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = SEQUEUE_DR_BASE(
			(USART1_BUFFER.TxBuffer)); //Ŀ���ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; //������Ϊ���ݴ����Ŀ�ĵ�
	DMA_InitStructure.DMA_BufferSize = 0; //���ݳ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //����Ĵ���������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //�ڴ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //�������ݿ�� 1�ֽ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //�ڴ����ݿ�� 1�ֽ�
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //����ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //�����ȼ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //���ڴ浽�ڴ洫��
	DMA_Init(USART1_DMA_CHANNEL_TX, &DMA_InitStructure);

	/*uart1 RX Config*/
	DMA_DeInit(USART1_DMA_CHANNEL_RX);
	DMA_InitStructure.DMA_PeripheralBaseAddr = DRIVE_DR_BASE(USART1);
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //����ѭ��ģʽ
	DMA_InitStructure.DMA_MemoryBaseAddr = SEQUEUE_DR_BASE(
			(USART1_BUFFER.RxBuffer));
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //�ڴ���Ϊ���ݴ����Ŀ�ĵ�
	DMA_InitStructure.DMA_BufferSize = USART1_RX_BUFFER_LEN;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; //�е����ȼ�
	DMA_Init(USART1_DMA_CHANNEL_RX, &DMA_InitStructure);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

/********************************************************************
 * ���� : Usart1_Dma_Open
 * ���� : USART1 DMA ��
 * ���� : ������
 * ��� : ��
 ***********************************************************************/
void Usart1_Dma_Open(u32 BaudRate)
{
	InitUsart(USART1, BaudRate);
	USART_Cmd(USART1, ENABLE); //  ����USART1
	DMA_Configuration_Usart_1(); //���DMA����
	//NVIC_Configuration_USART1();//���ô����ж�
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	DMA_Cmd(USART1_DMA_CHANNEL_RX, ENABLE);

	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	DMA_ITConfig(USART1_DMA_CHANNEL_TX, DMA_IT_TC, ENABLE);

}
/********************************************************************
 * ���� : DMA1_Channel5_IRQHandler
 * ���� : DMA 5 ͨ���ж�
 * ���� : ��
 * ��� : ��
 ***********************************************************************/
void DMA1_Channel5_IRQHandler(void)
{
	if (DMA_GetFlagStatus(DMA1_FLAG_TC5) != RESET)
	{
		DMA_ClearITPendingBit(DMA1_IT_TC5);
		(USART1_BUFFER.RxBuffer).rear = 0;
		(USART1_BUFFER.RxBuffer).len = (USART1_BUFFER.RxBuffer).len_max
				- (USART1_BUFFER.RxBuffer).front;
	}
}
/********************************************************************
 * ���� : DMA1_Channel4_IRQHandler
 * ���� : DMA 4 ͨ���ж�
 * ���� : ��
 * ��� : ��
 ***********************************************************************/
void DMA1_Channel4_IRQHandler(void)
{
	if (DMA_GetFlagStatus(DMA1_FLAG_TC4) != RESET)
	{
		DMA_ClearITPendingBit(DMA1_IT_TC4);
		(USART1_BUFFER.TxFinishedFlag) = True;
	}
}
/********************************************************************
 * ���� : Usart1_Buffer_Rx_Service
 * ���� : USART1 ������ ��ȡ������
 * ���� : ��
 * ��� : ��
 ***********************************************************************/
void Usart1_Buffer_Rx_Service()
{
	u16 len;
	len = (USART1_BUFFER.RxBuffer).len_max - (u32) USART1_DMA_CHANNEL_RX->CNDTR;
	if (len != 0 and len != (USART1_BUFFER.RxBuffer).len_max)
	{
		(USART1_BUFFER.RxBuffer).rear = len;
		if ((USART1_BUFFER.RxBuffer).rear == (USART1_BUFFER.RxBuffer).front)
		{
			if ((USART1_BUFFER.RxBuffer).len != 0)
			{
				(USART1_BUFFER.RxBuffer).len = (USART1_BUFFER.RxBuffer).len_max;
			}
		}
		else
		{

			(USART1_BUFFER.RxBuffer).len = ((USART1_BUFFER.RxBuffer).rear
					+ (USART1_BUFFER.RxBuffer).len_max
					- (USART1_BUFFER.RxBuffer).front)
					% ((USART1_BUFFER.RxBuffer).len_max);
		}
	}
	else
	{
		if ((USART1_BUFFER.RxBuffer).rear
				!= (len % (USART1_BUFFER.RxBuffer).len_max))
		{
			(USART1_BUFFER.RxBuffer).len = (USART1_BUFFER.RxBuffer).len_max
					- (USART1_BUFFER.RxBuffer).front;
			(USART1_BUFFER.RxBuffer).rear = 0;
		}
	}
}

/********************************************************************
 * ���� : Usart1_Buffer_Tx_Service
 * ���� : USART1 ������ ���ͷ�����
 * ���� : ��
 * ��� : ��
 ***********************************************************************/
void Usart1_Buffer_Tx_Service()
{
	static u8 un_send_time = 0;
	if ((Sequeue_Getlen(&(USART1_BUFFER.TxBuffer))
			< Sequeue_GetlenMax(&(USART1_BUFFER.TxBuffer)) / 3)
			and (un_send_time < 3))
	{
		un_send_time++;
		return;
	}
	else
	{
		un_send_time = 0;
	}
	if ((USART1_BUFFER.TxFinishedFlag))
	{
		DMA_Cmd(USART1_DMA_CHANNEL_TX, DISABLE);
		DMA_ITConfig(USART1_DMA_CHANNEL_TX, DMA_IT_TC, DISABLE);
		(USART1_BUFFER.TxBuffer).len -= (USART1_BUFFER.TxBuffer).len_should_cut;
		(USART1_BUFFER.TxBuffer).len_should_cut = 0;
		if ((USART1_BUFFER.TxBuffer).len)
		{
			if ((USART1_BUFFER.TxBuffer).rear >= (USART1_BUFFER.TxBuffer).front)
			{
				USART1_DMA_CHANNEL_TX->CNDTR = (USART1_BUFFER.TxBuffer).len;
				USART1_DMA_CHANNEL_TX->CMAR =
						(uint32_t) ((USART1_BUFFER.TxBuffer).dataspace
								+ (USART1_BUFFER.TxBuffer).front);
				(USART1_BUFFER.TxBuffer).len_should_cut =
						(USART1_BUFFER.TxBuffer).len;
				(USART1_BUFFER.TxBuffer).front = (USART1_BUFFER.TxBuffer).rear;
			}
			else
			{
				USART1_DMA_CHANNEL_TX->CNDTR = (USART1_BUFFER.TxBuffer).len_max
						- (USART1_BUFFER.TxBuffer).front;
				USART1_DMA_CHANNEL_TX->CMAR =
						(uint32_t) ((USART1_BUFFER.TxBuffer).dataspace
								+ (USART1_BUFFER.TxBuffer).front);
				(USART1_BUFFER.TxBuffer).front = 0;
				(USART1_BUFFER.TxBuffer).len_should_cut =
						(USART1_BUFFER.TxBuffer).len
								- (USART1_BUFFER.TxBuffer).rear;
			}
			(USART1_BUFFER.TxFinishedFlag) = False;
			DMA_ITConfig(USART1_DMA_CHANNEL_TX, DMA_IT_TC, ENABLE);
			DMA_Cmd(USART1_DMA_CHANNEL_TX, ENABLE);
		}
	}
}
#endif

#ifdef USART3_USED_
char _USART3_RX_BUFFER[USART3_RX_BUFFER_LEN];
char _USART3_TX_BUFFER[USART3_TX_BUFFER_LEN];
USART_BUFFER USART3_BUFFER;

/********************************************************************
 * ���� : NVIC_Configuration_USART3
 * ���� : ��ʼ������3�жϣ�������ʹ�ã�
 * ���� : ��
 * ��� : ��
 ***********************************************************************/
void NVIC_Configuration_USART3(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);    //ѡ����鷽ʽ0

	/* ʹ�� USART3 �ж� */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
/********************************************************************
 * ���� : USART3_IRQHandler
 * ���� : ����3�жϷ�������������ʹ�ã�
 * ���� : ��
 * ��� : ��
 ***********************************************************************/
void USART3_IRQHandler(void)
{
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		char a;
		a = USART_ReceiveData(USART3);
		USART_SendData(USART3, a);
	}
}
/********************************************************************
 * ���� : DMA1_Channel3_IRQHandler
 * ���� : DMA 3 ͨ���ж�
 * ���� : ��
 * ��� : ��
 ***********************************************************************/
void DMA1_Channel3_IRQHandler(void)
{
	if (DMA_GetFlagStatus(DMA1_FLAG_TC3) != RESET)
	{
		DMA_ClearITPendingBit(DMA1_IT_TC3);
		(USART3_BUFFER.RxBuffer).rear = 0;
		(USART3_BUFFER.RxBuffer).len = (USART3_BUFFER.RxBuffer).len_max
				- (USART3_BUFFER.RxBuffer).front;
	}
}
/********************************************************************
 * ���� : DMA1_Channel2_IRQHandler
 * ���� : DMA 2 ͨ���ж�
 * ���� : ��
 * ��� : ��
 ***********************************************************************/
void DMA1_Channel2_IRQHandler(void)
{
	if (DMA_GetFlagStatus(DMA1_FLAG_TC2) != RESET)
	{
		DMA_ClearITPendingBit(DMA1_IT_TC2);
		(USART3_BUFFER.TxFinishedFlag) = True;
	}
}
/********************************************************************
 * ���� : DMA_Configuration_Usart_3
 * ���� : USART1��DMA����(�Ƽ�ʹ��)
 * ���� : ��
 * ��� : ��
 ***********************************************************************/
void DMA_Configuration_Usart_3(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	/* DMA clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/*Buffer init*/
	USART_Buffer_Init(&USART3_BUFFER, _USART3_RX_BUFFER, _USART3_TX_BUFFER,
	USART3_RX_BUFFER_LEN, USART3_TX_BUFFER_LEN);
	
	/*uart3 TX Config*/
	DMA_DeInit(USART3_DMA_CHANNEL_TX);
	DMA_InitStructure.DMA_PeripheralBaseAddr = DRIVE_DR_BASE(USART3);    //Դ��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = SEQUEUE_DR_BASE(
			(USART3_BUFFER.TxBuffer)); //Ŀ���ַ
        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//������Ϊ���ݴ����Ŀ�ĵ�
	DMA_InitStructure.DMA_BufferSize = 0;//���ݳ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//����Ĵ���������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�ڴ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�������ݿ�� 1�ֽ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�ڴ����ݿ�� 1�ֽ�
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//����ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//�����ȼ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//���ڴ浽�ڴ洫��
	DMA_Init(USART3_DMA_CHANNEL_TX, &DMA_InitStructure);

	/*uart3 RX Config*/
	DMA_DeInit(USART3_DMA_CHANNEL_RX);
	DMA_InitStructure.DMA_PeripheralBaseAddr = DRIVE_DR_BASE(USART3);
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;    //����ѭ��ģʽ
	DMA_InitStructure.DMA_MemoryBaseAddr = SEQUEUE_DR_BASE(
			(USART3_BUFFER.RxBuffer)); //Ŀ���ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//�ڴ���Ϊ���ݴ����Ŀ�ĵ�
	DMA_InitStructure.DMA_BufferSize = USART3_RX_BUFFER_LEN;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//�е����ȼ�
	DMA_Init(USART3_DMA_CHANNEL_RX, &DMA_InitStructure);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void Usart3_Dma_Open(u32 BaudRate)
{
	InitUsart(USART3, BaudRate);
	USART_Cmd(USART3, ENABLE); //  ����USART1
	DMA_Configuration_Usart_3(); //���DMA����
	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
	DMA_Cmd(USART3_DMA_CHANNEL_RX, ENABLE);

	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
	DMA_ITConfig(USART3_DMA_CHANNEL_TX, DMA_IT_TC, ENABLE);

}
/********************************************************************
 * ���� : Usart3_Buffer_Rx_Service
 * ���� : USART3 ������ ��ȡ������
 * ���� : ��
 * ��� : ��
 ***********************************************************************/
void Usart3_Buffer_Rx_Service()
{
	u16 len;
	len = (USART3_BUFFER.RxBuffer).len_max - (u32) USART3_DMA_CHANNEL_RX->CNDTR;
	if (len != 0 and len != (USART3_BUFFER.RxBuffer).len_max)
	{
		(USART3_BUFFER.RxBuffer).rear = len;
		if ((USART3_BUFFER.RxBuffer).rear == (USART3_BUFFER.RxBuffer).front)
		{
			if ((USART3_BUFFER.RxBuffer).len != 0)
			{
				(USART3_BUFFER.RxBuffer).len = (USART3_BUFFER.RxBuffer).len_max;
			}
		}
		else
		{

			(USART3_BUFFER.RxBuffer).len = ((USART3_BUFFER.RxBuffer).rear
					+ (USART3_BUFFER.RxBuffer).len_max
					- (USART3_BUFFER.RxBuffer).front)
					% ((USART3_BUFFER.RxBuffer).len_max);
		}
	}
	else
	{
		if ((USART3_BUFFER.RxBuffer).rear
				!= (len % (USART3_BUFFER.RxBuffer).len_max))
		{
			(USART3_BUFFER.RxBuffer).len = (USART3_BUFFER.RxBuffer).len_max
					- (USART3_BUFFER.RxBuffer).front;
			(USART3_BUFFER.RxBuffer).rear = 0;
		}
	}
}

/***********************************************************************
 * ���� : Usart3_Buffer_Tx_Service
 * ���� : USART3 ������ ���ͷ�����
 * ���� : ��
 * ��� : ��
 ***********************************************************************/
void Usart3_Buffer_Tx_Service()
{
	static u8 un_send_time = 0;
	if ((Sequeue_Getlen(&(USART3_BUFFER.TxBuffer))
			< Sequeue_GetlenMax(&(USART3_BUFFER.TxBuffer)) / 3)
			and (un_send_time < 3))
	{
		un_send_time++;
		return;
	}
	else
	{
		un_send_time = 0;
	}
	if ((USART3_BUFFER.TxFinishedFlag))
	{
		DMA_Cmd(USART3_DMA_CHANNEL_TX, DISABLE);
		DMA_ITConfig(USART3_DMA_CHANNEL_TX, DMA_IT_TC, DISABLE);
		(USART3_BUFFER.TxBuffer).len -= (USART3_BUFFER.TxBuffer).len_should_cut;
		(USART3_BUFFER.TxBuffer).len_should_cut = 0;
		if ((USART3_BUFFER.TxBuffer).len)
		{
			if ((USART3_BUFFER.TxBuffer).rear >= (USART3_BUFFER.TxBuffer).front)
			{
				USART3_DMA_CHANNEL_TX->CNDTR = (USART3_BUFFER.TxBuffer).len;
				USART3_DMA_CHANNEL_TX->CMAR =
						(uint32_t) ((USART3_BUFFER.TxBuffer).dataspace
								+ (USART3_BUFFER.TxBuffer).front);
				(USART3_BUFFER.TxBuffer).len_should_cut =
						(USART3_BUFFER.TxBuffer).len;
				(USART3_BUFFER.TxBuffer).front = (USART3_BUFFER.TxBuffer).rear;
			}
			else
			{
				USART3_DMA_CHANNEL_TX->CNDTR = (USART3_BUFFER.TxBuffer).len_max
						- (USART3_BUFFER.TxBuffer).front;
				USART3_DMA_CHANNEL_TX->CMAR =
						(uint32_t) ((USART3_BUFFER.TxBuffer).dataspace
								+ (USART3_BUFFER.TxBuffer).front);
				(USART3_BUFFER.TxBuffer).front = 0;
				(USART3_BUFFER.TxBuffer).len_should_cut =
						(USART3_BUFFER.TxBuffer).len
								- (USART3_BUFFER.TxBuffer).rear;
			}
			(USART3_BUFFER.TxFinishedFlag) = False;
			DMA_ITConfig(USART3_DMA_CHANNEL_TX, DMA_IT_TC, ENABLE);
			DMA_Cmd(USART3_DMA_CHANNEL_TX, ENABLE);
		}
	}
}
#endif

/********************************************************************
 * ���� : USART_Buffer_Init
 * ���� : USART��������ʼ��
 * ���� : ��
 * ��� : ��
 ***********************************************************************/
void USART_Buffer_Init(USART_BUFFER* p, char* rx_buffer, char* tx_buffer,
		u32 rx_len, u32 tx_len)
{
	char_sequeue *temp = &(p->RxBuffer);
	temp->dataspace = rx_buffer;
	Sequeue_Set_Null(temp, rx_len);
	temp->len_should_cut = 0;
	temp = &(p->TxBuffer);
	temp->dataspace = tx_buffer;
	temp->len_should_cut = 0;
	Sequeue_Set_Null(temp, tx_len);
	p->TxFinishedFlag = 1;
}

/********************************************************************
 * ���� : InitUsart
 * ���� : ��ʼ������
 * ���� : USARTx BaudRate
 * ��� : �ɹ�Ϊ1 ���ɹ�Ϊ0
 USART1 : A9 TX  A10 RX
 USART3 : B10 TX  B11 RX
 ***********************************************************************/
u8 InitUsart(USART_TypeDef* USARTx, u32 BaudRate)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	USART_DeInit(USARTx);
#ifdef USART1_USED_
	if (USART1 == USARTx)
	{
		/*����ʱ��*/
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		/*��������*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;    //A9 TX
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);    // ��ʼ�� PA9

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;    //A10 RX
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);    // ��ʼ�� PA10

		/*���־*/
		USART_ClearFlag(USART1, USART_FLAG_TC);
		USART_InitStructure.USART_BaudRate = BaudRate;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl =
		USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART1, &USART_InitStructure);

		return 1;
	}
#endif

#ifdef USART3_USED_
	if(USART3 ==USARTx)
	{
		/*����ʱ��*/
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		/*��������*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //B10 TX
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);// ��ʼ�� PB10

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//B11 RX
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOB, &GPIO_InitStructure);// ��ʼ�� PB11

		/*���־*/
		USART_ClearFlag(USART3, USART_FLAG_TC);
		USART_InitStructure.USART_BaudRate = BaudRate;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART3, &USART_InitStructure);
                return 1;
	}
#endif
	return 0;
}
/********************************************************************
 * ���� : Usart_Send
 * ���� : ���ʹ���
 * ���� : USART_BUFFER data
 * ��� : �ɹ�Ϊ1 ���ɹ�Ϊ0
 ***********************************************************************/
u8 Usart_Send(USART_BUFFER* p, u8 data)
{
	if (
			(Sequeue_Getlen(&(p->TxBuffer))
			>= Sequeue_GetlenMax(&(p->TxBuffer)) / 2
			)
			and (p->TxFinishedFlag)
		)
	{
            #ifdef USART3_USED_
            Usart3_Buffer_Tx_Service();
            #endif
            #ifdef USART1_USED_
            Usart1_Buffer_Tx_Service();
            #endif
	}
	if (not Sequeue_Full(&(p->TxBuffer)))
	{
		Sequeue_In_Queue(&(p->TxBuffer),data);
		return 1;
	}
	return 0;
}


/*
 * NRF2401.c
 *
 *  Created on: 2014��5��11��
 *      Author: lvniqi
 *      Email: lvniqi@gmail.com
 */

#include "NRF2401.h"
void InitNrf2401(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	InitSpi(SPI1, True);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	/*--------------------------IRQ B0�� �ж�����-----------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;/*����ģʽ*/
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* �����ж���0Ϊ�½�����*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
	EXTI_InitStructure.EXTI_Line = EXTI_Line0; //��0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //�����ж�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/*--------------------------ʹ�� B1���ź�---------------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;/*�������*/
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/*--------------------------CSN�ź� A4 ��---------------------------------*/
	/*Enable SPI1.NSS as a GPIO*/
	SPI_SSOutputCmd(SPI1, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void Nrf2401_Open(void)
{
	//ʹ���ź�
	NRF24L01_CE = 0; //��ʹ�ܷ���
	NRF24L01_CSN = 1; //������
	SPI_Cmd(SPI1, ENABLE); //����Nrf2401
}
void Nrf2401_Close(void)
{
	//ʹ���ź�
	NRF24L01_CE = 0; //��ʹ�ܷ���
	NRF24L01_CSN = 1; //������
	SPI_Cmd(SPI1, DISABLE); //�ر�Nrf2401
}
//��ָ��λ�ö���ָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ 
u8 NRF24L01_Read_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status, u8_ctr;
	NRF24L01_CSN = 0;           //ʹ��SPI����
	status = SPI1_ReadWriteByte(reg);           //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
	for (u8_ctr = 0; u8_ctr < len; u8_ctr++)
	{
		pBuf[u8_ctr] = SPI1_ReadWriteByte(0XFF);           //��������
	}
	NRF24L01_CSN = 1;       //�ر�SPI����
	return status;        //���ض�����״ֵ̬
}
//��ָ��λ��дָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status, u8_ctr;
	NRF24L01_CSN = 0;          //ʹ��SPI����
	status = SPI1_ReadWriteByte(reg);          //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
	for (u8_ctr = 0; u8_ctr < len; u8_ctr++)
	{
		SPI1_ReadWriteByte(*pBuf++);
	}
	NRF24L01_CSN = 1;       //�ر�SPI����
	return status;          //���ض�����״ֵ̬
}

u8 Nrf2401_Check(void)
{
	u8 buf[5] =
	{ 0XA5, 0XA5, 0XA5, 0XA5, 0XA5 };
	u8 i;
	NRF24L01_Write_Buf(NRF_WRITE_REG + NRF_TX_ADDR, buf, 5);       //д��5���ֽڵĵ�ַ.
	NRF24L01_Read_Buf(NRF_TX_ADDR, buf, 5); //����д��ĵ�ַ
	printf("%d %d %d %d %d\n", buf[0], buf[1], buf[2], buf[3], buf[4]);
	for (i = 0; i < 5; i++)
	{
		if (buf[i] != 0XA5)
		{
			break;
		}
	}
	if (i != 5)
	{
		return 1; //���24L01����
	}
	return 0;		 //��⵽24L01
}

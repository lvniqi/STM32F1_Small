/*
 * SPI.c
 *
 *  Created on: 2014��5��4��
 *      Author: lvniqi
 *      Email: lvniqi@gmail.com
 */
#include "SPI.h"
/*
 PA4/SPI1_NSS
 PA5/SPI1_SCK
 PA6/SPI1_MISO
 PA7/SPI1_MOSI
 */
u8 InitSpi(SPI_TypeDef *SPIx, u8 master_flag)
{
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	/*SPI���Ϊ18M ��������GPIOΪ10M*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	if (SPI1 == SPIx)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		SPI_Cmd(SPI1, DISABLE);             //�����Ƚ���,���ܸı�MODE
		//����״̬
		if (0 != master_flag)
		{
			//MISO��������״̬
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			//CLK MOSI NSS �������
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
			//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_7;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
                        //A4 NSS
                        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
                        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
                        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			//SPI����
			SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //����ȫ˫��
			SPI_InitStructure.SPI_Mode = SPI_Mode_Master;       //��
			SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;      //8λ
			SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;        //CPOL=0 ʱ�����յ�
			SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;       //CPHA=0 ���ݲ����1��
			SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;        //���NSS

			//���棡 ʹ�ÿ⺯��ʱ��APB2ʱ��Ϊ72M ��SPI��Ҫ����18M ����������Ҫ4��Ƶ
			SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; //8��Ƶ 9Mhz
			SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;      //��λ��ǰ
			SPI_InitStructure.SPI_CRCPolynomial = 7; //CRC7 Ƭ�Ӹ�λ�󣬸�λ��ֵĬ��Ϊ0x07����˽�����Ϊ7��
			SPI_Init(SPI1, &SPI_InitStructure);
		}
		else
		{
			//MOSI CLK  NSS  ��������״̬
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_7;
			//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			// MISO  �������
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(GPIOA, &GPIO_InitStructure);

			//SPI����
			SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //����ȫ˫��
			SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;       //��
			SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;      //8λ
			SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;        //CPOL=0 ʱ�����յ�
			SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;       //CPHA=0 ���ݲ����1��
			SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;        //���NSS

			//���棡 ʹ�ÿ⺯��ʱ��APB2ʱ��Ϊ72M ��SPI��Ҫ����18M ����������Ҫ4��Ƶ
			SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; //4��Ƶ
			SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;      //��λ��ǰ
			SPI_InitStructure.SPI_CRCPolynomial = 7; //CRC7 Ƭ�Ӹ�λ�󣬸�λ��ֵĬ��Ϊ0x07����˽�����Ϊ7��
			SPI_Init(SPI1, &SPI_InitStructure);
		}
		return True;
	}
	return False;
	/*
	 Ӧ��2401���еİ�
	 //IRQ
	 GPIO_SetBits(GPIOB, GPIO_Pin_0);
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	 GPIO_Init(GPIOB, &GPIO_InitStructure);

	 GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
	 ///�����ж���0Ϊ�½�����*
	 EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	 EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	 EXTI_Init(&EXTI_InitStructure);

	 //PB2-CS
	 GPIO_SetBits(GPIOB, GPIO_Pin_2);//Ԥ��Ϊ��
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	 GPIO_Init(GPIOB, &GPIO_InitStructure);
	 //PC4-A0
	 GPIO_SetBits(GPIOC, GPIO_Pin_4);//Ԥ��Ϊ��
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	 GPIO_Init(GPIOC, &GPIO_InitStructure);
	 ///LED
	 GPIO_SetBits(GPIOB, GPIO_Pin_12);//Ԥ��Ϊ��
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	 GPIO_Init(GPIOB, &GPIO_InitStructure);*/

}
void SPI1_WriteByte(u8 TxData)
{
	while ((SPI1->SR & 1 << 1) == 0)
		;		//�ȴ���������
	SPI1->DR = TxData;	 	  		//����һ��byte
}

u8 SPI1_ReadByte()
{
	while ((SPI1->SR & 1 << 0) == 0)
		; 		//�ȴ�������һ��byte
	return SPI1->DR;          		//�����յ�������
}
u8 SPI1_ReadWriteByte(u8 TxData)
{
	SPI1_WriteByte(TxData);
	return SPI1_ReadByte();
}

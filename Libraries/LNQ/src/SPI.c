/*
 * SPI.c
 *
 *  Created on: 2014年5月4日
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
	/*SPI最高为18M 这里设置GPIO为10M*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	if (SPI1 == SPIx)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		SPI_Cmd(SPI1, DISABLE);             //必须先禁能,才能改变MODE
		//主机状态
		if (0 != master_flag)
		{
			//MISO处于悬空状态
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			//CLK MOSI NSS 推挽输出
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
			//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_7;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
                        //A4 NSS
                        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
                        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
                        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			//SPI设置
			SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //两线全双工
			SPI_InitStructure.SPI_Mode = SPI_Mode_Master;       //主
			SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;      //8位
			SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;        //CPOL=0 时钟悬空低
			SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;       //CPHA=0 数据捕获第1个
			SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;        //软件NSS

			//警告！ 使用库函数时，APB2时钟为72M 而SPI需要低于18M 所以至少需要4分频
			SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; //8分频 9Mhz
			SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;      //高位在前
			SPI_InitStructure.SPI_CRCPolynomial = 7; //CRC7 片子复位后，该位的值默认为0x07，因此将其设为7。
			SPI_Init(SPI1, &SPI_InitStructure);
		}
		else
		{
			//MOSI CLK  NSS  处于悬空状态
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_7;
			//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			// MISO  推挽输出
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(GPIOA, &GPIO_InitStructure);

			//SPI设置
			SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //两线全双工
			SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;       //从
			SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;      //8位
			SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;        //CPOL=0 时钟悬空低
			SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;       //CPHA=0 数据捕获第1个
			SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;        //软件NSS

			//警告！ 使用库函数时，APB2时钟为72M 而SPI需要低于18M 所以至少需要4分频
			SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; //4分频
			SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;      //高位在前
			SPI_InitStructure.SPI_CRCPolynomial = 7; //CRC7 片子复位后，该位的值默认为0x07，因此将其设为7。
			SPI_Init(SPI1, &SPI_InitStructure);
		}
		return True;
	}
	return False;
	/*
	 应该2401独有的吧
	 //IRQ
	 GPIO_SetBits(GPIOB, GPIO_Pin_0);
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	 GPIO_Init(GPIOB, &GPIO_InitStructure);

	 GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
	 ///配置中断线0为下降触发*
	 EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	 EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	 EXTI_Init(&EXTI_InitStructure);

	 //PB2-CS
	 GPIO_SetBits(GPIOB, GPIO_Pin_2);//预置为高
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	 GPIO_Init(GPIOB, &GPIO_InitStructure);
	 //PC4-A0
	 GPIO_SetBits(GPIOC, GPIO_Pin_4);//预置为高
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	 GPIO_Init(GPIOC, &GPIO_InitStructure);
	 ///LED
	 GPIO_SetBits(GPIOB, GPIO_Pin_12);//预置为高
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	 GPIO_Init(GPIOB, &GPIO_InitStructure);*/

}
void SPI1_WriteByte(u8 TxData)
{
	while ((SPI1->SR & 1 << 1) == 0)
		;		//等待发送区空
	SPI1->DR = TxData;	 	  		//发送一个byte
}

u8 SPI1_ReadByte()
{
	while ((SPI1->SR & 1 << 0) == 0)
		; 		//等待接收完一个byte
	return SPI1->DR;          		//返回收到的数据
}
u8 SPI1_ReadWriteByte(u8 TxData)
{
	SPI1_WriteByte(TxData);
	return SPI1_ReadByte();
}

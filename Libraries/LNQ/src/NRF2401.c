/*
 * NRF2401.c
 *
 *  Created on: 2014年5月11日
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
	/*--------------------------IRQ B0口 中断请求-----------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;/*上拉模式*/
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* 配置中断线0为下降触发*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
	EXTI_InitStructure.EXTI_Line = EXTI_Line0; //线0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //触发中断
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/*--------------------------使能 B1口信号---------------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;/*推挽输出*/
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/*--------------------------CSN信号 A4 口---------------------------------*/
	/*Enable SPI1.NSS as a GPIO*/
	SPI_SSOutputCmd(SPI1, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void Nrf2401_Open(void)
{
	//使能信号
	NRF24L01_CE = 0; //不使能发送
	NRF24L01_CSN = 1; //不启用
	SPI_Cmd(SPI1, ENABLE); //开启Nrf2401
}
void Nrf2401_Close(void)
{
	//使能信号
	NRF24L01_CE = 0; //不使能发送
	NRF24L01_CSN = 1; //不启用
	SPI_Cmd(SPI1, DISABLE); //关闭Nrf2401
}
//在指定位置读出指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//len:数据长度
//返回值,此次读到的状态寄存器值 
u8 NRF24L01_Read_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status, u8_ctr;
	NRF24L01_CSN = 0;           //使能SPI传输
	status = SPI1_ReadWriteByte(reg);           //发送寄存器值(位置),并读取状态值
	for (u8_ctr = 0; u8_ctr < len; u8_ctr++)
	{
		pBuf[u8_ctr] = SPI1_ReadWriteByte(0XFF);           //读出数据
	}
	NRF24L01_CSN = 1;       //关闭SPI传输
	return status;        //返回读到的状态值
}
//在指定位置写指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//len:数据长度
//返回值,此次读到的状态寄存器值
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status, u8_ctr;
	NRF24L01_CSN = 0;          //使能SPI传输
	status = SPI1_ReadWriteByte(reg);          //发送寄存器值(位置),并读取状态值
	for (u8_ctr = 0; u8_ctr < len; u8_ctr++)
	{
		SPI1_ReadWriteByte(*pBuf++);
	}
	NRF24L01_CSN = 1;       //关闭SPI传输
	return status;          //返回读到的状态值
}

u8 Nrf2401_Check(void)
{
	u8 buf[5] =
	{ 0XA5, 0XA5, 0XA5, 0XA5, 0XA5 };
	u8 i;
	NRF24L01_Write_Buf(NRF_WRITE_REG + NRF_TX_ADDR, buf, 5);       //写入5个字节的地址.
	NRF24L01_Read_Buf(NRF_TX_ADDR, buf, 5); //读出写入的地址
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
		return 1; //检测24L01错误
	}
	return 0;		 //检测到24L01
}

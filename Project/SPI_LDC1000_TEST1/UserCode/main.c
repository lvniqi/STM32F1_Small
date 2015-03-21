#include "stm32f10x.h"
#include <stdio.h>
#include "common.h"
#include "USART.h"
#include "Lcd.h"
#include "exti.h"
#include "DA.h"
#include "AD.h"
#include "SPI.h"
#define LDC1000_CMD_REVID 		0x00
#define LDC1000_CMD_RPMAX 		0x01
#define LDC1000_CMD_RPMIN 		0x02
#define LDC1000_CMD_SENSORFREQ 	0x03
#define LDC1000_CMD_LDCCONFIG 	0x04
#define LDC1000_CMD_CLKCONFIG 	0x05
#define LDC1000_CMD_THRESHILSB 	0x06
#define LDC1000_CMD_THRESHIMSB 	0x07
#define LDC1000_CMD_THRESLOLSB 	0x08
#define LDC1000_CMD_THRESLOMSB 	0x09
#define LDC1000_CMD_INTCONFIG 	0x0A
#define LDC1000_CMD_PWRCONFIG 	0x0B
#define LDC1000_CMD_STATUS	 	0x20
#define LDC1000_CMD_PROXLSB 	0x21
#define LDC1000_CMD_PROXMSB 	0x22
#define LDC1000_CMD_FREQCTRLSB	0x23
#define LDC1000_CMD_FREQCTRMID	0x24
#define LDC1000_CMD_FREQCTRMSB	0x25

// LDC BITMASKS
#define LDC1000_BIT_AMPLITUDE    0x18
#define LDC1000_BIT_RESPTIME     0x07
#define LDC1000_BIT_CLKSEL       0x02
#define LDC1000_BIT_CLKPD        0x01
#define LDC1000_BIT_INTMODE      0x07
#define LDC1000_BIT_PWRMODE      0x01
#define LDC1000_BIT_STATUSOSC    0x80
#define LDC1000_BIT_STATUSDRDYB  0x40
#define LDC1000_BIT_STATUSWAKEUP 0x20
#define LDC1000_BIT_STATUSCOMP   0x10

#define TEST_RPMAX_MAX 	0x13			  /**< maximum calibration value for RPMAX */
#define TEST_RPMAX_MIN 	0x10			  /**< minimum calibration value for RPMAX */
#define TEST_RPMAX_INIT TEST_RPMAX_MIN+1  /**< RPMAX initial value */
#define TEST_RPMIN_MAX 	0x3D   			  /**< maximum calibration value for RPMIN */
#define TEST_RPMIN_MIN 	0x3A    		  /**< minimum calibration value for RPMIN */
#define TEST_RPMIN_INIT TEST_RPMIN_MIN+1  /**< RPMIN initial value */
// Final Test Range
#define TEST_RP_MSB_MAX 0x12	/**< maximum value for proximity data */
#define TEST_RP_MSB_MIN 0x0A	/**< minimum value for proximity data */
#define TEST_FC_MAX 0x0D5D		/**< maximum value for frequency counter */
#define TEST_FC_MIN 0x0D39		/**< minimum value for frequency counter */

#define RPMIN 0x3F
#define RPMAX 0x00

/* Ƭѡʹ���ź� �õ�  */
#define SPI_FLASH_CS_LOW()     GPIO_ResetBits(GPIOA, GPIO_Pin_3)
/* Ƭѡʹ���ź� �ø�  */
#define SPI_FLASH_CS_HIGH()    GPIO_SetBits(GPIOA, GPIO_Pin_3)

void Spi_LCD1000_Init(void)
{
	SPI_InitTypeDef SPI_InitStructure;

	/* ����ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA, ENABLE);
	/* GPIO����*/
	GPIO_InitTypeDef GPIO_InitStructure;
	/* A5: SCK, A7: MOSI */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* MISO  Ϊ ��������*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*A3Ϊ���Ƭѡ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Deselect the FLASH: Chip Select high */
	SPI_FLASH_CS_HIGH();

	/* SPI1 configuration */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);

	/* Enable SPI1  */
	SPI_Cmd(SPI1, ENABLE);
}

void SPI_LCD1000_WriteByte(u8 addr, u8 data)
{
    SPI_FLASH_CS_LOW();
    addr &= ~0x80;
    SPI1_ReadWriteByte(addr);
    SPI1_ReadWriteByte(data);
    while ((SPI1->SR & 1 << 1) == 0);
    SPI_FLASH_CS_HIGH();
}

char spi_readBytes( u8 addr, u8* buffer, u8 len)
{
      SPI_FLASH_CS_LOW();
      addr |= 0x80;
      SPI1_ReadWriteByte(addr);
      while (len > 0) 
      {
          *buffer = SPI1_ReadWriteByte(0x00);
          buffer++;
          len--;
      }
      while ((SPI1->SR & 1 << 1) == 0);
      SPI_FLASH_CS_HIGH();
      return 0;
}
void GPIO_TimPWM(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO , ENABLE);//
 /*GPIOB Configuration: TIM2 channel2*/   //���ø�����Ϊ�����������,���TIM2 CH2��PWM���岨��
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //TIM_CH2
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void TIM2_Configuration(void)
{
 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 TIM_OCInitTypeDef  TIM_OCInitStructure;
 /* TIM2 clock enable */
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
 //����Ƶ��PWMƵ��=72000000/900=80Khz
 /* ---------------------------------------------------------------
 TIM2CLK ��PCLK1=36MHz
 TIM2 Configuration: generate 1 PWM signals :
        TIM2CLK = 36 MHz, Prescaler = 0x0, TIM2 counter clock = 36 MHz
        TIM2 ARR Register = 900 => TIM2 Frequency = TIM2 counter clock/(ARR + 1)
        TIM2 Frequency = 36 KHz.
        TIM2 Channel2 duty cycle = (TIM2_CCR2/ TIM2_ARR)* 100 
 TIM2CLK = 36 MHz, Prescaler = 0, TIM2 counter clock = 36MHz
 --------------------------------------------------------------- */
 /* Time base configuration */
 TIM_TimeBaseStructure.TIM_Period = 20; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ  80K
 TIM_TimeBaseStructure.TIM_Prescaler = 0; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
 TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
 TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
 /* Output Compare Active Mode configuration: Channel3 */
 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
 TIM_OCInitStructure.TIM_Pulse = 10;                   //���ô�װ�벶��ȽϼĴ���������ֵ����ʼ��ռ�ձ�
 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
 TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
 TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
//���������е�OC2ȷ������channle����Ҫ��OC3����channel 3  
    
 TIM_ARRPreloadConfig(TIM2, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ��� 

 /* TIM2 enable counter */
 TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx����
}
u8 proximtyData[2],frequencyData[3];
u8 t;
u32 a;
int main(void)
{
  SystemInit();
  u16 proximtyDataMAX;
  u32 frequencyDataMAX;
  Usart1_Dma_Open(115200);
  Spi_LCD1000_Init();
  GPIO_SetBits(GPIOA, GPIO_Pin_8);
  GPIO_TimPWM();
  TIM2_Configuration();
  //write to register
  SPI_LCD1000_WriteByte(LDC1000_CMD_PWRCONFIG,   0x00);
  SPI_LCD1000_WriteByte(LDC1000_CMD_RPMAX,RPMAX);
  SPI_LCD1000_WriteByte(LDC1000_CMD_RPMIN,RPMIN);
  SPI_LCD1000_WriteByte(LDC1000_CMD_SENSORFREQ,  216);
  SPI_LCD1000_WriteByte(LDC1000_CMD_LDCCONFIG,   0x12);
  SPI_LCD1000_WriteByte(LDC1000_CMD_CLKCONFIG,   0x00);

  SPI_LCD1000_WriteByte(LDC1000_CMD_THRESHILSB,  0x50);
  SPI_LCD1000_WriteByte(LDC1000_CMD_THRESHIMSB,  0x14);
  SPI_LCD1000_WriteByte(LDC1000_CMD_THRESLOLSB,  0xC0);
  SPI_LCD1000_WriteByte(LDC1000_CMD_THRESLOMSB,  0x12);
  SPI_LCD1000_WriteByte(LDC1000_CMD_INTCONFIG,   0x02);
  SPI_LCD1000_WriteByte(LDC1000_CMD_PWRCONFIG,   0x01);
  
  //read all registers

  

  
  while(1)
  {
    Usart1_Buffer_Tx_Service();
    Usart1_Buffer_Rx_Service();
    spi_readBytes(LDC1000_CMD_PROXLSB,proximtyData,2);
    spi_readBytes(LDC1000_CMD_FREQCTRLSB,frequencyData,3);
    a = ( proximtyData[1]<<8) + proximtyData [0];
    proximtyDataMAX = a*0.2+proximtyDataMAX*0.8;
    frequencyDataMAX = (frequencyData[2]<<16)+(frequencyData[1]<<8) + frequencyData[0];
    if((USART1_BUFFER.TxBuffer).len < (USART1_BUFFER.TxBuffer).len_max - 300)
    {
      u8 temp = (proximtyDataMAX>>8)&0xff;
      if(temp >= 0xff)
      {
        temp = 0xff;
      }
      putchar(temp);
      temp = (frequencyDataMAX)&0xff;
      if(temp >= 0xff)
      {
        temp = 0xff;
      }
      putchar(temp);
      putchar(1);
      putchar(1);
      putchar(1);
      putchar(1);
      putchar(1);
      putchar(0xff);
    }
  }
}

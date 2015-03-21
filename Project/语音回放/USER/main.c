#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "exti.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_dac.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_tim.h"
#define ADC1_DR_Address    ((uint32_t)0x4001244C)
#define SPI_MASTER_DR_Base   ((uint32_t)0x4000380C)
#define DAC_DHR12R1_Address    ((uint32_t)0x40007408)
u16 ADCConvertedValue1[2048]={0x0FFF,0x0FFF,0x0070,0x0500};
//u16 ADCConvertedValue2[1024]={0x2222,0xe55e,0x9045,0x00cd};
u8 keysign=0;
u8 keycode=0;
int ii;
u8 mm[5]="nnnn";
uint32_t FLASH_WriteAddress;
ADC_InitTypeDef ADC_InitStructure;
DMA_InitTypeDef DMA_InitStructure;
SPI_InitTypeDef	SPI_InitStructure;
DAC_InitTypeDef DAC_InitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
void TIM_Configuration(void);
void RCC_Configuration(void);
void GPIO_Configuration(void);
void DMAG1_Configuration(void);
void DMAG2_Configuration(void);
void ADC_Configuration(void);
void DMA_DAC_Configuration(void);
void DMAS1_Configuration(void);
void DMAS2_Configuration(void);
u8* T(u16 shu);
/******************************************************************************************/
/* Select SPI FLASH: ChipSelect pin low  */
#define SPI_FLASH_CS_LOW()     GPIO_ResetBits(GPIOA, GPIO_Pin_3)
/* Deselect SPI FLASH: ChipSelect pin high */
#define SPI_FLASH_CS_HIGH()    GPIO_SetBits(GPIOA, GPIO_Pin_3)

/* Exported functions ------------------------------------------------------- */
/*----- High layer function -----*/
void SPI_FLASH_Init(void);
void SPI_FLASH_SectorErase(u32 SectorAddr);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_PageWrite(u16* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferWrite(u16* pBuffer, u32 WriteAddr, u32 NumByteToWrite);
void SPI_FLASH_BufferRead(u16* pBuffer, u32 ReadAddr, u32 NumByteToRead);
u32 SPI_FLASH_ReadID(void);
void SPI_FLASH_StartReadSequence(u32 ReadAddr);

/*----- Low layer function -----*/
u8 SPI_FLASH_ReadByte(void);
u8 SPI_FLASH_SendByte(u8 byte);
u16 SPI_FLASH_SendHalfWord(u16 HalfWord);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);

//#endif /* __SPI_FLASH_H */

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
#define SPI_FLASH_PageSize 256

#define WRITE      0x02  /* Write to Memory instruction */
#define WRSR       0x01  /* Write Status Register instruction */ 
#define WREN       0x06  /* Write enable instruction */

#define READ       0x03  /* Read from Memory instruction */
#define RDSR       0x05  /* Read Status Register instruction  */
#define RDID       0x9F  /* Read identification */
#define SE         0xD8  /* Sector Erase instruction */
#define BE         0xC7  /* Bulk Erase instruction */

#define WIP_Flag   0x01  /* Write In Progress (WIP) flag */

#define Dummy_Byte 0xA5
/******************************************************************************/




















/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
//#define  FLASH_WriteAddress     0x700000
//#define  FLASH_ReadAddress      FLASH_WriteAddress
//#define  FLASH_SectorToErase    FLASH_WriteAddress
#define  M25P64_FLASH_ID        0X202015
#define  BufferSize              32    //(countof(Tx_Buffer)-1)

/* Private macro -------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/ 
u16 Tx_Buffer[32] ={ 2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095, 4056,
                      3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909, 
                      599, 344, 155, 38, 0, 38, 155, 344, 599, 909, 1263, 1647};
u16 i, Rx_Buffer[BufferSize];
u32 k;
volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = PASSED;
vu32 FLASH_ID = 0;
ErrorStatus  HSEStartUpStatus;

/* Private function prototypes -----------------------------------------------*/
void  Delay_Ms(u16 time);
void  RCC_Configuration(void);
void  GPIO_Configuration(void);
void  NVIC_Configuration(void);
void  SPI_Configuration(void);
u16 Buffercmp(u16* pBuffer1, u16* pBuffer2, u16 BufferLength);



/* Private functions ---------------------------------------------------------*/ 
/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint32_t DualSine12bit[32]={0xE01, 0xE02, 0xE03, 0xE04, 0xE05, 0xE06,
                                            0xE07, 0xE08, 0xE09, 0xE0A, 0xF0B, 0xF0C,
                                            0xF0D, 0xF0E, 0xF0F, 0xF10, 0xF11, 0xF12,
                                            0xF13, 0xF14, 0xF15, 0xF16, 0xF17, 0xF18,
                                            0xF19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E,
                                            0xF1F, 0xF20};

int main(void)
{


	delay_init();	    	 //?óê±o¯êy3?ê??¯	  
	NVIC_Configuration(); 	 //éè??NVIC?D??·?×é2:2???à??ó??è????2???ìó|ó??è??
	uart_init(9600);
	LCD_Init();
	EXTI_init();
    RCC_Configuration();
    GPIO_Configuration();
	LCD_ShowHanjian(140,290,26);
	LCD_ShowString(110,250,200,16,16,"Test project for Recording playback");
  	SPI_FLASH_Init();
	while(1)
	{
		if(keysign)
		{
			switch(keycode)
			{
				case 0x00:
									FLASH_WriteAddress=0x070000;
				 					LCD_ShowString(140,230,200,16,16,"record begin");
									for(k=500;k>0;k--)
									{
										ADC_Configuration();
										DMAG1_Configuration();							
										while (!DMA_GetFlagStatus(DMA1_FLAG_TC1));
										SPI_FLASH_BufferWrite(ADCConvertedValue1,FLASH_WriteAddress,1024);
										FLASH_WriteAddress=FLASH_WriteAddress+2048;
									}
									LCD_ShowString(140,180,250,16,16,"record over");
									break;
				case 0x01:
									LCD_Clear1(WHITE);
									LCD_ShowString(140,230,200,16,16,"Playback Star");
									FLASH_WriteAddress=0x070000;
									
									SPI_FLASH_BufferRead(ADCConvertedValue1,FLASH_WriteAddress,1024);									
									DMA_DAC_Configuration();
									for(k=400000;k>0;k--)
									{
										while (!DMA_GetFlagStatus(DMA2_FLAG_TC4));
										FLASH_WriteAddress=FLASH_WriteAddress+2048;
										SPI_FLASH_BufferRead(ADCConvertedValue1,FLASH_WriteAddress,1024);
										DMA_DAC_Configuration();
										DAC_Cmd(DAC_Channel_1, DISABLE);
										DAC_Cmd(DAC_Channel_2, DISABLE);
										DMA_Cmd(DMA2_Channel4, DISABLE);
									}

									LCD_ShowString(140,180,200,16,16,"Playback End"); break;
				case 0x02:
									LCD_ShowString(140,230,200,16,16,"Erase Begin");
									SPI_FLASH_BulkErase();
									Delay_Ms(2000);
									LCD_ShowString(140,200,200,16,16,"Erase Success");break;
				case 0x03:
									POINT_COLOR=GRED;
									LCD_ShowChar(210,200,'3',16);break;
				case 0x04:
									POINT_COLOR=GBLUE;
									LCD_ShowChar(180,180,'4',16);break;
				case 0x05:
									POINT_COLOR=RED;
									LCD_ShowChar(190,180,'5',16);break;
				case 0x06:
									POINT_COLOR=MAGENTA;
									LCD_ShowChar(200,180,'6',16);break;
				case 0x07:
									POINT_COLOR=GREEN;
									LCD_ShowChar(210,180,'7',16);break;
				case 0x08:
									POINT_COLOR=CYAN;
									LCD_ShowChar(180,160,'8',16);break;
				case 0x09:
									POINT_COLOR=YELLOW;
									LCD_ShowChar(190,160,'9',16);break;
				case 0x0a:
									POINT_COLOR=BROWN;
									LCD_ShowChar(200,160,'a',16);break;
				case 0x0b:
									POINT_COLOR=BRRED;
									LCD_ShowChar(210,160,'b',16);break;
				case 0x0c:
									POINT_COLOR=GRAY;
									LCD_ShowChar(180,140,'c',16);break;
				case 0x0d:
									POINT_COLOR=DARKBLUE;
									LCD_ShowChar(190,140,'d',16);break;
				case 0x0e:
									POINT_COLOR=LIGHTBLUE;
									LCD_ShowChar(200,140,'e',16);break;
				case 0x0f:
									POINT_COLOR=GRAYBLUE;
									LCD_ShowChar(210,140,'f',16);break;
			}
			keysign=0;
		}
	}  

/**************************************************************************************************/

}
u8* T(u16 shu)
{
int i;

for(i=3;i>=0;i--)
{
if(shu%16==15)
mm[i]='F';
else if(shu%16==14)
mm[i]='E';
else if(shu%16==13)
mm[i]='D';
else if(shu%16==12)
mm[i]='C';
else if(shu%16==11)
mm[i]='B';
else if(shu%16==10)
mm[i]='A';
else
mm[i]='0'+shu%16;
shu=shu/16;
}

//mm[4]='/0';
return mm;
}

void RCC_Configuration(void)
{ 
   /* Enable peripheral clocks ------------------------------------------------*/
  RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_2);
  RCC_ADCCLKConfig(RCC_PCLK2_Div4); 
  /* DMA2 clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

  /* GPIOA Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  /* DAC Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
  /* TIM2 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
}
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
}
void DMAG1_Configuration(void)
{
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADCConvertedValue1;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize =1024;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode =DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  DMA_Cmd(DMA1_Channel1,ENABLE);
}
void DMAG2_Configuration(void)
{
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADCConvertedValue1;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 1024;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  DMA_Cmd(DMA1_Channel1,ENABLE);
}
void ADC_Configuration(void)
{
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);
  ADC_DMACmd(ADC1, ENABLE);
  ADC_Cmd(ADC1, ENABLE);
  ADC_ResetCalibration(ADC1);
  while(ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void TIM_Configuration(void)
{
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  TIM_TimeBaseStructure.TIM_Period = 1;          
  TIM_TimeBaseStructure.TIM_Prescaler = 1003;       
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
  TIM_Cmd(TIM2, ENABLE);
}

void DMA_DAC_Configuration(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;
  /* System Clocks Configuration */
   
#if !defined STM32F10X_LD_VL && !defined STM32F10X_MD_VL
  /* DMA2 clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
#else
  /* DMA1 clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
#endif
  /* GPIOA Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  /* DAC Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
  /* TIM2 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* TIM2 Configuration */
  /* Time base configuration */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  TIM_TimeBaseStructure.TIM_Period = 1;          
  TIM_TimeBaseStructure.TIM_Prescaler = 504;       
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* TIM2 TRGO selection */
  TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);

  /* DAC channel1 Configuration */
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);

  /* DAC channel2 Configuration */
  DAC_Init(DAC_Channel_2, &DAC_InitStructure);

//#if !defined STM32F10X_LD_VL && !defined STM32F10X_MD_VL
  /* DMA2 channel4 configuration */
  DMA_DeInit(DMA2_Channel4);
//#else
  /* DMA1 channel4 configuration */
  //DMA_DeInit(DMA1_Channel4);
//#endif
  DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R1_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DualSine12bit;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = 32;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

  DMA_Init(DMA2_Channel4, &DMA_InitStructure);
  /* Enable DMA2 Channel4 */
  DMA_Cmd(DMA2_Channel4, ENABLE);
//#else
  //DMA_Init(DMA1_Channel4, &DMA_InitStructure);
  /* Enable DMA1 Channel4 */
 // DMA_Cmd(DMA1_Channel4, ENABLE);
//#endif

  /* Enable DAC Channel1: Once the DAC channel1 is enabled, PA.04 is 
     automatically connected to the DAC converter. */
  DAC_Cmd(DAC_Channel_1, ENABLE);
  /* Enable DAC Channel2: Once the DAC channel2 is enabled, PA.05 is 
     automatically connected to the DAC converter. */
  DAC_Cmd(DAC_Channel_2, ENABLE);

  /* Enable DMA for DAC Channel2 */
  DAC_DMACmd(DAC_Channel_2, ENABLE);
  DAC_DMACmd(DAC_Channel_1, ENABLE);
  /* TIM2 enable counter */
  TIM_Cmd(TIM2, ENABLE);
}
void EXTI1_IRQHandler(void)
{
		keycode=KEY_RAM;
		keycode&=0x0f;
		keysign=1;
	EXTI_ClearITPendingBit(EXTI_Line1);
}



















/*******************************************************************************
* Function Name  : Delay_Ms
* Description    : delay 1 ms.
* Input          : time (ms)
* Output         : None
* Return         : None
*******************************************************************************/
void Delay_Ms(u16 time) 
{ 
	u16 i,j;
	for(i=0;i<time;i++)
  		for(j=0;j<10260;j++)
    		;
}

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures the nested vectored interrupt controller.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/


/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

/*******************************************************************************
* Function Name  : Buffercmp
* Description    : Compares two buffers.
* Input          : - pBuffer1, pBuffer2: buffers to be compared.
*                : - BufferLength: buffer's length
* Output         : None
* Return         : PASSED: pBuffer1 identical to pBuffer2
*                  FAILED: pBuffer1 differs from pBuffer2
*******************************************************************************/
u16 Buffercmp(u16* pBuffer1, u16* pBuffer2, u16 BufferLength)
{
    u16 aa=0;
  	while(BufferLength--)
  	{
    	if(*pBuffer1 != *pBuffer2)
    	{
    	  	aa++;
    	}
    
    	pBuffer1++;
    	pBuffer2++;
  	}

  return aa;  
}
/***********************************************************************************************/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : SPI_FLASH_Init
* Description    : Initializes the peripherals used by the SPI FLASH driver.
* Input          : None
* Output         : None
* Return         : None
CS PA3
SCK PA5
MISO PA6
MOSI PA7
*******************************************************************************/
void SPI_FLASH_Init(void)
{
  	SPI_InitTypeDef  SPI_InitStructure;
  	GPIO_InitTypeDef GPIO_InitStructure;
   
  /* Enable SPI1 and GPIOA clocks */
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA, ENABLE);
  
  /* Configure SPI1 pins: NSS, SCK, MISO and MOSI */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure PA.4 as Output push-pull, used as Flash Chip select */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Deselect the FLASH: Chip Select high */
  	SPI_FLASH_CS_HIGH();

  /* SPI1 configuration */ 
  	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  	SPI_InitStructure.SPI_CRCPolynomial = 7;
  	SPI_Init(SPI1, &SPI_InitStructure);
  
  /* Enable SPI1  */
  	SPI_Cmd(SPI1, ENABLE);   
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SectorErase
* Description    : Erases the specified FLASH sector.
* Input          : SectorAddr: address of the sector to erase.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_SectorErase(u32 SectorAddr)
{
  /* Send write enable instruction */
  	SPI_FLASH_WriteEnable();

  /* Sector Erase */ 
  /* Select the FLASH: Chip Select low */
  	SPI_FLASH_CS_LOW();
  /* Send Sector Erase instruction */
  	SPI_FLASH_SendByte(SE);
  /* Send SectorAddr high nibble address byte */
  	SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  /* Send SectorAddr medium nibble address byte */
  	SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  /* Send SectorAddr low nibble address byte */
  	SPI_FLASH_SendByte(SectorAddr & 0xFF);
  /* Deselect the FLASH: Chip Select high */
  	SPI_FLASH_CS_HIGH();

  /* Wait the end of Flash writing */
  	SPI_FLASH_WaitForWriteEnd();
}

/*******************************************************************************
* Function Name  : SPI_FLASH_BulkErase
* Description    : Erases the entire FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_BulkErase(void)
{
  /* Send write enable instruction */
  	SPI_FLASH_WriteEnable();

  /* Bulk Erase */ 
  /* Select the FLASH: Chip Select low */
  	SPI_FLASH_CS_LOW();
  /* Send Bulk Erase instruction  */
  	SPI_FLASH_SendByte(BE);
  /* Deselect the FLASH: Chip Select high */
  	SPI_FLASH_CS_HIGH();

  /* Wait the end of Flash writing */
  	SPI_FLASH_WaitForWriteEnd();
}

/*******************************************************************************
* Function Name  : SPI_FLASH_PageWrite
* Description    : Writes more than one byte to the FLASH with a single WRITE
*                  cycle(Page WRITE sequence). The number of byte can't exceed
*                  the FLASH page size.
* Input          : - pBuffer : pointer to the buffer  containing the data to be 
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH,
*                    must be equal or less than "SPI_FLASH_PageSize" value. 
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_PageWrite(u16* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  /* Enable the write access to the FLASH */
  	SPI_FLASH_WriteEnable();
  
  /* Select the FLASH: Chip Select low */
  	SPI_FLASH_CS_LOW();
  /* Send "Write to Memory " instruction */
  	SPI_FLASH_SendByte(WRITE);
  /* Send WriteAddr high nibble address byte to write to */
  	SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  /* Send WriteAddr medium nibble address byte to write to */
  	SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);  
  /* Send WriteAddr low nibble address byte to write to */
  	SPI_FLASH_SendByte(WriteAddr & 0xFF);
  
  /* while there is data to be written on the FLASH */
  	while(NumByteToWrite--) 
  	{
    	/* Send the current byte */
    	SPI_FLASH_SendByte((*pBuffer)&0xFF);
    	SPI_FLASH_SendByte(((*pBuffer)&0xFF00)>>8);
    	/* Point on the next byte to be written */
    	pBuffer++; 
  	}
  
  	/* Deselect the FLASH: Chip Select high */
  	SPI_FLASH_CS_HIGH();
  
  	/* Wait the end of Flash writing */
  	SPI_FLASH_WaitForWriteEnd();
}

/*******************************************************************************
* Function Name  : SPI_FLASH_BufferWrite
* Description    : Writes block of data to the FLASH. In this function, the
*                  number of WRITE cycles are reduced, using Page WRITE sequence.
* Input          : - pBuffer : pointer to the buffer  containing the data to be 
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_BufferWrite(u16* pBuffer, u32 WriteAddr, u32 NumByteToWrite)
{
  	u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  	Addr = WriteAddr % SPI_FLASH_PageSize;
  	count = SPI_FLASH_PageSize - Addr;
  	NumOfPage =  2*NumByteToWrite / SPI_FLASH_PageSize;
  	NumOfSingle = (2*NumByteToWrite) % SPI_FLASH_PageSize;
  
  	if(Addr == 0) /* WriteAddr is SPI_FLASH_PageSize aligned  */
  	{
    	if(NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
    	{
      		SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    	}
    	else /* NumByteToWrite > SPI_FLASH_PageSize */ 
    	{
      		while(NumOfPage--)
      		{
        		SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize/2);
        		WriteAddr +=  SPI_FLASH_PageSize;
        		pBuffer += SPI_FLASH_PageSize/2;  
      		}    
     
      		SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle/2);
  	 	}
  	}
  	else /* WriteAddr is not SPI_FLASH_PageSize aligned  */
  	{
    	if(NumOfPage== 0) /* NumByteToWrite < SPI_FLASH_PageSize */
    	{
      		if(NumOfSingle > count) /* (NumByteToWrite + WriteAddr) > SPI_FLASH_PageSize */
      		{	
        		temp = NumOfSingle - count;
      
		        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count/2);
		        WriteAddr +=  count;
		        pBuffer += count/2; 
        
        		SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp/2);
		     }
		     else
		     {
		        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite/2);
		     }
    	}
    	else /* NumByteToWrite > SPI_FLASH_PageSize */
    	{
      		NumByteToWrite -= count;
	      	NumOfPage =  (2*NumByteToWrite) / SPI_FLASH_PageSize;
	      	NumOfSingle = (2*NumByteToWrite) % SPI_FLASH_PageSize;
	      
	      	SPI_FLASH_PageWrite(pBuffer, WriteAddr, count/2);
	      	WriteAddr +=  count;
	      	pBuffer += count/2;  
     
      		while(NumOfPage--)
      		{
        		SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize/2);
        		WriteAddr +=  SPI_FLASH_PageSize;
        		pBuffer += SPI_FLASH_PageSize/2;
      		}
      
      		if(NumOfSingle != 0)
      		{
        		SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle/2);
      		}
    	}
  	}
}

/*******************************************************************************
* Function Name  : SPI_FLASH_BufferRead
* Description    : Reads a block of data from the FLASH.
* Input          : - pBuffer : pointer to the buffer that receives the data read 
*                    from the FLASH.
*                  - ReadAddr : FLASH's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_BufferRead(u16* pBuffer, u32 ReadAddr, u32 NumByteToRead)
{
  /* Select the FLASH: Chip Select low */
  	SPI_FLASH_CS_LOW();
  	//NumByteToRead=NumByteToRead+1;
  /* Send "Read from Memory " instruction */
  	SPI_FLASH_SendByte(READ);
  
  /* Send ReadAddr high nibble address byte to read from */
  	SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte to read from */
  	SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte to read from */
  	SPI_FLASH_SendByte(ReadAddr & 0xFF);

  	while(NumByteToRead--) /* while there is data to be read */
  	{
    	/* Read a byte from the FLASH */
    	*pBuffer =((SPI_FLASH_SendByte(Dummy_Byte))<<8)+(SPI_FLASH_SendByte(Dummy_Byte));
    	/* Point to the next location where the byte read will be saved */
    	pBuffer++;
  	}
  
 	 /* Deselect the FLASH: Chip Select high */
  	SPI_FLASH_CS_HIGH();
}
/*******************************************************************************
* Function Name  : SPI_FLASH_ReadID
* Description    : Reads FLASH identification.
* Input          : None
* Output         : None
* Return         : FLASH identification
*******************************************************************************/
u32 SPI_FLASH_ReadID(void)
{
  	u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  	/* Select the FLASH: Chip Select low */
  	SPI_FLASH_CS_LOW();
  
  /* Send "RDID " instruction */
  	SPI_FLASH_SendByte(0x9F);

  /* Read a byte from the FLASH */
  	Temp0 = SPI_FLASH_SendByte(Dummy_Byte);

  /* Read a byte from the FLASH */
  	Temp1 = SPI_FLASH_SendByte(Dummy_Byte);

  /* Read a byte from the FLASH */
  	Temp2 = SPI_FLASH_SendByte(Dummy_Byte);

  /* Deselect the FLASH: Chip Select high */
  	SPI_FLASH_CS_HIGH();
         
  	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  	return Temp;
}

/*******************************************************************************
* Function Name  : SPI_FLASH_StartReadSequence
* Description    : Initiates a read data byte (READ) sequence from the Flash.
*                  This is done by driving the /CS line low to select the device,
*                  then the READ instruction is transmitted followed by 3 bytes
*                  address. This function exit and keep the /CS line low, so the
*                  Flash still being selected. With this technique the whole
*                  content of the Flash is read with a single READ instruction.
* Input          : - ReadAddr : FLASH's internal address to read from.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_StartReadSequence(u32 ReadAddr)
{
  /* Select the FLASH: Chip Select low */
  	SPI_FLASH_CS_LOW();
  
  /* Send "Read from Memory " instruction */
  	SPI_FLASH_SendByte(READ);

/* Send the 24-bit address of the address to read from -----------------------*/  
  /* Send ReadAddr high nibble address byte */
  	SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte */
  	SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte */
  	SPI_FLASH_SendByte(ReadAddr & 0xFF);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_ReadByte
* Description    : Reads a byte from the SPI Flash.
*                  This function must be used only if the Start_Read_Sequence
*                  function has been previously called.
* Input          : None
* Output         : None
* Return         : Byte Read from the SPI Flash.
*******************************************************************************/
u8 SPI_FLASH_ReadByte(void)
{
  	return (SPI_FLASH_SendByte(Dummy_Byte));
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SendByte
* Description    : Sends a byte through the SPI interface and return the byte 
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
u8 SPI_FLASH_SendByte(u8 byte)
{
  /* Loop while DR register in not emplty */
  	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

  /* Send byte through the SPI1 peripheral */
  	SPI_I2S_SendData(SPI1, byte);

  /* Wait to receive a byte */
  	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  	return SPI_I2S_ReceiveData(SPI1);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SendHalfWord
* Description    : Sends a Half Word through the SPI interface and return the  
*                  Half Word received from the SPI bus.
* Input          : Half Word : Half Word to send.
* Output         : None
* Return         : The value of the received Half Word.
*******************************************************************************/
u16 SPI_FLASH_SendHalfWord(u16 HalfWord)
{
  /* Loop while DR register in not emplty */
  	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

  /* Send Half Word through the SPI1 peripheral */
  	SPI_I2S_SendData(SPI1, HalfWord);

  /* Wait to receive a Half Word */
  	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the Half Word read from the SPI bus */
  	return SPI_I2S_ReceiveData(SPI1);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_WriteEnable
* Description    : Enables the write access to the FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_WriteEnable(void)
{
  /* Select the FLASH: Chip Select low */
  	SPI_FLASH_CS_LOW();
  
  /* Send "Write Enable" instruction */
  	SPI_FLASH_SendByte(WREN);
  
  /* Deselect the FLASH: Chip Select high */
  	SPI_FLASH_CS_HIGH();
}

/*******************************************************************************
* Function Name  : SPI_FLASH_WaitForWriteEnd
* Description    : Polls the status of the Write In Progress (WIP) flag in the  
*                  FLASH's status  register  and  loop  until write  opertaion
*                  has completed.  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_WaitForWriteEnd(void)
{
  	u8 FLASH_Status = 0;
  
  /* Select the FLASH: Chip Select low */
  	SPI_FLASH_CS_LOW();
  
  /* Send "Read Status Register" instruction */
  	SPI_FLASH_SendByte(RDSR);
  
  /* Loop as long as the memory is busy with a write cycle */
  	do
  	{
    	/* Send a dummy byte to generate the clock needed by the FLASH 
    	and put the value of the status register in FLASH_Status variable */
   	 	FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);

  	} while((FLASH_Status & WIP_Flag) == SET); /* Write in progress */

  /* Deselect the FLASH: Chip Select high */
  	SPI_FLASH_CS_HIGH();
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE***********************************/

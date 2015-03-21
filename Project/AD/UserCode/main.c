#include "stm32f10x.h"
#include <stdio.h>
#include "common.h"
#include "USART.h"
#include "AD.h"






/*初始化ADC1 */
void ADC1_Init(void)
{
	ADC1_GPIO_Config();
	ADC1_Mode_Config();
}

int main(void)
{
	u32 AD_value;
	/* 配置系统时钟为72M */
	SystemInit();
	/* 初始化ADC1 */
	ADC1_Init();
	Usart1_Dma_Open(115200);
	printf("\r\n -------这是一个ADC实验------\r\n");
	printf("\r\n -------ADC采样的是PA0口------\r\n");

	while (1)
	{
		delay_ms(20);
		Usart1_Buffer_Tx_Service();
		AD_value = 3300000 / 4096 * ADC_ConvertedValue / 1000;
		printf("AD value = %d mV  \r\n", AD_value);
	}
}


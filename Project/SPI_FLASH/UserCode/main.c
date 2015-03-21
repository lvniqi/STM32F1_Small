#include "stm32f10x.h"
#include <stdio.h>
#include "common.h"
#include "USART.h"
#include "Lcd.h"
#include "exti.h"
#include "DA.h"
#include "AD.h"
#include "SPI_FLASH.h"
#include "Timer.h"
u16 addr = 0;
void Ad_To_Flash(void);
void Flash_To_Da(void);
int main(void)
{
	LCD_Init();
	EXTI_init();
	Dac_Init(DAC_Channel_1, DAC_Trigger_T2_TRGO);
	TIM2_Configuration();
	ADC1_GPIO_Config();
	ADC1_Mode_Config();
	Spi_Flash_Init();
	int i = 0;
	LCD_ShowString(140, 250, 18, "Record Test ^_^");
	LCD_ShowString(140, 230, 18, "Wait For Init");
	int temp;
	u32 count = 0;
	/*µÈ´ý×¢Âú»º³åÇø*/
	while (Sequeue_Getlen(&ADC_BUFFER) < Sequeue_GetlenMax(&ADC_BUFFER) / 2)
	{
		Ad_Get_Service();
	}
	count = 0;
	/*µÈ´ý°´¼ü*/
	LCD_ShowString(140, 230, 18, "PRESS ANY KEY TO START!");
	Wait_For_Any_Key();
	LCD_ShowString(140, 230, 18, "Press 1 To Start Record          ");
	LCD_ShowString(140, 210, 18, "Press 2 To Start Play               ");
	while (True)
	{
		if (MAIN_KEY.keysign)
		{
			MAIN_KEY.keysign = 0;
			if (MAIN_KEY.keycode == 1)
			{
				LCD_ShowString(140, 230, 18, "Start Record                   ");
				LCD_ShowString(140, 210, 18,
						"WAIT For Erase Flash                   ");
				SPI_FLASH_BulkErase();
				LCD_ShowString(140, 210, 18,
						"Press 2 To Stop                   ");
				Ad_To_Flash();
				LCD_ShowString(140, 230, 18,
						"Press 1 To Start Record          ");
				LCD_ShowString(140, 210, 18,
						"Press 2 To Start Play               ");
			}

			else if (MAIN_KEY.keycode == 2)
			{
				LCD_ShowString(140, 230, 18, "Start Play                   ");
				LCD_ShowString(140, 210, 18,
						"Press 1 To Stop                   ");
				Flash_To_Da();
				LCD_ShowString(140, 230, 18,
						"Press 1 To Start Record          ");
				LCD_ShowString(140, 210, 18,
						"Press 2 To Start Play               ");
			}
		}
	}
}
void Ad_To_Flash(void)
{
	u16 temp = 0;
	u16 time = 0;
	u32 count = 0;
	SPI_FLASH_BUFFER.address_max = 0x0f0000;
	SPI_FLASH_BUFFER.address = 0;
	Sequeue_Init(&(SPI_FLASH_BUFFER.buffer), _SPI_FLASH_BUFFER,
			SPI_FLASH_BUFFER_LEN);
	while (1)
	{

		Ad_Get_Service();
		while (Sequeue_Getlen(&ADC_BUFFER) and not Sequeue_Full(&(SPI_FLASH_BUFFER.buffer)))
		{
			temp = Sequeue_Get_Front(&ADC_BUFFER);
			Sequeue_In_Queue(&(SPI_FLASH_BUFFER.buffer), temp & 0xfff);
			Sequeue_Out_Queue(&ADC_BUFFER);
			count++;
		}
		SPI_FLASH_Sequeue_Write(&SPI_FLASH_BUFFER);
		if (MAIN_KEY.keysign)
		{
			MAIN_KEY.keysign = 0;
			if (MAIN_KEY.keycode == 2)
			{
				break;
			}
		}
		if (count > 300000)
		{
			break;
		}
		if (count / 10000 != time)
		{
			sprintf(LCD_BUFFER, "TIME:%d", time);
			LCD_ShowString(140, 190, 18, LCD_BUFFER);
                        time = count/10000;

		}

	}
	LCD_ShowString(140, 230, 18, "Stop Record                   ");
	LCD_ShowString(140, 210, 18, "                              ");
	LCD_ShowString(140, 190, 18, "                              ");
	delay_ms(100);
}
void Flash_To_Da(void)
{
	u16 temp;
        u32 count = 0;
        u16 time = 0;
	if (SPI_FLASH_BUFFER.address != 0)
	{
		SPI_FLASH_BUFFER.address_max = SPI_FLASH_BUFFER.address;
	}
	else
	{
		SPI_FLASH_BUFFER.address_max = 300000;
	}
	SPI_FLASH_BUFFER.address = 0;
	SPI_FLASH_Sequeue_Read(&SPI_FLASH_BUFFER);
	while (1)
	{
		SPI_FLASH_Sequeue_Read(&SPI_FLASH_BUFFER);
		while (Sequeue_Getlen(&(SPI_FLASH_BUFFER.buffer)) and not Sequeue_Full(&(DAC1_BUFFER.TxBuffer)))
		{
			temp = Sequeue_Get_Front(&(SPI_FLASH_BUFFER.buffer));
			Sequeue_In_Queue(&(DAC1_BUFFER.TxBuffer),temp);
			Sequeue_Out_Queue(&(SPI_FLASH_BUFFER.buffer));
                        count++;
		}
		Dac_Send_Service();
                if (count / 10000 != time)
		{
			sprintf(LCD_BUFFER, "TIME:%d", time);
			LCD_ShowString(140, 190, 18, LCD_BUFFER);
                        time = count/10000;
		}
		if (SPI_FLASH_BUFFER.address >= SPI_FLASH_BUFFER.address_max)
		{
			break;
		}
		if (MAIN_KEY.keysign)
		{
			MAIN_KEY.keysign = 0;
			if (MAIN_KEY.keycode == 1)
			{
				break;
			}
		}
	}
	LCD_ShowString(140, 230, 18, "Stop Play                   ");
	LCD_ShowString(140, 210, 18, "                              ");
        LCD_ShowString(140, 190, 18, "                              ");
	delay_ms(100);
}

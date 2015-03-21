#include "stm32f10x.h"
#include <stdio.h>
#include "common.h"
#include "USART.h"
#include "Lcd.h"
#include "exti.h"
#include "DA.h"
#include "AD.h"
#include "SPI_FLASH.h"
void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = 20; //设置0.05ms一次TIM2比较的周期
	TIM_TimeBaseStructure.TIM_Prescaler = 71; //系统主频72M，这里分频72，相当于1M的定时器2时钟
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update); //发生时间 产生DAC
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //下面详细说明
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //TIM_OutputState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 4;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //如果是PWM1要为Low，PWM2则为High
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);

	TIM_Cmd(TIM2, ENABLE);
	TIM_CtrlPWMOutputs(TIM2, ENABLE);

}
u16 test[1024];
u16 addr = 0;
int main(void)
{
	LCD_Init();
	EXTI_init();
	Dac_Init(DAC_Channel_1, DAC_Trigger_T2_TRGO);
	TIM2_Configuration();
	ADC1_GPIO_Config();
	ADC1_Mode_Config();
	Spi_Flash_Init();
        SPI_FLASH_BUFFER.address_max = 0x070000;
	SPI_FLASH_BUFFER.address = 0;
        Sequeue_Init(&(SPI_FLASH_BUFFER.buffer), _SPI_FLASH_BUFFER,SPI_FLASH_BUFFER_LEN);
	
        
	int i = 0;
	LCD_ShowString(140, 250, 18, "Dispaly Test ^_^");
	int temp = 0;
	int count = 0;
        //SPI_FLASH_BulkErase();
        while(not Sequeue_Full(&(SPI_FLASH_BUFFER.buffer)))
        {
          Sequeue_In_Queue(&(SPI_FLASH_BUFFER.buffer),temp);
          temp++;
        }
        i = Sequeue_Getlen(&(SPI_FLASH_BUFFER.buffer));
        SPI_FLASH_Sequeue_Write(&SPI_FLASH_BUFFER);
        
        sprintf(LCD_BUFFER, "END1:%d", temp);
        LCD_ShowString(140,200,18,LCD_BUFFER);   
        SPI_FLASH_BUFFER.address_max = SPI_FLASH_BUFFER.address;
        SPI_FLASH_BUFFER.address = 0;
        SPI_FLASH_Sequeue_Read(&SPI_FLASH_BUFFER);
        
        for(temp = 0;temp<i;temp++)
        {
          sprintf(LCD_BUFFER, "I:%d  DATA:%d",temp, Sequeue_Get_Front(&(SPI_FLASH_BUFFER.buffer)));
          LCD_ShowString(140,200,18,LCD_BUFFER);
          Sequeue_Out_Queue(&(SPI_FLASH_BUFFER.buffer));
          if(temp != test[temp])
          {
            pass;
          }
          delay_ms(10);
        }
}

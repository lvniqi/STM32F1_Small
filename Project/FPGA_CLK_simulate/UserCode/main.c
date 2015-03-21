/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "common.h"
#include <stdio.h>
#include "stm32f10x_it.h"
#include "USART.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
  
/* Private functions ---------------------------------------------------------*/




#define FPGA_Clk_Set() GPIO_SetBits(GPIOC,GPIO_Pin_13|GPIO_Pin_14)
#define FPGA_Clk_Reset() GPIO_ResetBits(GPIOC,GPIO_Pin_13|GPIO_Pin_14)
#define FPGA_Clk_Next() (GPIOC->ODR ^= GPIO_Pin_13|GPIO_Pin_14)
#define FPGA_Clk_Read() GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_13)
#define FPGA_Clk_In() GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_15)
#define FPGA_Data_Send(data) GPIO_Write(GPIOA,data)

void Gpio_Init(void)
{
  /* GPIOC*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure; 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);    
  
  /* GPIOA*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* GPIOC IN*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}
#include "AD.h"

void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = 20; //设置0.05ms一次TIM2比较的周期
	TIM_TimeBaseStructure.TIM_Prescaler = 8; //系统主频72M，这里分频72，相当于1M的定时器2时钟
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update); //发生时间 产生DAC
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //下面详细说明
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //TIM_OutputState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 10;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //如果是PWM1要为Low，PWM2则为High
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);

	TIM_Cmd(TIM2, ENABLE);
	TIM_CtrlPWMOutputs(TIM2, ENABLE);

}
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
#define ADC_LEN 3000
u8 temp_2[ADC_LEN];
int main(void)
{
  Gpio_Init();
  FPGA_Clk_Reset();
  Usart1_Dma_Open(115200);
  u8 in_data;
  u16 j;
  u16 time = 0;
  TIM2_Configuration();
  ADC1_GPIO_Config();
  ADC1_Mode_Config();
  /*等待注满缓冲区*/
  while(Sequeue_Getlen(&ADC_BUFFER) < Sequeue_GetlenMax(&ADC_BUFFER)/2 )
  {
    Ad_Get_Service();
  }
  while (1)
  {
    Usart1_Buffer_Rx_Service();
    Usart1_Buffer_Tx_Service();
    while((USART1_BUFFER.RxBuffer).len)
    {
        if((USART1_BUFFER.TxBuffer).len >(USART1_BUFFER.TxBuffer).len_max-1 )
        {

            Usart1_Buffer_Tx_Service();
            break;
        }
        Usart_Send(&USART1_BUFFER,Sequeue_Get_Front(&(USART1_BUFFER.RxBuffer)));

        Sequeue_Out_Queue(&(USART1_BUFFER.RxBuffer));
    }
    while(Sequeue_Getlen(&ADC_BUFFER)<ADC_LEN*2){Ad_Get_Service();}
    //start
    /*while(Sequeue_Get_Front(&ADC_BUFFER) < 2000
          or Sequeue_Get_Front(&ADC_BUFFER) > 2024
          or Sequeue_Get_One(&ADC_BUFFER,1)>Sequeue_Get_Front(&ADC_BUFFER)
          or Sequeue_Get_One(&ADC_BUFFER,4)>Sequeue_Get_One(&ADC_BUFFER,1)
          or Sequeue_Get_One(&ADC_BUFFER,5)>Sequeue_Get_One(&ADC_BUFFER,3)
          or Sequeue_Get_One(&ADC_BUFFER,6)>Sequeue_Get_One(&ADC_BUFFER,4)
          or Sequeue_Get_One(&ADC_BUFFER,10)>Sequeue_Get_One(&ADC_BUFFER,6)
            )
    {
      Sequeue_Out_Queue(&ADC_BUFFER);
      while(Sequeue_Getlen(&ADC_BUFFER)<ADC_LEN){Ad_Get_Service();}
    }*/
    //end
    for(j=0;j<ADC_LEN;j++)
    {
      temp_2[j] = (Sequeue_Get_Front(&ADC_BUFFER))>>4;
      Sequeue_Out_Queue(&ADC_BUFFER);
    }
    
    /* Set C13 */
    //第一个数据
    in_data = FPGA_Clk_In();
    FPGA_Data_Send(91);
    FPGA_Clk_Set();
    while(FPGA_Clk_In() == in_data); 
    in_data = FPGA_Clk_In();
    //后续255数据
    
    for(time=0;time<ADC_LEN;time++)
    {
      //int temp = Sequeue_Get_Front(&ADC_BUFFER);
      //Sequeue_Out_Queue(&ADC_BUFFER);
      
      FPGA_Data_Send(temp_2[time]);
      FPGA_Clk_Next();
      while(FPGA_Clk_In() == in_data); 
      in_data = FPGA_Clk_In();
      
    }
    while(FPGA_Clk_In() == in_data); 
    FPGA_Clk_Reset();
    delay_ms(1);
    //if(not FPGA_Clk_Read())
    //   FPGA_Clk_Set();//最后一一次置高
  }
}


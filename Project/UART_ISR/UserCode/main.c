/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include "stm32f10x_it.h"
#include "USART.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
  

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

  InitUsart(USART1,115200);
  NVIC_Configuration_USART1();
  USART_Cmd(USART1,ENABLE);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  //USART_ITConfig(USART1, USART_IT_TXE, ENABLE);

  printf("\n\rUSART Printf Example: retarget the C library printf function to the USART\n\r");

  while (1)
  {
     /*char a;
     if(USART_GetFlagStatus(USART1,USART_IT_RXNE)==SET)
     {           
       a = USART_ReceiveData(USART1);
       USART_SendData(USART1, a); 
     }*/
     
  }
}




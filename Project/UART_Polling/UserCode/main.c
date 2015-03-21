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
  char uart1_get_data;
  InitUsart(USART1,115200);
  USART_Cmd(USART1,ENABLE);

  while (1)
  {
    if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
    {
        USART_SendData(USART1, USART_ReceiveData(USART1));
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    }
  }
}
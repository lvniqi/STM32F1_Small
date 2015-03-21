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







/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* GPIOD Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;
  /* Configure PD0 and PD2 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  Usart1_Dma_Open(115200);
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
      /* Set C13 */
      GPIO_SetBits(GPIOC,GPIO_Pin_13|GPIO_Pin_14);
      delay_ms(1);
      /* Reset C13 */
      GPIO_ResetBits(GPIOC,GPIO_Pin_13|GPIO_Pin_14);
      delay_ms(1);
  }
}







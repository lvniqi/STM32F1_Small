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

  Usart1_Dma_Open(115200);
 
  
  while (1)
  {
      
      
      delay_ms(10);
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
  }
}







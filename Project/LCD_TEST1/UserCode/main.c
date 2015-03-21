/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "common.h"
#include <stdio.h>
#include "stm32f10x_it.h"
#include "USART.h"
#include "LCD.h"
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
  LCD_Init();
  
  while (1)
  {
      
      
      delay_ms(20);
      Usart1_Buffer_Rx_Service();
      Usart1_Buffer_Tx_Service();
      //if(USART_1_TxBuffer.len == 0)
      {
        while(USART1_BUFFER.TxBuffer.len)
        {
            if(USART1_BUFFER.TxBuffer.len >USART1_BUFFER.TxBuffer.len_max-1 )
            {
                Usart1_Buffer_Tx_Service();
                break;
            }
            Sequeue_In_Queue(&USART1_BUFFER.TxBuffer,Sequeue_Get_Front(&USART1_BUFFER.RxBuffer));
            Sequeue_Out_Queue(&USART1_BUFFER.RxBuffer);
            
        }
      }
  }
}







/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "common.h"
#include <stdio.h>
#include "stm32f10x_it.h"
#include "USART.h"
#include "NRF2401.h"
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
  u8 temp;

  Usart1_Dma_Open(115200);
  //InitSpi(SPI1,True);
  //SPI_Cmd(SPI1, ENABLE);//¿ªÆôNrf2401
  InitNrf2401();
  Nrf2401_Open();
  
  while (1)
  {
      
      
      delay_ms(10);
      Usart1_Buffer_Rx_Service();
      Usart1_Buffer_Tx_Service();
      
      /*
      if(USART_1_TxBuffer.len == 0)
      {
        while(USART_1_RxBuffer.len)
        {
            if(USART_1_TxBuffer.len >USART1_BUFFER_LEN-1 )
            {
                break;
            }
            
            temp = SPI2_ReadWriteByte(Sequeue_Get_Front(&USART_1_RxBuffer));
            Sequeue_Out_Queue(&USART_1_RxBuffer);
            
            Sequeue_In_Queue(&USART_1_TxBuffer,temp);
            
            
        }
      }*/
        if( not Nrf2401_Check())
        {
          printf("OK,FIND!\n");
        }
        else
        {
          printf("WRONG, NOT FIND!\n");
        }
  }
}







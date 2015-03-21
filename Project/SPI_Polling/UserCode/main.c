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
volatile u8 temp;
int main(void)
{
  
  Usart1_Dma_Open(115200);
  InitSpi(SPI1,True);
  SPI_Cmd(SPI1, ENABLE);//¿ªÆôNrf2401
  //InitNrf2401();
  //Nrf2401_Open();
  GPIO_ResetBits(GPIOA, GPIO_Pin_4);
  GPIO_SetBits(GPIOA, GPIO_Pin_4);
  GPIO_ResetBits(GPIOA, GPIO_Pin_4);
  while (1)
  {
      delay_ms(10);
      Usart1_Buffer_Rx_Service();
      Usart1_Buffer_Tx_Service();
      temp = SPI1_ReadWriteByte(0x4A);
      if( temp != 0x4A)
      {
        temp = 1;
        GPIO_SetBits(GPIOA, GPIO_Pin_4);
        delay_ms(10);
        GPIO_ResetBits(GPIOA, GPIO_Pin_4);
      }
      if(USART1_BUFFER.TxBuffer.len == 0)
      {
        while(USART1_BUFFER.RxBuffer.len)
        {
            if(USART1_BUFFER.TxBuffer.len >USART1_TX_BUFFER_LEN-1 )
            {
                break;
            }
            temp = SPI1_ReadWriteByte(Sequeue_Get_Front(&(USART1_BUFFER.RxBuffer)));
            Sequeue_Out_Queue(&(USART1_BUFFER.RxBuffer));
            Sequeue_In_Queue(&(USART1_BUFFER.TxBuffer),temp);
            
        }
        /*if( not Nrf2401_Check())
        {
          printf("OK,FIND!\n");
        }
        else
        {
          printf("WRONG, NOT FIND!\n");
        }*/
      }
  }
}







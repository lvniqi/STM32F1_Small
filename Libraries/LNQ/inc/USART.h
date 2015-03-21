/*
 * USART.h
 *
 *  Created on: 2014年4月28日
 *      Author: lvniqi
 *      Email: lvniqi@gmail.com
 */
#ifndef USART_H_
#define USART_H_
/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "Sequeue.h"

/*char队列定义-------------------------------*/
typedef struct _usart_buffer
{
	char_sequeue TxBuffer;
	char_sequeue RxBuffer;
	u8 TxFinishedFlag;
} USART_BUFFER;
/* Public define ------------------------------------------------------------*/
#ifdef  USART1_USED_
#define USART1_DMA_CHANNEL_TX DMA1_Channel4
#define USART1_DMA_CHANNEL_RX DMA1_Channel5
#endif
#ifdef  USART3_USED_
#define USART3_DMA_CHANNEL_TX DMA1_Channel2
#define USART3_DMA_CHANNEL_RX DMA1_Channel3
#endif
/*队列长度-------------------------------*/
#ifdef  USART1_USED_
#pragma message("使用 USART1")
#define USART1_RX_BUFFER_LEN (100)
#define USART1_TX_BUFFER_LEN (500)
extern char _USART1_RX_BUFFER[USART1_RX_BUFFER_LEN];
extern char _USART1_TX_BUFFER[USART1_TX_BUFFER_LEN];
extern USART_BUFFER USART1_BUFFER;
#endif

#ifdef  USART3_USED_
#pragma message("使用 USART3")
#define USART3_RX_BUFFER_LEN (100)
#define USART3_TX_BUFFER_LEN (250)
extern char _USART3_RX_BUFFER[USART3_RX_BUFFER_LEN];
extern char _USART3_TX_BUFFER[USART3_TX_BUFFER_LEN];
extern USART_BUFFER USART3_BUFFER;
#endif
/* Defined functions ------------------------------------------------------- */
/*Sequeue地址*/
#define SEQUEUE_DR_BASE(SEQUEUE) ((uint32_t)((SEQUEUE.dataspace)))
/* Exported functions ------------------------------------------------------- */
#ifdef  USART1_USED_
void NVIC_Configuration_USART1(void);
void DMA_Configuration_Usart_1(void);
void Usart1_Buffer_Tx_Service();
void Usart1_Buffer_Rx_Service();
void Usart1_Dma_Open(u32 BaudRate);
#endif

#ifdef  USART3_USED_
void NVIC_Configuration_USART3(void);
void DMA_Configuration_Usart_3(void);
void Usart3_Buffer_Tx_Service();
void Usart3_Buffer_Rx_Service();
void Usart3_Dma_Open(u32 BaudRate);
#endif

#if (defined USART1_USED_) || (defined USART3_USED_)
u8 InitUsart(USART_TypeDef* USARTx, u32 BaudRate);
u8 Usart_Send(USART_BUFFER* p, u8 data);
void USART_Buffer_Init(USART_BUFFER* p, char* rx_buffer, char* tx_buffer,
		u32 rx_len, u32 tx_len);
#endif
/* Interrupt functions ------------------------------------------------------- */
void USART1_IRQHandler(void);
void USART3_IRQHandler(void);
void DMA1_Channel2_IRQHandler(void);
void DMA1_Channel3_IRQHandler(void);
void DMA1_Channel4_IRQHandler(void);
void DMA1_Channel5_IRQHandler(void);

/* Exported Variable ------------------------------------------------------- */

#endif /* USART_H_ */

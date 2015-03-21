/*
 * COMMON.c
 *
 *  Created on: 2014年5月4日
 *      Author: lvniqi
 *      Email: lvniqi@gmail.com
 */
#include "common.h"
/********************************************************************
 * 名称 : assert_failed
 * 功能 : 帮助提示
 * 输入 : 无
 * 输出 : 无
 ***********************************************************************/
#ifdef  USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	printf("Wrong parameters value: file %s on line %d\r\n", file, line);
	/* Infinite loop */
	while (1)
	{
	}
}
#endif
/********************************************************************
 * 名称 : PUTCHAR_PROTOTYPE
 * 功能 : 重置printf中的putchar函数（非用户使用）
 * 输入 : 无
 * 输出 : 无
 ***********************************************************************/
#ifdef DMA_USED_
int fputc(int ch, FILE *f)
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
  #ifdef  USART1_USED_
	Usart_Send(&USART1_BUFFER, (u8) ch);
  #endif
  #ifdef  USART3_USED_
	Usart_Send(&USART3_BUFFER, (u8) ch);
  #endif
	return ch;
}
#else
int fputc(int ch, FILE *f)
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
        #ifdef  USART1_USED_
	USART_SendData(USART1, (uint8_t) ch);
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{}
        #endif 
        #ifdef  USART3_USED_
        USART_SendData(USART3, (uint8_t) ch);
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
	{}
        #endif
	return ch;
}
#endif
u16 Num_Len(u32 num)
{
  int i=0;
  while(num)
  {
    i++;
    num /= 10;
  }
  return i;
}
void delay_ms(volatile u16 time)
{
	volatile u32 i = 0;
	while (time--)
	{
		i = 9000;  //自己定义
		while (i--)
			;
	}
}

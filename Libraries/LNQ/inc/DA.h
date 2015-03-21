/*
 * DA.h
 *
 *  Created on: 2014Äê7ÔÂ27ÈÕ
 *      Author: lvniqi
 */

#ifndef DA_H_
#define DA_H_
#include "common.h"
#include "Sequeue.h"
#include "PINGPANG.h"
#define DAC_Buffer_LEN 10000
#define DAC1_DR_BASE ((uint32_t)0x40007408)
typedef struct _dac_buffer
{
	int_sequeue TxBuffer;
	u8 TxFinishedFlag;
} DAC_BUFFER;
extern DAC_BUFFER DAC1_BUFFER;
extern int Dac_Init(uint32_t DAC_Channel, uint32_t DAC_TRIGGER);
extern void DAC_Config(void); 
extern void DAC_Task(u16 i);
extern void Dac_Send_Service();
#endif /* DA_H_ */

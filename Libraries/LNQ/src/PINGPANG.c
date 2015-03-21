/*
 * PINGPANG.c
 *
 *  Created on: 2014年5月6日
 *      Author: lvniqi
 *      Email: lvniqi@gmail.com
 */
#include "PINGPANG.h"
/*乒乓通道1*/
pingpang pingpang_1;
/*乒乓数据列表*/
_pingpang_data _PINGPANG_DATA_LIST[PINGPANG_DATA_LIST_LEN];
/*乒乓空闲数据列表*/
_pingpang_data* _PINGPANG_FREE[PINGPANG_DATA_FREE_LIST_LEN];
/********************************************************************
* 名称 : PingPang_Service
* 功能 : 乒乓服务函数 为了实时得到空闲乒乓数据
* 输入 : 无
* 输出 : 无
***********************************************************************/
void PingPang_Service()
{
	u8 i,j;
	for(i=0,j=0;(i<PINGPANG_DATA_LIST_LEN) and (j<PINGPANG_DATA_FREE_LIST_LEN);i++)
	{
		
		if( (_PINGPANG_DATA_LIST+i)->status == PINGPANG_NULL)
		{
			_PINGPANG_FREE[j] = (_PINGPANG_DATA_LIST+i);
			j++;
		}
	}
}
/********************************************************************
* 名称 : PingPang_GetFree
* 功能 : 得到空闲乒乓数据
* 输入 : 无
* 输出 : _pingpang_data* 乒乓数据指针
***********************************************************************/
_pingpang_data* PingPang_GetFree()
{
	char i;
	_pingpang_data* temp;
	for(i=0;i<PINGPANG_DATA_FREE_LIST_LEN;i++)
	{
		if(_PINGPANG_FREE[i])
		{
			temp = _PINGPANG_FREE[i];
			temp->status = PINGPANG_USED;
			temp->len = 0;
			_PINGPANG_FREE[i] = NULL;
			return temp;
		}
	}
	return NULL;
}
/********************************************************************
* 名称 : PingPang_Init
* 功能 : 乒乓缓冲通道1初始化
* 输入 : 乒乓缓冲通道指针
* 输出 : 无
***********************************************************************/
void PingPang_Init(pingpang *data)
{	
	u8 i;
	(data)->busy = PingPang_GetFree();
	for(i=0;i<PINGPANG_GETED_LEN;i++)
	{
		(data)->geted[i] = NULL;
	}
}
/********************************************************************
* 名称 : _PingPang_Data_free_Init
* 功能 : 乒乓缓冲空闲通道初始化
* 输入 : 乒乓缓冲空闲通道指针
* 输出 : 无
***********************************************************************/
void _PingPang_Data_free_Init(_pingpang_data* p[])
{
	u8 i;
	for(i=0;i<PINGPANG_DATA_FREE_LIST_LEN;i++)
	{
		p[i] = &(_PINGPANG_DATA_LIST[i]);
	}
}
/********************************************************************
* 名称 : PingPang_In
* 功能 : 从数据传入乒乓通道 
* 输入 : pingpang*通道  PINGPANG_DATATYPE data 数据
* 输出 : 成功 1 ， 0
***********************************************************************/
u8 PingPang_In(pingpang* i,PINGPANG_DATATYPE data)
{
	_pingpang_data* temp=(i->busy);
	int count;
	if(temp)
	{
		//未溢出
		if((temp->len) < PINGPANG_LEN)
		{
			(temp->data)[temp->len] = data;
			(temp->len)++;
		}
		else
		{
			//上次的还没处理....
			if(i->geted[0])
			{
				return 0;
			}
			//移到已得到的地方
			else
			{
				temp->status = PINGPANG_FULL;
				for(count=PINGPANG_GETED_LEN-1;count>=0;count--)
				{
					if(i->geted[count] == NULL)
					{
						i->geted[count] = temp;
						break;
					}
				}
				i->busy = PingPang_GetFree();
				PingPang_In(i,data);
			}
		}
		return 1;
	}
	else
	{
		return 0;
	}
}
/********************************************************************
* 名称 : PingPang_Out
* 功能 : 从乒乓通道传出数据
* 输入 : pingpang*  乒乓通道指针
* 输出 : _pingpang_data* 乒乓数据指针
***********************************************************************/
_pingpang_data* PingPang_Out(pingpang* i)
{
	_pingpang_data* temp;
	if((i->geted[PINGPANG_GETED_LEN-1]))
	{
		temp = i->geted[PINGPANG_GETED_LEN-1];
		int count;
		for(count = PINGPANG_GETED_LEN-1;count >0;count--)
		{
			i->geted[count] = i->geted[count-1];
		}
		i->geted[count] = NULL;
		return temp;
	}
	else if((i->busy) and ((i->busy)->len))
	{	
		temp = (i->busy);
		i->busy = PingPang_GetFree();
		return temp;
	}
	else
	{
		return NULL;
	}
}
/********************************************************************
* 名称 : PingPang_Process
* 功能 : 模拟DMA中断处理
* 输入 : _pingpang_data*  乒乓数据
* 输出 : 无
***********************************************************************/
void PingPang_Process(_pingpang_data* data)
{
	u16 i;
	for(i=0;i<(data->len);i++)
	{
		printf("%d ",((data->data)[i]));
	}
	data->status = PINGPANG_NULL;
	data->len = 0;
}

/********************************************************************
* 名称 : _Display_PingpPang
* 功能 : 显示pingpang数据
* 输入 : _pingpang
* 输出 : 无
***********************************************************************/
void _Display_PingpPang(pingpang p1)
{
	_pingpang_data* temp;
	u16 i;
	temp = p1.geted[2];
	if(temp)
	{
		for(i=0;i<temp->len;i++)
		{
			printf("geted3:%d:\t%d\n",i,(temp->data)[i]);
		}
	}
	temp = p1.geted[1];
	if(temp)
	{
		for(i=0;i<temp->len;i++)
		{
			printf("geted2:%d:\t%d\n",i,(temp->data)[i]);
		}
	}
	temp = p1.geted[0];
	if(temp)
	{
		for(i=0;i<temp->len;i++)
		{
			printf("geted1:%d:\t%d\n",i,(temp->data)[i]);
		}
	}
	temp = p1.busy;
	if(temp->status != PINGPANG_NULL)
	{
		for(i=0;i<temp->len;i++)
		{
			printf("busy:%d:\t%d\n",i,(temp->data)[i]);
		}
	}
}

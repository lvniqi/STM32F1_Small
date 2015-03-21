/*
 * PingPang.h
 *
 *  Created on: 2014年5月6日
 *      Author: lvniqi
 *      Email: lvniqi@gmail.com
 */
#ifndef PINGPANG_H_
#define PINGPANG_H_
#include "common.h"
#define PINGPANG_LEN 256
#define PINGPANG_NULL 0
#define PINGPANG_USED 1
#define PINGPANG_FULL 2

#define PINGPANG_DATA_LIST_LEN 5
#define PINGPANG_DATA_FREE_LIST_LEN 4
#define PINGPANG_GETED_LEN 3
typedef u16 PINGPANG_DATATYPE;
/*乒乓数据*/
typedef struct __pingpang_data
{
	PINGPANG_DATATYPE data[PINGPANG_LEN];
	u16 len;
	u8 status;
}_pingpang_data;
/*乒乓通道*/
typedef struct _pingpang
{
	_pingpang_data *busy;
	_pingpang_data *geted[PINGPANG_GETED_LEN];
}pingpang;

/*乒乓数据列表*/
extern _pingpang_data _PINGPANG_DATA_LIST[PINGPANG_DATA_LIST_LEN];
/*乒乓空闲数据列表*/
extern _pingpang_data* _PINGPANG_FREE[PINGPANG_DATA_FREE_LIST_LEN];
/*乒乓通道1*/
extern pingpang pingpang_1;

#endif /* PINGPANG_H_ */

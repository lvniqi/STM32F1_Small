#ifndef _PT_H_
#define _PT_H_
typedef struct pt//需要时间触发的线程调度 
{
  u16 lc;
  u16 count;
  u16 load;
  u8 ready;
} PT;
/*等待这一步完成*/

#define PT_WAITING 0
#define PT_YIELDED 1
#define PT_EXITED  2
#define PT_ENDED   3
#define PT_RELOAD_TIME(pt)       ((pt)->count=(pt)->load);         
#define PT_INIT(pt,a)	((pt)->lc)=0;((pt)->count)=a;((pt)->load)=a;
#define PT_THREAD(name_args) char name_args
#define PT_BEGIN(pt)  {char PT_YIELD_FLAG = 1; switch((pt)->lc) { case 0:
#define PT_END(pt) } PT_YIELD_FLAG = 0; PT_INIT(pt,((pt)->load)); return PT_ENDED; }
#define PT_WAIT_UNTIL(pt, condition) \
  do{						\
		((pt)->lc) = __LINE__; case __LINE__:\
		if(!(condition)) \
		{				\
			return PT_WAITING;			\
		}						\
	} while(0)
#define PT_WAIT_WHILE(pt, cond)  PT_WAIT_UNTIL((pt), !(cond))
#define PT_WAIT_THREAD(pt, thread) PT_WAIT_WHILE((pt), PT_SCHEDULE(thread))
#define PT_SPAWN(pt, child, thread)		\
  do {						\
		PT_INIT((child));				\
		PT_WAIT_THREAD((pt), (thread));		\
	 } while(0)
#define PT_RESTART(pt)				\
  do {						\
		PT_INIT(pt,((pt)->load));				\
		return PT_WAITING;			\
	 } while(0)
#define PT_EXIT(pt)				\
  do {						\
    PT_INIT(pt,((pt)->load));				\
    return PT_EXITED;			\
  } while(0)
  #define PT_SCHEDULE(f) ((f) < PT_EXITED)
#define PT_YIELD(pt)				\
  do {						\
		PT_YIELD_FLAG = 0;				\
		((pt)->lc) = __LINE__; case __LINE__:\
		if(PT_YIELD_FLAG == 0) \
		{			\
			return PT_YIELDED;			\
		}						\
  } while(0)
#define PT_YIELD_UNTIL(pt, cond)		\
  do {						\
		PT_YIELD_FLAG = 0;				\
		((pt)->lc) = __LINE__; case __LINE__:\
		if((PT_YIELD_FLAG == 0) || !(cond)) \
		{	\
		return PT_YIELDED;			\
		}						\
	 } while(0)
#endif
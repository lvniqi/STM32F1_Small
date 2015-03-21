/*
 * DAC_CODE.h
 *
 *  Created on: 2014年8月5日
 *      Author: lvniqi
 */

#ifndef DAC_CODE_H_
#define DAC_CODE_H_
#include "common.h"
#include "LCD.h"
#define FPGA_RAM_DATABASE 0x6003f800
#define FPGA_RAM_WRITE(_x,i) (*(volatile unsigned short *)(FPGA_RAM_DATABASE|_x<<1) = i)
#define FPGA_SET_FREQ_L(i) ((*((volatile unsigned short *) 0x6003c000)) = i)
#define FPGA_RED_FREQ_L() (*((volatile unsigned short *) 0x6003c002))
#define FPGA_SET_FREQ_H(i) ((*((volatile unsigned short *) 0x6003c004)) = i)
#define FPGA_RED_FREQ_H() (*((volatile unsigned short *) 0x6003c006))
/*正弦波曲线*/
#define WAVE_NUM 4
extern const u8 WaveData_s[1024];   
#define Dac_Wave_Init() \
{       \
    Dac_Wave[0] = Dac_Wave_Use_S;\
    Dac_Wave[1] = Dac_Wave_Use_F;\
    Dac_Wave[2] = Dac_Wave_Use_T;\
    Dac_Wave[3] = Dac_Wave_Use_C;\
}
#define LCD_Select_Chinese()\
{       \
  if(0 == count)        \
  {     \
    LCD_ShowChinese_ZHENGXIAN(420,240,16,YELLOW);       \
  }     \
  else if(1 == count)   \
  {     \
    LCD_ShowChinese_FANGBO(420,200,16,YELLOW);  \
  }     \
  else if(2 == count)   \
  {     \
    LCD_ShowChinese_SANJIAO(420,160,16,YELLOW); \
  }     \
  else if(3 == count)   \
  {     \
    LCD_ShowChinese_ZUHE(420,120,16,YELLOW);    \
  }     \
}
#define LCD_Select_Wave()\
{       \
  if(0 == count)        \
  {     \
    Dac_Wave_Use_S(&Lcd_Curve1);        \
  }     \
  else if(1 == count)   \
  {     \
    Dac_Wave_Use_F(&Lcd_Curve1);        \
  }     \
  else if(2 == count)   \
  {     \
    Dac_Wave_Use_T(&Lcd_Curve1);        \
  }     \
  else if(3 == count)   \
  {     \
    Dac_Wave_Use_C(&Lcd_Curve1);        \
  }     \
}
#define Set_Vpp(x)      \
{       \
  VPP = x;      \
  DAC_Task((VPP-1960)*1.3470394736842104);      \
}
#define FPGA_Set_Freq(x) \
{       \
  FREQ = x;     \
  _FREQ_TEMP = FREQ/0.0232827;      \
  FPGA_SET_FREQ_H((_FREQ_TEMP&0xffff0000)>>16);       \
  FPGA_SET_FREQ_L((_FREQ_TEMP&0x0000ffff));     \
  Freq_Service(0);      \
}

/*正弦波显示*/
extern  void Dac_Wave_Use_S(Lcd_Curve* Lcd_Curve1);
/*方波显示*/
extern void Dac_Wave_Use_F(Lcd_Curve* Lcd_Curve1);
/*三角波显示*/
extern void Dac_Wave_Use_T(Lcd_Curve* Lcd_Curve1);
/*组合波显示*/
extern void Dac_Wave_Use_C(Lcd_Curve* Lcd_Curve1);
extern void Vpp_Service(int L);
extern void Freq_Service(int L);
extern void Select_R(int L);
extern u32 _FREQ_TEMP;
extern u32 FREQ;
extern u32 VPP;
#endif /* DAC_CODE_H_ */

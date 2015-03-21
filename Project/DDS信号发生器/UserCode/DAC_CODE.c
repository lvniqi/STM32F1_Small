/*
 * DAC_CODE.c
 *
 *  Created on: 2014年8月5日
 *      Author: lvniqi
 */
#include "DAC_CODE.h"
/*正弦波曲线*/
u32 _FREQ_TEMP;
u32 FREQ = 100000;
u32 VPP = 2040;
const u8 WaveData_s[1024]=
{
  128,128,129,130,131,131,132,133,134,135,135,136,137,138,138,
  139,140,141,142,142,143,144,145,146,146,147,148,149,149,150,
  151,152,152,153,154,155,156,156,157,158,159,159,160,161,162,
  162,163,164,165,165,166,167,168,168,169,170,171,171,172,173,
  174,174,175,176,176,177,178,179,179,180,181,182,182,183,184,
  184,185,186,186,187,188,189,189,190,191,191,192,193,193,194,
  195,195,196,197,197,198,199,199,200,201,201,202,202,203,204,
  204,205,206,206,207,207,208,209,209,210,211,211,212,212,213,
  213,214,215,215,216,216,217,217,218,219,219,220,220,221,221,
  222,222,223,223,224,224,225,225,226,226,227,227,228,228,229,
  229,230,230,231,231,232,232,233,233,233,234,234,235,235,236,
  236,236,237,237,238,238,238,239,239,240,240,240,241,241,241,
  242,242,243,243,243,244,244,244,245,245,245,245,246,246,246,
  247,247,247,247,248,248,248,249,249,249,249,250,250,250,250,
  250,251,251,251,251,251,252,252,252,252,252,253,253,253,253,
  253,253,253,254,254,254,254,254,254,254,254,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,254,254,254,254,254,254,254,254,
  253,253,253,253,253,253,253,252,252,252,252,252,251,251,251,
  251,251,250,250,250,250,250,249,249,249,249,248,248,248,247,
  247,247,247,246,246,246,245,245,245,245,244,244,244,243,243,
  243,242,242,241,241,241,240,240,240,239,239,238,238,238,237,
  237,236,236,236,235,235,234,234,233,233,233,232,232,231,231,
  230,230,229,229,228,228,227,227,226,226,225,225,224,224,223,
  223,222,222,221,221,220,220,219,219,218,217,217,216,216,215,
  215,214,213,213,212,212,211,211,210,209,209,208,207,207,206,
  206,205,204,204,203,202,202,201,201,200,199,199,198,197,197,
  196,195,195,194,193,193,192,191,191,190,189,189,188,187,186,
  186,185,184,184,183,182,182,181,180,179,179,178,177,176,176,
  175,174,174,173,172,171,171,170,169,168,168,167,166,165,165,
  164,163,162,162,161,160,159,159,158,157,156,156,155,154,153,
  152,152,151,150,149,149,148,147,146,146,145,144,143,142,142,
  141,140,139,138,138,137,136,135,135,134,133,132,131,131,130,
  129,128,128,127,126,125,124,124,123,122,121,120,120,119,118,
  117,117,116,115,114,113,113,112,111,110,109,109,108,107,106,
  106,105,104,103,103,102,101,100,99,99,98,97,96,96,95,94,93,
  93,92,91,90,90,89,88,87,87,86,85,84,84,83,82,81,81,80,79,79,
  78,77,76,76,75,74,73,73,72,71,71,70,69,69,68,67,66,66,65,64,
  64,63,62,62,61,60,60,59,58,58,57,56,56,55,54,54,53,53,52,51,
  51,50,49,49,48,48,47,46,46,45,44,44,43,43,42,42,41,40,40,39,
  39,38,38,37,36,36,35,35,34,34,33,33,32,32,31,31,30,30,29,29,
  28,28,27,27,26,26,25,25,24,24,23,23,22,22,22,21,21,20,20,19,
  19,19,18,18,17,17,17,16,16,15,15,15,14,14,14,13,13,12,12,12,
  11,11,11,10,10,10,10,9,9,9,8,8,8,8,7,7,7,6,6,6,6,5,5,5,5,5,
  4,4,4,4,4,3,3,3,3,3,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,3,3,3,3,3,4,
  4,4,4,4,5,5,5,5,5,6,6,6,6,7,7,7,8,8,8,8,9,9,9,10,10,10,10,
  11,11,11,12,12,12,13,13,14,14,14,15,15,15,16,16,17,17,17,
  18,18,19,19,19,20,20,21,21,22,22,22,23,23,24,24,25,25,26,
  26,27,27,28,28,29,29,30,30,31,31,32,32,33,33,34,34,35,35,
  36,36,37,38,38,39,39,40,40,41,42,42,43,43,44,44,45,46,46,
  47,48,48,49,49,50,51,51,52,53,53,54,54,55,56,56,57,58,58,
  59,60,60,61,62,62,63,64,64,65,66,66,67,68,69,69,70,71,71,
  72,73,73,74,75,76,76,77,78,79,79,80,81,81,82,83,84,84,85,
  86,87,87,88,89,90,90,91,92,93,93,94,95,96,96,97,98,99,99,
  100,101,102,103,103,104,105,106,106,107,108,109,109,110,
  111,112,113,113,114,115,116,117,117,118,119,120,120,121,
  122,123,124,124,125,126,127
};  
/*正弦波显示*/
void Dac_Wave_Use_S(Lcd_Curve* Lcd_Curve1) 
{       
  u16 i;    
  Lcd_Curve1->finish_flag = 0;
  (Lcd_Curve1->Now_x) = 0;
  for(i=0;i<1024;i++)     
  {       
    FPGA_RAM_WRITE(i,WaveData_s[i]);      
  }       
  for(i=0;i<400;i++)      
  {     
    LCD_Curve_Show(Lcd_Curve1,WaveData_s[i*1024/400],1);       
  }     
}

/*方波显示*/
void Dac_Wave_Use_F(Lcd_Curve* Lcd_Curve1) 
{       
  u16 i;   
  Lcd_Curve1->finish_flag = 0;
  (Lcd_Curve1->Now_x) = 0;
  for(i=0;i<512;i++)     
  {       
    FPGA_RAM_WRITE(i,255);      
  }       
  for(i=512;i<1024;i++)     
  {       
    FPGA_RAM_WRITE(i,0);      
  }       
  for(i=0;i<100;i++)      
  {     
    LCD_Curve_Show(Lcd_Curve1,250,1);       
  }     
  for(i=100;i<200;i++)      
  {     
    LCD_Curve_Show(Lcd_Curve1,10,1);       
  }     
  for(i=200;i<300;i++)      
  {     
    LCD_Curve_Show(Lcd_Curve1,250,1);       
  }     
  for(i=300;i<400;i++)      
  {     
    LCD_Curve_Show(Lcd_Curve1,10,1);       
  }     
}
/*三角波显示*/
void Dac_Wave_Use_T(Lcd_Curve* Lcd_Curve1) 
{       
  u16 i;
  Lcd_Curve1->finish_flag = 0;
  (Lcd_Curve1->Now_x) = 0;
  for(i=0;i<512;i++)     
  {       
    FPGA_RAM_WRITE(i,i/2);      
  }       
  FPGA_RAM_WRITE(512,255);
  FPGA_RAM_WRITE(513,255);
  for(i=514;i<1024;i++)     
  {       
    FPGA_RAM_WRITE(i,512-i/2);      
  }     
  for(i=0;i<100;i++)      
  {     
    LCD_Curve_Show(Lcd_Curve1,2*i,1);       
  }     
  for(i=100;i<200;i++)      
  {     
    LCD_Curve_Show(Lcd_Curve1,2*(200-i),1);       
  }     
  for(i=200;i<300;i++)      
  {     
    LCD_Curve_Show(Lcd_Curve1,2*(i-200),1);       
  }     
  for(i=300;i<400;i++)      
  {     
    LCD_Curve_Show(Lcd_Curve1,2*(400-i),1);       
  }     
}
/*组合波显示*/
void Dac_Wave_Use_C(Lcd_Curve* Lcd_Curve1) 
{       
  u16 i;
  Lcd_Curve1->finish_flag = 0;
  (Lcd_Curve1->Now_x) = 0;
  for(i=0;i<512;i++)     
  {       
    FPGA_RAM_WRITE(i,WaveData_s[i]);      
  }
  for(i=512;i<768;i++)     
  {       
    FPGA_RAM_WRITE(i,(768-i)/2);      
  } 
  for(i=768;i<1024;i++)     
  {       
    FPGA_RAM_WRITE(i,(i-768)/2);      
  }       
  for(i=0;i<200;i++)      
  {     
    LCD_Curve_Show(Lcd_Curve1,WaveData_s[i*1024/400],1); 
  }     
  for(i=200;i<300;i++)      
  {     
    LCD_Curve_Show(Lcd_Curve1,1.28*(300-i),1);       
  }     
  for(i=300;i<400;i++)      
  {     
    LCD_Curve_Show(Lcd_Curve1,1.28*(i-300),1);       
  }     
}
void Select_R(int L)
{
  static int count = 0;
  u16 temp_color = BACK_COLOR;
  /*上次删除*/
  LCD_Select_Chinese();
  if(L) 
  {     
    count = ((count+1)%4);      
  }     
  else
  {
    count = ((count+4-1)%4);
  }
  BACK_COLOR = RED;
  LCD_Select_Chinese();
  BACK_COLOR = temp_color;
  LCD_Select_Wave();
}
void Freq_Service(int L)
{
  static int position=0;
  u16 temp_color = BACK_COLOR;
  u32 temp=10;
  u16 i,num;
  u16 len = Num_Len(FREQ);
  if(position >= len)
  {
    if(len > 0)
    {
      position = len-1;
    }
    else
    {
      position = 0;
    }
  }
  if(L == 0)
  {
    BACK_COLOR = RED;
    for(i=0;i<position;i++)
    {
      temp*=10;
    }
    num = (FREQ%temp)/(temp/10);
    if(position >= 6)
    {
      LCD_ShowCharBig(164-position*16-16*2,0,num,YELLOW);
    }
    else if(position >= 3)
    {
      LCD_ShowCharBig(164-position*16-16,0,num,YELLOW);
    }
    else
    {
      LCD_ShowCharBig(164-position*16,0,num,YELLOW);
    }
  }
  /*左移*/
  else if(L == 1)
  {
    for(i=0;i<position;i++)
    {
      temp*=10;
    }
    num = (FREQ%temp)/(temp/10);
    if(position >= 6)
    {
      LCD_ShowCharBig(164-position*16-16*2,0,num,YELLOW);
    }
    else if(position >= 3)
    {
      LCD_ShowCharBig(164-position*16-16,0,num,YELLOW);
    } 
    else
    {
      LCD_ShowCharBig(164-position*16,0,num,YELLOW);
    }
    BACK_COLOR = RED;
    if(position+1 < len)
    {
      position += 1;
    }
    temp = 10;
    for(i=0;i<position;i++)
    {
      temp*=10;
    }
    num = (FREQ%temp)/(temp/10);
    if(position >= 6)
    {
      LCD_ShowCharBig(164-position*16-16*2,0,num,YELLOW);
    }
    else if(position >= 3)
    {
      LCD_ShowCharBig(164-position*16-16,0,num,YELLOW);
    }
    else
    {
      LCD_ShowCharBig(164-position*16,0,num,YELLOW);
    }
  }
  /*右移*/
  else if(L == 2)
  {
    for(i=0;i<position;i++)
    {
      temp*=10;
    }
    num = (FREQ%temp)/(temp/10);
    if(position >= 6)
    {
      LCD_ShowCharBig(164-position*16-16*2,0,num,YELLOW);
    }
    else if(position >= 3)
    {
      LCD_ShowCharBig(164-position*16-16,0,num,YELLOW);
    } 
    else
    {
      LCD_ShowCharBig(164-position*16,0,num,YELLOW);
    }
    BACK_COLOR = RED;
    if(position > 0)
    {
      position -= 1;
    }
    temp = 10;
    for(i=0;i<position;i++)
    {
      temp*=10;
    }
    num = (FREQ%temp)/(temp/10);
    if(position >= 6)
    {
      LCD_ShowCharBig(164-position*16-16*2,0,num,YELLOW);
    }
    else if(position >= 3)
    {
      LCD_ShowCharBig(164-position*16-16,0,num,YELLOW);
    }
    else
    {
      LCD_ShowCharBig(164-position*16,0,num,YELLOW);
    }
  }
  /*加数*/
  else if(L == 3)
  {
    temp = 1;
    for(i=0;i<position;i++)
    {
      temp*=10;
    }
    if(temp + FREQ < 5000000)
    {
      FREQ += temp;
    }
    Freq_Service(0);
  }
  /*减数*/
  else if(L == 4)
  {
    temp = 1;
    for(i=0;i<position;i++)
    {
      temp*=10;
    }
    if(temp < FREQ)
    {
      FREQ -= temp;
    }
  }
  /*清除关注*/
  else if(L == 5)
  {
    for(i=0;i<position;i++)
    {
      temp*=10;
    }
    num = (FREQ%temp)/(temp/10);
    if(position >= 6)
    {
      LCD_ShowCharBig(164-position*16-16*2,0,num,YELLOW);
    }
    else if(position >= 3)
    {
      LCD_ShowCharBig(164-position*16-16,0,num,YELLOW);
    }
    else
    {
      LCD_ShowCharBig(164-position*16,0,num,YELLOW);
    }
  }
  BACK_COLOR = temp_color;
}
void Vpp_Service(int L)
{
  static int position=0;
  u16 temp_color = BACK_COLOR;
  u32 temp=10;
  u16 i,num;
  u16 len = Num_Len(VPP);
  if(position >= len)
  {
    if(len > 0)
    {
      position = len-1;
    }
    else
    {
      position = 0;
    }
  }
  if(L == 0)
  {
    BACK_COLOR = RED;
    for(i=0;i<position;i++)
    {
      temp*=10;
    }
    num = (VPP%temp)/(temp/10);
    if(position >= 6)
    {
      LCD_ShowCharBig(354-position*16-16*2,0,num,YELLOW);
    }
    else if(position >= 3)
    {
      LCD_ShowCharBig(354-position*16-16,0,num,YELLOW);
    }
    else
    {
      LCD_ShowCharBig(354-position*16,0,num,YELLOW);
    }
  }
  /*左移*/
  else if(L == 1)
  {
    for(i=0;i<position;i++)
    {
      temp*=10;
    }
    num = (VPP%temp)/(temp/10);
    if(position >= 6)
    {
      LCD_ShowCharBig(354-position*16-16*2,0,num,YELLOW);
    }
    else if(position >= 3)
    {
      LCD_ShowCharBig(354-position*16-16,0,num,YELLOW);
    } 
    else
    {
      LCD_ShowCharBig(354-position*16,0,num,YELLOW);
    }
    BACK_COLOR = RED;
    if(position+1 < len)
    {
      position += 1;
    }
    temp = 10;
    for(i=0;i<position;i++)
    {
      temp*=10;
    }
    num = (VPP%temp)/(temp/10);
    if(position >= 6)
    {
      LCD_ShowCharBig(354-position*16-16*2,0,num,YELLOW);
    }
    else if(position >= 3)
    {
      LCD_ShowCharBig(354-position*16-16,0,num,YELLOW);
    }
    else
    {
      LCD_ShowCharBig(354-position*16,0,num,YELLOW);
    }
  }
  /*右移*/
  else if(L == 2)
  {
    for(i=0;i<position;i++)
    {
      temp*=10;
    }
    num = (VPP%temp)/(temp/10);
    if(position >= 6)
    {
      LCD_ShowCharBig(354-position*16-16*2,0,num,YELLOW);
    }
    else if(position >= 3)
    {
      LCD_ShowCharBig(354-position*16-16,0,num,YELLOW);
    } 
    else
    {
      LCD_ShowCharBig(354-position*16,0,num,YELLOW);
    }
    BACK_COLOR = RED;
    if(position > 0)
    {
      position -= 1;
    }
    temp = 10;
    for(i=0;i<position;i++)
    {
      temp*=10;
    }
    num = (VPP%temp)/(temp/10);
    if(position >= 6)
    {
      LCD_ShowCharBig(354-position*16-16*2,0,num,YELLOW);
    }
    else if(position >= 3)
    {
      LCD_ShowCharBig(354-position*16-16,0,num,YELLOW);
    }
    else
    {
      LCD_ShowCharBig(354-position*16,0,num,YELLOW);
    }
  }
  /*加数*/
  else if(L == 3)
  {
    temp = 1;
    for(i=0;i<position;i++)
    {
      temp*=10;
    }
    if(temp + VPP <= 5000)
    {
      VPP += temp;
    }
    Vpp_Service(0);
  }
  /*减数*/
  else if(L == 4)
  {
    temp = 1;
    for(i=0;i<position;i++)
    {
      temp*=10;
    }
    if(2040 <= VPP-temp)
    {
      VPP -= temp;
    }
  }
  /*清除关注*/
  else if(L == 5)
  {
    for(i=0;i<position;i++)
    {
      temp*=10;
    }
    num = (VPP%temp)/(temp/10);
    if(position >= 6)
    {
      LCD_ShowCharBig(354-position*16-16*2,0,num,YELLOW);
    }
    else if(position >= 3)
    {
      LCD_ShowCharBig(354-position*16-16,0,num,YELLOW);
    }
    else
    {
      LCD_ShowCharBig(354-position*16,0,num,YELLOW);
    }
  }
  BACK_COLOR = temp_color;
}

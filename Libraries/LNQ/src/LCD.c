#include "LCD.h"
#include "FONT.h" 	   

#include "Lcd.h"
/*
 * LCD.c
 *
 *  Created on: 2014年5月21日
 *      Author: lvniqi
 *      Email: lvniqi@gmail.com
 */

//LCD的画笔颜色和背景色	   
u16 POINT_COLOR = 0xFFFF;	//画笔颜色
u16 BACK_COLOR = 0x0000;  //背景色
Lcd_Curve Lcd_Curve1;
/*缓冲数组源*/
u8 _LCD_Curve_Buffer[LCD_CURVE_BUFFER_LEN];
/*sprintf缓冲*/
char LCD_BUFFER[100];
//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
void inline LCD_SetCursor(u16 Xpos, u16 Ypos)
{
	LCD_WRITE_CMD(0x002A);  //Set column start address
	LCD_WRITE_DATA(Xpos >> 8);
	LCD_WRITE_DATA(0x00FF & Xpos);
	Xpos += 7;
	LCD_WRITE_DATA(Xpos >> 8);
	LCD_WRITE_DATA(0x00FF & Xpos);
	LCD_WRITE_CMD(0x002B);  //Set page start address
	LCD_WRITE_DATA(Ypos >> 8);
	LCD_WRITE_DATA(0x00FF & Ypos);
	Ypos += 15;
	LCD_WRITE_DATA(Ypos >> 8);
	LCD_WRITE_DATA(0x00FF & Ypos);
}

void LCD_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef FSMC_NORSRAMTimingInitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);	//使能FSMC时钟
	RCC_APB2PeriphClockCmd(
			RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE
					| RCC_APB2Periph_GPIOG | RCC_APB2Periph_AFIO, ENABLE);//使能PORTB,D,E,G以及AFIO复用功能时钟

	//PORTD复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4
			| GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11
			| GPIO_Pin_14 | GPIO_Pin_15;				 //	//PORTD复用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	//PORTE复用推挽输出  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9
			| GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13
			| GPIO_Pin_14 | GPIO_Pin_15;				 //	//PORTD复用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Enable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =
			FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity =
			FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive =
			FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait =
			FSMC_AsynchronousWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct =
			&FSMC_NORSRAMTimingInitStructure;
	FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = 7; //3<8，TFT控制芯片的地址建立时间最快是10ns，所以这个和下一行的参数加起来要保证能让地址建立的时间高于这个数字，
	FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 3; 		 //1or2<16
	FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = 6; //5<64（+1）,相比译码写入的速度慢很多 因此加大写入延时可以提高数据稳定性，这也是与tft和fpga通形唯一差异的地方
	FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 1;//以上参数通过调试已经比极限要高一点，极限要求低于10MHz的点数据传输速度。
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

	/* Enable FSMC Bank1_SRAM Bank */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
        LCD_REST = 1;
	LCD_REST = 0;
	delay_ms(10);
	LCD_REST = 1;

	LCD_WRITE_CMD(0x0011);
	delay_ms(10);
	LCD_WRITE_CMD(0x00D0);
	LCD_WRITE_DATA(0x0007);
	LCD_WRITE_DATA(0x0041);
	LCD_WRITE_DATA(0x0018);

	LCD_WRITE_CMD(0x00D1);
	LCD_WRITE_DATA(0x0000);
	LCD_WRITE_DATA(0x000a);
	LCD_WRITE_DATA(0x0010);

	LCD_WRITE_CMD(0x00D2);
	LCD_WRITE_DATA(0x0001);
	LCD_WRITE_DATA(0x0011);

	LCD_WRITE_CMD(0x00C0);
	LCD_WRITE_DATA(0x0010);
	LCD_WRITE_DATA(0x003B);
	LCD_WRITE_DATA(0x0000);
	LCD_WRITE_DATA(0x0002);
	LCD_WRITE_DATA(0x0011);

	LCD_WRITE_CMD(0x00C1);
	LCD_WRITE_DATA(0x0010);
	LCD_WRITE_DATA(0x0013);
	LCD_WRITE_DATA(0x0088);

	LCD_WRITE_CMD(0x00C5);
	LCD_WRITE_DATA(0x0002);

	LCD_WRITE_CMD(0x00C8);
	LCD_WRITE_DATA(0x0000);
	LCD_WRITE_DATA(0x0037);
	LCD_WRITE_DATA(0x0025);
	LCD_WRITE_DATA(0x0006);
	LCD_WRITE_DATA(0x0004);
	LCD_WRITE_DATA(0x001e);
	LCD_WRITE_DATA(0x0026);
	LCD_WRITE_DATA(0x0042);
	LCD_WRITE_DATA(0x0077);
	LCD_WRITE_DATA(0x0044);
	LCD_WRITE_DATA(0x000f);
	LCD_WRITE_DATA(0x0012);

	LCD_WRITE_CMD(0x00F3);
	LCD_WRITE_DATA(0x0040);
	LCD_WRITE_DATA(0x000A);

	LCD_WRITE_CMD(0x00F6);
	LCD_WRITE_DATA(0x0080);

	LCD_WRITE_CMD(0x00F7);
	LCD_WRITE_DATA(0x0080);

	LCD_WRITE_CMD(0x0036);												//显示行列设置
	LCD_WRITE_DATA(0x0029);

	LCD_WRITE_CMD(0x003a);
	LCD_WRITE_DATA(0x0055);

	LCD_WRITE_CMD(0x0020);
//	 LCD_WRITE_CMD(0x0021);

	LCD_WRITE_CMD(0x002A);
	LCD_WRITE_DATA(0x0000);
	LCD_WRITE_DATA(0x0000);
	LCD_WRITE_DATA(0x0001);
	LCD_WRITE_DATA(0x003F);

	LCD_WRITE_CMD(0x002B);
	LCD_WRITE_DATA(0x0000);
	LCD_WRITE_DATA(0x0000);
	LCD_WRITE_DATA(0x0001);
	LCD_WRITE_DATA(0x00df);
        
        LCD_WRITE_CMD(0x00C1);
	LCD_WRITE_DATA(0x0000);
	LCD_WRITE_DATA(0x0010);
	LCD_WRITE_DATA(0x0022);

	delay_ms(10);
	LCD_WRITE_CMD(0x0029);
	delay_ms(10);
	LCD_WRITE_CMD(0x002C);
//		

	LCD_Clear1(BLACK);
}

void LCD_ShowChinese(u16 x, u16 y, u8 size, u8 num,u16 color)
{;
	u8 temp, t1;
        int t;
	LCD_SetCursor(x, y);		//设置光标位置
	LCD_WriteRAM_Prepare();	//开始写入GRAM
	for (t = size - 1; t > 0; t--)
	{
		temp = asc4_1608[num][t];		 //调用1608字体
		for (t1 = 0; t1 < 8; t1++)
		{
			if (temp & 0x80)
                          LCD_WRITE_DATA(color);
			else
                          LCD_WRITE_DATA(BACK_COLOR);
			temp <<= 1;
		}
	}
        LCD_SetCursor(x+8, y);
        LCD_WriteRAM_Prepare();	//开始写入GRAM
        for (t = size - 1; t > 0; t--)
	{
		temp = asc4_1608[num+1][t];		 //调用1608字体
		for (t1 = 0; t1 < 8; t1++)
		{
			if (temp & 0x80)
                          LCD_WRITE_DATA(color);
			else
                          LCD_WRITE_DATA(BACK_COLOR);
			temp <<= 1;
		}
	}
}
//在指定位置显示一个字符
void LCD_ShowChar(u16 x, u16 y2, u8 num, u8 size,u16 color)
{
	u8 a0 = 1;
	u8 temp, t1, t;
	u16 color_2 = POINT_COLOR;
	num = num - ' ';
	LCD_SetCursor(x, y2);		//设置光标位置
	LCD_WriteRAM_Prepare();	//开始写入GRAM
	for (t = size - 1; t > 0; t--)
	{
		temp = asc3_1608[num][t];		//调用1608字体
		for (t1 = 0; t1 < 8; t1++)
		{
			if (temp & 0x80)
				POINT_COLOR = color;
			else
				POINT_COLOR = BACK_COLOR;
			LCD_WRITE_DATA(POINT_COLOR);
			temp <<= 1;
		}
		if (((a0 == 1) & (t == 1)) == 1)
		{
			t++;
			a0 = 0;
		}
	}
	POINT_COLOR = color_2;
}
void LCD_ShowCharBig(u16 x,u16 y,u8 num,u16 color)
{
	int temp, t1, t;
	u16 color_2 = POINT_COLOR;
	LCD_SetCursor(x, y+16);		//设置光标位置
	LCD_WriteRAM_Prepare();	//开始写入GRAM
	for (t = 15; t >= 0; t--)
	{
		temp = asc_3216[num][t];		//调用1608字体
		for (t1 = 0; t1 < 8; t1++)
		{
			if (temp & 0x80)
				POINT_COLOR = color;
			else
				POINT_COLOR = BACK_COLOR;
			LCD_WRITE_DATA(POINT_COLOR);
			temp <<= 1;
		}
	}
        LCD_SetCursor(x, y);		//设置光标位置
        LCD_WriteRAM_Prepare();	//开始写入GRAM
	for (t = 31; t >= 16; t--)
	{
		temp = asc_3216[num][t];		//调用1608字体
		for (t1 = 0; t1 < 8; t1++)
		{
			if (temp & 0x80)
				POINT_COLOR = color;
			else
				POINT_COLOR = BACK_COLOR;
			LCD_WRITE_DATA(POINT_COLOR);
			temp <<= 1;
		}
	}
        LCD_SetCursor(x+8, y+16);//设置光标位置
        LCD_WriteRAM_Prepare();	//开始写入GRAM
	for (t = 47; t >= 32; t--)
	{
		temp = asc_3216[num][t];		//调用1608字体
		for (t1 = 0; t1 < 8; t1++)
		{
			if (temp & 0x80)
				POINT_COLOR = color;
			else
				POINT_COLOR = BACK_COLOR;
			LCD_WRITE_DATA(POINT_COLOR);
			temp <<= 1;
		}
	}
        LCD_SetCursor(x+8, y);			//设置光标位置
        LCD_WriteRAM_Prepare();	//开始写入GRAM
	for (t = 63; t >= 48; t--)
	{
		temp = asc_3216[num][t];		//调用1608字体
		for (t1 = 0; t1 < 8; t1++)
		{
			if (temp & 0x80)
				POINT_COLOR = color;
			else
				POINT_COLOR = BACK_COLOR;
			LCD_WRITE_DATA(POINT_COLOR);
			temp <<= 1;
		}
	}
	POINT_COLOR = color_2;
}
void LCD_ShowNumBig(u16 x,u16 y,u32 num,u16 color)
{
    char temp[20],len = 0,len_max,i,p=0;
    while(num)
    {
      temp[len] = num%10;
      num /= 10;
      len++;
      if((len-p)%3 == 0)
      {
        temp[len] = 10;
        len++;
        p++;
      }
    }
    len_max = len;
    len--;
    if(temp[len] == 10)
    {
      len--;
      len_max--;
    }
    for(i=0;i<len_max;i++)
    {
      LCD_ShowCharBig(x+i*16,y,temp[len],color);
      len--;
    }
}
void LCD_ShowNumBig_L(u16 x,u16 x_end,u16 y,u32 num,u16 color)
{
    int temp[20],len = 0,len_max,i,p=0;
    while(num)
    {
      temp[len] = num%10;
      num /= 10;
      len++;
      if((len-p)%3 == 0)
      {
        temp[len] = 10;
        len++;
        p++;
      }
    }
    len_max = len;
    len--;
    if(temp[len] == 10)
    {
      len--;
      len_max--;
    }
    if(len_max == 0)
    {
      len = 0;
      temp[len] = 0;
      len_max = 1;
    }
    if(len_max)
    {
      x-= len_max*16;
      for(i=x_end;i<=x;i+=16)
      {
        LCD_ShowCharBig(i,y,14,color);
      }
      for(i=0;i<len_max;i++)
      {
        LCD_ShowCharBig(x+i*16,y,temp[len],color);
        len--;
      }
    }
}
void LCD_ShowString(u16 x, u16 y2, u8 size, char *p,u16 color)
{
	while ((*p <= '~') && (*p >= ' '))		 //判断是不是非法字符!
	{
		LCD_ShowChar(x, y2, *p, size,color);
		x += size / 2;
		p++;
	}
}

void LCD_Clear1(u16 Colour)
{
	u32 i;
	LCD_WRITE_CMD(0x002A);		 //Set column address
	LCD_WRITE_DATA(0);
	LCD_WRITE_DATA(0);
	LCD_WRITE_DATA(1);
	LCD_WRITE_DATA(223);
	LCD_WRITE_CMD(0x002B);		 //Set page address
	LCD_WRITE_DATA(0);
	LCD_WRITE_DATA(0);
	LCD_WRITE_DATA(1);
	LCD_WRITE_DATA(63);
	LCD_WRITE_CMD(0x002C); //write memory start
	for (i = 0; i < 0x25800; i++)
		LCD_WRITE_DATA(Colour);
}
/*写入准备*/
inline void LCD_WriteRAM_Prepare()
{
	LCD_WRITE_CMD(0x002C);
}
/*从键盘读取数字 */
u32 LCD_GetChar(u16 x, u16 y2, u8 size)
{
	u32 data = 0;
	u8 i;
	LCD_ShowString(x, y2, size, "                        ",YELLOW);
	for (i = 1; i < 8; i++)
	{
		while (True)
		{
			if (MAIN_KEY.keysign)
			{
				MAIN_KEY.keysign = 0;
				break;
			}
		}
		if (MAIN_KEY.keycode == 0 and data == 0)
		{
			i = 0;
		}
		else if (MAIN_KEY.keycode < 10)
		{
			data *= 10;
			data += MAIN_KEY.keycode;
			LCD_ShowChar(x, y2, '0' + MAIN_KEY.keycode, size,YELLOW);
			x += size / 2;
		}
		else if (MAIN_KEY.keycode == 15)
		{
			break;
		}
		else
		{
			i--;
		}

	}
	sprintf(LCD_BUFFER, "GET_DATA:%d", data);
	LCD_ShowString(x, y2, size, LCD_BUFFER,BLUE);
	return data;
}
void inline _LCD_Set_Curve_Pixel(Lcd_Curve* Lcd_Curvel1,u16 x,u16 y,u16 color)
{
	u16 x1= x-Lcd_Curvel1->Start_x;
	u16 y1= y-Lcd_Curvel1->Start_y;
	if(
			(y1%32==0 and (x1<6 or x1>394 or x1%40==0))
			or
			(x1%40==0 and (y1<6 or y1>250 or y1%32==0))
					)
	{

	}
	else
	{
		_LCD_Set_Pixel(x,y,color);
	}
}
/*插值补偿*/
inline void _LCD_Show_Curve_Compensation(Lcd_Curve* Lcd_Curve1,u16 x1, u16 y1, u16 x2, u16 y2_before,
		u16 color)
{
	int i, j = 0;
	int x_different = (int) x2 - x1;
	int y_different = (int) y2_before - y1;
	int y_now = y1;
	int count = y_different / x_different;
	for (i = 0; i < x_different; i++)
	{
		if (count > 0)
		{
			for (j = 0; j < (count + 1) / 2; j++)
			{
				y_now = y_different * i / x_different + y1 + j;
				_LCD_Set_Curve_Pixel(Lcd_Curve1,x1, y_now, color);
			}
			x1++;
			for (j = count / 2; j <= count; j++)
			{
				y_now = y_different * i / x_different + y1 + j;
				_LCD_Set_Curve_Pixel(Lcd_Curve1,x1, y_now, color);
			}
		}
		else if (count < 0)
		{
			for (j = 0; j > (count - 1) / 2; j--)
			{
				y_now = y_different * i / x_different + y1 + j;
				_LCD_Set_Curve_Pixel(Lcd_Curve1,x1, y_now, color);
			}
			x1++;
			for (j = count / 2; j >= count; j--)
			{
				y_now = y_different * i / x_different + y1 + j;
				_LCD_Set_Curve_Pixel(Lcd_Curve1,x1, y_now, color);
			}
		}
		else
		{
			y_now = y_different * i / x_different + y1;
			_LCD_Set_Curve_Pixel(Lcd_Curve1,x1, y_now, color);
			x1++;
		}
	}
        
	if (y_different > 0)
	{
		for (i = y_now; i < y2_before; i++)
		{
			_LCD_Set_Curve_Pixel(Lcd_Curve1,x1, i, color);
		}
	}
	else if (y_different < 0)
	{
		for (i = y_now; i > y2_before; i--)
		{
			_LCD_Set_Curve_Pixel(Lcd_Curve1,x1, i, color);
		}
	}

}
/*显示曲线*/
void LCD_Curve_Show(Lcd_Curve* Lcd_Curve1, u16 y2, u8 step)
{
	if (not y2)
		y2 += 1;
	u16  len =Lcd_Curve1->End_x-Lcd_Curve1->Start_x;
	u16 y3 =(Lcd_Curve1->Curve_last_y)[((Lcd_Curve1->Now_x) + step + len) % len];
	u16 x1 = ((Lcd_Curve1->Now_x) - step + len) % len;
	u16 y1_after = (Lcd_Curve1->Curve_last_y)[x1];
	u16 y2_before = (Lcd_Curve1->Curve_last_y)[(Lcd_Curve1->Now_x)];
	u16 color = Lcd_Curve1->point_color;
	static u16 y1_before;

	if (Lcd_Curve1->finish_flag != 1)
	{
		if ((Lcd_Curve1->Now_x) == 0)
		{
			_LCD_Set_Curve_Pixel(Lcd_Curve1,(Lcd_Curve1->Now_x)+Lcd_Curve1->Start_x,
					(Lcd_Curve1->Curve_last_y)[(Lcd_Curve1->Now_x)]+Lcd_Curve1->Start_y, BACK_COLOR);
			_LCD_Show_Curve_Compensation(Lcd_Curve1,(Lcd_Curve1->Now_x)+Lcd_Curve1->Start_x, y2_before+Lcd_Curve1->Start_y,
					(Lcd_Curve1->Now_x) + step+Lcd_Curve1->Start_x, y3+Lcd_Curve1->Start_y, BACK_COLOR);
			(Lcd_Curve1->Curve_last_y)[(Lcd_Curve1->Now_x)] = y2;
			_LCD_Set_Curve_Pixel(Lcd_Curve1,(Lcd_Curve1->Now_x)+Lcd_Curve1->Start_x, y2+Lcd_Curve1->Start_y, color);
		}
		else
		{
			/*首先清除数据*/
			_LCD_Set_Curve_Pixel(Lcd_Curve1,(Lcd_Curve1->Now_x + step+Lcd_Curve1->Start_x), y3+Lcd_Curve1->Start_y, BACK_COLOR);
			_LCD_Show_Curve_Compensation(Lcd_Curve1,(Lcd_Curve1->Now_x+Lcd_Curve1->Start_x), y2_before+Lcd_Curve1->Start_y,
					(Lcd_Curve1->Now_x) + step+Lcd_Curve1->Start_x, y3+Lcd_Curve1->Start_y, BACK_COLOR);
			/*再添加数据*/
			(Lcd_Curve1->Curve_last_y)[(Lcd_Curve1->Now_x)] = y2;
			_LCD_Set_Curve_Pixel(Lcd_Curve1,(Lcd_Curve1->Now_x)+Lcd_Curve1->Start_x, y2+Lcd_Curve1->Start_y, color);
			/*写入现有数据*/
			_LCD_Show_Curve_Compensation(Lcd_Curve1,(Lcd_Curve1->Now_x) - step+Lcd_Curve1->Start_x, y1_after+Lcd_Curve1->Start_y,
					(Lcd_Curve1->Now_x)+Lcd_Curve1->Start_x, y2+Lcd_Curve1->Start_y, color);
		}
		y1_before = y2_before;
		(Lcd_Curve1->Now_x) += step;
		if (Lcd_Curve1->Now_x >= len)
		{
			Lcd_Curve1->finish_flag = 1;
		}
	}
}
void LCD_Curve_Init(Lcd_Curve* Lcd_Curve1, u16 start_x1, u16 start_y1,u16 color)
{
	Lcd_Curve1->Start_x = start_x1;
	Lcd_Curve1->Start_y = start_y1;
	Lcd_Curve1->End_x =start_x1+400;
	Lcd_Curve1->End_y = start_y1+256;
	Lcd_Curve1->point_color = color;
	Sequeue_Init(&(Lcd_Curve1->LCD_Curve_Buffer),_LCD_Curve_Buffer,LCD_CURVE_BUFFER_LEN);
        u16 i;
        for(i=Lcd_Curve1->Start_y;i<=Lcd_Curve1->End_y;i++)
        {
        	_LCD_Set_Pixel(Lcd_Curve1->Start_x-1,i,WHITE);
        	_LCD_Set_Pixel(Lcd_Curve1->End_x+1,i,WHITE);
        }
        for(i=Lcd_Curve1->Start_x;i<=Lcd_Curve1->End_x;i++)
        {
        	_LCD_Set_Pixel(i,Lcd_Curve1->Start_y-1,WHITE);
        	_LCD_Set_Pixel(i,Lcd_Curve1->End_y+1,WHITE);
        }
        LCD_Curve_Grid(Lcd_Curve1);
}
void LCD_Show_Rect(u16 x,u16 y,u16 lenth,u16 width,u16 point_color)
{
	u16 i,j;
	for(i=0;i<lenth;i++)
        {
          _LCD_Set_Pixel(x+i,y+width,point_color);
          _LCD_Set_Pixel(x+i,y,point_color);
	}
        for(j=0;j<width;j++)
        {
          _LCD_Set_Pixel(x+lenth,y+j,point_color);
          _LCD_Set_Pixel(x,y+j,point_color);
	}
}
void LCD_Curve_Grid(Lcd_Curve* Lcd_Curve1)
{
	u16 i,j;
	int k;
	for(j=32;j<256;j+=32)
	{
		for(i=0;i<6;i++)
		{

			_LCD_Set_Pixel(i+Lcd_Curve1->Start_x,Lcd_Curve1->Start_y+j,WHITE);
			_LCD_Set_Pixel(400-i+Lcd_Curve1->Start_x,Lcd_Curve1->Start_y+j,WHITE);
		}
	}
        for(j=32;j<256;j+=32)
	{
		for(i=40;i<400;i+=40)
		{

			_LCD_Set_Pixel(i+Lcd_Curve1->Start_x,Lcd_Curve1->Start_y+j,WHITE);
		}
	}
	for(i=40;i<400;i+=40)
	{
		for(j=0;j<6;j++)
		{
			_LCD_Set_Pixel(i+Lcd_Curve1->Start_x,Lcd_Curve1->Start_y+j,WHITE);
			_LCD_Set_Pixel(i+Lcd_Curve1->Start_x,Lcd_Curve1->Start_y+256-j,WHITE);
		}
	}
}
void LCD_Clear_Curve(Lcd_Curve * Lcd_Curve1, u8 step)
{
	u16 i = 0;
	u16 color = Lcd_Curve1->point_color;
	Lcd_Curve1->point_color = BACK_COLOR;
	Lcd_Curve1->finish_flag = 0;
        (Lcd_Curve1->Now_x) = 0;
	for (i = 0; i < Lcd_Curve1->End_x-Lcd_Curve1->Start_x; i += step)
	{
		LCD_Curve_Show(Lcd_Curve1,0,step);
	}
	LCD_Curve_Init(Lcd_Curve1, Lcd_Curve1->Start_x, Lcd_Curve1->Start_y, color);
}

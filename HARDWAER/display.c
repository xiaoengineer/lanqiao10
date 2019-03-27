#include "display.h"

u8 code seg_ment[] ={0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,0x80, 0x90};
u8 seg_buff[] = {0xff, 0xff,0xff, 0xff,0xff, 0xff,0xff, 0xff};

/*
函数名称：show_number(u8 mode, u16 dat)
参数： void
返回值： void
说明：更新seg_buff[]
作者：王慧云
时间：2019.3.26
*/
void led_scan(void)
{
	static u8 index = 0;
	P2 = (P2 & 0X1F) | 0XE0;
	P0 = 0XFF;
	P2 = (P2 & 0X1F);
	
	P2 = (P2 & 0X1F) | 0XC0;
	P0 = 0X80 >> index;
	P2 = (P2 & 0X1F);
	
	P2 = (P2 & 0X1F) | 0XE0;
	P0 = seg_buff[index] ;
	P2 = (P2 & 0X1F);
	
	if(index < 7)
	{
		index ++;
	}
	else
		index = 0;
}

/*
函数名称：show_number(u8 mode, u16 dat)
参数： u8 mode, u16 dat
返回值： void
说明：  mode是显示模式选择0为显示频率， 1为显示电压,2为关
		dat为显示的数据
作者：王慧云
时间：2019.3.26
*/
void show_number(u8 mode, u16 dat)
{
	u8 unit,dec,hundred,kilo, i;
	u8 buff[4];
	if(0 == mode)
	{
		for(i = 0; i < 4; i ++)
		{
			buff[i] = dat % 10;
			dat = dat /10;		
		}
		for(i = 3; i >= 0; i --)
		{
			if(buff[i] == 0xff)
			{
				buff[i] = 0xff;
			}
			else
			{
				break;
			}
		}

		seg_buff[7] = 0x8E;	
		seg_buff[0] = seg_ment[buff[0]];
		seg_buff[1] = seg_ment[buff[1]];
		seg_buff[2] = seg_ment[buff[2]];
		seg_buff[3] = seg_ment[buff[3]];
	}
	if(1 == mode)
	{
		unit = dat % 10;
		dec = dat / 10 % 10;
		hundred = dat / 100 % 10;
		kilo = dat / 1000;

		seg_buff[7] = 0xC1;	
		seg_buff[0] = seg_ment[unit];
		seg_buff[1] = seg_ment[dec];
		seg_buff[2] = seg_ment[hundred] & 0x7f;
		seg_buff[3] = 0xff;
	}
	if(2 == mode)
	{
		seg_buff[7] = 0xff;	
		seg_buff[0] = 0xff;
		seg_buff[1] = 0xff;
		seg_buff[2] = 0xff;
		seg_buff[3] = 0xff;
	}
}
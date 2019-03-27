#include <STC15F2K60S2.H>
#include "display.h"
#include "key.h"
#include "pcf8951.h"

/*定义LED端口*/
sbit LED1 = P0^0;
sbit LED2 = P0^1;
sbit LED3 = P0^2;
sbit LED4 = P0^3;

void close(void);
void Timer1Init(void);		//2毫秒@11.0592MHz
void Timer0Init();    //NE555计数
unsigned int frequency; //NE55的值
unsigned int ne555_i; //NE555计数周期
void led_show(u16 voltage, u16 frequenc, u8 chose); //LED显示函数

void main(void)
{
	u8 s7_tmp_flag = 0xe0; 
	u8 s6_tmp_flag = 0xd0;
	u8 s5_tmp_flag = 0xc1;
	u8 s4_tmp_flag = 0xb0; //各个按键临时存放变量
	float vol; //电压值
	u8 vol_out; //输出电压值
	close(); //关闭外设
	Timer0Init(); //定时器0初始化，用来ne555计数
	Timer1Init(); //定时器1初始化，用来扫描
	while(1)
	{		
		vol_out = get_ADCVluce();//获取电压值
		vol = vol_out / 51.2 * 100;//处理电压值
		
		/********按键键值检测*******/
		/*******开始*******/
		if(0xe1 == key_drive())
		{
			s7_tmp_flag = 0xe1;
		}
		if(0xe0 == key_drive())
		{
			s7_tmp_flag = 0xe0;	
		}
		if(0xd1 == key_drive())
		{
			s6_tmp_flag = 0xd1;
		}
		if(0xd0 == key_drive())
		{
			s6_tmp_flag = 0xd0;	
		}		
		if(0xc1 == key_drive())
		{
			s5_tmp_flag = 0xc1;
		}
		if(0xc0 == key_drive())
		{
			s5_tmp_flag = 0xc0;	
		}
		if(0xb1 == key_drive())
		{
			s4_tmp_flag = 0xb1;
		}
		if(0xb0 == key_drive())
		{
			s4_tmp_flag = 0xb0;	
		}
		/********结束********/
		
		if(0xe0 == s7_tmp_flag) //当s7按下的时候（或者初始化）
		{
			if(0xb1 == s4_tmp_flag) //如果s4按下
			{
				show_number(0, frequency); //显示频率
			}
			if(0xb0 == s4_tmp_flag) //如果s4按下（或者初始化）
			{
				if(0xc1 == s5_tmp_flag) //同步输出dac
				{
					show_number(1, vol); //显示电压
					ADC_OUT(vol_out); //输出电压
				}
				if(0xc0 == s5_tmp_flag) //定值输出
				{
					show_number(1, 200);
					ADC_OUT(102);
				}
			}
		}
		if(0xe1 == s7_tmp_flag) //当s7按下的时候
		{
				show_number(2, 000); //关闭显示器
		}
		if(0xd0 == s6_tmp_flag) //当s6按下的时候（或者初始化）
		{
			led_show(vol, frequency, s4_tmp_flag); //led显示
		}
		if(0xd1 == s6_tmp_flag) //当s6按下的时候
		{
			led_show(vol, frequency, 0); //LED关闭
		}
		
	}
}

/*
函数名称：close(void)
参数： void
返回值： void
说明：关闭外设，LED，蜂鸣器，继电器
作者：王慧云
时间：2019.3.26
*/
void close(void)
{
	P2 = (P2 & 0X1F) | 0XA0;
	P0 = 0XAF;
	P2 = P2 & 0X1F;
	
	P2 = (P2 & 0X1F) | 0X90;
	P0 = 0XFF;
	P2 = P2 & 0X1F;
}

void Timer1Init(void)		//2毫秒@11.0592MHz
{
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x10;		//设置定时器模式
	TL1 = 0xCD;		//设置定时初值
	TH1 = 0xF8;		//设置定时初值
	EA = 1;
	ET1 = 1;
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
}

/*
函数名称：led_show(u16 voltage, u16 frequenc, u8 chose)
参数： u16 voltage, u16 frequenc, u8 chose
返回值： void
说明：u16 voltage：被比较的电压
	 u16 frequenc：被比较的频率
	  u8 chose：选择显示模式
作者：王慧云
时间：2019.3.26
*/
void led_show(u16 voltage, u16 frequenc, u8 chose)
{
	P2 = (P2 & 0X1F) | 0X90;
	P0 = 0xff;
	if(0 == chose)
	{
		LED1 = 1;
		LED2 = 1;
		LED3 = 1;
		LED4 = 1;
	}
	else
	{
		if(voltage <= 150)
		{
			LED3 = 1;
		}
		if((voltage >= 150) && (voltage < 250))
		{
			LED3 = 0;
		}
		if((voltage >= 250) && (voltage < 350))
		{
			LED3 = 1;
		}
		if(voltage >= 350)
		{
			LED3 = 0;
		}
		
		if(frequenc <= 500)
		{
			LED4 = 1;
		}
		if((frequenc >= 1500) && (frequenc < 2500))
		{
			LED4 = 0;
		}
		if((frequenc >= 2500) && (frequenc < 3500))
		{
			LED4 = 1;
		}
		if(frequenc >= 3500)
		{
			LED4 = 0;
		}	

		if(0xb0 == chose)
		{
			LED1 = 0;
			LED2 = 1;
		}
		if(0xb1 == chose)
		{
			LED1 = 1;
			LED2 = 0;
		}
	}
	P2 = (P2 & 0X1F);
}

void Timer0Init()		
{
	TMOD|=0x04;  //设为计数模式
	TL0=0;		
	TH0=0;		
	TF0=0;		
	TR0=0;		
}

void Timer1_IRQ() interrupt 3
{
	ne555_i ++;
	TL1 = 0xCD;		//设置定时初值
	TH1 = 0xF8;		//设置定时初值
	led_scan();
	key_scan();
	
	if(ne555_i >= 450)
	{
		ne555_i=0;
		TR0=0;
		frequency = TH0*256+TL0;
		TH0=0;
		TL0=0;
		TR0=1;
	}
}
#inlude "ne555.h"

void Timer0Init()		
{
	TMOD|=0x04;  //设为计数模式
	TL0=0;		
	TH0=0;		
	TF0=0;		
	TR0=0;		
}

void T_1() interrupt 3
{
	static uint8 i;
	i++;
	if(i>=50)
	{
		i=0;
		TR0=0;
		m=TH0*256+TL0;
		TH0=0;
		TL0=0;
		TR0=1;
	}
	display(m);//数码管显示频率值
}


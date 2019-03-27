#include <STC15F2K60S2.H>
#include "display.h"
#include "key.h"
#include "pcf8951.h"

/*����LED�˿�*/
sbit LED1 = P0^0;
sbit LED2 = P0^1;
sbit LED3 = P0^2;
sbit LED4 = P0^3;

void close(void);
void Timer1Init(void);		//2����@11.0592MHz
void Timer0Init();    //NE555����
unsigned int frequency; //NE55��ֵ
unsigned int ne555_i; //NE555��������
void led_show(u16 voltage, u16 frequenc, u8 chose); //LED��ʾ����

void main(void)
{
	u8 s7_tmp_flag = 0xe0; 
	u8 s6_tmp_flag = 0xd0;
	u8 s5_tmp_flag = 0xc1;
	u8 s4_tmp_flag = 0xb0; //����������ʱ��ű���
	float vol; //��ѹֵ
	u8 vol_out; //�����ѹֵ
	close(); //�ر�����
	Timer0Init(); //��ʱ��0��ʼ��������ne555����
	Timer1Init(); //��ʱ��1��ʼ��������ɨ��
	while(1)
	{		
		vol_out = get_ADCVluce();//��ȡ��ѹֵ
		vol = vol_out / 51.2 * 100;//�����ѹֵ
		
		/********������ֵ���*******/
		/*******��ʼ*******/
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
		/********����********/
		
		if(0xe0 == s7_tmp_flag) //��s7���µ�ʱ�򣨻��߳�ʼ����
		{
			if(0xb1 == s4_tmp_flag) //���s4����
			{
				show_number(0, frequency); //��ʾƵ��
			}
			if(0xb0 == s4_tmp_flag) //���s4���£����߳�ʼ����
			{
				if(0xc1 == s5_tmp_flag) //ͬ�����dac
				{
					show_number(1, vol); //��ʾ��ѹ
					ADC_OUT(vol_out); //�����ѹ
				}
				if(0xc0 == s5_tmp_flag) //��ֵ���
				{
					show_number(1, 200);
					ADC_OUT(102);
				}
			}
		}
		if(0xe1 == s7_tmp_flag) //��s7���µ�ʱ��
		{
				show_number(2, 000); //�ر���ʾ��
		}
		if(0xd0 == s6_tmp_flag) //��s6���µ�ʱ�򣨻��߳�ʼ����
		{
			led_show(vol, frequency, s4_tmp_flag); //led��ʾ
		}
		if(0xd1 == s6_tmp_flag) //��s6���µ�ʱ��
		{
			led_show(vol, frequency, 0); //LED�ر�
		}
		
	}
}

/*
�������ƣ�close(void)
������ void
����ֵ�� void
˵�����ر����裬LED�����������̵���
���ߣ�������
ʱ�䣺2019.3.26
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

void Timer1Init(void)		//2����@11.0592MHz
{
	AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TMOD |= 0x10;		//���ö�ʱ��ģʽ
	TL1 = 0xCD;		//���ö�ʱ��ֵ
	TH1 = 0xF8;		//���ö�ʱ��ֵ
	EA = 1;
	ET1 = 1;
	TF1 = 0;		//���TF1��־
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
}

/*
�������ƣ�led_show(u16 voltage, u16 frequenc, u8 chose)
������ u16 voltage, u16 frequenc, u8 chose
����ֵ�� void
˵����u16 voltage�����Ƚϵĵ�ѹ
	 u16 frequenc�����Ƚϵ�Ƶ��
	  u8 chose��ѡ����ʾģʽ
���ߣ�������
ʱ�䣺2019.3.26
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
	TMOD|=0x04;  //��Ϊ����ģʽ
	TL0=0;		
	TH0=0;		
	TF0=0;		
	TR0=0;		
}

void Timer1_IRQ() interrupt 3
{
	ne555_i ++;
	TL1 = 0xCD;		//���ö�ʱ��ֵ
	TH1 = 0xF8;		//���ö�ʱ��ֵ
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
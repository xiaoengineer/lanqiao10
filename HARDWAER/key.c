#include "key.h"
 
sbit KEY1 = P3^0;
sbit KEY2 = P3^1;
sbit KEY3 = P3^2;
sbit KEY4 = P3^3;

unsigned char keySta[4] = {1,1,1,1};

/*
�������ƣ�key_drive(void)
������ void
����ֵ�� void
˵��������key_buff[]
���ߣ�������
ʱ�䣺2019.3.26
*/
void key_scan()
{
    unsigned char i;
    static unsigned char key_suff[4] ={0xff,0xff,0xff,0xff};

    key_suff[0] = (key_suff[0] << 1) | KEY1;
    key_suff[1] = (key_suff[1] << 1) | KEY2;
    key_suff[2] = (key_suff[2] << 1) | KEY3;
    key_suff[3] = (key_suff[3] << 1) | KEY4;
    
    for(i = 0; i < 4; i ++)
    {
        if((key_suff[i] & 0x0f) == 0x0f)
        {
            keySta[i] = 1;
        }
        if((key_suff[i] & 0x0f) == 0x00)
        {
            keySta[i] = 0;
        }
        else
        {}
    }
}

/*
�������ƣ�key_drive(void)
������ void
����ֵ�� unsigned char res
˵������������ֵΪ S7:0xeX; S6:0xdX
				S5:0xcX; S4: 0XbX
				X[0,1]: 0ʱΪ��һ�ΰ���
						1ʱΪ�ڶ��ΰ���
���ߣ�������
ʱ�䣺2019.3.26
*/
unsigned char key_drive(void)
{
    unsigned char i, res;
	static unsigned char flag_s7 = 0; //S7��ת��־
	static unsigned char flag_s6 = 0; //S6��ת��־
	static unsigned char flag_s5 = 0; //S5��ת��־
	static unsigned char flag_s4 = 0; //S4��ת��־
    static unsigned char backup[4] = {1,1,1,1};
    for(i = 0; i < 4; i ++)
    {
		if(backup[i] != keySta[i])
		{
			if(backup[i] != 0)
			{
				switch(i)
				{
					case 0: 
						flag_s7 = flag_s7 + 1;
						flag_s7 &= 0x01;
						if(0 == flag_s7)
						{
							res = 0xe0; //s7��һ�ΰ��µķ���ֵ
						}
						else if(1 == flag_s7)
						{
							res = 0xe1; //s7�ڶ��ΰ��µķ���ֵ
						} break;
						
					case 1: 
						++ flag_s6 ;
						flag_s6 &= 0x01;
						if(0 == flag_s6)
						{
							res = 0xd0; //s6��һ�ΰ��µķ���ֵ
						}
						else if(1 == flag_s6)
						{
							res = 0xd1; //s6�ڶ��ΰ��µķ���ֵ
						} break;

					case 2: 
						++ flag_s5;
						flag_s5 &= 0x01;
						if(0 == flag_s5)
						{
							res = 0xc0; //s5��һ�ΰ��µķ���ֵ
						}
						else if(1 == flag_s5)
						{
							res = 0xc1; //s5�ڶ��ΰ��µķ���ֵ
						} break;
						
					case 3: 
						++ flag_s4;
						flag_s4 &= 0x01;
						if(0 == flag_s4)
						{
							res = 0xb0; //s4��һ�ΰ��µķ���ֵ
						}
						else if(1 == flag_s4)
						{
							res = 0xb1; //s4�ڶ��ΰ��µķ���ֵ
						} break;						
				}
			}
         }
         backup[i] = keySta[i];  
    }
	return res;
}


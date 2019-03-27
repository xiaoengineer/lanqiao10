#include "key.h"
 
sbit KEY1 = P3^0;
sbit KEY2 = P3^1;
sbit KEY3 = P3^2;
sbit KEY4 = P3^3;

unsigned char keySta[4] = {1,1,1,1};

/*
函数名称：key_drive(void)
参数： void
返回值： void
说明：更新key_buff[]
作者：王慧云
时间：2019.3.26
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
函数名称：key_drive(void)
参数： void
返回值： unsigned char res
说明：函数返回值为 S7:0xeX; S6:0xdX
				S5:0xcX; S4: 0XbX
				X[0,1]: 0时为第一次按下
						1时为第二次按下
作者：王慧云
时间：2019.3.26
*/
unsigned char key_drive(void)
{
    unsigned char i, res;
	static unsigned char flag_s7 = 0; //S7反转标志
	static unsigned char flag_s6 = 0; //S6反转标志
	static unsigned char flag_s5 = 0; //S5反转标志
	static unsigned char flag_s4 = 0; //S4反转标志
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
							res = 0xe0; //s7第一次按下的返回值
						}
						else if(1 == flag_s7)
						{
							res = 0xe1; //s7第二次按下的返回值
						} break;
						
					case 1: 
						++ flag_s6 ;
						flag_s6 &= 0x01;
						if(0 == flag_s6)
						{
							res = 0xd0; //s6第一次按下的返回值
						}
						else if(1 == flag_s6)
						{
							res = 0xd1; //s6第二次按下的返回值
						} break;

					case 2: 
						++ flag_s5;
						flag_s5 &= 0x01;
						if(0 == flag_s5)
						{
							res = 0xc0; //s5第一次按下的返回值
						}
						else if(1 == flag_s5)
						{
							res = 0xc1; //s5第二次按下的返回值
						} break;
						
					case 3: 
						++ flag_s4;
						flag_s4 &= 0x01;
						if(0 == flag_s4)
						{
							res = 0xb0; //s4第一次按下的返回值
						}
						else if(1 == flag_s4)
						{
							res = 0xb1; //s4第二次按下的返回值
						} break;						
				}
			}
         }
         backup[i] = keySta[i];  
    }
	return res;
}


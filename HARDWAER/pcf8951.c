#include "pcf8951.h"

/*
�������ƣ�unsigned char get_ADCVluce()
������ void
����ֵ�� unsigned char val
˵��������ADC��ֵ
���ߣ�������
ʱ�䣺2019.3.26
*/
unsigned char get_ADCVluce()
{
	unsigned char val; 
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x40 | 3);
	IIC_WaitAck();
	IIC_Stop();
	
	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();
	val = IIC_RecByte();
	IIC_SendAck(0);
	IIC_Stop();
	
	val = val * 500 / 255;
	return val;
}

/*
�������ƣ�void ADC_OUT(unsigned char vol)
������ unsigned char vol
����ֵ�� void
˵����DACת��
���ߣ�������
ʱ�䣺2019.3.26
*/
void ADC_OUT(unsigned char vol)
{
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x40);
	IIC_WaitAck();
	IIC_SendByte(vol);
	IIC_WaitAck();
	IIC_Stop();
} 
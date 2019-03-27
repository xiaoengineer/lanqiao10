#include "pcf8951.h"

/*
函数名称：unsigned char get_ADCVluce()
参数： void
返回值： unsigned char val
说明：接收ADC的值
作者：王慧云
时间：2019.3.26
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
函数名称：void ADC_OUT(unsigned char vol)
参数： unsigned char vol
返回值： void
说明：DAC转换
作者：王慧云
时间：2019.3.26
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
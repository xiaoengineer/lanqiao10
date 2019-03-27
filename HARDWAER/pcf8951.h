#ifndef __PCF8951_H
#define	__PCF8951_H

#include <STC15F2K60S2.H>
#include "iic.h"

unsigned char get_ADCVluce();
void ADC_OUT(unsigned char vol);
#endif

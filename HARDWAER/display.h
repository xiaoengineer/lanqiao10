#ifndef __DISPLAY_H
#define __DISPLAY_H

#include <STC15F2K60S2.H>
typedef unsigned char u8;
typedef unsigned int u16;

void led_scan(void);
void show_number(u8 mode, u16 dat);

#endif

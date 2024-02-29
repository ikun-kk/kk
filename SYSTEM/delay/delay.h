#ifndef _DELAY_H
#define _DELAY_H

#include "sys.h"

void SysTick_Init(u8 SYSCLK);
void delay_ms(u32 nms);
void delay_us(u32 nus);
void delay_xms(u32 nms);



#endif

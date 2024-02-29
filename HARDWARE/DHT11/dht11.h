#ifndef  _DHT11_H_		//防止头文件重定义
#define  _DHT11_H_

#include "stm32f10x.h"
#include "delay.h"


static void DHT11_Mode_IPU(void);
static void DHT11_Mode_Out_PP(void);
void Dht11_Init(void);
u8 Mcu_Control_Dht11(u8 *p);


typedef struct
{
	int temp;
	int humi;
	int pm25;
	int fd1;
	int fd2;
	int fd3;
	int fd4;
	int  fc_sum;
	int  f9_sum;
	int  f1_sum;
	int usart1_1;
	int usart1_A;
	int usart1_B;
	int usart1_C;
	int usart1_D;
	int usart1_E;
	int usart1_F;
	int usart1_G;
	int usart1_H;
	int usart1_I;
	int usart1_J;
	int usart1_K;
	int usart1_L;
	int usart1_N;
	int usart1_delay;
	int a;
	int b;
	int c;
	int t_w;
	
}ALL_N;


extern ALL_N all_n;

typedef struct
{
	int Y_A;
	int Y_B;
	int Y_C;
	int Y_D;
	int Y_E;
	int Y_F;
	int Y_G;
	int Y_H;
	int Y_I;
	int Y_J;
	int Y_Y;
}VOICE;

extern VOICE voice;
#endif




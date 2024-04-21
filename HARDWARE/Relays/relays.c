#include "FreeRTOS.h"
#include "task.h"
#include "relays.h"
//IN输入高电平 COM与NC相连  IN输入低电平 COM与NO相连
void relay_init(void)//对继电器初始化，即对PB7口初始化
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);
}          
 
void relay_on(void)//PB7置1
{
  GPIO_SetBits(GPIOB,GPIO_Pin_1);
}
 
void relay_off(void)//PB7置0
{ 
  GPIO_ResetBits(GPIOB,GPIO_Pin_1);
}
void soil_on(void)//PB7置1
{
  GPIO_SetBits(GPIOB,GPIO_Pin_9);
}
 
void soil_off(void)//PB7置0
{ 
  GPIO_ResetBits(GPIOB,GPIO_Pin_9);
}


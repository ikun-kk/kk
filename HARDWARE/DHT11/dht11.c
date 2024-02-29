#include "dht11.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"
//DHT11初始化函数
//DHT11 DATA---PB0


void Dht11_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//开B组时钟和AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOF ,ENABLE);	
	//初始化IO口
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;					//选定管脚0
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;			//配置为开漏输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;			//输出速度为2MHZ
	GPIO_Init( GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;			//开漏，这样不用去切换输出输入方向
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStruct);
	
	GPIO_WriteBit(GPIOB,  GPIO_Pin_0, Bit_SET);
	
							//延时1s越过不稳定状态
	
}





/*
 * 函数名：DHT11_Mode_IPU
 * 描述  ：使DHT11-DATA引脚变为上拉输入模式
 * 输入  ：无
 * 输出  ：无
 */
static void DHT11_Mode_IPU(void)
{
 	  GPIO_InitTypeDef GPIO_InitStructure;
	  	/*选择要控制的DHT11_PORT引脚*/	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	   /*设置引脚模式为浮空输入模式*/ 
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; 
	  /*调用库函数，初始化DHT11_PORT*/
	  GPIO_Init(GPIOB, &GPIO_InitStructure);	 
}
/*
 * 函数名：DHT11_Mode_Out_PP
 * 描述  ：使DHT11-DATA引脚变为推挽输出模式
 * 输入  ：无
 * 输出  ：无
 */
static void DHT11_Mode_Out_PP(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;

	 	/*选择要控制的DHT11_PORT引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	

	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/*调用库函数，初始化DHT11_PORT*/
  	GPIO_Init(GPIOB, &GPIO_InitStructure);	 	 
}




//MCU发送开始信号函数
void Mcu_Send_Start_Sign(void)
{
  DHT11_Mode_Out_PP();
	GPIO_WriteBit(GPIOB,  GPIO_Pin_0, Bit_RESET);	
	delay_ms(18);	//拉低总线18ms以上
	GPIO_WriteBit(GPIOB,  GPIO_Pin_0, Bit_SET);		//拉高总线等待DHT11响应
  DHT11_Mode_IPU();
}

//等待DHT11响应函数
//返回值  1---响应失败   0-----响应成功
u8 Dht11_Ack(void)
{
	u16 i = 0;
	while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 1)
	{
		i++;
		delay_us(1);
		if(i>50)
		{
			return 1;			//Dht11响应失败
		}
	}
	//代码运行到这里代表DHT11响应成功
	while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0);//跳过DHT11响应时间
	while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 1);//跳过DHT11测量时间

	return 0;					//DHT11响应成功
}

//读DHT11测到的数据

u8 Read_Dht11_Data(void)
{
	u8 i,data = 0;
	for(i=0;i<8;i++)
	{
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0);//跳过数据的1bit低电平开始位
		delay_us(40);
//		data = data<<1;
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 1)		//数据1
		{
            while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 1);//跳过数据'1'的剩余高电平时间
//			data |= 1;
            data|=(uint8_t)(0x01<<(7-i));
		}
    else{
            data&=(uint8_t)~(0x01<<(7-i)); //把第7-i位置0，MSB先行
        }
		
	}
	
	return data;
}

u8 Mcu_Control_Dht11(u8 *p)
{
	u8 ret;
	Mcu_Send_Start_Sign();			//发送开始信号
	ret = Dht11_Ack();
	if(ret)
	{
		return 1;					//测量失败
	}

	
	p[0] = Read_Dht11_Data();	//湿度整数数据
	p[1] = Read_Dht11_Data();	//湿度小数数据
	p[2] = Read_Dht11_Data();	//温度整数数据
	p[3] = Read_Dht11_Data();	//温度小数数据
	p[4] = Read_Dht11_Data();   //8bit校验和
	if(p[0]+p[1]+p[2]+p[3] != p[4])
	{
		return 1;					//测量失败
	}
	
	return 0;						//测量成功
}


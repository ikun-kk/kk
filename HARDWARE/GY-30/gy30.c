#include "GY30.h"
 #include "FreeRTOS.h"
#include <task.h>
/*

	在访问I2C设备前，请先调用 i2c_CheckDevice() 检测I2C设备是否正常，该函数会配置GPIO

*/
static void I2C_GPIO_Config(void);

/*
*********************************************************************************************************

功能说明: I2C总线位延迟，最快400KHz

*********************************************************************************************************
*/
static void i2c_Delay(void)
{
	uint8_t i;

	/*　
	 	下面的时间是通过逻辑分析仪测试得到的。
    工作条件：CPU主频72MHz ，MDK编译环境，1级优化
  
		循环次数为10时，SCL频率 = 205KHz 
		循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
	 	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 
	*/
	for (i = 0; i < 10; i++);
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线启动信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Start(void)
{
	/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
	SDA_H;
	SCL_H;
	i2c_Delay();
	SDA_L;
	i2c_Delay();
	SCL_L;
	i2c_Delay();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Stop
*	功能说明: CPU发起I2C总线停止信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Stop(void)
{
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
	SDA_L;
	SCL_H;
	i2c_Delay();
	SDA_H;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_SendByte
*	功能说明: CPU向I2C总线设备发送8bit数据
*	形    参：_ucByte ： 等待发送的字节
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* 先发送字节的高位bit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			SDA_H;
		}
		else
		{
			SDA_L;
		}
		i2c_Delay();
		SCL_H;
		i2c_Delay();	
		SCL_L;
		if (i == 7)
		{
			 SDA_H; // 释放总线
		}
		_ucByte <<= 1;	/* 左移一个bit */
		i2c_Delay();
	}
}

/*
*********************************************************************************************************
*	函 数 名: i2c_ReadByte
*	功能说明: CPU从I2C总线设备读取8bit数据
*	形    参：无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* 读到第1个bit为数据的bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		SCL_H;
		i2c_Delay();
		if (SDA_read)
		{
			value++;
		}
		SCL_L;
		i2c_Delay();
	}
	return value;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_WaitAck
*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参：无
*	返 回 值: 返回0表示正确应答，1表示无器件响应
*********************************************************************************************************
*/
uint8_t i2c_WaitAck(void)
{
	uint8_t re;

	SDA_H;	/* CPU释放SDA总线 */
	i2c_Delay();
	SCL_H;	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	i2c_Delay();
	if (SDA_read)	/* CPU读取SDA口线状态 */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	SCL_L;
	i2c_Delay();
	return re;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Ack
*	功能说明: CPU产生一个ACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Ack(void)
{
	SDA_L;	/* CPU驱动SDA = 0 */
	i2c_Delay();
	SCL_H;	/* CPU产生1个时钟 */
	i2c_Delay();
	SCL_L;
	i2c_Delay();
	SDA_H;	/* CPU释放SDA总线 */
}

/*
*********************************************************************************************************
*	函 数 名: i2c_NAck
*	功能说明: CPU产生1个NACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_NAck(void)
{
	SDA_H;	/* CPU驱动SDA = 1 */
	i2c_Delay();
	SCL_H;	/* CPU产生1个时钟 */
	i2c_Delay();
	SCL_L;
	i2c_Delay();	
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Gpio_config
*	功能说明: 配置I2C总线的GPIO，采用模拟IO的方式实现
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void I2C_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 
	
	/*使能I2C使用的GPIO的时钟*/
	RCC_APB2PeriphClockCmd(SCL_RCC_CLOCK | SDA_RCC_CLOCK,ENABLE);
	
	 /* I2C时钟、I2C数据*/
  GPIO_InitStructure.GPIO_Pin = SCL_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	       // 开漏输出
  GPIO_Init(SCL_PORT, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = SDA_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	       // 开漏输出
  GPIO_Init(SDA_PORT, &GPIO_InitStructure); 

	/* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
	i2c_Stop();	
}

/*
*********************************************************************************************************
*	函 数 名: i2c_CheckDevice
*	功能说明: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*	形    参：_Address：设备的I2C总线地址
*	返 回 值: 返回值 0 表示正确， 返回1表示未探测到
*********************************************************************************************************
*/
uint8_t i2c_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;

	I2C_GPIO_Config();		/* 配置GPIO */

	
	i2c_Start();		/* 发送启动信号 */

	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	i2c_SendByte(_Address | I2C_WR);
	ucAck = i2c_WaitAck();	/* 检测设备的ACK应答 */
	i2c_Stop();			/* 发送停止信号 */

	return ucAck;
}
//BH1750数据发送
void BH_send(u8 send)
{
	do{
	i2c_Start();
	i2c_SendByte( SlaveAddress );
	}while( i2c_WaitAck() );
	i2c_SendByte(send);
	i2c_WaitAck();
	i2c_Stop();
}

//BH1750数据读取

uint16_t BH_Read()
{
	uint16_t GQ;
	i2c_Start();
	i2c_SendByte( 0x47 );
	while(i2c_WaitAck());
	GQ = i2c_ReadByte();
	GQ = GQ << 8;
	i2c_Ack();
	GQ += 0x00ff & i2c_ReadByte();      //读取并保存低八位数据
	i2c_NAck();
	i2c_Stop();
	return GQ;
}

//BH1750初始化
void Init_BH1750(void)
{
	//启动BH1750
	BH_send(0x01);
	printf("BH1750启动成功\n");
	//清除BH1750的寄存器
	BH_send(0x07);
	printf("BH1750清除成功\n");
}


//BH1750检测
void BH_Test(void)
{
	if(i2c_CheckDevice(0x46) == 1)
  	{
				printf("BH1750异常\n");
    }
	else
    {
        printf("BH1750正常\n");
    }
}

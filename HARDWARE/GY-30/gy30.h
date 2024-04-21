#ifndef __GY30_H
#define __GY30_H
#include "delay.h"
#include "sys.h"
#include "stm32f10x.h"
#define I2C_WR	0		/* 写控制bit */
#define I2C_RD	1		/* 读控制bit */

/* 这里定义的是I2C的GPIO,通过更改可更换GPIO */
//I2C时钟线
#define		SCL_PORT			GPIOB
#define		SCL_PIN				GPIO_Pin_11
#define 	SCL_H         GPIO_SetBits(SCL_PORT, SCL_PIN)		/* SCL = 1 */
#define 	SCL_L         GPIO_ResetBits(SCL_PORT, SCL_PIN)		/* SCL = 0 */
//I2C数据线
#define		SDA_PORT			GPIOB
#define		SDA_PIN				GPIO_Pin_10
#define 	SDA_H         GPIO_SetBits(SDA_PORT, SDA_PIN)		/* SDA = 1 */
#define 	SDA_L         GPIO_ResetBits(SDA_PORT, SDA_PIN)		/* SDA = 0 */
#define 	SDA_read      GPIO_ReadInputDataBit(SDA_PORT, SDA_PIN)	/* 读SDA口线状态 */

/////////////
#define 	SCL_RCC_CLOCK  RCC_APB2Periph_GPIOB
#define		SDA_RCC_CLOCK  RCC_APB2Periph_GPIOB


#define 	SCL_read      SCL_PORT->IDR  & SCL_PIN

void i2c_Start(void);
void i2c_Stop(void);
void i2c_SendByte(uint8_t _ucByte);
uint8_t i2c_ReadByte(void);
uint8_t i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);
uint8_t i2c_CheckDevice(uint8_t _Address);
/*************************************************************/
 
#define SlaveAddress     0x46      //从机地址+最后写方向位
#define BHAddRead      0x47      //从机地址+最后读方向位
#define BHPowDown      0x00      //关闭模块
#define BHPowOn        0x01      //打开模块等待测量指令
#define BHReset        0x07      //重置数据寄存器值在PowerOn模式下有效
#define BHModeH1       0x10      //高分辨率 单位1lx 测量时间120ms
#define BHModeH2       0x11      //高分辨率模式2 单位0.5lx 测量时间120ms
#define BHModeL        0x13      //低分辨率 单位4lx 测量时间16ms
#define BHSigModeH     0x20      //一次高分辨率 测量 测量后模块转到 PowerDown模式
#define BHSigModeH2    0x21      //同上类似
#define BHSigModeL     0x23      // 上类似
/*************************************************************/

void Init_BH1750(void);
void BH_send(u8 send);
uint16_t BH_Read(void);
void BH_Test(void);

void Multiple_read_BH1750(void);

#endif

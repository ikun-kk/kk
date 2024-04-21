#ifndef __GY30_H
#define __GY30_H
#include "delay.h"
#include "sys.h"
#include "stm32f10x.h"
#define I2C_WR	0		/* д����bit */
#define I2C_RD	1		/* ������bit */

/* ���ﶨ�����I2C��GPIO,ͨ�����Ŀɸ���GPIO */
//I2Cʱ����
#define		SCL_PORT			GPIOB
#define		SCL_PIN				GPIO_Pin_11
#define 	SCL_H         GPIO_SetBits(SCL_PORT, SCL_PIN)		/* SCL = 1 */
#define 	SCL_L         GPIO_ResetBits(SCL_PORT, SCL_PIN)		/* SCL = 0 */
//I2C������
#define		SDA_PORT			GPIOB
#define		SDA_PIN				GPIO_Pin_10
#define 	SDA_H         GPIO_SetBits(SDA_PORT, SDA_PIN)		/* SDA = 1 */
#define 	SDA_L         GPIO_ResetBits(SDA_PORT, SDA_PIN)		/* SDA = 0 */
#define 	SDA_read      GPIO_ReadInputDataBit(SDA_PORT, SDA_PIN)	/* ��SDA����״̬ */

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
 
#define SlaveAddress     0x46      //�ӻ���ַ+���д����λ
#define BHAddRead      0x47      //�ӻ���ַ+��������λ
#define BHPowDown      0x00      //�ر�ģ��
#define BHPowOn        0x01      //��ģ��ȴ�����ָ��
#define BHReset        0x07      //�������ݼĴ���ֵ��PowerOnģʽ����Ч
#define BHModeH1       0x10      //�߷ֱ��� ��λ1lx ����ʱ��120ms
#define BHModeH2       0x11      //�߷ֱ���ģʽ2 ��λ0.5lx ����ʱ��120ms
#define BHModeL        0x13      //�ͷֱ��� ��λ4lx ����ʱ��16ms
#define BHSigModeH     0x20      //һ�θ߷ֱ��� ���� ������ģ��ת�� PowerDownģʽ
#define BHSigModeH2    0x21      //ͬ������
#define BHSigModeL     0x23      // ������
/*************************************************************/

void Init_BH1750(void);
void BH_send(u8 send);
uint16_t BH_Read(void);
void BH_Test(void);

void Multiple_read_BH1750(void);

#endif

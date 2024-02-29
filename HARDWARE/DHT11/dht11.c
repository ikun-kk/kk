#include "dht11.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"
//DHT11��ʼ������
//DHT11 DATA---PB0


void Dht11_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//��B��ʱ�Ӻ�AFIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOF ,ENABLE);	
	//��ʼ��IO��
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;					//ѡ���ܽ�0
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;			//����Ϊ��©���
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;			//����ٶ�Ϊ2MHZ
	GPIO_Init( GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;			//��©����������ȥ�л�������뷽��
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStruct);
	
	GPIO_WriteBit(GPIOB,  GPIO_Pin_0, Bit_SET);
	
							//��ʱ1sԽ�����ȶ�״̬
	
}





/*
 * ��������DHT11_Mode_IPU
 * ����  ��ʹDHT11-DATA���ű�Ϊ��������ģʽ
 * ����  ����
 * ���  ����
 */
static void DHT11_Mode_IPU(void)
{
 	  GPIO_InitTypeDef GPIO_InitStructure;
	  	/*ѡ��Ҫ���Ƶ�DHT11_PORT����*/	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	   /*��������ģʽΪ��������ģʽ*/ 
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; 
	  /*���ÿ⺯������ʼ��DHT11_PORT*/
	  GPIO_Init(GPIOB, &GPIO_InitStructure);	 
}
/*
 * ��������DHT11_Mode_Out_PP
 * ����  ��ʹDHT11-DATA���ű�Ϊ�������ģʽ
 * ����  ����
 * ���  ����
 */
static void DHT11_Mode_Out_PP(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;

	 	/*ѡ��Ҫ���Ƶ�DHT11_PORT����*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	

	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/*���ÿ⺯������ʼ��DHT11_PORT*/
  	GPIO_Init(GPIOB, &GPIO_InitStructure);	 	 
}




//MCU���Ϳ�ʼ�źź���
void Mcu_Send_Start_Sign(void)
{
  DHT11_Mode_Out_PP();
	GPIO_WriteBit(GPIOB,  GPIO_Pin_0, Bit_RESET);	
	delay_ms(18);	//��������18ms����
	GPIO_WriteBit(GPIOB,  GPIO_Pin_0, Bit_SET);		//�������ߵȴ�DHT11��Ӧ
  DHT11_Mode_IPU();
}

//�ȴ�DHT11��Ӧ����
//����ֵ  1---��Ӧʧ��   0-----��Ӧ�ɹ�
u8 Dht11_Ack(void)
{
	u16 i = 0;
	while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 1)
	{
		i++;
		delay_us(1);
		if(i>50)
		{
			return 1;			//Dht11��Ӧʧ��
		}
	}
	//�������е��������DHT11��Ӧ�ɹ�
	while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0);//����DHT11��Ӧʱ��
	while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 1);//����DHT11����ʱ��

	return 0;					//DHT11��Ӧ�ɹ�
}

//��DHT11�⵽������

u8 Read_Dht11_Data(void)
{
	u8 i,data = 0;
	for(i=0;i<8;i++)
	{
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0);//�������ݵ�1bit�͵�ƽ��ʼλ
		delay_us(40);
//		data = data<<1;
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 1)		//����1
		{
            while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 1);//��������'1'��ʣ��ߵ�ƽʱ��
//			data |= 1;
            data|=(uint8_t)(0x01<<(7-i));
		}
    else{
            data&=(uint8_t)~(0x01<<(7-i)); //�ѵ�7-iλ��0��MSB����
        }
		
	}
	
	return data;
}

u8 Mcu_Control_Dht11(u8 *p)
{
	u8 ret;
	Mcu_Send_Start_Sign();			//���Ϳ�ʼ�ź�
	ret = Dht11_Ack();
	if(ret)
	{
		return 1;					//����ʧ��
	}

	
	p[0] = Read_Dht11_Data();	//ʪ����������
	p[1] = Read_Dht11_Data();	//ʪ��С������
	p[2] = Read_Dht11_Data();	//�¶���������
	p[3] = Read_Dht11_Data();	//�¶�С������
	p[4] = Read_Dht11_Data();   //8bitУ���
	if(p[0]+p[1]+p[2]+p[3] != p[4])
	{
		return 1;					//����ʧ��
	}
	
	return 0;						//�����ɹ�
}


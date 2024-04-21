/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   FreeRTOS v9.0.0 + STM32 ��̬����������
  *********************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� STM32ȫϵ�п����� 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  **********************************************************************
  */ 
 
/*
*************************************************************************
*                             ������ͷ�ļ�
*************************************************************************
*/ 
/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
/* ������Ӳ��bspͷ�ļ� */
#include "led.h"
#include "usart.h"
#include "dht11.h"
#include "delay.h"
#include "relays.h"
#include "key.h"
#include "YL-69.h"
#include "cJSON.h"
#include "esp8266.h"
#include "MqttKit.h"
#include "onenet.h"
#include "gy30.h"
u8 p[5];/*dht11����*/
/*����ʪ��*/
float temp,light;
char text[256];
int shidu;
int sgp30_dat, CO2Data;
int  b;
int light1;
/**************************** ������ ********************************/
/* 
 * ��������һ��ָ�룬����ָ��һ�����񣬵����񴴽���֮�����;�����һ��������
 * �Ժ�����Ҫ��������������Ҫͨ�������������������������������Լ�����ô
 * ����������ΪNULL��
 */
 /* ���������� */
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* QT������ */
static TaskHandle_t QT_Task_Handle = NULL;
/* autowater������ */
static TaskHandle_t autowater_Task_Handle = NULL;
/* DHTģ�������� */
static TaskHandle_t DHT_Task_Handle = NULL;
/* �̵���ģ�������� */
static TaskHandle_t Light_Task_Handle = NULL;
/* wifiģ�������� */
static TaskHandle_t wifi_Task_Handle = NULL;
/* ����ʪ�������� */
static TaskHandle_t shidu_Task_Handle = NULL;
/* CO2������ */
static TaskHandle_t CO2_Task_Handle = NULL;


/********************************** �ں˶����� *********************************/
/*
 * �ź�������Ϣ���У��¼���־�飬�����ʱ����Щ�������ں˵Ķ���Ҫ��ʹ����Щ�ں�
 * ���󣬱����ȴ����������ɹ�֮��᷵��һ����Ӧ�ľ����ʵ���Ͼ���һ��ָ�룬������
 * �ǾͿ���ͨ��������������Щ�ں˶���
 *
 * �ں˶���˵���˾���һ��ȫ�ֵ����ݽṹ��ͨ����Щ���ݽṹ���ǿ���ʵ��������ͨ�ţ�
 * �������¼�ͬ���ȸ��ֹ��ܡ�������Щ���ܵ�ʵ��������ͨ��������Щ�ں˶���ĺ���
 * ����ɵ�
 * 
 */


/******************************* ȫ�ֱ������� ************************************/
/*
 * ��������дӦ�ó����ʱ�򣬿�����Ҫ�õ�һЩȫ�ֱ�����
 */


/*
*************************************************************************
*                             ��������
*************************************************************************
*/
static void AppTaskCreate(void);/* ���ڴ������� */

static void QT_Task(void* pvParameters);/* QT���ڽ�������ʵ�� */
static void autowater_Task(void* pvParameters);/* �Զ���ˮ����ʵ�� */
static void DHT_Task(void* pvParameters);/* DHT11����ʵ�� */
static void Light_Task(void* pvParameters);/* �̵�������ʵ�� */
static void wifi_Task(void* pvParameters);/* wifi����ʵ�� */
static void shidu_Task(void* pvParameters);/* wifi����ʵ�� */
static void CO2_Task(void* pvParameters);/* CO2����ʵ�� */
static void BSP_Init(void);/* ���ڳ�ʼ�����������Դ */

/*****************************************************************
  * @brief  ������
  * @param  ��
  * @retval ��
  * @note   ��һ����������Ӳ����ʼ�� 
            �ڶ���������APPӦ������
            ������������FreeRTOS����ʼ���������
  ****************************************************************/
int main(void)
{	SysTick_Init(72);
  BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  /* ������Ӳ����ʼ�� */
  BSP_Init();
   /* ����AppTaskCreate���� */
  xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* ������ں��� */
                        (const char*    )"AppTaskCreate",/* �������� */
                        (uint16_t       )1024,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )9, /* ��������ȼ� */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* ������ƿ�ָ�� */ 

  if(pdPASS == xReturn)
    vTaskStartScheduler();   /* �������񣬿������� */
  else
    return -1;   			  
  while(1);   /* ��������ִ�е����� */    
}


/***********************************************************************
  * @ ������  �� AppTaskCreate
  * @ ����˵���� Ϊ�˷���������е����񴴽����������������������
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  **********************************************************************/
static void AppTaskCreate(void)
{
  BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  taskENTER_CRITICAL();           //�����ٽ���
  /* ����LED1_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )QT_Task, /* ������ں��� */
                        (const char*    )"QT_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )7,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&QT_Task_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
    printf("����QT_Task����ɹ�!\r\n");
	/* ����LED2_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )autowater_Task, /* ������ں��� */
                        (const char*    )"autowater_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )6,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&autowater_Task_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
    printf("����autowater_Task����ɹ�!\r\n");
  /* ����DHT_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )DHT_Task, /* ������ں��� */
                        (const char*    )"DHT_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )5,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&DHT_Task_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
    printf("����DHT_Task����ɹ�!\r\n");
	  /* ����CO2_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )CO2_Task, /* ������ں��� */
                        (const char*    )"CO2_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )4,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&CO2_Task_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
    printf("����CO2_Task����ɹ�!\r\n");
	  /* ����Relays_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )Light_Task, /* ������ں��� */
                        (const char*    )"Light_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )3,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&Light_Task_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
    printf("����Light_Task����ɹ�!\r\n");
	  /* ����wifi���� */
  xReturn = xTaskCreate((TaskFunction_t )wifi_Task, /* ������ں��� */
                        (const char*    )"wifi_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )2,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&wifi_Task_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
    printf("����wifi����ɹ�!\r\n");	
	xReturn = xTaskCreate((TaskFunction_t )shidu_Task, /* ������ں��� */
                        (const char*    )"shidu_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )2,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&shidu_Task_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
    printf("����shidu����ɹ�!\r\n");	
	
  vTaskDelete(AppTaskCreate_Handle); //ɾ��AppTaskCreate����
  
  taskEXIT_CRITICAL();            //�˳��ٽ���
}



/**********************************************************************
  * @ ������  �� LED_Task
  * @ ����˵���� LED_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void QT_Task(void* parameter)
{		
    while (1)
    {	vTaskDelay(500);
    	if(USART_RX_STA&0x8000)
			{			   		
				if(USART_RX_BUF[0] == 0x00)
				{
						relay_on();
				}
				else if(USART_RX_BUF[0] == 0x01)
				{relay_off();}
				else if(USART_RX_BUF[0] == 0x02)
				{	LED0=0;}
				else if(USART_RX_BUF[0] == 0x03)
				{	LED0=1;}
				USART_RX_STA=0;
			}		
    }
}

/**********************************************************************
  * @ ������  �� LED_Task
  * @ ����˵���� LED_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void autowater_Task(void* parameter)
{		vTaskDelay(500);
    while (1)
    {	
			vTaskDelay(1000);
			b=YL69DO();
			if(b==1)
			{	
				soil_on();
			}
			else
			{
				soil_off();
			}
			
    }
}
static void DHT_Task(void* parameter)
{		
	 
    while (1)
    {	vTaskDelay(1000);
			vTaskDelay(1000);
      Mcu_Control_Dht11(p);	
//		printf("��ǰ�¶ȣ�%d ��ǰʪ�ȣ�%d ����ʪ�ȣ�%d\r\n",p[2],p[0],shidu);
//    printf("#J%dMK%dM\r\n",p[2],p[0]);
    }
}
static void CO2_Task(void* parameter)
{		
	  
    while (1)
    {	vTaskDelay(1000);
			SGP30_Write(0x20,0x08);
			sgp30_dat = SGP30_Read();//��ȡSGP30��ֵ	
			CO2Data = (sgp30_dat & 0xffff0000) >> 16;
    }
}
static void Light_Task(void* parameter)
{		
	  
    while (1)
    {	vTaskDelay(1000);
			light= BH_Read()/1.2;
			light1=BH_Read()/1.2;
    }
}
static void shidu_Task(void* parameter)
{
		while(1)
		{
			vTaskDelay(1000);
		 shidu=Get_Adc_Average(1,10);
		}

}
static void wifi_Task(void* parameter)
{	  vTaskDelay(100);
		ESP8266_Init();	
    const char *topics[] = {"username","abc"};
    unsigned short timeCount = 0;		
    unsigned char *dataPtr = NULL;
    while(OneNet_DevLink())			
    vTaskDelay(500);
    OneNet_Subscribe(topics, 2);
	  while(1)
	 {	
		    if(++timeCount >= 500)								
			{ //UsartPrintf(USART_DEBUG, "EMQX_Publish\r\n");
				sprintf(text,"{\"temp\":%d,\"humi\":%d,\"shidu\":%d,\"Light\":%.2f,\"CO2\":%d}",p[2],p[0],shidu,light,CO2Data);
				printf("AB%dCD%dEF%dGH%dIJ%dK\r\n",p[2],p[0],shidu,light1,CO2Data);
				//printf("temp:%d,humi:%d,shidu:%d,Light:%.2f,CO2:%d",p[2],p[0],shidu,light,CO2Data);
				OneNet_Publish("username",text);
				memset(text, 0, sizeof(text));
			  timeCount = 0;
			  ESP8266_Clear();
			}		
				dataPtr = ESP8266_GetIPD(3);
				if(dataPtr != NULL)
				OneNet_RevPro(dataPtr);
				vTaskDelay(10);
	}
}
/***********************************************************************
  * @ ������  �� BSP_Init
  * @ ����˵���� �弶�����ʼ�������а����ϵĳ�ʼ�����ɷ��������������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  *********************************************************************/
static void BSP_Init(void)
{
	/*
	 * STM32�ж����ȼ�����Ϊ4����4bit��������ʾ��ռ���ȼ�����ΧΪ��0~15
	 * ���ȼ�����ֻ��Ҫ����һ�μ��ɣ��Ժ������������������Ҫ�õ��жϣ�
	 * ��ͳһ��������ȼ����飬ǧ��Ҫ�ٷ��飬�мɡ�
	 */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	/* LED ��ʼ�� */
	LED_Init();
	/* ���ڳ�ʼ��	*/
	uart_init(115200);
	Usart2_Init(115200);
	/* ����ģ���ʼ��	*/
  relay_init();
	Dht11_Init();
	KEY_Init();
	Adc_Init();

	BH_Test();
	Init_BH1750();
	BH_send(0x11);//���ø߷ֱ���ģʽ
	
	SGP30_Init();
	
	
}
/********************************END OF FILE****************************/

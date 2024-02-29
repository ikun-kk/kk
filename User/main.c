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
#include "gy30.h"
u8 p[5];/*dht11����*/
/*����ʪ��*/
float temp;
char text[256];
int shidu;
int sgp30_dat, CO2Data;
/**************************** ������ ********************************/
/* 
 * ��������һ��ָ�룬����ָ��һ�����񣬵����񴴽���֮�����;�����һ��������
 * �Ժ�����Ҫ��������������Ҫͨ�������������������������������Լ�����ô
 * ����������ΪNULL��
 */
 /* ���������� */
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* LED1������ */
static TaskHandle_t LED1_Task_Handle = NULL;
/* LED2������ */
static TaskHandle_t LED2_Task_Handle = NULL;
/* DHTģ�������� */
static TaskHandle_t DHT_Task_Handle = NULL;
/* �̵���ģ�������� */
static TaskHandle_t RELAY_Task_Handle = NULL;
/* wifiģ�������� */
static TaskHandle_t wifi_Task_Handle = NULL;
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

static void LED1_Task(void* pvParameters);/* LED1_Task����ʵ�� */
static void LED2_Task(void* pvParameters);/* LED2_Task����ʵ�� */
static void DHT_Task(void* pvParameters);/* DHT11����ʵ�� */
static void Relays_Task(void* pvParameters);/* �̵�������ʵ�� */
static void wifi_Task(void* pvParameters);/* wifi����ʵ�� */
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
                        (uint16_t       )512,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )5, /* ��������ȼ� */
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
  xReturn = xTaskCreate((TaskFunction_t )LED1_Task, /* ������ں��� */
                        (const char*    )"LED1_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )2,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&LED1_Task_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
    printf("����LED1_Task����ɹ�!\r\n");
	/* ����LED2_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )LED2_Task, /* ������ں��� */
                        (const char*    )"LED2_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )2,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&LED2_Task_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
    printf("����LED2_Task����ɹ�!\r\n");
  /* ����DHT_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )DHT_Task, /* ������ں��� */
                        (const char*    )"DHT_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )3,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&DHT_Task_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
    printf("����DHT_Task����ɹ�!\r\n");
	  /* ����Relays_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )Relays_Task, /* ������ں��� */
                        (const char*    )"Relays_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )5,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&RELAY_Task_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
    printf("����Relays_Task����ɹ�!\r\n");
	  /* ����wifi���� */
  xReturn = xTaskCreate((TaskFunction_t )wifi_Task, /* ������ں��� */
                        (const char*    )"wifi_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )2,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&wifi_Task_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
    printf("����wifi����ɹ�!\r\n");	
	
	
  vTaskDelete(AppTaskCreate_Handle); //ɾ��AppTaskCreate����
  
  taskEXIT_CRITICAL();            //�˳��ٽ���
}



/**********************************************************************
  * @ ������  �� LED_Task
  * @ ����˵���� LED_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void LED1_Task(void* parameter)
{		vTaskDelay(500);
    while (1)
    {
    	if(USART_RX_STA&0x8000)
			{			   		
				if(USART_RX_BUF[0] == 0x00)
				{
						relay_off();
						vTaskDelay(500);
						relay_on();
				}
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
static void LED2_Task(void* parameter)
{		vTaskDelay(500);
    while (1)
    {
			vTaskDelay(1000);
			
    }
}
static void DHT_Task(void* parameter)
{		
	 
    while (1)
    {	vTaskDelay(1000);
			vTaskDelay(1000);
      Mcu_Control_Dht11(p);
		  shidu=Get_Adc_Average(1,10);
//		printf("��ǰ�¶ȣ�%d ��ǰʪ�ȣ�%d ����ʪ�ȣ�%d\r\n",p[2],p[0],shidu);
//    printf("#J%dMK%dM\r\n",p[2],p[0]);
    }
}

static void Relays_Task(void* parameter)
{		
	  u8 key=0;
    while (1)
    {	key=KEY_Scan(0);
			if(key==KEY_UP_PRESS)	
			{
				relay_off();
				vTaskDelay(5000);
				relay_on();
			}	
				vTaskDelay(20);
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
			{ UsartPrintf(USART_DEBUG, "EMQX_Publish\r\n");
				sprintf(text,"{\"temp\":%d,\"humi\":%d,\"shidu\":%d,\"Light intensity\":%f,\"CO2\":%d}",p[2],p[0],shidu,LIght_Intensity(),CO2Data);
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

	BH1750_Init();
	i2c_CheckDevice(BH1750_Addr);
	
	SGP30_Init();
	SGP30_Write(0x20,0x08);
	sgp30_dat = SGP30_Read();//��ȡSGP30��ֵ	
	CO2Data = (sgp30_dat & 0xffff0000) >> 16;
}
/********************************END OF FILE****************************/

#include "SWM181.h"
#include "pt.h"

#include <math.h>

/***************************************************************/
static int counter1,counter2;
static struct pt pt1, pt2;
 
static int protothread1(struct pt *pt)
{
	PT_BEGIN(pt);//�߳̿�ʼ
	
	while(1)
	{
		PT_WAIT_UNTIL(pt, counter1 == 1);//���ʱ����1�����ִ�У������¼���е㲢�˳��߳�1
		GPIO_InvBit(GPIOB, PIN8);
		counter1 = 0;
	}
	
	PT_END(pt);//�߳̽���
}
 
static int protothread2(struct pt *pt)
{
	PT_BEGIN(pt);//�߳̿�ʼ
	
	while(1)
	{
		PT_WAIT_UNTIL(pt, counter2 == 5);//���ʱ����5�����ִ�У������¼���е㲢�˳��߳�2
		GPIO_InvBit(GPIOB, PIN9);
		counter2 = 0;
	}
	
	PT_END(pt);//�߳̽���
}

/***************************************************************/

void SerialInit(void);

int main(void)
{
 	uint32_t i;
 	
 	SystemInit();
	
	SerialInit();
	
	PT_INIT(&pt1);
	PT_INIT(&pt2);
	
	GPIO_Init(GPIOB, PIN8, 1, 0, 0, 0);			//GPIOA.4����Ϊ������ţ��������
	GPIO_Init(GPIOB, PIN9, 1, 0, 0, 0);			//GPIOA.5����Ϊ������ţ��������
	GPIO_ClrBit(GPIOA, PIN4);
 	GPIO_SetBit(GPIOA, PIN5);
//����ProtoThread��SWM181����   
 	while(1==1)
 	{
		protothread1(&pt1);
		protothread2(&pt2);
 		for(i=0; i<5000000; i++) i=i;
		
		counter1++;
		counter2++;
  		printf("Hi, World!\r\n");
		printf("this is the ProtoThrea test!\r\n");
		printf("2023-8-21 16:59:51\r\n");
 	}
}

void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTA, PIN0, FUNMUX_UART0_RXD, 1);	//GPIOA.0����ΪUART0��������
	PORT_Init(PORTA, PIN1, FUNMUX_UART0_TXD, 0);	//GPIOA.1����ΪUART0�������
 	
 	UART_initStruct.Baudrate = 115200;
	UART_initStruct.DataBits = UART_DATA_8BIT;
	UART_initStruct.Parity = UART_PARITY_NONE;
	UART_initStruct.StopBits = UART_STOP_1BIT;
	UART_initStruct.RXThreshold = 3;
	UART_initStruct.RXThresholdIEn = 0;
	UART_initStruct.TXThreshold = 3;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutTime = 10;
	UART_initStruct.TimeoutIEn = 0;
 	UART_Init(UART0, &UART_initStruct);
	UART_Open(UART0);
}

/****************************************************************************************************************************************** 
* ��������: fputc()
* ����˵��: printf()ʹ�ô˺������ʵ�ʵĴ��ڴ�ӡ����
* ��    ��: int ch		Ҫ��ӡ���ַ�
*			FILE *f		�ļ����
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
int fputc(int ch, FILE *f)
{
	UART_WriteByte(UART0, ch);
	
	while(UART_IsTXBusy(UART0));
 	
	return ch;
}

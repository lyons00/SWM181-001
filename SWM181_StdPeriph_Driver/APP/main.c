#include "SWM181.h"
#include "pt.h"

#include <math.h>

/***************************************************************/
static int counter1,counter2;
static struct pt pt1, pt2;
 
static int protothread1(struct pt *pt)
{
	PT_BEGIN(pt);//线程开始
	
	while(1)
	{
		PT_WAIT_UNTIL(pt, counter1 == 1);//如果时间满1秒继续执行，否则记录运行点并退出线程1
		GPIO_InvBit(GPIOB, PIN8);
		counter1 = 0;
	}
	
	PT_END(pt);//线程结束
}
 
static int protothread2(struct pt *pt)
{
	PT_BEGIN(pt);//线程开始
	
	while(1)
	{
		PT_WAIT_UNTIL(pt, counter2 == 5);//如果时间满5秒继续执行，否则记录运行点并退出线程2
		GPIO_InvBit(GPIOB, PIN9);
		counter2 = 0;
	}
	
	PT_END(pt);//线程结束
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
	
	GPIO_Init(GPIOB, PIN8, 1, 0, 0, 0);			//GPIOA.4配置为输出引脚，推挽输出
	GPIO_Init(GPIOB, PIN9, 1, 0, 0, 0);			//GPIOA.5配置为输出引脚，推挽输出
	GPIO_ClrBit(GPIOA, PIN4);
 	GPIO_SetBit(GPIOA, PIN5);
//测试ProtoThread在SWM181运行   
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
	
	PORT_Init(PORTA, PIN0, FUNMUX_UART0_RXD, 1);	//GPIOA.0配置为UART0输入引脚
	PORT_Init(PORTA, PIN1, FUNMUX_UART0_TXD, 0);	//GPIOA.1配置为UART0输出引脚
 	
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
* 函数名称: fputc()
* 功能说明: printf()使用此函数完成实际的串口打印动作
* 输    入: int ch		要打印的字符
*			FILE *f		文件句柄
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
int fputc(int ch, FILE *f)
{
	UART_WriteByte(UART0, ch);
	
	while(UART_IsTXBusy(UART0));
 	
	return ch;
}

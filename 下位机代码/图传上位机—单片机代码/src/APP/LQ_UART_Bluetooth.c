/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�TC264DA���İ�
����    д��ZYF/chiusir
��E-mail  ��chiusir@163.com
������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
�������¡�2020��10��28��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://longqiu.taobao.com
------------------------------------------------
��dev.env.��AURIX Development Studio1.2.2�����ϰ汾
��Target �� TC264DA/TC264D
��Crystal�� 20.000Mhz
��SYS PLL�� 200MHz
________________________________________________________________
����iLLD_1_0_1_11_0�ײ����,

ʹ�����̵�ʱ�򣬽������û�пո��Ӣ��·����
����CIFΪTC264DA�����⣬�����Ĵ������TC264D
����Ĭ�ϳ�ʼ����EMEM��512K������û�ʹ��TC264D��ע�͵�EMEM_InitConfig()��ʼ��������
������\Libraries\iLLD\TC26B\Tricore\Cpu\CStart\IfxCpu_CStart0.c��164�����ҡ�
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include <stdio.h>
#include "../Driver/LQ_STM.h"
#include "../Driver/LQ_UART.h"
#include "LQ_GPIO_LED.h"

//////////////////////////////////���²���Ϊ���ܲ���/////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////




/*************************************************************************
*  �������ƣ�void Test_Bluetooth(void);
*  ����˵����UART���Ժ���
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��
*************************************************************************/
void Test_Bluetooth(void)
{
    unsigned char  cnt=0;

	UART_InitConfig(UART0_RX_P14_1,UART0_TX_P14_0, 115200);
	UART_InitConfig(UART1_RX_P02_3,UART1_TX_P02_2, 9600);
	UART_InitConfig(UART2_RX_P14_3,UART2_TX_P14_2, 19200);
	UART_InitConfig(UART3_RX_P00_1,UART3_TX_P00_0, 115200);
	while(1)
	{
		UART_PutChar(UART0, 'L');                      //���� �ֽڵ�UART��
		delayms(500);                                  //��ʱ�ȴ�
		UART_PutChar(UART0,'Q');                       //���� �ֽڵ�UART��

		UART_PutStr(UART0,"UART0 LongQiu \r\n");       //�����ַ�������λ��
		printf("UART0 printf() test,cnt= %03d \r\n",cnt++);
		UART_PutStr(UART1,"UART1 LongQiu \r\n");                //�����ַ�������λ��
		UART_PutStr(UART2,"UART2 LongQiu \r\n");                //�����ַ�������λ��
		UART_PutStr(UART3,"UART3 LongQiu \r\n");                //�����ַ�������λ��

    	LED_Ctrl(LEDALL,RVS);        //��ƽ��ת,LED��˸
		delayms(1000);              //��ʱ�ȴ�
	}
}

//

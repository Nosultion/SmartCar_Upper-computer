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

#include <IfxGtm_PinMap.h>
#include <stdio.h>

#include "../Driver/LQ_GTM.h"
#include "../Driver/LQ_STM.h"
#include "../Driver/LQ_UART.h"
#include "LQ_GPIO_LED.h"


/*************************************************************************
*  �������ƣ�void Test_GTM_ATOM_PWM(void)
*  ����˵����GTM���Ժ���
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��22��
*  ��    ע��P20_9��ΪPWM����ڣ�P15_0��ΪTIM����ڣ����߶̽Ӻ󣬴���P14.0���͵���λ��
*  Ĭ��Ƶ����125HZ��ռ�ձ�50%
*************************************************************************/
void LQ_TIM_InputCature(void)
{
	char txt[16];
	float measuredPwmFreq_Hz, measuredPwmDutyCycle;
	TIM_InitConfig(IfxGtm_TIM1_3_TIN71_P15_0_IN);//P15_0 ��Ϊ���岶��ܽ�
	TOM_PWM_InitConfig(IfxGtm_TOM0_13_TOUT65_P20_9_OUT, 5000, 125);//��ʼ��P20_9 ��ΪPWM����� Ƶ��125Hz ռ�ձ� �ٷ�֮(5000/TOM_PWM_MAX)*100
	while(1)
	{
		TIM_GetPwm(IfxGtm_TIM1_3_TIN71_P15_0_IN, &measuredPwmFreq_Hz, &measuredPwmDutyCycle);
		sprintf(txt,"\nHz:%05f;dty:%f  ", measuredPwmFreq_Hz, measuredPwmDutyCycle);

		//���ڷ��͵���λ��
		UART_PutStr(UART0,txt);//�����������λ�����磺Hz:00124;dty:00050���������������Ϊ�����
    	LED_Ctrl(LED0,RVS);        //��ƽ��ת,LED��˸
		delayms(500);              //��ʱ�ȴ�
	}
}

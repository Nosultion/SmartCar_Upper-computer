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

#include "../Driver/LQ_CCU6.h"



 /*************************************************************************
 void LQ_CCU6_Timer (void)
 *  ����˵�������Գ���
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺2020��3��10��
 *  ��    ע�����İ��ϵ�LED����˸���ж�ʱP10.5/P10.6����
 *************************************************************************/
 void LQ_CCU6_Timer (void)
  {
     CCU6_InitConfig(CCU60, CCU6_Channel0, 200000);//CCU6��ʼ��
     CCU6_InitConfig(CCU61, CCU6_Channel0, 400000);//CCU6��ʼ��
     CCU6_InitConfig(CCU60, CCU6_Channel1, 500000);//CCU6��ʼ��
     CCU6_InitConfig(CCU61, CCU6_Channel1, 1000000);//CCU6��ʼ��
#pragma warning 557         // ���ξ���
	//�жϷ������з�תLED
  	while(1)
  	{
    	//LED_Ctrl(LED0,RVS);        //��ƽ��ת,LED��˸
//		delayms(500);              //��ʱ�ȴ�
  	}
  }

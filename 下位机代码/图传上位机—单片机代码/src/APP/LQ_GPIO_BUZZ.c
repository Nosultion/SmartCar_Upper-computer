/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�TC264DA���İ�
����    д��YXLZYF/chiusir
��E-mail  ��2623387606@qq.com
������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
�������¡�2022��11��29��
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
#include "LQ_GPIO_BUZZ.h"
#include "../Driver/LQ_GPIO.h"
#include "../Driver/LQ_STM.h"

/*************************************************************************
*  �������ƣ�GPIO_BUZZ_Init(void)
*  ����˵����GPIO��ʼ������ BUZZ����P33.8��ʼ��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2022��11��29��
*  ��    ע��
*************************************************************************/
void GPIO_BUZZ_Init(void)
{
      // ��ʼ��,����ڣ��͵�ƽ
      PIN_InitConfig(BUZZ, PIN_MODE_OUTPUT, 0);
}

/*************************************************************************
*  �������ƣ�void BUZZ_Ctrl(BUZZn_e BUZZno, BUZZs_e sta)
*  ����˵����BUZZ����
*  ����˵����BUZZn_e BUZZno���,BUZZs_e sta״̬��/����/��ת
*  �������أ���
*  �޸�ʱ�䣺2022��11��29��
*  ��    ע��
*************************************************************************/
void BUZZ_Ctrl(BUZZn_e BUZZno, BUZZs_e sta)
{
    switch(BUZZno)
    {
    case BUZZ0:
      if(sta==BUZZ_ON)       PIN_Write(BUZZ,1);
      else if(sta==BUZZ_OFF) PIN_Write(BUZZ,0);
      else if(sta==BUZZ_RVS) PIN_Reverse(BUZZ);
    break;

    default:
    break;
    }
}
/*************************************************************************
*  �������ƣ�void BUZZ_Work(uint8_t OpeningRoad_Flag,uint8_t L_CircleFlag ,uint8_t R_CircleFlag,uint8_t Barrier_Flag)
*  ����˵����Ԫ�ط�������
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2022��12��1��
*  ��    ע��
*************************************************************************/
void BUZZ_Work(uint8_t *OpeningRoad_Flag, uint8_t *L_CircleFlag, uint8_t *R_CircleFlag, uint8_t *Barrier_Flag)
{


}


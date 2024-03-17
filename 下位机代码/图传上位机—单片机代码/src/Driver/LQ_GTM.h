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

#ifndef _LQ_GTMPWM_H_
#define _LQ_GTMPWM_H_

#include "IfxGtm_PinMap.h"
#include <Platform_Types.h>
#include <stdint.h>
#include <IfxGtm.h>
#include <IfxGtm_Atom_Pwm.h>
#include <IfxGtm_cfg.h>
#include <IfxGtm_Cmu.h>
#include <IfxGtm_PinMap.h>
#include <IfxGtm_reg.h>
#include <IfxGtm_Tim_In.h>
#include <IfxGtm_Tom.h>
#include <IfxGtm_Tom_Pwm.h>
#include <IfxPort.h>

// ATOM ʱ�� 100MHz
#define ATOM_PWM_CLK  100000000

// ATOM ���ռ�ձ� �������޸�
#define ATOM_PWM_MAX  10000.0f

// TOM ʱ�� 6.25MHz/2
#define TOM_PWM_CLK  6250000

// TOM ���ռ�ձ� �������޸�
#define TOM_PWM_MAX  10000.0f


////////////////ATOM_PWM//////////////////////
void ATOM_PWM_InitConfig(IfxGtm_Atom_ToutMap pin, unsigned long duty, unsigned long pwmFreq_Hz);
void ATOM_PWM_SetDuty(IfxGtm_Atom_ToutMap pin, unsigned long duty, unsigned long pwmFreq_Hz);



////////////////TOM_TIM//////////////////////
void TOM_PWM_InitConfig(IfxGtm_Tom_ToutMap pin, unsigned long duty, unsigned long pwmFreq_Hz);
void TOM_PWM_SetDuty(IfxGtm_Tom_ToutMap pin, unsigned long duty, unsigned long pwmFreq_Hz);


void TIM_InitConfig(IfxGtm_Tim_TinMap pin);
void TIM_GetPwm(IfxGtm_Tim_TinMap pin, float32 *Period, float32 *Duty);


#endif /* 0_APPSW_TRICORE_APP_LQ_GTM6PWM_H_ */

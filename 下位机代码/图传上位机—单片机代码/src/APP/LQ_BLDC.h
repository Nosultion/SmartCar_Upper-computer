/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�TC3xxxA���İ�
����    д��chiusir
��E-mail��chiusir@163.com
������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
�������¡�2020��10��28��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://longqiu.taobao.com
------------------------------------------------
��dev.env.��AURIX Development Studio1.2.2�����ϰ汾
��Target �� TC3xxxA
��Crystal�� 20.000Mhz
��SYS PLL�� 300MHz
________________________________________________________________
����iLLD_1_0_1_11_0�ײ����,
ʹ�����̵�ʱ�򣬽������û�пո��Ӣ��·����
=================================================================
����������Ƶ��ַ��https://space.bilibili.com/95313236
________________________________________________________________
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef __LQ_BLDC_H_
#define __LQ_BLDC_H_
#include "Platform_Types.h"
#include "stdint.h"


//#define TRUE 1
//#define FALSE 0

typedef struct _BLDC_MANAGER
{
  uint8 step;
  uint16 counts;
  int16_t duty;
  uint8 Electricity_flag;//�ϵ��������־
}BLDC_MANAGER, *PBLDC_MANAGER;
extern BLDC_MANAGER g_sBLDCMag;


void BLDC_MotorInit(uint16 freq);//��ˢ������ų�ʼ��
void BLDC_HallInit(void );//������ʼ��
void TestMotorBLDC (void);//��ˢ������Ժ���
void LQ_BLDCShow(void);//��ˢ����������չʾ

float PID_velocity(float error);//pid���ٽ���
void LQ_BLDCCurrent_detection(void);//������⣬������·
void LQ_BLDCCorotation(void);//��ˢ�����ת
void LQ_BLDCReversal(void);//��ˢ�����ת
void LQ_BLDCStop(void);//�ر���ˢ�����ֹͣת��
void BLDC_Motor_Hall_Run(int16_t motor_duty);//��ˢ�����������

#endif/* 0_APPSW_TRICORE_APP_LQ_ASC_H_ */

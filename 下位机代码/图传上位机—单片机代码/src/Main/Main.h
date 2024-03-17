/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�TC264DA���İ�
����    д��chiusir
��E-mail  ��chiusir@163.com
������汾��V1.0
�������¡�2020��4��10��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://longqiu.taobao.com
------------------------------------------------
��dev.env.��Hightec4.9.3/Tasking6.3�����ϰ汾
��Target �� TC264DA
��Crystal�� 20.000Mhz
��SYS PLL�� 200MHz
����iLLD_1_0_1_11_0�ײ����
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef SRC_APPSW_TRICORE_MAIN_MAIN_H_
#define SRC_APPSW_TRICORE_MAIN_MAIN_H_

#include "../../Libraries/iLLD/TC26B/Tricore/Cpu/Std/IfxCpu.h"
#include "../../Libraries/iLLD/TC26B/Tricore/Cpu/Std/Platform_Types.h"
#include "../Driver/include.h"//����ģ���ͷ�ļ�

extern App_Cpu0 g_AppCpu0; // brief CPU 0 global data
extern IfxCpu_mutexLock mutexCpu0InitIsOk;   /** CPU0 ��ʼ����ɱ�־λ  */
extern volatile char mutexCpu0TFTIsOk;       /** TFT18ʹ�ñ�־λ  */
/**
 * ��ʱ�� 5ms��50ms��־λ
 */
extern volatile uint8 cpu1Flage5ms;
extern volatile uint8 cpu1Flage50ms;

/* �����ٶ�  */
extern volatile sint16 targetSpeed;

/* ���ϱ�־λ */
extern volatile uint8 evadibleFlage;

int core0_main (void);

#endif /* SRC_APPSW_TRICORE_MAIN_MAIN_H_ */

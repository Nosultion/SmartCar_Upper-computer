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

STM��Ϊϵͳ����ȫ�ֶ�ʱ��������Ϊϵͳ�ṩ����ʱ�ӣ����������ȽϼĴ�����
�ɲ��������������жϣ�����������
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/


#include "LQ_STM.h"

#include "../APP/LQ_GPIO_LED.h"


/*************************************************************************
*  �������ƣ�
*  ����˵����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��30��
*  ��    ע��STM�ж�
*************************************************************************/
IFX_INTERRUPT(STM0_CH0_IRQHandler, STM0_VECTABNUM, STM0_CH0_PRIORITY);
IFX_INTERRUPT(STM0_CH1_IRQHandler, STM0_VECTABNUM, STM0_CH1_PRIORITY);
IFX_INTERRUPT(STM1_CH0_IRQHandler, STM1_VECTABNUM, STM1_CH0_PRIORITY);
IFX_INTERRUPT(STM1_CH1_IRQHandler, STM1_VECTABNUM, STM1_CH1_PRIORITY);

/** �ⲿ�ж�CPU��� */
const uint8 StmIrqVectabNum[2]  = {STM0_VECTABNUM, STM1_VECTABNUM};

/** �ⲿ�ж����ȼ� */
const uint8 StmIrqPriority[4]   = {STM0_CH0_PRIORITY, STM0_CH1_PRIORITY, STM1_CH0_PRIORITY, STM1_CH1_PRIORITY};

/** �ⲿ�жϷ�������ַ */
const void *StmIrqFuncPointer[4] = {&STM0_CH0_IRQHandler, &STM0_CH1_IRQHandler, &STM1_CH0_IRQHandler, &STM1_CH1_IRQHandler};


//	STM��ʼ�����ýṹ��
IfxStm_CompareConfig g_StmCompareConfig[4];

/***********************************************************************************************/
/********************************STM�ⲿ�ж�  ������******************************************/
/***********************************************************************************************/

/*************************************************************************
*  �������ƣ�void STM0_CH0_IRQHandler(void)
*  ����˵����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��30��
*  ��    ע��STM0_CH0ʹ�õ��жϷ�����
*************************************************************************/
void STM0_CH0_IRQHandler(void)
{
	/* ����CPU�ж�  �����жϲ���Ƕ�� */
	IfxCpu_enableInterrupts();

	//����жϱ�־
	IfxStm_clearCompareFlag(&MODULE_STM0, g_StmCompareConfig[0].comparator);

	//�����µ��ж����ã���ʼ�´��ж�
	IfxStm_increaseCompare(&MODULE_STM0, g_StmCompareConfig[0].comparator, g_StmCompareConfig[0].ticks);

	/* �û����� */
	LED_Ctrl(LED0,RVS);        //��ƽ��ת,LED��˸
}

/*************************************************************************
*  �������ƣ�void STM0_CH1_IRQHandler(void)
*  ����˵����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��30��
*  ��    ע��STM0_CH1ʹ�õ��жϷ�����
*************************************************************************/
void STM0_CH1_IRQHandler(void)
{
	/* ����CPU�ж�  �����жϲ���Ƕ�� */
	IfxCpu_enableInterrupts();

	//����жϱ�־
	IfxStm_clearCompareFlag(&MODULE_STM0, g_StmCompareConfig[1].comparator);

	//�����µ��ж����ã���ʼ�´��ж�
	IfxStm_increaseCompare(&MODULE_STM0, g_StmCompareConfig[1].comparator, g_StmCompareConfig[1].ticks);

	/* �û����� */
	LED_Ctrl(LED1,RVS);        //��ƽ��ת,LED��˸

}

/*************************************************************************
*  �������ƣ�void STM1_CH0_IRQHandler(void)
*  ����˵����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��30��
*  ��    ע��STM1_CH0ʹ�õ��жϷ�����
*************************************************************************/
void STM1_CH0_IRQHandler(void)
{
	/* ����CPU�ж�  �����жϲ���Ƕ�� */
	IfxCpu_enableInterrupts();

	//����жϱ�־
	IfxStm_clearCompareFlag(&MODULE_STM1, g_StmCompareConfig[2].comparator);

	//�����µ��ж����ã���ʼ�´��ж�
	IfxStm_increaseCompare(&MODULE_STM1, g_StmCompareConfig[2].comparator, g_StmCompareConfig[2].ticks);

	/* �û����� */
	LED_Ctrl(LED1,RVS);        //��ƽ��ת,LED��˸

}

/*************************************************************************
*  �������ƣ�void STM1_CH1_IRQHandler(void)
*  ����˵����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��30��
*  ��    ע��STM1_CH1ʹ�õ��жϷ�����
*************************************************************************/
void STM1_CH1_IRQHandler(void)
{
	/* ����CPU�ж�  �����жϲ���Ƕ�� */
	IfxCpu_enableInterrupts();

	//����жϱ�־
	IfxStm_clearCompareFlag(&MODULE_STM1, g_StmCompareConfig[3].comparator);

	//�����µ��ж����ã���ʼ�´��ж�
	IfxStm_increaseCompare(&MODULE_STM1, g_StmCompareConfig[3].comparator, g_StmCompareConfig[3].ticks);

	/* �û����� */

}


 /*************************************************************************
 *  �������ƣ�void STM_Init(STM_t stm, STM_Channel_t channel, uint32 us)
 *  ����˵����STM��ʼ��
 *  ����˵����
 * @param    stm      �� STM���     STM0 �� STM1
 * @param    channel  �� STMͨ��     STM_Channel_0  STM_Channel_1
 * @param    us       �� STM��ʱ�ж� ʱ��
 *  �������أ���
 *  �޸�ʱ�䣺2020��3��10��
 *  ��    ����STM_InitConfig(STM0, STM_Channel_0, 50000);  //STM0 ͨ��0��ʼ�� 50000us ����һ���ж�
 *  ��    ע���ⲿ�жϷ�������LQ_STM.c��  �ж����ȼ�������LQ_STM.h�� ���������޸�
 *************************************************************************/
 void STM_InitConfig(STM_t stm, STM_Channel_t channel, uint32 us)
 {
 	char      interruptState;
 	interruptState = disableInterrupts();

 	Ifx_STM * STM = IfxStm_getAddress((IfxStm_Index)stm);
 	unsigned char Index = (unsigned char)IfxStm_getIndex(STM) * 2 + (unsigned char)channel;

 	IfxStm_CompareConfig *stmCompareConfig = &g_StmCompareConfig[Index];

 	//���ýṹ��ʵ������
 	IfxStm_initCompareConfig(stmCompareConfig);

     //����
 	IfxStm_enableOcdsSuspend(STM);

 	//��ȡSTMģ���΢���ʱ tick ��ֵ
 	sint32 ticks = IfxStm_getTicksFromMicroseconds((Ifx_STM *)STM, us);

 	//�޸�ticks��ֵ��ʹ�ܴ������
 	stmCompareConfig->ticks = ticks;
 	stmCompareConfig->comparator           = (IfxStm_Comparator)channel;
 	stmCompareConfig->comparatorInterrupt  = (IfxStm_ComparatorInterrupt)channel; //�ж�ѡ��
 	stmCompareConfig->triggerPriority      = StmIrqPriority[Index];               //�ж����ȼ�����
 	stmCompareConfig->typeOfService        = StmIrqVectabNum[stm];                //�����ں�

 	//�ȽϹ��ܳ�ʼ��
 	IfxStm_initCompare(STM, stmCompareConfig);
 	IfxCpu_Irq_installInterruptHandler((void*)StmIrqFuncPointer[Index], StmIrqPriority[Index]);//�����жϺ������жϺ�

 	restoreInterrupts(interruptState);
 }


 /*************************************************************************
 *  �������ƣ�void STM_DelayUs(STM_t stm, uint32 us)
 *  ����˵����STM��ʱ
 *  ����˵����
 * @param    stm      �� STM���     STM0 �� STM1
 * @param    us       �� STM��ʱ     ʱ�� us
 *  �������أ���
 *  �޸�ʱ�䣺2020��3��10��
 *  ��    ����STM_DelayUs(STM0, 5000);  //��ʱ5000us
 *  ��    ע��ʹ����ʱ����ʱ����ȷ����ӦSTMģ����ù�STM_Init����
 *************************************************************************/
 void STM_DelayUs(STM_t stm, uint32 us)
 {
 	Ifx_STM * STM = IfxStm_getAddress((IfxStm_Index)stm);
 	uint32 tick = IfxStm_getTicksFromMicroseconds(STM, us);

 	IfxStm_waitTicks(STM, tick);

 }

 /*************************************************************************
 *  �������ƣ�void delayms(unsigned short stmms)
 *  ����˵������ʱ����
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺2020��3��10��
 *  ��    ע��delayms(1);��ʱ1 ms
 *************************************************************************/
 void delayms(unsigned short stmms)	//
 {
#if 1
	while(stmms--)
	{
	     STM_DelayUs(STM0, 1000);
	}
#else   //����ȷ��ʱ
	volatile unsigned long i = 0;
	while(stmms--)
	{
		for (i = 0; i < 16580; ++i)
		{
			__asm("NOP"); /* delay */
		}
	}
#endif
 }

 /*************************************************************************
 *  �������ƣ�void delayus(unsigned short us)
 *  ����˵������ʱ����
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺2020��3��10��
 *  ��    ע��delayus(1);��ʱ1 us
 *************************************************************************/
 void delayus(unsigned short us)
 {
#if 0

	 STM_DelayUs(STM0, us);

#else   //����ȷ��ʱ
	volatile unsigned long i = 0;
	while(us--)
	{
		for (i = 0; i < 16; ++i)
		{
			__asm("NOP"); /* delay */
		}
	}
#endif

 }
 /*************************************************************************
 *  �������ƣ�uint32  STM_GetNowUs(STM_t stm)
 *  ����˵����STM��ȡ��ǰʱ��
 *  ����˵����
 * @param    stm      �� STM���     STM0 �� STM1
 * @param    us       �� STM��ʱ     ʱ�� us
 *  �������أ���
 *  �޸�ʱ�䣺2020��3��10��
 *  ��    ����int32_t NowTime =  STM_GetNowUs(STM0);  //��ȡSTM0 ��ǰʱ��
 *  ��    ע��ʹ����ʱ����ʱ����ȷ����ӦSTMģ����ù�STM_Init����
 *************************************************************************/
 uint32  STM_GetNowUs(STM_t stm)
 {
 	Ifx_STM * STM = IfxStm_getAddress((IfxStm_Index)stm);
 	sint32 freq = (sint32)IfxStm_getFrequency(STM)/1000000;
 	Ifx_TickTime stmNow;
 	char      interruptState;

 	interruptState = disableInterrupts();
 	stmNow         = (Ifx_TickTime)IfxStm_get(STM) & TIME_INFINITE;
 	restoreInterrupts(interruptState);

 	return (unsigned long)(stmNow/freq);
 }


 /*************************************************************************
 *  �������ƣ�STM_DisableInterrupt(STM_t stm, STM_Channel_t channel)
 *  ����˵����STMֹͣ�ж�
 *  ����˵����
 * @param    stm      �� STM���     STM0 �� STM1
 * @param    channel  �� STMͨ��     STM_Channel_0  STM_Channel_1
 *  �������أ���
 *  �޸�ʱ�䣺2020��5��7��
 *  ��    ����
 *  ��    ע��
 *************************************************************************/
void STM_DisableInterrupt(STM_t stm, STM_Channel_t channel)
{
	Ifx_STM * STM = IfxStm_getAddress((IfxStm_Index)stm);
	IfxStm_clearCompareFlag(STM, (IfxStm_Comparator)channel);
	IfxStm_disableComparatorInterrupt(STM, (IfxStm_Comparator)channel);
}


/*************************************************************************
*  �������ƣ�STM_EnableInterrupt(STM_t stm, STM_Channel_t channel)
*  ����˵����STMʹ���ж�
*  ����˵����
* @param    stm      �� STM���     STM0 �� STM1
* @param    channel  �� STMͨ��     STM_Channel_0  STM_Channel_1
*  �������أ���
*  �޸�ʱ�䣺2020��5��7��
*  ��    ����
*  ��    ע��
*************************************************************************/
void STM_EnableInterrupt(STM_t stm, STM_Channel_t channel)
{
	Ifx_STM * STM = IfxStm_getAddress((IfxStm_Index)stm);
	IfxStm_clearCompareFlag(STM, (IfxStm_Comparator)channel);
	IfxStm_enableComparatorInterrupt(STM, (IfxStm_Comparator)channel);
}





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
  */ 



#include "LQ_GPSR.h"

#include <CompilerTasking.h>
#include <IfxCpu.h>
#include <IfxCpu_Irq.h>
#include <IfxInt_reg.h>
#include <IfxSrc.h>
#include <IfxSrc_reg.h>
#include <IfxSrc_regdef.h>
#include <Platform_Types.h>


/** cpu����ж����ȼ� */
const uint8 CpuSoftIrqPriority[8] = {CPU0_SOFT_IRQ0_PRIORITY, CPU0_SOFT_IRQ1_PRIORITY, CPU0_SOFT_IRQ2_PRIORITY, CPU0_SOFT_IRQ3_PRIORITY,
		CPU1_SOFT_IRQ0_PRIORITY,CPU1_SOFT_IRQ1_PRIORITY, CPU1_SOFT_IRQ2_PRIORITY,CPU1_SOFT_IRQ3_PRIORITY};





/**
  * @brief    CPU0_SOFT_IRQ0Handler�жϷ�����
  * @note     cpu0 ����ж�0�жϷ�����
  */
void CPU0_SOFT_IRQ0Handler(void)
{
	SRC_GPSR00.B.SRR = 0;
	/* �û����� */
}

void CPU0_SOFT_IRQ1Handler(void)
{
	SRC_GPSR01.B.SRR = 0;
	/* �û����� */
}

void CPU0_SOFT_IRQ2Handler(void)
{
	SRC_GPSR02.B.SRR = 0;
	/* �û����� */
}

void CPU0_SOFT_IRQ3Handler(void)
{
	SRC_GPSR03.B.SRR = 0;
	/* �û����� */
}

void CPU1_SOFT_IRQ0Handler(void)
{
	SRC_GPSR10.B.SRR = 0;
	/* �û����� */
}

void CPU1_SOFT_IRQ1Handler(void)
{
	SRC_GPSR11.B.SRR = 0;
	/* �û����� */
}

void CPU1_SOFT_IRQ2Handler(void)
{
	SRC_GPSR12.B.SRR = 0;
	/* �û����� */
}

void CPU1_SOFT_IRQ3Handler(void)
{
	SRC_GPSR13.B.SRR = 0;
	/* �û����� */
}



/* cpu���ж� */
IFX_INTERRUPT(CPU0_SOFT_IRQ0Handler, 0, CPU0_SOFT_IRQ0_PRIORITY);
IFX_INTERRUPT(CPU0_SOFT_IRQ1Handler, 0, CPU0_SOFT_IRQ1_PRIORITY);
IFX_INTERRUPT(CPU0_SOFT_IRQ2Handler, 0, CPU0_SOFT_IRQ2_PRIORITY);
IFX_INTERRUPT(CPU0_SOFT_IRQ3Handler, 0, CPU0_SOFT_IRQ3_PRIORITY);
IFX_INTERRUPT(CPU1_SOFT_IRQ0Handler, 1, CPU1_SOFT_IRQ0_PRIORITY);
IFX_INTERRUPT(CPU1_SOFT_IRQ1Handler, 1, CPU1_SOFT_IRQ1_PRIORITY);
IFX_INTERRUPT(CPU1_SOFT_IRQ2Handler, 1, CPU1_SOFT_IRQ2_PRIORITY);
IFX_INTERRUPT(CPU1_SOFT_IRQ3Handler, 1, CPU1_SOFT_IRQ3_PRIORITY);


/** cpu���жϷ�������ַ */
const void *CpuSoftIrqFuncPointer[8] = {&CPU0_SOFT_IRQ0Handler, &CPU0_SOFT_IRQ1Handler, &CPU0_SOFT_IRQ2Handler,
									&CPU0_SOFT_IRQ3Handler, &CPU1_SOFT_IRQ0Handler, &CPU1_SOFT_IRQ1Handler,
									&CPU1_SOFT_IRQ2Handler, &CPU1_SOFT_IRQ3Handler};


/*************************************************************************
*  �������ƣ�GPSR_InitConfig(IfxSrc_Tos cpu, uint8 softIrq)
*  ����˵��������еĳ�ʼ��
*  ����˵����cpu     �� IfxSrc_Tos_cpu0   IfxSrc_Tos_cpu1
*  ����˵����softIrq �� ÿ��CPU��4������ж�, irq���
*  �������أ���
*  �޸�ʱ�䣺2020��5��6��
*  ��    ע��
*************************************************************************/
void GPSR_InitConfig(IfxSrc_Tos cpu, SOFT_IRQ index)
{
	/* �ر��ж� */
	boolean        interruptState = IfxCpu_disableInterrupts();

	uint8 temp = cpu * 4 + index;
	SRC_GPSR00.B.SRR = 0;
	SRC_GPSR01.B.SRR = 0;
	SRC_GPSR02.B.SRR = 0;
	SRC_GPSR03.B.SRR = 0;

	SRC_GPSR10.B.SRR = 0;
	SRC_GPSR11.B.SRR = 0;
	SRC_GPSR12.B.SRR = 0;
	SRC_GPSR13.B.SRR = 0;

	/* �����ж� */
	volatile Ifx_SRC_SRCR *src = &MODULE_SRC.GPSR.GPSR[(int)cpu].SR0;
	IfxSrc_init(src, (int)cpu, (uint32)CpuSoftIrqPriority[temp]);
	IfxSrc_enable(src);

	src = &MODULE_SRC.GPSR.GPSR[(int)cpu].SR1;
	IfxSrc_init(src, (int)cpu, (uint32)CpuSoftIrqPriority[temp]);
	IfxSrc_enable(src);

	src = &MODULE_SRC.GPSR.GPSR[(int)cpu].SR2;
	IfxSrc_init(src, (int)cpu, (uint32)CpuSoftIrqPriority[temp]);
	IfxSrc_enable(src);

	src = &MODULE_SRC.GPSR.GPSR[(int)cpu].SR3;
	IfxSrc_init(src, (int)cpu, (uint32)CpuSoftIrqPriority[temp]);
	IfxSrc_enable(src);

	/* ��װ�жϷ����� */
	IfxCpu_Irq_installInterruptHandler((void*)CpuSoftIrqFuncPointer[temp],     (uint32)CpuSoftIrqPriority[temp]);

	/* �����ж� */
	IfxCpu_restoreInterrupts(interruptState);
}

/*************************************************************************
*  �������ƣ�CPSR_Trig(IfxSrc_Tos cpu, uint8 softIrq)
*  ����˵������������ж�
*  ����˵����cpu     �� IfxSrc_Tos_cpu0   IfxSrc_Tos_cpu1
*  ����˵����softIrq �� ÿ��CPU��4������ж�, irq���
*  �������أ���
*  �޸�ʱ�䣺2020��5��6��
*  ��    ע��
*************************************************************************/
void CPSR_Trig(IfxSrc_Tos cpu, SOFT_IRQ index)
{
	if(cpu == IfxSrc_Tos_cpu0)
	{
		INT_SRB0.U = 1 << (uint8)index;
	}
	else
	{
		INT_SRB1.U = 1 << (uint8)index;
	}
}











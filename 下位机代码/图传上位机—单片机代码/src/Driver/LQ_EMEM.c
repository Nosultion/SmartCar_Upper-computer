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



#include <IfxEmem.h>
#include "LQ_EMEM.h"

/*************************************************************************
*  �������ƣ�EMEM_InitConfig(void)
*  ����˵������ʼ��оƬ�ڲ� EMEM�ڴ�  512K
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��TC264 DA��׺��оƬ����EMEM��512K ram
*************************************************************************/
void EMEM_InitConfig(void)
{
	IfxEmem_setClockEnableState(IfxEmem_State_enabled);

	if (IfxEmem_LockedState_locked == IfxEmem_getLockedState())
	{
		/* apply unlock sequence */
		IfxEmem_setUnlockStandbyLockFlag(0x1);
		IfxEmem_setUnlockStandbyLockFlag(0x3);
		IfxEmem_setUnlockStandbyLockFlag(0x7);
		/* wait one cycle for unlock */

		for(int i = 0; i < 10000; i++)
		{
			__asm("NOP");
		}
	}

	IfxEmem_setTileConfigMode(IfxEmem_TileConfigMode_calibMode,IfxEmem_TileNumber_0);
	IfxEmem_setTileConfigMode(IfxEmem_TileConfigMode_calibMode,IfxEmem_TileNumber_1);
	IfxEmem_setTileConfigMode(IfxEmem_TileConfigMode_calibMode,IfxEmem_TileNumber_2);
	IfxEmem_setTileConfigMode(IfxEmem_TileConfigMode_calibMode,IfxEmem_TileNumber_3);
	IfxEmem_setTileConfigMode(IfxEmem_TileConfigMode_calibMode,IfxEmem_TileNumber_4);
	IfxEmem_setTileConfigMode(IfxEmem_TileConfigMode_calibMode,IfxEmem_TileNumber_5);
	IfxEmem_setTileConfigMode(IfxEmem_TileConfigMode_calibMode,IfxEmem_TileNumber_6);
	IfxEmem_setTileConfigMode(IfxEmem_TileConfigMode_calibMode,IfxEmem_TileNumber_7);
}

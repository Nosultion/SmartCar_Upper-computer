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


#include "../Driver/LQ_EEPROM.h"

#include "LQ_OLED096.h"


/////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////���²���Ϊ���ܲ���/////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////




/*************************************************************************
*  �������ƣ�void Test_EEPROM(void)
*  ����˵����������ʾ
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��
*************************************************************************/
void Test_EEPROM(void)
{
	int i;

	OLED_Init();
	OLED_P6x8Str(0,3,"eeprom");         //�ַ���
	unsigned long u32wBuff[24];
	unsigned long u32rBuff[24];

	float f32wBuff[24];
	float f32rBuff[24];

	/* ��ֵ */
	for(i = 0; i < 24; i++)
	{
		u32wBuff[i] = i * 100;
		f32wBuff[i] = i * 2.5f;
	}

	EEPROM_EraseSector(0);
	EEPROM_EraseSector(1);

	OLED_P6x8Str(0,1,"eeporm u write");

	/* д�� */
	EEPROM_Write(0, 0, u32wBuff, 24);

	OLED_P6x8Str(0,2,"eeporm f write");

	EEPROM_Write(1, 0, (unsigned long*)f32wBuff, 24);

	OLED_P6x8Str(0,3,"eeporm u read");

	/* ���� */
	EEPROM_Read(0, 0, u32rBuff, 24);

	OLED_P6x8Str(0,4,"eeporm f read");

	EEPROM_Read(1, 0, (unsigned long*)f32rBuff, 24);

	/* �Ƚ� */
	for(i = 0; i < 24; i++)
	{
		if(u32wBuff[i] != u32rBuff[i])
		{
			OLED_P6x8Str(0,5,"u32error");
		}
		if(f32wBuff[i] != f32rBuff[i])
		{
			OLED_P6x8Str(0,6,"ferror");
		}
	}
	OLED_P6x8Str(0,7,"eeprom is ok");

#pragma warning 557         // ���ξ���
	while (1);
#pragma warning default     // �򿪾���
}





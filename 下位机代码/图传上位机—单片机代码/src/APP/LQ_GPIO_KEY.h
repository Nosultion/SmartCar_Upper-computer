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
#ifndef __LQ_KEY_H_
#define __LQ_KEY_H_

//����ģ���
typedef enum
{
    KEY0 = 0,  //ĸ���ϰ���0
    KEY1 = 1,
    KEY2 = 2,
    DSW0 = 3,    //ĸ���ϲ��뿪��0
    DSW1 = 4,
} KEYn_e;

typedef enum
{
    LOW = 0,  //����
    HIGH = 1, //�ɿ�
    FAIL = 0xff, //����
} KEYs_e;

typedef enum //
{
    NOKEYDOWN = 0, KEY0DOWN = 0x01,  //ĸ���ϰ���0
    KEY1DOWN = 0x02,
    KEY2DOWN = 0x04,
    KEY01DOWN = 0x03,  //ĸ���ϰ���0
    KEY02DOWN = 0x05,
    KEY12DOWN = 0x06,
    KEY012DOWN = 0x07,
    KEYError = 0xFF,
} KEYdown_e;
//����Ĺܽ�Ҫ��Ӧʵ�ʰ���
#define KEY0p      P22_0  //ĸ���ϰ���0
#define KEY1p      P22_1  //ĸ���ϰ���1
#define KEY2p      P22_2  //ĸ���ϰ���2
#define DSW0p      P33_9  //ĸ���ϲ��뿪��0,Ĭ�ϲ����·��Ǹߵ�ƽ���Ϸ�Ϊ�͵�ƽ
#define DSW1p      P33_11 //ĸ���ϲ��뿪��1
#define REEDp      P15_4  //ĸ���ϸɻɹ�
/*********************** UART���ܺ��� **************************/
//��ʼ��
void GPIO_KEY_Init (void);
unsigned char KEY_Read (KEYn_e KEYno);
unsigned char KEY_Read_All (void);
void Test_GPIO_KEY (void);
void Test_ComKEY_Tft (void);
/*************************************************************************
*  �������ƣ�void Reed_Init(void)
*  ����˵�����ɻɹ�GPIO���жϳ�ʼ������
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��11��21��
*  ��    ע��   �ж����Ǹ��˳�ʼ�����ж�VECTABNUM�ű����Ӧ��cpu,���򲻻�����жϺ���
*          �жϷ�����PIN_INT0_IRQHandler��LQ_GPIO.c��
*************************************************************************/
void Reed_Init(void);
#endif/* 0_APPSW_TRICORE_APP_LQ_ASC_H_ */

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�TC264DA���İ�
����    д��chiusir
��E-mail��chiusir@163.com
������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
�������¡�2021��6��16��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://longqiu.taobao.com
------------------------------------------------
��dev.env.��AURIX Development Studio1.4�����ϰ汾
��Target �� TC264DA/TC264D
��Crystal�� 20.000Mhz
��SYS PLL�� 200MHz
________________________________________________________________
����iLLD_1_0_1_11_0�ײ����,

ʹ�����̵�ʱ�򣬽������û�пո��Ӣ��·����
����CIFΪTC264DA�����⣬�����Ĵ������TC264D
����Ĭ�ϳ�ʼ����EMEM��512K������û�ʹ��TC264D��ע�͵�EMEM_InitConfig()��ʼ��������
������\Libraries\iLLD\TC26B\Tricore\Cpu\CStart\IfxCpu_CStart0.c��164�����ҡ�
=================================================================
����������Ƶ��ַ��https://space.bilibili.com/95313236
=================================================================
����ͷ�ӿ�                  �������ۻ���OV7725ģ��
�� ���ݶ˿ڣ�P02.0-P02.7�ڣ���8λ��������ͷ�����ݶ˿ڣ�
�� ʱ�����أ��ⲿ�жϵ�0�飺P00_4��
�� ���źţ��ⲿ�жϵ�3�飺P15_1��
-----------------------------------------------------------------
�Ƽ�GPT12ģ�飬������ʵ��5·�����������������������ݴ�������������źŲɼ�������ѡ����·���ɣ�
P33_7, P33_6   ����TCĸ�������1
P02_8, P33_5   ����TCĸ�������2
P10_3, P10_1   ����TCĸ�������3
P20_3, P20_0   ����TCĸ�������4
-----------------------------------------------------------------
��е�ѹ�ɼ�ģ�������˷�ģ��
�Ƽ�ʹ��AN0-7������·ADC����������chirp�����źż���ų���е�ѹ�ɼ���
AN0-3          ����TC���ĸ���˷�ģ����ߵ��
-----------------------------------------------------------------
Ĭ�ϵ���ӿ�
ʹ��GTMģ�飬ATOM�ĸ�ͨ���ɲ���4*8��32·PWM�����Ҹ���Ƶ�ʺ�ռ�ձȿɵ����Ƽ�ʹ��ATOM0��0-7ͨ����
��һ��˫·ȫ������
P23_1         ����TCĸ��MOTOR1_P
P32_4         ����TCĸ��MOTOR1_N
P21_2         ����TCĸ��MOTOR2_P
P22_3         ����TCĸ��MOTOR2_N
�ڶ���˫·ȫ������
P21_4         ����TCĸ��MOTOR3_P
P21_3         ����TCĸ��MOTOR3_N
P20_8         ����TCĸ��MOTOR4_P
P21_5         ����TCĸ��MOTOR4_N
-----------------------------------------------------------------
Ĭ�϶���ӿ�
ʹ��GTMģ�飬ATOM�ĸ�ͨ���ɲ���4*8��32·PWM�����Ҹ���Ƶ�ʺ�ռ�ձȿɵ����Ƽ�ʹ��ATOM2��
P33_10        ����TCĸ����1
P33_13        ����TCĸ����2
-----------------------------------------------------------------
 Ĭ����Ļ��ʾ�ӿڣ��û�����ʹ��TFT����OLEDģ��
TFTSPI_CS     P20_14     ����TCĸ�� CS�ܽ� Ĭ�����ͣ����Բ���
TFTSPI_SCK    P20_11     ����TCĸ�� SPI SCK�ܽ�
TFTSPI_SDI    P20_10     ����TCĸ�� SPI MOSI�ܽ�
TFTSPI_DC     P20_12     ����TCĸ�� D/C�ܽ�
TFTSPI_RST    P20_13     ����TCĸ�� RESET�ܽ�
-----------------------------------------------------------------
OLED_CK       P20_14     ����TCĸ��OLED CK�ܽ�
OLED_DI       P20_11     ����TCĸ��OLED DI�ܽ�
OLED_RST      P20_10     ����TCĸ��OLED RST�ܽ�
OLED_DC       P20_12     ����TCĸ��OLED DC�ܽ�
OLED_CS       P20_13     ����TCĸ��OLED CS�ܽ� Ĭ�����ͣ����Բ���
----------------------------------------------------------------
Ĭ�ϰ����ӿ�
KEY0p         P22_0      ����TCĸ���ϰ���0
KEY1p         P22_1      ����TCĸ���ϰ���1
KEY2p         P22_2      ����TCĸ���ϰ���2
DSW0p         P33_9      ����TCĸ���ϲ��뿪��0
DSW1p         P33_11     ����TCĸ���ϲ��뿪��1
-----------------------------------------------------------------
Ĭ��LED�ӿ�
LED0p         P10_6      ����TCĸ����İ���LED0 ����
LED1p         P10_5      ����TCĸ����İ���LED1 ����
LED2p         P20_6      ����TCĸ����LED0
LED3p         P20_7      ����TCĸ����LED1
-----------------------------------------------------------------
Ĭ�Ϸ������ӿ�
BEEPp         P33_8      ����TCĸ���Ϸ������ӿ�
-----------------------------------------------------------------
��ʱ��
������CCU6ģ��  ÿ��ģ��������������ʱ��  ������ʱ���ж�
�Ƽ�ʹ��CCU6ģ�飬STM����ϵͳʱ�ӻ�����ʱ��
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include <IfxCpu.h>
#include <IfxScuCcu.h>
#include <IfxScuWdt.h>
#include <IfxStm.h>
#include <IfxStm_reg.h>

#include <stdio.h>
#include <stdarg.h>
#include <IfxGtm_Atom.h>
#include <IfxGtm_regdef.h>
#include <Platform_Types.h>

#include "src/APP/LQ_ADC_test.h"
#include "src/APP/LQ_Atom_Motor.h"
#include "src/APP/LQ_CAMERA.h"
#include "src/APP/LQ_EEPROM_TEST.h"
#include "src/APP/LQ_FFT_TEST.h"
#include "src/APP/LQ_GPIO_ExInt.h"
#include "src/APP/LQ_GPIO_KEY.h"
#include "src/APP/LQ_GPIO_LED.h"
#include "src/APP/LQ_GPT_mini512.h"
#include "src/APP/LQ_I2C_9AX.h"
#include "src/APP/LQ_I2C_VL53.h"
#include "src/APP/LQ_ICM20602.h"
#include "src/APP/LQ_OLED096.h"
#include "src/APP/LQ_STM_Timer.h"
#include "src/APP/LQ_TFT18.h"
#include "src/APP/LQ_Tim_InputCature.h"
#include "src/APP/LQ_Tom_Servo.h"
#include "src/APP/LQ_UART_Bluetooth.h"
#include "src/Driver/include.h"
#include "src/Driver/LQ_STM.h"
#include "src/Driver/LQ_UART.h"
#include "src/User/LQ_MotorServo.h"
#include "src/User/LQ_PID.h"
#include <LQ_IIC_Gyro.h>
#include "LQ_TFT2.h"
#include "LQ_ADC.h"
#include "LQ_CCU6.h"
#include "LQ_BLDC.h"
#include "LQ_GPIO_BUZZ.h"
#include "LQ_GPIO_Infrared.h"
#include "LQ_ImageProcess.h"
#include "LQ_SOFTI2C.h"
#include "LQ_MPU6050_DMP.h"
#include "TFT_Key_control.h"
#include "Image_Process.h"
#include "stdlib.h"
#include "LQ_GTM.h"
#include "LQ_GPT12_ENC.h"
#include "LQ_EEPROM.h"

App_Cpu0 g_AppCpu0;                       // brief CPU 0 global data
IfxCpu_mutexLock mutexCpu0InitIsOk = 1;   // CPU0 ��ʼ����ɱ�־λ
volatile char mutexCpu0TFTIsOk=0;         // CPU1 0ռ��/1�ͷ� TFT


int16_t j = 0;
unsigned char Show_TFT_Flag=1;   //���밴��
extern float g_sSteeringError;
char txt_main[16];
extern int ROAD_MAIN_ROW;
extern volatile sint16 Error_high;
extern volatile sint16 high;

extern uint8_t leftup[2] ;
extern uint8_t rightup[2] ;
extern uint8_t leftup_1[2];
extern uint8_t rightup_1[2] ;

extern unsigned short THRESHOLD;

extern int yuzhi;
extern int min_tag;
extern int Voltage,vol_last;
extern uint8_t motor_flag;
extern int out_PWML,out_PWMR;
uint8_t hough_space[100][45];
extern float K_Pid_R;
extern float K_adjust;
extern uint8_t open_flag;
extern uint8_t Obstacle_flag;
extern uint8_t slope_flag;
extern uint8_t Park_flag;
extern uint8_t Circle_flag_L;//��Բ����־λ
extern uint8_t Circle_flag_R;//��Բ����־λ
extern uint8_t cross_flag;
extern uint8_t rectangle_flag;
extern int Obstacle_turn_flag;//·�����ҹձ�־λ
extern int motor_turn_flag;//�������ҹձ�־λ
extern int Speed_turn_flag;//�ٶ�ģʽ��־λ

extern volatile int ECPULSE_open;
extern volatile int ECPULSE_Obstacle_L;
extern volatile int ECPULSE_Obstacle_R;

extern int Obstacle_num,Circle_num;
extern int ADC_Value[4];       //��ŵĲɼ�ֵ
extern float Cirle_PWM;
extern int X_minL,X_minR,X_min;
extern uint16 MagneticField;
extern int meun_flag;
extern float Anglex,Angley,Anglez;//�����ǽǶ�

/*************************************************************************
*  �������ƣ�int core0_main (void)
*  ����˵����CPU0������
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��
*************************************************************************/
int core0_main (void)
{
//    unsigned char res;

	// �ر�CPU���ж�
	IfxCpu_disableInterrupts();

	// �رտ��Ź�����������ÿ��Ź�ι����Ҫ�ر�
	IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
	IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

	// ��ȡ����Ƶ��
	g_AppCpu0.info.pllFreq = IfxScuCcu_getPllFrequency();
	g_AppCpu0.info.cpuFreq = IfxScuCcu_getCpuFrequency(IfxCpu_getCoreIndex());
	g_AppCpu0.info.sysFreq = IfxScuCcu_getSpbFrequency();
	g_AppCpu0.info.stmFreq = IfxStm_getFrequency(&MODULE_STM0);

    TFTSPI_Init(1);               // TFT2.0��ʼ��0:����  1������
    TFTSPI_CLS(u16BLACK);         // ����
//    TFTSPI_Show_Logo(0,37);       // ��ʾ����LOGO
//    TFTSPI_P16x16Str(0,0,(unsigned char*)"�����������ܿƼ�",u16RED,u16BLUE);// �ַ�����ʾ

    EncInit();                    // ��������ʼ��
    GPIO_KEY_Init();              // ������ʼ��
    GPIO_LED_Init();              // LED������P10.6��P10.5��ʼ��
    ADC_InitConfig(ADC7, 80000);  //��ʼ�������ʹ������ĸ�塢����ѹ��ĵ�ص�ѹ��������Կ�ĸ��ԭ��ͼ
    GPIO_BUZZ_Init();             //��������ʼ��
    GPIO_Infrared_Init();         //���⴫������ʼ��
    TFTSPI_CLS(u16BLACK);         // ����
    BUZZ_Ctrl(BUZZ0,BUZZ_ON);     //��ʼ������
	// ����P14.0�ܽ����,P14.1���룬������115200
	UART_InitConfig(UART0_RX_P14_1,UART0_TX_P14_0, 115200);
	UART_PutStr(UART0,"UART0 Test \r\n");       //�����ַ�������λ��
	// ����CPU���ж�
	IfxCpu_enableInterrupts();

	// ֪ͨCPU1��CPU0��ʼ�����
	IfxCpu_releaseMutex(&mutexCpu0InitIsOk);
	// �м�CPU0,CPU1...������ͬʱ������Ļ��ʾ�������ͻ����ʾ
	mutexCpu0TFTIsOk=0;          // CPU0�� 0ռ��/1�ͷ� TFT
    // ���²��Ժ������ڽ���ѭ�����û��ɵ�������ģ��ĳ�ʼ������д������ʵ���Լ�������______________________________________________________________________________
//	  Test_LQDMP();          //PASS������DMP��6050�����ǵĶ�ȡ������TFT1.8�����������ʾ����
//	  Test_ADC_OLED();       //PASS,����ADC����ʱ��  OLED����ʾ ADC����10K��ʱ��
//	  Test_ADC_TFT();        //PASS,����ADC����ʱ�� TFT����ʾ ADC����10K��ʱ��
//	  Test_ADC_7mic();       //PASS,����ADC\UART0��STM��ʱ�����ƣ�ͨ��UART0��ӡ AN0--AN7��8��ͨ��ADCת������
//    LQ_Atom_Motor_8chPWM();//PASS,����GTM_ATOM���ɲ�ͬƵ���µ�8·PWM
//    LQ_ATom_Servo_2chPWM();//PASS,����GTM_ATOM��STM��ʱ�����ƣ�P33.10��P33.13��ΪATOM����ڿ��ƶ��
//    LQ_Tom_Servo_2chPWM(); //PASS,����GTM_TOM��STM��ʱ�����ƣ�P33.10��P33.13��ΪTOM����ڿ��ƶ��
//    Test_GPIO_Extern_Int();//PASS,�����ⲿ��1���ж�P15.8��P10.6��P10.5����
//    Test_GPIO_LED();       //PASS,����GPIO��P10.6��P10.5����
//    Test_GPIO_KEY();       //PASS,�����ⲿ�������룬P22.0--2   ���°��� LED��
//    Test_ComKEY_Tft();     //PASS,�����ⲿ��ϰ������벢TFT1.8��ʾ��P22.0--2
//    LQ_GPT_4mini512();     //PA SS,���Ա�������������,OLED��UART���
//    LQ_GPT_4mini512TFT();  //PASS,���Ա�������������,TFT1.8��UART���
//    Test_OLED();           //PASS,����OLED0.96��ʹ��P20.14--10����ʾ�ַ�������̬����
//    LQ_STM_Timer();        //PASS,���Զ�ʱ�жϡ�����
//    Test_TFT18();          //PASS,����TFT1.8����ʹ��P20.14--10����ʾ�ַ�������̬����
//    LQ_TIM_InputCature();  //PASS,����GTM_TOM_TIM��P20_9��ΪPWM����ڣ�P15_0��ΪTIM����ڣ����߶̽Ӻ󣬴���P14.0���͵���λ��
//    Test_Bluetooth();      //PASS,����UART0(P14.0RX/P14.1TX)��
//    Test_EEPROM();         //PASS,�����ڲ�EEPROM��д����  OLED��ʾ�Ƿ�д��ɹ�
//    Test_Vl53();           //PASS,����VL53  IIC����   P13_1��SCL  P13_2��SDA OLED��ʾԭʼ����
//    Test_9AX();            //PASS,����������� IIC����   P13_1��SCL  P13_2��SDA OLED��ʾԭʼ����
//    Test_MPU6050();        //PASS,����MPU6050����ICM20602 IIC����   P13_1��SCL  P13_2��SDA OLED��ʾԭʼ����
//    Test_ICM20602();       //PASS,����ICM20602 SPI����   P15_8��SCL  P15_5��SDA  P15_7��SA  P15_2��CS OLED��ʾԭʼ����

//    Test_SoftFft();        //PASS,����ILLD������FFT����
//    Test_FFT();            //PASS,����Ӳ��FFT  ע����ҪоƬ��׺��DA�Ĳ���Ӳ��FFT����
//    TestMotor();           //PASS,������·�����ͨ���궨��ѡ��ͬ��������
//	  TestEncoder();         //PASS,������·������
//	  Test_IIC_Gyro();       //PASS,����ͨ��IIC����ȡ6050������
//	  TestServo();           //PASS,�������ƣ�P33.10��P33.13��ΪTOM����ڿ��ƶ��
	//PID��������
	PidInit(&LSpeed_PID);       //pid��ʼ��
	PidInit(&RSpeed_PID);
	PidInit(&ESpeed_PID);
	Pid_Value();                //pid��������

	/*�����ǳ�ʼ��  */
	IIC_Init();                 //IIC��ʼ��
//	delayms(100);
//	res = Gyro_Chose();         //�ж��������ͺ�
//	LQ_DMP_Init();              //DMP��ʼ��

//    if(LQ9AX_Init())
//    {
//        TFTSPI_P8X16Str(1,0,"Test Fail",u16WHITE,u16BLACK);
//#pragma warning 557         // ���ξ���
//        while (1);
//#pragma warning default     // �򿪾���
//    }

    //  /* ����ͷ��ʼ�� */
     CAMERA_Init(50);
	/*ADC��ʼ��*/
	ADC_InitConfig(ADC0, 80000);
	ADC_InitConfig(ADC1, 80000);
	ADC_InitConfig(ADC2, 80000);
	ADC_InitConfig(ADC3, 80000);

	//����CCU6��ʱ��
//	CCU6_InitConfig(CCU60, CCU6_Channel1, 5*1000);  // 5ms����һ���ж�  ��ȡ����ֵ��������ƺ�ת����ƶ�ʱ�ж�
//    CCU6_InitConfig(CCU60, CCU6_Channel0,16*1000);  // 16ms  ���� ����һ���ж�  Ԫ�ش���ʱ�ж�
//	CCU6_InitConfig(CCU61, CCU6_Channel1, 5*1000);  // 30ms����һ���ж� LED0��˸&&���������� ��Ųɼ�

	TFTSPI_Init(1);
	TFTSPI_CLS(u16YELLOW);   //��ɫ��Ļ
	MotorInit();                  // �����ʼ����������ǰ���ϵ������ת

	//PIN_InitConfig(P15_2, PIN_MODE_OUTPUT, 0);//�����ж�ʱ������
	 BUZZ_Ctrl(BUZZ0,BUZZ_OFF);//��ʼ������
	 Test_CAMERA();         //PASS,����������������ͷ������Ļ����ʾ  LQ_CAMERA.h ��ѡ����Ļ
    while (1)	//��ѭ��
    {
        //������ֲ���
//        ATOM_PWM_SetDuty(ATOMPWM5, K_adjust, 17000);
//        ATOM_PWM_SetDuty(ATOMPWM4, 0, 17000);
//
//        ATOM_PWM_SetDuty(ATOMPWM7, K_adjust, 17000);
//        ATOM_PWM_SetDuty(ATOMPWM6, 0, 17000);

       //������
//       if(motor_flag==1
//        ||Obstacle_flag==2
//        ||open_flag==1
//        ||slope_flag==1
//        ||Circle_flag_L==3
//        ||Circle_flag_L==4
//        ||Circle_flag_R==3
//        ||Circle_flag_R==4
//        ||Park_flag==2
//        )
//       {
//            //����ʶ��Ԫ������
//            BUZZ_Ctrl(BUZZ0,BUZZ_ON);
//       }
//       else
//       {
//           BUZZ_Ctrl(BUZZ0,BUZZ_OFF);
//       }

       //���ڵ���
       UART_Test();
       //����������Ļ��Ϣ��ʾ
       menu();
       //����
       if(PIN_Read(P13_0)==0)
       {
           motor_turn_flag=0;
           Anglez=0;   //�����ǻ�����0
           Anglex=0;
           Angley=0;
           Obstacle_num=0;
           motor_flag=1;
       }
       if(PIN_Read(P15_2)==0)
       {
           motor_turn_flag=1;
           Anglez=0;   //�����ǻ�����0
           Anglex=0;
           Angley=0;
           Obstacle_num=0;
           motor_flag=1;
       }
       //��ų�����ͣ��
       if(MagneticField<1000&&Obstacle_flag==0&&(motor_flag==2||motor_flag==0))
       {
          start_V_L=0;
          motor_flag=0;
          BUZZ_Ctrl(BUZZ0,BUZZ_OFF);
       }

       //�������
       //key_control();
       //��Ļ��ʾ
       //TFT_Show_Camera_Info();
//       if(Show_TFT_Flag==1||Show_TFT_Flag==2)
//       {
//           TFT_show();
//       }
//       else if(Show_TFT_Flag==4)
//       {
//          TFTSPI_Road(0, 0, LCDH, LCDW,Image_Use_BLACK);
//         // TFTSPI_BinRoadSide(ImageSide);          //���ұ��ߺ�����
//       }
//       else if(Show_TFT_Flag==3)
//       {
//         TFTSPI_Road(0, 0, LCDH, LCDW,Image_Data);
//       }

    }
}

/*******************6050DMP��ȡ���Ժ���*****************************/
void Test_LQDMP(void)
{

    unsigned char res;
    char txt[30];
    /*��Ļ��ʼ��*/
    TFTSPI_Init(1);               // TFT2.0��ʼ��0:����  1������
    TFTSPI_CLS(u16BLACK);         // ����
    /*�����ǳ�ʼ��  */
    //IIC_Init();                 //IIC��ʼ��//IIC��ʼ��
    delayms(100);
    res = Gyro_Chose();           //�ж��������ͺ�

    sprintf(txt,"%-3d",res);
    TFTSPI_P6X8Str(0, 0, txt, u16RED, u16BLUE);
    TFTSPI_CLS(u16BLACK);         // ����
    LQ_DMP_Init();
    while(1)
    {
        LQ_DMP_Read();
        sprintf((char*)txt,"Pitch:%.2f", Pitch);
        TFTSPI_P8X16Str(0,1,txt,u16RED,u16BLACK);
        sprintf((char*)txt,"Roll:%.2f", Roll);
        TFTSPI_P8X16Str(0,2,txt,u16RED,u16BLACK);
        sprintf((char*)txt,"Yaw:%.2f", Yaw);
        TFTSPI_P8X16Str(0,3,txt,u16RED,u16BLACK);

        sprintf((char*)txt,"ax:%06d",accel[0]);
        TFTSPI_P8X16Str(0,4,txt,u16RED,u16BLACK);
        sprintf((char*)txt,"ay:%06d",accel[1]);
        TFTSPI_P8X16Str(0,5,txt,u16RED,u16BLACK);
        sprintf((char*)txt,"az:%06d",accel[2]);
        TFTSPI_P8X16Str(0,6,txt,u16RED,u16BLACK);
        sprintf((char*)txt,"gx:%06d",gyro[0]);
        TFTSPI_P8X16Str(0,7,txt,u16RED,u16BLACK);
        sprintf((char*)txt,"gy:%06d",gyro[1]);
        TFTSPI_P8X16Str(0,8,txt,u16RED,u16BLACK);
        sprintf((char*)txt,"gz:%06d",gyro[2]);
        TFTSPI_P8X16Str(0,9,txt,u16RED,u16BLACK);
    }
}


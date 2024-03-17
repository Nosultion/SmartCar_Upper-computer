/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 ��ƽ    ̨�������������ܿƼ�TC264DA���İ�
 ����    д��chiusir
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
 ʹ�����̵�ʱ�򣬽������û�����ļ��ո��Ӣ��·����
 ����CIFΪTC264DA�����⣬�����Ĵ������TC264D
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef SRC_APPSW_TRICORE_USER_LQ_MOTORCONTROL_H_
#define SRC_APPSW_TRICORE_USER_LQ_MOTORCONTROL_H_
#include "IfxGtm_PinMap.h"
#include "../../Libraries/iLLD/TC26B/Tricore/Cpu/Std/Platform_Types.h"

//���Ƶ��
#define MOTOR_FREQUENCY   17000

//���PWM �궨��
#define MOTOR1_P          IfxGtm_ATOM0_6_TOUT42_P23_1_OUT
#define MOTOR1_N          IfxGtm_ATOM0_5_TOUT40_P32_4_OUT

#define MOTOR2_P          IfxGtm_ATOM0_0_TOUT53_P21_2_OUT
#define MOTOR2_N          IfxGtm_ATOM0_4_TOUT50_P22_3_OUT


#define MOTOR3_P          IfxGtm_ATOM0_7_TOUT64_P20_8_OUT
#define MOTOR3_N          IfxGtm_ATOM0_3_TOUT56_P21_5_OUT


#define MOTOR4_P          IfxGtm_ATOM0_2_TOUT55_P21_4_OUT
#define MOTOR4_N          IfxGtm_ATOM0_1_TOUT54_P21_3_OUT


#define ATOMSERVO1       IfxGtm_ATOM2_0_TOUT32_P33_10_OUT
#define ATOMSERVO2       IfxGtm_ATOM2_5_TOUT35_P33_13_OUT

//#define USE7843or7971   //USEDRV8701 ʹ������ͬ������ģ�飬ѡ���Ӧ�ĺ궨��
#define USEDRV8701

#define  Kbat       2    // ��ص�ѹ�Ե��ռ�ձȵ�Ӱ��ϵ��
#define  Kencoder   5    // �������ٶȶԵ��ռ�ձȵ�Ӱ��ϵ��
#define  Koffset    1    // �������ʶԵ��ռ�ձȵ�Ӱ��ϵ��

#define TURN_LEFT    0   // ��ת
#define TURN_RIGHT   1   // ��ת
#define IN_GARAGE    1   // ���
#define OUT_GARAGE   0   // ����

#define Servo_Delta 150

#define Servo_Delta 150            //�������ת���Ĳ�ֵ�������ͺţ����˺Ͷ���۳��й�
#define Servo_Center_Mid 780      //���ֱ����ֵ��
#define Servo_Left_Max   (Servo_Center_Mid+Servo_Delta)      //�����ת����ֵ
#define Servo_Right_Min  (Servo_Center_Mid-Servo_Delta)      //�����ת����ֵ����ֵ��������÷�ʽ�йأ���ʽ

#define MtTargetDuty  4200  //���ռ�ձ�ֵ�����ֵ��ʵ����Ҫ��ȥ��ص�ѹ�����������������ʣ��ﲻ����ֵ

extern sint16 ServoDuty;
extern sint16 MotorDuty1;      // �������ռ�ձ���ֵ
extern sint16 MotorDuty2;      // �������ռ�ձ���ֵ
extern sint16 MotorDuty_L;       //������PWM
extern sint16 MotorDuty_R;
extern sint16 MotorDuty_L2;
extern sint16 MotorDuty_R2;

extern sint32 NowTime;
extern unsigned char Camera_Flag; // ����ʶ
extern uint8 CircleNumber;// �뻷������0������Ĭ��1�� ;���ĸ���һ���ڱ���ǰ��������ʱ��֪����
extern sint16 OFFSET0;    // ��Զ������������ֵ�ۺ�ƫ����
extern sint16 OFFSET1;    // �ڶ���
extern sint16 OFFSET2;    // �����������

void EncInit(void);
void TestEncoder(void);

void MotorInit(void);
void MotorCtrl(sint32 motor1, sint32 motor2);
void MotorCtrl4w(sint32 motor1, sint32 motor2,sint32 motor3, sint32 motor4);
void TestMotor(void);

void ServoInit(void);
void ServoCtrl(uint32 duty);
void TestServo(void);
uint8 ReadOutInGarageMode (void);
void OutInGarage (uint8 inout, uint8 lr);
void OutInGarageTft (uint8 inout, uint8 lr);// �����ã�����Ļ��ʾ����Ҫ��ͻ
uint8 SetCircleNum (void);
void CircleDetect (void);




#endif /* SRC_APPSW_TRICORE_USER_LQ_MOTORCONTROL_H_ */

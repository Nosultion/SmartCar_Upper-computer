/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 【平    台】北京龙邱智能科技TC264DA核心板
 【编    写】chiusir
 【E-mail  】chiusir@163.com
 【软件版本】V1.1 版权所有，单位使用请先联系授权
 【最后更新】2020年10月28日
 【相关信息参考下列地址】
 【网    站】http://www.lqist.cn
 【淘宝店铺】http://longqiu.taobao.com
 ------------------------------------------------
 【dev.env.】AURIX Development Studio1.2.2及以上版本
 【Target 】 TC264DA/TC264D
 【Crystal】 20.000Mhz
 【SYS PLL】 200MHz
 ________________________________________________________________
 基于iLLD_1_0_1_11_0底层程序,
 使用例程的时候，建议采用没有中文及空格的英文路径，
 除了CIF为TC264DA独有外，其它的代码兼容TC264D
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef SRC_APPSW_TRICORE_USER_LQ_MOTORCONTROL_H_
#define SRC_APPSW_TRICORE_USER_LQ_MOTORCONTROL_H_
#include "IfxGtm_PinMap.h"
#include "../../Libraries/iLLD/TC26B/Tricore/Cpu/Std/Platform_Types.h"

//电机频率
#define MOTOR_FREQUENCY   17000

//电机PWM 宏定义
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

//#define USE7843or7971   //USEDRV8701 使用龙邱不同的驱动模块，选择对应的宏定义
#define USEDRV8701

#define  Kbat       2    // 电池电压对电机占空比的影响系数
#define  Kencoder   5    // 编码器速度对电机占空比的影响系数
#define  Koffset    1    // 赛道曲率对电机占空比的影响系数

#define TURN_LEFT    0   // 左转
#define TURN_RIGHT   1   // 右转
#define IN_GARAGE    1   // 入库
#define OUT_GARAGE   0   // 出库

#define Servo_Delta 150

#define Servo_Delta 150            //舵机左右转动的差值，与舵机型号，拉杆和舵机臂长有关
#define Servo_Center_Mid 780      //舵机直行中值，
#define Servo_Left_Max   (Servo_Center_Mid+Servo_Delta)      //舵机左转极限值
#define Servo_Right_Min  (Servo_Center_Mid-Servo_Delta)      //舵机右转极限值，此值跟舵机放置方式有关，立式

#define MtTargetDuty  4200  //电机占空比值得最大值，实际需要减去电池电压，编码器，赛道曲率，达不到该值

extern sint16 ServoDuty;
extern sint16 MotorDuty1;      // 电机驱动占空比数值
extern sint16 MotorDuty2;      // 电机驱动占空比数值
extern sint16 MotorDuty_L;       //左轮总PWM
extern sint16 MotorDuty_R;
extern sint16 MotorDuty_L2;
extern sint16 MotorDuty_R2;

extern sint32 NowTime;
extern unsigned char Camera_Flag; // 场标识
extern uint8 CircleNumber;// 入环次数，0结束；默认1次 ;环的个数一般在比赛前测试赛道时就知道了
extern sint16 OFFSET0;    // 最远处，赛道中心值综合偏移量
extern sint16 OFFSET1;    // 第二格
extern sint16 OFFSET2;    // 最近，第三格

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
void OutInGarageTft (uint8 inout, uint8 lr);// 测试用，有屏幕显示，不要冲突
uint8 SetCircleNum (void);
void CircleDetect (void);




#endif /* SRC_APPSW_TRICORE_USER_LQ_MOTORCONTROL_H_ */

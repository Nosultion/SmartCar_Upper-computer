/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 【平    台】北京龙邱智能科技TC264DA核心板
 【编    写】ZYF/chiusir
 【E-mail  】chiusir@163.com
 【软件版本】V1.1 版权所有，单位使用请先联系授权
 【最后更新】2020年10月28日
 【相关信息参考下列地址】
 【网    站】http:// www.lqist.cn
 【淘宝店铺】http:// longqiu.taobao.com
 ------------------------------------------------
 【dev.env.】AURIX Development Studio1.2.2及以上版本
 【Target 】 TC264DA/TC264D
 【Crystal】 20.000Mhz
 【SYS PLL】 200MHz
 ________________________________________________________________
 基于iLLD_1_0_1_11_0底层程序,

 使用例程的时候，建议采用没有空格的英文路径，
 除了CIF为TC264DA独有外，其它的代码兼容TC264D
 本库默认初始化了EMEM：512K，如果用户使用TC264D，注释掉EMEM_InitConfig()初始化函数。
 工程下\Libraries\iLLD\TC26B\Tricore\Cpu\CStart\IfxCpu_CStart0.c第164行左右。
 ________________________________________________________________

 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ
 *  备    注：TC264 有两个CCU6模块  每个模块有两个独立定时器  触发定时器中断
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "LQ_CCU6.h"

#include <CompilerTasking.h>
#include <LQ_STEER_PID.h>
#include <math.h>
#include <stdlib.h>

#include "LQ_PID.h"
#include "../APP/LQ_GPIO_LED.h"
#include "LQ_GPT12_ENC.h"
#include "LQ_MotorServo.h"
#include "LQ_ImageProcess.h"
#include "LQ_ADC.h"
#include "LQ_UART.h"
#include "LQ_GPIO_BUZZ.h"
#include "LQ_MPU6050_DMP.h"
#include "LQ_IIC_Gyro.h"
#include "LQ_Atom_Motor.h"
#include "LQ_GTM.h"
#include "LQ_ADC.h"
#include "TFT_Key_control.h"
#include "LQ_GPIO.h"
#include "LQ_TFT2.h"
#include "LQ_TFT18.h"
int ROAD_MAIN_ROW=50;

volatile sint16 ECPULSEL[10];             // 速度全局变量
volatile sint16 ECPULSER[10];
int ECPULSE_L=0;
int ECPULSE_R=0;
volatile sint16 ECPULSE1 = 0;
volatile sint16 ECPULSE2 = 0;             // 速度全局变量
volatile int ECPULSE_open = 0;
volatile int ECPULSE_Obstacle_L = 0;
volatile int ECPULSE_Obstacle_R = 0;

volatile sint32 RAllPulse = 0;            // 速度全局变量

volatile float Target_Speed1 ;       // 目标速度全局变量
volatile float Target_Speed2 ;       // 目标速度全局变量

volatile float Real_Speed1=400;// 实际速度全局变量
volatile float Real_Speed2=500;

volatile double start_V_L=500;
volatile double start_V_R=500;

volatile sint16 Target_Duty1=0;// 目标占空比变量
volatile sint16 Target_Duty2=0;

volatile sint16 Real_Duty1=0;// 实际占空比变量
volatile sint16 Real_Duty2=0;

uint8_t motor_flag = 0;                   //电机启停标志位  0不启动   1启动
float K_Pid_L=0.5;
float K_Pid_R=1;
float K_error=0.15;
float K_adjust=2000;
float V_error;

extern sint16 OUT_PWM;                    //转向PWM的值
extern uint8_t L_CircleFlag;
extern uint8_t R_CircleFlag;
extern uint8_t Barrier_Flag;
extern float g_sSteeringError;
extern float last_g_sSteeringError;
int filter[20]={0};

volatile sint16 Error_flag;
/** @brief  十字路口标志位  */
extern uint8_t Crossroad_Flag;         //十字路口标志位 0:不是十字路口 2-3:是十字路口

extern uint8_t leftup[2] ;
extern uint8_t rightup[2] ;
extern uint8_t leftup_1[2];
extern uint8_t rightup_1[2] ;

extern uint8_t left_point[4];
extern uint8_t right_point[4];

extern uint8_t ImageSide[LCDH][2];         //左右边线数组

extern uint8_t point_num;

extern int min_tag;

int out_PWML=0,out_PWMR=0;
float KZ_L=1.1;
extern float Anglex,Angley,Anglez;//陀螺仪角度
extern int Feedback_SpeedL,Feedback_Speed_tmpL,Feedback_SpeedR,Feedback_Speed_tmpR;

extern short left_X,left_V,right_X, right_V;
extern float ALL_AD;

int Fan_PWML=1000;
int Fan_PWMR=1000;

extern int X_minL,X_minR,X_min;
extern int ADC_Value[4];       //电磁的采集值
extern uint16 MagneticField;
extern int B_lk;
extern char TXT;
extern char txt[16];
uint8_t rectangle_flag=0;
uint8_t open_flag=0;  //断路标志位  电磁标志位
uint8_t Obstacle_flag=0;//路障标志位
uint8_t slope_flag=0;  //坡道标志位
uint8_t Park_flag=0;  //车库标志位
uint8_t Circle_flag_L=0;//左圆环标志位
uint8_t Circle_flag_R=0;//右圆环标志位
uint8_t cross_flag=0; //十字标志位
int Obstacle_turn_flag=0;//路障左右拐标志位
int motor_turn_flag=0;//发车左右拐标志位
int Speed_turn_flag=0;//速度模式标志位
int Park_turn_flag=0; //车库模式切换
int Obstacle_num=0,Circle_num=0;
int Park_num=0;
IFX_INTERRUPT(CCU60_CH0_IRQHandler, CCU60_VECTABNUM, CCU60_CH0_PRIORITY);
IFX_INTERRUPT(CCU60_CH1_IRQHandler, CCU60_VECTABNUM, CCU60_CH1_PRIORITY);
IFX_INTERRUPT(CCU61_CH0_IRQHandler, CCU61_VECTABNUM, CCU61_CH0_PRIORITY);
IFX_INTERRUPT(CCU61_CH1_IRQHandler, CCU61_VECTABNUM, CCU61_CH1_PRIORITY);

/** CCU6中断CPU标号 */
const uint8 Ccu6IrqVectabNum[2] = {CCU60_VECTABNUM, CCU61_VECTABNUM};

/** CCU6中断优先级 */
const uint8 Ccu6IrqPriority[4] = {CCU60_CH0_PRIORITY, CCU60_CH1_PRIORITY, CCU61_CH0_PRIORITY, CCU61_CH1_PRIORITY};

/** CCU6中断服务函数地址 */
const void *Ccu6IrqFuncPointer[4] = {&CCU60_CH0_IRQHandler, &CCU60_CH1_IRQHandler, &CCU61_CH0_IRQHandler,
        &CCU61_CH1_IRQHandler};

/***********************************************************************************************/
/********************************CCU6外部中断  服务函数******************************************/
/***********************************************************************************************/

/*************************************************************************
 *  函数名称：void CCU60_CH0_IRQHandler(void)
 *  功能说明：
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2022年11月28日
 *  备   注：CCU60_CH0使用的中断服务函数
 *************************************************************************/
float Cirle_PWM=0;
float curvature=0;
int L_min_range=2750,L_max_range=5000,R_min_range=2500,R_max_range=5700;
void CCU60_CH0_IRQHandler (void)
{
    /* 开启CPU中断  否则中断不可嵌套 */
//   IfxCpu_enableInterrupts();
// 清除中断标志
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU60, IfxCcu6_InterruptSource_t12PeriodMatch);

    /* 用户代码 */
    //没有障碍物才发车
   if(motor_flag==1&&abs(Anglez)<90)
   {
       if(Obstacle_num>=0&&Obstacle_num<100)
       {
           out_PWML=0;
           out_PWMR=0;
       }
       else if(Obstacle_num>=100&&Obstacle_num<150)
       {
           out_PWML=3500;
           out_PWMR=3500;
       }
       else if(motor_turn_flag==0)
       {
           out_PWML=0;
           out_PWMR=7000;
       }
       else if(motor_turn_flag==1)
       {
           out_PWML=7000;
           out_PWMR=0;
       }
   }
   else if(motor_flag==1&&abs(Anglez)>90)
   {
       Obstacle_num=0;
       Anglez=0;   //陀螺仪积分置0
       motor_flag=2;
       if(Speed_turn_flag==0)   //模式0为高速全元素 S406全元素37秒
       {
           start_V_L=450;
       }
       else if(Speed_turn_flag==1)  //模式1 和 2为低速模式
       {
           start_V_L=400;
       }
       else if(Speed_turn_flag==2)  //模式1 和 2为低速模式
       {
           start_V_L=350;
       }
   }
    //摄像头元素信息处理
    //CameraCar();
    //判断是否存在类似矩形断路
    rectangle_flag=0;
    if(X_minR==X_minL&&X_min>30
    //&&ImageSide[X_min+5][0]>25&&ImageSide[X_min+5][1]<163
    &&ImageSide[X_min+1][0]>ImageSide[X_min+3][0]
    &&ImageSide[X_min+3][0]>ImageSide[X_min+5][0]
    &&ImageSide[X_min+5][0]>ImageSide[X_min+7][0]

    &&ImageSide[X_min+1][1]<ImageSide[X_min+3][1]
    &&ImageSide[X_min+3][1]<ImageSide[X_min+5][1]
    &&ImageSide[X_min+5][1]<ImageSide[X_min+7][1])//X_min>25可能是弯道或者断路
    {
        rectangle_flag=1;
    }
    if(motor_flag==2&&(Speed_turn_flag==0||Speed_turn_flag==1))
    {
        //曲率计算
        lkcongzhongjiansaomiao();
        //路况判断
        lukuangudge();
    }
    if(motor_flag==2||motor_flag==0)
    {
        //入库判断
        judge_Park();
        if(Speed_turn_flag==0||Speed_turn_flag==1) //模式0 和 1进行元素识别
        {
            if(Park_flag==0)
            {
                //圆环判断
                judge_Circle();
                if(Circle_flag_R!=2&&Circle_flag_R!=3&&Circle_flag_R!=4
                 &&Circle_flag_L!=2&&Circle_flag_L!=3&&Circle_flag_L!=4)
                {
                    //坡道判断
                    judge_slope();
                    if(slope_flag==0||slope_flag==2)
                    {
                        //路障判断
                        judge_Obstacle();
                        if(Obstacle_flag==0||Obstacle_flag==3)
                        {
                            //断路判断
                            judge_open();
                        }
                    }
                }
            }
        }
    }

    //摄像头方向环控制
   g_sSteeringError = RoadGetSteeringError(ImageSide, (unsigned char)ROAD_MAIN_ROW);//摄像头获取偏差
   if(slope_flag==1)
   {
       g_sSteeringError=0;
   }
   if(open_flag==0&&motor_flag==2)//没有障碍物才进行
   {
       if(g_sSteeringError>120)g_sSteeringError=120;
       if(g_sSteeringError<-120)g_sSteeringError=-120;
//       if(abs(g_sSteeringError)>60)
//       {
//           start_V_L=380;
//       }
//       else
//       {
//           start_V_L=450;
//       }
//      if(abs(last_g_sSteeringError)>80&&abs(g_sSteeringError)<2)//防止误差突变
//      {
//        g_sSteeringError=last_g_sSteeringError;
//      }
//      last_g_sSteeringError=g_sSteeringError;
//
//       g_sSteeringError=(float)updateDeviation(filter,20);  //滤波

//       if(Circle_flag==2)//判断左右环岛一定时间后打角赋值
//       {
//           g_sSteeringError=Cirle_PWM;//打角偏差
//       }
//       if(abs(g_sSteeringError)>10)
//       {
           MotorDuty2 = (int)PidLocCtrl(&RSpeed_PID, g_sSteeringError);//摄像头
//       }
//       if(MotorDuty2>5000)MotorDuty2=5000;
//       if(MotorDuty2<-5000)MotorDuty2=-5000;
   }
   //电磁方向环控制
   if(open_flag!=0&&motor_flag==2)//没有障碍物才进行
   {
       //ALL_AD=(float)updateDeviation(filter,20);  //滤波
       MotorDuty2 = (int)PidLocCtrl(&ESpeed_PID, ALL_AD);//电磁
   }
    //速度环控制
   if(motor_flag==0||motor_flag==2)
   {
       Target_Speed1=start_V_L;
       Target_Speed2=start_V_L;
       V_error=(float)((start_V_L-((Real_Speed1+Real_Speed2)/2))/20);
       MotorDuty1 = (int)PidIncCtrl(&LSpeed_PID, V_error);

       out_PWML=MotorDuty1+MotorDuty2;
       out_PWMR=MotorDuty1-MotorDuty2;
   }

  //正反转限幅
   if(open_flag==0)
   {
       if(out_PWML>6300)  //7300
       {
           out_PWML=6300;
       }
       if(out_PWMR>6000) //7300
       {
           out_PWMR=6000;
       }
//       if(ECPULSE1>0&&out_PWML<900)  //7300
//       {
//           out_PWML=900;
//       }
//       if(out_PWMR>0&&out_PWMR<900) //7300
//       {
//           out_PWMR=900;
//       }


       if(out_PWML<(-1*6000))  //5500
       {
           out_PWML=(-1*6000);
       }
       if(out_PWMR<(-1*6500))  //6500
       {
           out_PWMR=(-1*6500);
       }
       if(motor_flag==2)
       {
           if(abs(ECPULSE1)==0)  //7300
           {
               out_PWML=-2500;
           }
           if(abs(ECPULSE2)==0)  //7300
           {
               out_PWMR=-2500;
           }
          if(out_PWML<0&&out_PWML>-2000)  //7300
          {
              out_PWML=-2000;
          }
          if(out_PWMR<0&&out_PWMR>-2200) //7300
          {
              out_PWMR=-2200;
          }
       }

   }
   if(open_flag!=0)
   {
//       if(out_PWML>5300)  //7300
//       {
//           out_PWML=5300;
//       }
//       if(out_PWMR>5000) //7300
//       {
//           out_PWMR=5000;
//       }
//
//       if(out_PWML<(-1*5000))  //5500
//       {
//           out_PWML=(-1*5000);
//       }
//       if(out_PWMR<(-1*5500))  //6500
//       {
//           out_PWMR=(-1*5500);
//       }
//       if(motor_flag==2)
//       {
//           if(abs(ECPULSE1)==0)  //7300
//           {
//               out_PWML=-2500;
//           }
//           if(abs(ECPULSE2)==0)  //7300
//           {
//               out_PWMR=-2500;
//           }
//          if(out_PWML<0&&out_PWML>-2000)  //7300
//          {
//              out_PWML=-2000;
//          }
//          if(out_PWMR<0&&out_PWMR>-2000) //7300
//          {
//              out_PWMR=-2000;
//          }
//       }
   }
   //闭环电机输出控制
   if(Obstacle_flag==0
    &&Circle_flag_L!=2&&Circle_flag_L!=4
    &&Circle_flag_R!=2&&Circle_flag_R!=4
    &&Park_flag!=2
    )
   {
       if(out_PWML>=0)
       {
           ATOM_PWM_SetDuty(ATOMPWM5, 1*out_PWML, 17000);
           ATOM_PWM_SetDuty(ATOMPWM4, 0, 17000);
       }
       else if(out_PWML<0)
       {
           ATOM_PWM_SetDuty(ATOMPWM5, 0, 17000);
           ATOM_PWM_SetDuty(ATOMPWM4, -1*out_PWML, 17000);
       }

       if(out_PWMR>=0)
       {
           ATOM_PWM_SetDuty(ATOMPWM7, out_PWMR, 17000);
           ATOM_PWM_SetDuty(ATOMPWM6, 0, 17000);
       }
       else if(out_PWMR<0)
       {
           ATOM_PWM_SetDuty(ATOMPWM7, 0, 17000);
           ATOM_PWM_SetDuty(ATOMPWM6, (-1)*out_PWMR, 17000);
       }
   }
   //停车
   if(motor_flag == 0)
   {
       start_V_L=0;
       Target_Speed1=0;
       Target_Speed2=0;
       MotorDuty1=0;
       MotorDuty2=0;
       out_PWML=0;
       out_PWMR=0;
//      ATOM_PWM_SetDuty(ATOMPWM4, 0, 17000);
//      ATOM_PWM_SetDuty(ATOMPWM5, 0, 17000);
//      ATOM_PWM_SetDuty(ATOMPWM6, 0, 17000);
//      ATOM_PWM_SetDuty(ATOMPWM7, 0, 17000);
   }
    //PIN_Reverse(P15_2);//测试中断时间
}

/*************************************************************************
 *  函数名称：void CCU60_CH1_IRQHandler(void)
 *  功能说明：
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2022年11月28日
 *  备    注：CCU60_CH1使用的中断服务函数
 *************************************************************************/
int meun_flag;
void CCU60_CH1_IRQHandler (void)
{
    /* 开启CPU中断  否则中断不可嵌套 */
    IfxCpu_enableInterrupts();
    // 清除中断标志
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU60, IfxCcu6_InterruptSource_t13PeriodMatch);

    /* 用户代码 */
    meun_flag=1;
    /* 获取编码器值 */
    ECPULSE1= (-1)*ENC_GetCounter(ENC6_InPut_P20_3);  // 电机 母板上编码器2，小车左轮 小车前进为正值
    ECPULSE2= ENC_GetCounter(ENC4_InPut_P02_8); // 电机 母板上编码器1，小车右轮 小车前进为负值
    //陀螺仪获取角速度
    Get_aac();
    //获取电感值
    GetlastAdcValue();
    //定时器延时
    if(motor_flag==1
     //矩形形状
     ||rectangle_flag==1
     //障碍物
     ||Obstacle_flag==1||Obstacle_flag==2||Obstacle_flag==3
     //断路
     ||open_flag==1||open_flag==2
     //左圆环
     ||Circle_flag_L==1||Circle_flag_L==2||Circle_flag_L==3||Circle_flag_L==4||Circle_flag_L==5
     //右圆环
     ||Circle_flag_R==1||Circle_flag_R==2||Circle_flag_R==3||Circle_flag_R==4||Circle_flag_R==5
     //坡道
     ||slope_flag==1||slope_flag==2
     )
    {
        Obstacle_num++;
    }
    if( Park_flag==1||Park_flag==2)
    {
        Park_num++;
    }
    //编码器计数延时
    if(Obstacle_flag==1)
    {
        ECPULSE_Obstacle_L+=ECPULSE1;  //左轮计数
    }
    if(Obstacle_flag==2||open_flag==1||open_flag==2||slope_flag==1)
    {
        ECPULSE_Obstacle_R+=ECPULSE2;  //右轮计数
    }
   //速度滤波
//    Real_Speed1=(float)Left_Low_Pass_Filter(ECPULSE1,Feedback_SpeedL,Feedback_Speed_tmpL);
//    Real_Speed2=(float)Right_Low_Pass_Filter(ECPULSE2,Feedback_SpeedR,Feedback_Speed_tmpR);
    Real_Speed1=(float)(5.2051*ECPULSE1);  //实际转速   转每分钟
    Real_Speed2=(float)(5.2051*ECPULSE2);  //实际转速
//    x++;
    //电压显示
 //   Voltage_Get();

    /* 速度控制 */
//    if (motor_flag==1)
//    {
        //速度转向处理
        //AngleControl();  //确定OUT_PWM的值
        //速度闭环
//        if(x==20)
//        {
//            PIN_Reverse(P15_2);//测试中断时间
//            x=0;


//           Real_Speed1=(float)(5.2051*ECPULSE_L - 1.3837);  //实际转速   转每分钟
//           Real_Speed2=(float)(5.5562*ECPULSE_R - 11.058);  //实际转速   转每分钟

//           Target_Speed1=start_V_L;
//           Target_Speed2=start_V_L;
//
//           V_error=(float)((Target_Speed1-((Real_Speed1+Real_Speed2)/2))/5);
//
//           if(Obstacle_flag==0)
//           {
//               MotorDuty1 = (int)PidIncCtrl(&LSpeed_PID, V_error);
//               if(MotorDuty1>5500)
//               {
//                   MotorDuty1=5500;
//               }
//               if(MotorDuty1<=-5000)
//               {
//                   MotorDuty1=-5000;
//               }
//           }
//       }
//     }
}

/*************************************************************************
 *  函数名称：void CCU61_CH0_IRQHandler(void)
 *  功能说明：
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2022年11月28日
 *  备    注：CCU61_CH0使用的中断服务函数
 *************************************************************************/
void CCU61_CH0_IRQHandler (void)
{
    /* 开启CPU中断  否则中断不可嵌套 */
    IfxCpu_enableInterrupts();

    // 清除中断标志
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU61, IfxCcu6_InterruptSource_t12PeriodMatch);

    /* 用户代码 */
}

/*************************************************************************
 *  函数名称：void CCU61_CH1_IRQHandler(void)
 *  功能说明：
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2022年11月28日
 *  备   注：CCU61_CH1使用的中断服务函数
 *************************************************************************/
extern uint16 MagneticField;
int i_delay=0;
void CCU61_CH1_IRQHandler (void)
{
    //static uint8_t i = 0;
    /* 开启CPU中断  否则中断不可嵌套 */
    IfxCpu_enableInterrupts();

    // 清除中断标志
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU61, IfxCcu6_InterruptSource_t13PeriodMatch);

    /* 用户代码 */


}

/*************************************************************************
 *  函数名称：CCU6_InitConfig CCU6
 *  功能说明：定时器周期中断初始化
 *  参数说明：ccu6    ： ccu6模块            CCU60 、 CCU61
 *  参数说明：channel ： ccu6模块通道  CCU6_Channel0 、 CCU6_Channel1
 *  参数说明：us      ： ccu6模块  中断周期时间  单位us
 *  函数返回：无
 *  修改时间：2020年3月30日
 *  备    注：    CCU6_InitConfig(CCU60, CCU6_Channel0, 100);  // 100us进入一次中断
 *************************************************************************/
void CCU6_InitConfig (CCU6_t ccu6, CCU6_Channel_t channel, uint32 us)
{
    IfxCcu6_Timer_Config timerConfig;

    Ifx_CCU6 * module = IfxCcu6_getAddress((IfxCcu6_Index) ccu6);

    uint8 Index = ccu6 * 2 + channel;

    uint32 period = 0;

    uint64 clk = 0;

    /* 关闭中断 */
    boolean interrupt_state = disableInterrupts();

    IfxCcu6_Timer_initModuleConfig(&timerConfig, module);

    clk = IfxScuCcu_getSpbFrequency();

    /* 设置时钟频率  */
    uint8 i = 0;
    while (i++ < 16)
    {
        period = (uint32) (clk * us / 1000000);
        if (period < 0xffff)
        {
            break;
        }
        else
        {
            clk = clk / 2;
        }
    }
    switch (channel)
    {
        case CCU6_Channel0 :
            timerConfig.timer = IfxCcu6_TimerId_t12;
            timerConfig.interrupt1.source = IfxCcu6_InterruptSource_t12PeriodMatch;
            timerConfig.interrupt1.serviceRequest = IfxCcu6_ServiceRequest_1;
            timerConfig.base.t12Frequency = (float) clk;
            timerConfig.base.t12Period = period;                                  // 设置定时中断
            timerConfig.clock.t12countingInputMode = IfxCcu6_CountingInputMode_internal;
            timerConfig.timer12.counterValue = 0;
            timerConfig.interrupt1.typeOfService = Ccu6IrqVectabNum[ccu6];
            timerConfig.interrupt1.priority = Ccu6IrqPriority[Index];
            break;

        case CCU6_Channel1 :
            timerConfig.timer = IfxCcu6_TimerId_t13;
            timerConfig.interrupt2.source = IfxCcu6_InterruptSource_t13PeriodMatch;
            timerConfig.interrupt2.serviceRequest = IfxCcu6_ServiceRequest_2;
            timerConfig.base.t13Frequency = (float) clk;
            timerConfig.base.t13Period = period;
            timerConfig.clock.t13countingInputMode = IfxCcu6_CountingInputMode_internal;
            timerConfig.timer13.counterValue = 0;
            timerConfig.interrupt2.typeOfService = Ccu6IrqVectabNum[ccu6];
            timerConfig.interrupt2.priority = Ccu6IrqPriority[Index];
            break;
    }

    timerConfig.trigger.t13InSyncWithT12 = FALSE;

    IfxCcu6_Timer Ccu6Timer;

    IfxCcu6_Timer_initModule(&Ccu6Timer, &timerConfig);

    IfxCpu_Irq_installInterruptHandler((void*) Ccu6IrqFuncPointer[Index], Ccu6IrqPriority[Index]);          // 配置中断函数和中断号

    restoreInterrupts(interrupt_state);

    IfxCcu6_Timer_start(&Ccu6Timer);
}

/*************************************************************************
 *  函数名称：CCU6_DisableInterrupt(CCU6_t ccu6, CCU6_Channel_t channel)
 *  功能说明：停止CCU6通道中断
 *  参数说明：ccu6    ： ccu6模块            CCU60 、 CCU61
 *  参数说明：channel ： ccu6模块通道  CCU6_Channel0 、 CCU6_Channel1
 *  函数返回：无
 *  修改时间：2020年5月6日
 *  备    注：
 *************************************************************************/
void CCU6_DisableInterrupt (CCU6_t ccu6, CCU6_Channel_t channel)
{
    Ifx_CCU6 * module = IfxCcu6_getAddress((IfxCcu6_Index) ccu6);
    IfxCcu6_clearInterruptStatusFlag(module, (IfxCcu6_InterruptSource) (7 + channel * 2));
    IfxCcu6_disableInterrupt(module, (IfxCcu6_InterruptSource) (7 + channel * 2));

}

/*************************************************************************
 *  函数名称：CCU6_DisableInterrupt(CCU6_t ccu6, CCU6_Channel_t channel)
 *  功能说明：使能CCU6通道中断
 *  参数说明：ccu6    ： ccu6模块            CCU60 、 CCU61
 *  参数说明：channel ： ccu6模块通道  CCU6_Channel0 、 CCU6_Channel1
 *  函数返回：无
 *  修改时间：2020年5月6日
 *  备    注：
 *************************************************************************/
void CCU6_EnableInterrupt (CCU6_t ccu6, CCU6_Channel_t channel)
{
    Ifx_CCU6 * module = IfxCcu6_getAddress((IfxCcu6_Index) ccu6);
    IfxCcu6_clearInterruptStatusFlag(module, (IfxCcu6_InterruptSource) (7 + channel * 2));
    IfxCcu6_enableInterrupt(module, (IfxCcu6_InterruptSource) (7 + channel * 2));

}


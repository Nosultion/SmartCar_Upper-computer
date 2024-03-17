/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技TC3xxxA核心板
【编    写】chiusir
【E-mail】chiusir@163.com
【软件版本】V1.1 版权所有，单位使用请先联系授权
【最后更新】2020年10月28日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【dev.env.】AURIX Development Studio1.2.2及以上版本
【Target 】 TC3xxxA
【Crystal】 20.000Mhz
【SYS PLL】 300MHz
________________________________________________________________
基于iLLD_1_0_1_11_0底层程序,
使用例程的时候，建议采用没有空格的英文路径，
=================================================================
程序配套视频地址：https://space.bilibili.com/95313236
________________________________________________________________
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef __LQ_BLDC_H_
#define __LQ_BLDC_H_
#include "Platform_Types.h"
#include "stdint.h"


//#define TRUE 1
//#define FALSE 0

typedef struct _BLDC_MANAGER
{
  uint8 step;
  uint16 counts;
  int16_t duty;
  uint8 Electricity_flag;//上电检测电流标志
}BLDC_MANAGER, *PBLDC_MANAGER;
extern BLDC_MANAGER g_sBLDCMag;


void BLDC_MotorInit(uint16 freq);//无刷电机引脚初始化
void BLDC_HallInit(void );//霍尔初始化
void TestMotorBLDC (void);//无刷电机测试函数
void LQ_BLDCShow(void);//无刷电机相关数据展示

float PID_velocity(float error);//pid控速介绍
void LQ_BLDCCurrent_detection(void);//电流检测，保护电路
void LQ_BLDCCorotation(void);//无刷电机正转
void LQ_BLDCReversal(void);//无刷电机反转
void LQ_BLDCStop(void);//关闭无刷电机，停止转动
void BLDC_Motor_Hall_Run(int16_t motor_duty);//无刷电机驱动函数

#endif/* 0_APPSW_TRICORE_APP_LQ_ASC_H_ */

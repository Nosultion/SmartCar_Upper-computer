#include "LQ_GPIO_Infrared.h"

#include "../Driver/LQ_GPIO.h"
#include "../Driver/LQ_STM.h"
#include "LQ_GPIO_LED.h"
#include "LQ_TFT18.h"

/*************************************************************************
*  函数名称：GPIO_Infrared_Init(void)
*  功能说明：红外传感器初始化函数
*  参数说明：无
*  函数返回：无
*  修改时间：2022年11月30日
*  备   注：红外传感器BUZZ所用P33.13初始化（舵机1口）
*************************************************************************/
void GPIO_Infrared_Init(void)
{
      // 初始化,输入口，高电平
    PIN_InitConfig(INFRARED, PIN_MODE_INPUT, 1);

}
/*************************************************************************
*  函数名称：unsigned char Infrared_Read(void)
*  功能说明：读取红外情况
*  参数说明：无
*  函数返回：红外检测状态，0/1
*  修改时间：2022年11月30日
*  备    注：
*************************************************************************/
unsigned char Infrared_Read(void)
{
    return PIN_Read(INFRARED);  //
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技TC264DA核心板
【编    写】YXLZYF/chiusir
【E-mail  】2623387606@qq.com
【软件版本】V1.1 版权所有，单位使用请先联系授权
【最后更新】2022年11月29日
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

使用例程的时候，建议采用没有空格的英文路径，
除了CIF为TC264DA独有外，其它的代码兼容TC264D
本库默认初始化了EMEM：512K，如果用户使用TC264D，注释掉EMEM_InitConfig()初始化函数。
工程下\Libraries\iLLD\TC26B\Tricore\Cpu\CStart\IfxCpu_CStart0.c第164行左右。
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "LQ_GPIO_BUZZ.h"
#include "../Driver/LQ_GPIO.h"
#include "../Driver/LQ_STM.h"

/*************************************************************************
*  函数名称：GPIO_BUZZ_Init(void)
*  功能说明：GPIO初始化函数 BUZZ所用P33.8初始化
*  参数说明：无
*  函数返回：无
*  修改时间：2022年11月29日
*  备    注：
*************************************************************************/
void GPIO_BUZZ_Init(void)
{
      // 初始化,输入口，低电平
      PIN_InitConfig(BUZZ, PIN_MODE_OUTPUT, 0);
}

/*************************************************************************
*  函数名称：void BUZZ_Ctrl(BUZZn_e BUZZno, BUZZs_e sta)
*  功能说明：BUZZ控制
*  参数说明：BUZZn_e BUZZno编号,BUZZs_e sta状态响/不响/翻转
*  函数返回：无
*  修改时间：2022年11月29日
*  备    注：
*************************************************************************/
void BUZZ_Ctrl(BUZZn_e BUZZno, BUZZs_e sta)
{
    switch(BUZZno)
    {
    case BUZZ0:
      if(sta==BUZZ_ON)       PIN_Write(BUZZ,1);
      else if(sta==BUZZ_OFF) PIN_Write(BUZZ,0);
      else if(sta==BUZZ_RVS) PIN_Reverse(BUZZ);
    break;

    default:
    break;
    }
}
/*************************************************************************
*  函数名称：void BUZZ_Work(uint8_t OpeningRoad_Flag,uint8_t L_CircleFlag ,uint8_t R_CircleFlag,uint8_t Barrier_Flag)
*  功能说明：元素反馈函数
*  参数说明：
*  函数返回：无
*  修改时间：2022年12月1日
*  备    注：
*************************************************************************/
void BUZZ_Work(uint8_t *OpeningRoad_Flag, uint8_t *L_CircleFlag, uint8_t *R_CircleFlag, uint8_t *Barrier_Flag)
{


}


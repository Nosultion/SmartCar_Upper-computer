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
测试ADC函数，一般用几个MIC就开启几个ADC即可。
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "../Driver/LQ_ADC.h"

#include <stdio.h>

#include "../Driver/LQ_STM.h"
#include "../Driver/LQ_UART.h"
#include "LQ_GPIO_LED.h"
#include "LQ_OLED096.h"
#include "LQ_TFT18.h"

/*************************************************************************
*  函数名称：void Test_ADC_OLED(void)
*  功能说明：ADC采集测试
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：   ADC悬空不为零，随机的
*************************************************************************/
void Test_ADC_OLED(void)
{
	unsigned long i;
    char txt[16];

    ADC_InitConfig(ADC0, 80000); //初始化
    ADC_InitConfig(ADC1, 80000);
    ADC_InitConfig(ADC2, 80000);
    ADC_InitConfig(ADC3, 80000);
    ADC_InitConfig(ADC4, 80000);
    ADC_InitConfig(ADC5, 80000);
    ADC_InitConfig(ADC6, 80000);
    ADC_InitConfig(ADC7, 80000);

    OLED_Init();
    OLED_P6x8Str(0,0,"adc test");
    while(1)
    {
    	unsigned long nowTime = STM_GetNowUs(STM0);//采样时间开始
    	for( i = 0; i < 10000; i++)
    	{
    		ADC_Read(ADC0);
    	}
    	int time = STM_GetNowUs(STM0) - nowTime;   //采样结束
    	sprintf(txt, "time : %d", time);           //转换为字符串
    	OLED_P6x8Str(0,3,txt);                      //显示采样时间
    }
}
void Test_ADC_TFT(void)
{
    //unsigned long i;
    char txt[16];
    int v=0;

    ADC_InitConfig(ADC0, 80000); //初始化
    ADC_InitConfig(ADC1, 80000);
    ADC_InitConfig(ADC2, 80000);
    ADC_InitConfig(ADC3, 80000);
    ADC_InitConfig(ADC4, 80000);
    ADC_InitConfig(ADC5, 80000);
    ADC_InitConfig(ADC6, 80000);
    ADC_InitConfig(ADC7, 80000);

    TFTSPI_Init(1);               // TFT1.8初始化0:横屏  1：竖屏
    TFTSPI_CLS(u16BLACK);         // 清屏
    TFTSPI_P8X16Str(0,0,"ADC TFT",u16RED,u16BLUE);// 字符串显示
    while(1)
    {
        /*
        unsigned long nowTime = STM_GetNowUs(STM0);//采样时间开始
        for( i = 0; i < 10000; i++)
        {
            ADC_Read(ADC0);
        }
        int time = STM_GetNowUs(STM0) - nowTime;    //采样结束
        sprintf(txt, "time : %d", time);            //转换为字符串
        TFTSPI_P8X16Str(0,3,txt,u16WHITE,u16BLACK); //字符串显示
        */
        v=ADC_Read(ADC0);
        sprintf(txt, "ADC0: %05d", v);            //转换为字符串
        TFTSPI_P8X16Str(0,4,txt,u16WHITE,u16BLACK); //字符串显示
        v=ADC_Read(ADC1);
        sprintf(txt, "ADC1: %05d", v);            //转换为字符串
        TFTSPI_P8X16Str(0,5,txt,u16WHITE,u16BLACK); //字符串显示
        v=ADC_Read(ADC2);
        sprintf(txt, "ADC2: %05d", v);            //转换为字符串
        TFTSPI_P8X16Str(0,6,txt,u16WHITE,u16BLACK); //字符串显示
        v=ADC_Read(ADC3);
        sprintf(txt, "ADC3: %05d", v);            //转换为字符串
        TFTSPI_P8X16Str(0,7,txt,u16WHITE,u16BLACK); //字符串显示
        v=ADC_Read(ADC3) + ADC_Read(ADC2) + ADC_Read(ADC1) + ADC_Read(ADC0);
        sprintf(txt, "ALL1: %05d", v);            //转换为字符串
        TFTSPI_P8X16Str(0,8,txt,u16WHITE,u16BLACK); //字符串显示
//        delayms(200);              //延时等待
    }
}
/*************************************************************************
*  函数名称：void Test_Beacon7mic(void)
*  功能说明：采集测试6+1麦克风阵列
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：    ADC悬空不为零
*************************************************************************/
void Test_ADC_7mic(void)
{
    char txt[200];
    unsigned int MICVmin[8];

    ADC_InitConfig(ADC0, 80000); //初始化
    ADC_InitConfig(ADC1, 80000); //初始化
    ADC_InitConfig(ADC2, 80000); //初始化
    ADC_InitConfig(ADC3, 80000); //初始化
    ADC_InitConfig(ADC4, 80000); //初始化
    ADC_InitConfig(ADC5, 80000); //初始化
    ADC_InitConfig(ADC6, 80000); //初始化
    ADC_InitConfig(ADC7, 80000); //初始化   如果使用龙邱母板  则测分压后的电池电压，具体可以看母板原理图
    delayms(500);                //延时等待
	UART_PutStr(UART0,"\n-------------------------------------------------\n");//分界线
	while(1)
	{
		MICVmin[0]=ADC_Read(ADC0);
		MICVmin[1]=ADC_Read(ADC1);
		MICVmin[2]=ADC_Read(ADC2);
		MICVmin[3]=ADC_Read(ADC3);
		MICVmin[4]=ADC_Read(ADC4);
		MICVmin[5]=ADC_Read(ADC5);
		MICVmin[6]=ADC_Read(ADC6);
		MICVmin[7]=ADC_Read(ADC7);

		//转换结果为字符串，串口发送或者屏幕显示
		sprintf(txt,"[0]:%05d  [1]:%05d  [2]:%05d  [3]:%05d  [4]:%05d  [5]:%05d  [6]:%05d  [7]:%05d  \r\n",MICVmin[0],MICVmin[1],MICVmin[2],MICVmin[3],MICVmin[4],MICVmin[5],MICVmin[6],MICVmin[7]);
		//串口发送到上位机
		UART_PutStr(UART0,txt);//AN0--AN7十进制数据输出到上位机


    	LED_Ctrl(LED0,RVS);        //电平翻转,LED闪烁
		delayms(500);              //延时等待
	}
}
//

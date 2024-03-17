/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技TC264DA核心板
【编    写】chiusir
【E-mail】chiusir@163.com
【软件版本】V1.1 版权所有，单位使用请先联系授权
【最后更新】2021年6月16日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【dev.env.】AURIX Development Studio1.4及以上版本
【Target 】 TC264DA/TC264D
【Crystal】 20.000Mhz
【SYS PLL】 200MHz
________________________________________________________________
基于iLLD_1_0_1_11_0底层程序,

使用例程的时候，建议采用没有空格的英文路径，
除了CIF为TC264DA独有外，其它的代码兼容TC264D
本库默认初始化了EMEM：512K，如果用户使用TC264D，注释掉EMEM_InitConfig()初始化函数。
工程下\Libraries\iLLD\TC26B\Tricore\Cpu\CStart\IfxCpu_CStart0.c第164行左右。
=================================================================
程序配套视频地址：https://space.bilibili.com/95313236
=================================================================
摄像头接口                  龙邱神眼或者OV7725模块
● 数据端口：P02.0-P02.7口，共8位，接摄像头的数据端口；
● 时钟像素：外部中断第0组：P00_4；
● 场信号：外部中断第3组：P15_1；
-----------------------------------------------------------------
推荐GPT12模块，共可以实现5路正交解码增量编码器（兼容带方向编码器）信号采集，任意选择四路即可；
P33_7, P33_6   龙邱TC母板编码器1
P02_8, P33_5   龙邱TC母板编码器2
P10_3, P10_1   龙邱TC母板编码器3
P20_3, P20_0   龙邱TC母板编码器4
-----------------------------------------------------------------
电感电压采集模块或者麦克风模块
推荐使用AN0-7，共八路ADC，可以满足chirp声音信号及电磁车电感电压采集；
AN0-3          龙邱TC接四个麦克风模块或者电感
-----------------------------------------------------------------
默认电机接口
使用GTM模块，ATOM四个通道可产生4*8共32路PWM，而且各自频率和占空比可调，推荐使用ATOM0的0-7通道；
第一组双路全桥驱动
P23_1         龙邱TC母板MOTOR1_P
P32_4         龙邱TC母板MOTOR1_N
P21_2         龙邱TC母板MOTOR2_P
P22_3         龙邱TC母板MOTOR2_N
第二组双路全桥驱动
P21_4         龙邱TC母板MOTOR3_P
P21_3         龙邱TC母板MOTOR3_N
P20_8         龙邱TC母板MOTOR4_P
P21_5         龙邱TC母板MOTOR4_N
-----------------------------------------------------------------
默认舵机接口
使用GTM模块，ATOM四个通道可产生4*8共32路PWM，而且各自频率和占空比可调，推荐使用ATOM2；
P33_10        龙邱TC母板舵机1
P33_13        龙邱TC母板舵机2
-----------------------------------------------------------------
 默认屏幕显示接口，用户可以使用TFT或者OLED模块
TFTSPI_CS     P20_14     龙邱TC母板 CS管脚 默认拉低，可以不用
TFTSPI_SCK    P20_11     龙邱TC母板 SPI SCK管脚
TFTSPI_SDI    P20_10     龙邱TC母板 SPI MOSI管脚
TFTSPI_DC     P20_12     龙邱TC母板 D/C管脚
TFTSPI_RST    P20_13     龙邱TC母板 RESET管脚
-----------------------------------------------------------------
OLED_CK       P20_14     龙邱TC母板OLED CK管脚
OLED_DI       P20_11     龙邱TC母板OLED DI管脚
OLED_RST      P20_10     龙邱TC母板OLED RST管脚
OLED_DC       P20_12     龙邱TC母板OLED DC管脚
OLED_CS       P20_13     龙邱TC母板OLED CS管脚 默认拉低，可以不用
----------------------------------------------------------------
默认按键接口
KEY0p         P22_0      龙邱TC母板上按键0
KEY1p         P22_1      龙邱TC母板上按键1
KEY2p         P22_2      龙邱TC母板上按键2
DSW0p         P33_9      龙邱TC母板上拨码开关0
DSW1p         P33_11     龙邱TC母板上拨码开关1
-----------------------------------------------------------------
默认LED接口
LED0p         P10_6      龙邱TC母板核心板上LED0 翠绿
LED1p         P10_5      龙邱TC母板核心板上LED1 蓝灯
LED2p         P20_6      龙邱TC母板上LED0
LED3p         P20_7      龙邱TC母板上LED1
-----------------------------------------------------------------
默认蜂鸣器接口
BEEPp         P33_8      龙邱TC母板上蜂鸣器接口
-----------------------------------------------------------------
定时器
有两个CCU6模块  每个模块有两个独立定时器  触发定时器中断
推荐使用CCU6模块，STM用作系统时钟或者延时；
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
IfxCpu_mutexLock mutexCpu0InitIsOk = 1;   // CPU0 初始化完成标志位
volatile char mutexCpu0TFTIsOk=0;         // CPU1 0占用/1释放 TFT


int16_t j = 0;
unsigned char Show_TFT_Flag=1;   //拨码按键
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
extern uint8_t Circle_flag_L;//左圆环标志位
extern uint8_t Circle_flag_R;//右圆环标志位
extern uint8_t cross_flag;
extern uint8_t rectangle_flag;
extern int Obstacle_turn_flag;//路障左右拐标志位
extern int motor_turn_flag;//发车左右拐标志位
extern int Speed_turn_flag;//速度模式标志位

extern volatile int ECPULSE_open;
extern volatile int ECPULSE_Obstacle_L;
extern volatile int ECPULSE_Obstacle_R;

extern int Obstacle_num,Circle_num;
extern int ADC_Value[4];       //电磁的采集值
extern float Cirle_PWM;
extern int X_minL,X_minR,X_min;
extern uint16 MagneticField;
extern int meun_flag;
extern float Anglex,Angley,Anglez;//陀螺仪角度

/*************************************************************************
*  函数名称：int core0_main (void)
*  功能说明：CPU0主函数
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：
*************************************************************************/
int core0_main (void)
{
//    unsigned char res;

	// 关闭CPU总中断
	IfxCpu_disableInterrupts();

	// 关闭看门狗，如果不设置看门狗喂狗需要关闭
	IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
	IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

	// 读取总线频率
	g_AppCpu0.info.pllFreq = IfxScuCcu_getPllFrequency();
	g_AppCpu0.info.cpuFreq = IfxScuCcu_getCpuFrequency(IfxCpu_getCoreIndex());
	g_AppCpu0.info.sysFreq = IfxScuCcu_getSpbFrequency();
	g_AppCpu0.info.stmFreq = IfxStm_getFrequency(&MODULE_STM0);

    TFTSPI_Init(1);               // TFT2.0初始化0:横屏  1：竖屏
    TFTSPI_CLS(u16BLACK);         // 清屏
//    TFTSPI_Show_Logo(0,37);       // 显示龙邱LOGO
//    TFTSPI_P16x16Str(0,0,(unsigned char*)"北京龙邱智能科技",u16RED,u16BLUE);// 字符串显示

    EncInit();                    // 编码器初始化
    GPIO_KEY_Init();              // 按键初始化
    GPIO_LED_Init();              // LED灯所用P10.6和P10.5初始化
    ADC_InitConfig(ADC7, 80000);  //初始化、如果使用龙邱母板、则测分压后的电池电压，具体可以看母板原理图
    GPIO_BUZZ_Init();             //蜂鸣器初始化
    GPIO_Infrared_Init();         //红外传感器初始化
    TFTSPI_CLS(u16BLACK);         // 清屏
    BUZZ_Ctrl(BUZZ0,BUZZ_ON);     //初始化鸣叫
	// 串口P14.0管脚输出,P14.1输入，波特率115200
	UART_InitConfig(UART0_RX_P14_1,UART0_TX_P14_0, 115200);
	UART_PutStr(UART0,"UART0 Test \r\n");       //发送字符串到上位机
	// 开启CPU总中断
	IfxCpu_enableInterrupts();

	// 通知CPU1，CPU0初始化完成
	IfxCpu_releaseMutex(&mutexCpu0InitIsOk);
	// 切记CPU0,CPU1...不可以同时开启屏幕显示，否则冲突不显示
	mutexCpu0TFTIsOk=0;          // CPU0： 0占用/1释放 TFT
    // 以下测试函数，内建死循环，用户可调用所用模块的初始化及读写函数来实现自己的任务______________________________________________________________________________
//	  Test_LQDMP();          //PASS，测试DMP对6050陀螺仪的读取，并在TFT1.8寸彩屏上面显示出来
//	  Test_ADC_OLED();       //PASS,测试ADC采样时间  OLED上显示 ADC采样10K次时间
//	  Test_ADC_TFT();        //PASS,测试ADC采样时间 TFT上显示 ADC采样10K次时间
//	  Test_ADC_7mic();       //PASS,测试ADC\UART0、STM延时和闪灯，通过UART0打印 AN0--AN7共8个通道ADC转换数据
//    LQ_Atom_Motor_8chPWM();//PASS,测试GTM_ATOM生成不同频率下的8路PWM
//    LQ_ATom_Servo_2chPWM();//PASS,测试GTM_ATOM、STM延时和闪灯，P33.10和P33.13作为ATOM输出口控制舵机
//    LQ_Tom_Servo_2chPWM(); //PASS,测试GTM_TOM、STM延时和闪灯，P33.10和P33.13作为TOM输出口控制舵机
//    Test_GPIO_Extern_Int();//PASS,测试外部第1组中断P15.8，P10.6和P10.5闪灯
//    Test_GPIO_LED();       //PASS,测试GPIO，P10.6和P10.5闪灯
//    Test_GPIO_KEY();       //PASS,测试外部按键输入，P22.0--2   按下按键 LED亮
//    Test_ComKEY_Tft();     //PASS,测试外部组合按键输入并TFT1.8显示，P22.0--2
//    LQ_GPT_4mini512();     //PA SS,测试编码器正交解码,OLED和UART输出
//    LQ_GPT_4mini512TFT();  //PASS,测试编码器正交解码,TFT1.8和UART输出
//    Test_OLED();           //PASS,测试OLED0.96屏使用P20.14--10，显示字符串及动态数据
//    LQ_STM_Timer();        //PASS,测试定时中断、闪灯
//    Test_TFT18();          //PASS,测试TFT1.8彩屏使用P20.14--10，显示字符串及动态数据
//    LQ_TIM_InputCature();  //PASS,测试GTM_TOM_TIM、P20_9作为PWM输出口，P15_0作为TIM输入口，两者短接后，串口P14.0发送到上位机
//    Test_Bluetooth();      //PASS,测试UART0(P14.0RX/P14.1TX)，
//    Test_EEPROM();         //PASS,测试内部EEPROM擦写功能  OLED提示是否写入成功
//    Test_Vl53();           //PASS,测试VL53  IIC接线   P13_1接SCL  P13_2接SDA OLED显示原始数据
//    Test_9AX();            //PASS,测试龙邱九轴 IIC接线   P13_1接SCL  P13_2接SDA OLED显示原始数据
//    Test_MPU6050();        //PASS,测试MPU6050或者ICM20602 IIC接线   P13_1接SCL  P13_2接SDA OLED显示原始数据
//    Test_ICM20602();       //PASS,测试ICM20602 SPI接线   P15_8接SCL  P15_5接SDA  P15_7接SA  P15_2接CS OLED显示原始数据

//    Test_SoftFft();        //PASS,测试ILLD库的软件FFT函数
//    Test_FFT();            //PASS,测试硬件FFT  注意需要芯片后缀带DA的才有硬件FFT功能
//    TestMotor();           //PASS,测试四路电机，通过宏定义选择不同的驱动板
//	  TestEncoder();         //PASS,测试两路编码器
//	  Test_IIC_Gyro();       //PASS,测试通过IIC来读取6050陀螺仪
//	  TestServo();           //PASS,测试闪灯，P33.10和P33.13作为TOM输出口控制舵机
	//PID参数设置
	PidInit(&LSpeed_PID);       //pid初始化
	PidInit(&RSpeed_PID);
	PidInit(&ESpeed_PID);
	Pid_Value();                //pid参数设置

	/*陀螺仪初始化  */
	IIC_Init();                 //IIC初始化
//	delayms(100);
//	res = Gyro_Chose();         //判断陀螺仪型号
//	LQ_DMP_Init();              //DMP初始化

//    if(LQ9AX_Init())
//    {
//        TFTSPI_P8X16Str(1,0,"Test Fail",u16WHITE,u16BLACK);
//#pragma warning 557         // 屏蔽警告
//        while (1);
//#pragma warning default     // 打开警告
//    }

    //  /* 摄像头初始化 */
     CAMERA_Init(50);
	/*ADC初始化*/
	ADC_InitConfig(ADC0, 80000);
	ADC_InitConfig(ADC1, 80000);
	ADC_InitConfig(ADC2, 80000);
	ADC_InitConfig(ADC3, 80000);

	//开启CCU6定时器
//	CCU6_InitConfig(CCU60, CCU6_Channel1, 5*1000);  // 5ms进入一次中断  获取编码值、电机控制和转向控制定时中断
//    CCU6_InitConfig(CCU60, CCU6_Channel0,16*1000);  // 16ms  方向环 进入一次中断  元素处理定时中断
//	CCU6_InitConfig(CCU61, CCU6_Channel1, 5*1000);  // 30ms进入一次中断 LED0闪烁&&蜂鸣器警报 电磁采集

	TFTSPI_Init(1);
	TFTSPI_CLS(u16YELLOW);   //黄色屏幕
	MotorInit();                  // 电机初始化，不放在前面上电电机会疯转

	//PIN_InitConfig(P15_2, PIN_MODE_OUTPUT, 0);//测试中断时间引脚
	 BUZZ_Ctrl(BUZZ0,BUZZ_OFF);//初始化结束
	 Test_CAMERA();         //PASS,测试龙邱神眼摄像头并在屏幕上显示  LQ_CAMERA.h 中选择屏幕
    while (1)	//主循环
    {
        //电机车轮测试
//        ATOM_PWM_SetDuty(ATOMPWM5, K_adjust, 17000);
//        ATOM_PWM_SetDuty(ATOMPWM4, 0, 17000);
//
//        ATOM_PWM_SetDuty(ATOMPWM7, K_adjust, 17000);
//        ATOM_PWM_SetDuty(ATOMPWM6, 0, 17000);

       //蜂鸣器
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
//            //测试识别元素鸣叫
//            BUZZ_Ctrl(BUZZ0,BUZZ_ON);
//       }
//       else
//       {
//           BUZZ_Ctrl(BUZZ0,BUZZ_OFF);
//       }

       //串口调试
       UART_Test();
       //按键控制屏幕信息显示
       menu();
       //发车
       if(PIN_Read(P13_0)==0)
       {
           motor_turn_flag=0;
           Anglez=0;   //陀螺仪积分置0
           Anglex=0;
           Angley=0;
           Obstacle_num=0;
           motor_flag=1;
       }
       if(PIN_Read(P15_2)==0)
       {
           motor_turn_flag=1;
           Anglez=0;   //陀螺仪积分置0
           Anglex=0;
           Angley=0;
           Obstacle_num=0;
           motor_flag=1;
       }
       //电磁出赛道停车
       if(MagneticField<1000&&Obstacle_flag==0&&(motor_flag==2||motor_flag==0))
       {
          start_V_L=0;
          motor_flag=0;
          BUZZ_Ctrl(BUZZ0,BUZZ_OFF);
       }

       //按键检测
       //key_control();
       //屏幕显示
       //TFT_Show_Camera_Info();
//       if(Show_TFT_Flag==1||Show_TFT_Flag==2)
//       {
//           TFT_show();
//       }
//       else if(Show_TFT_Flag==4)
//       {
//          TFTSPI_Road(0, 0, LCDH, LCDW,Image_Use_BLACK);
//         // TFTSPI_BinRoadSide(ImageSide);          //左右边线和中线
//       }
//       else if(Show_TFT_Flag==3)
//       {
//         TFTSPI_Road(0, 0, LCDH, LCDW,Image_Data);
//       }

    }
}

/*******************6050DMP读取测试函数*****************************/
void Test_LQDMP(void)
{

    unsigned char res;
    char txt[30];
    /*屏幕初始化*/
    TFTSPI_Init(1);               // TFT2.0初始化0:横屏  1：竖屏
    TFTSPI_CLS(u16BLACK);         // 清屏
    /*陀螺仪初始化  */
    //IIC_Init();                 //IIC初始化//IIC初始化
    delayms(100);
    res = Gyro_Chose();           //判断陀螺仪型号

    sprintf(txt,"%-3d",res);
    TFTSPI_P6X8Str(0, 0, txt, u16RED, u16BLUE);
    TFTSPI_CLS(u16BLACK);         // 清屏
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


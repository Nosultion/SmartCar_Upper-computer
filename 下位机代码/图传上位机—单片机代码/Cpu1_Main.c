/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技TC264DA核心板
【编    写】chiusir
【E-mail】chiusir@163.com
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
● 场信号：    外部中断第3组：P15_1；
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
定时器
有两个CCU6模块  每个模块有两个独立定时器  触发定时器中断
推荐使用CCU6模块，STM用作系统时钟或者延时；
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include <IfxCpu.h>
#include <IfxScuWdt.h>
#include <Platform_Types.h>
#include "LQ_ImageProcess.h"
#include "src/APP/LQ_TFT2.h"
#include "src/Main/Main.h"
#include "../APP/LQ_CAMERA.h"
#include "LQ_DMA.h"
#include "LQ_CCU6.h"
#include "LQ_TFT18.h"
#include "TFT_Key_control.h"
#include "Image_Process.h"
#include "LQ_UART.h"
#include "LQ_GPIO_KEY.h"

// 定时器 5ms和50ms标志位
volatile uint8 cpu1Flage5ms = 0;
volatile uint8 cpu1Flage50ms = 0;

extern int ROAD_MAIN_ROW;
extern unsigned char Show_TFT_Flag;

extern float g_sSteeringError;
char txt_main1[16];
extern volatile sint16 Error_high;
extern volatile sint16 high;

extern uint8_t leftup[2] ;
extern uint8_t rightup[2] ;
extern uint8_t leftup_1[2];
extern uint8_t rightup_1[2] ;

extern unsigned short THRESHOLD;

int yuzhi=35;
extern int min_tag;
extern uint8_t motor_flag;
extern int out_PWML,out_PWMR;
extern float K_Pid_R;
extern float K_adjust;
extern int ipts0[160][2],ipts1[160][2];
extern int stepL,stepR;
extern int L_pointX[2],L_pointY[2],R_pointX[2],R_pointY[2],L_point_num,R_point_num;
extern char TXT;
extern int thres;
int core1_main (void)
{
    // 开启CPU总中断
    IfxCpu_enableInterrupts();

    // 关闭看门狗
    IfxScuWdt_disableCpuWatchdog (IfxScuWdt_getCpuWatchdogPassword ());

    // 等待CPU0 初始化完成
    while(!IfxCpu_acquireMutex(&mutexCpu0InitIsOk));

    // 所有含有中断的测试都默认在CPU0中执行，如果需要用CPU1请参考龙邱B站视频。
    // 程序配套视频地址：https://space.bilibili.com/95313236




    while(1)//主循环
    {
//      UART_PutChar(UART0, 100);  //帧头
//      if (Camera_Flag == 2)
//      {
//          Camera_Flag = 0;
//
//          //画120x160黑边框
//         for (int j = 0; j < LCDH; j++)     //神眼宽188
//         {
//             Image_Data[j][14] =70;
//             Image_Data[j][174] = 70;
//         }
//         //显示黑色背景，也用来存放种子生长走过的路径 同时也删除上一次存放的路径
//          for (int i = 0; i < IMAGEH; i ++)          //神眼高 120
//          {
//              for (int j = 14; j < IMAGEW-14+1; j ++)     //神眼宽188
//              {
//                  Image_Use_BLACK[i][j] = 0;
//                  //二值化
//                  if (Image_Data[i][j] > (unsigned char)thres) //数值越大，显示的内容越多，较浅的图像也能显示出来
//                      Bin_Image[i][j] = 255;
//                  else
//                      Bin_Image[i][j] = 0;
//
//                  //printf("%d",Bin_Image[i][j]);
//              }
//          }
//          //上海交大  种子生长法
//          //process_image(Image_Data);
//          //图像压缩
//          compressImage(Bin_Image,image_min);//图像压缩
//
//      }
//
//      UART_PutChar(UART0, 200);  //帧尾
    }

          //拐点坐标清除
//          L_pointX[0]=0;L_pointY[1]=0;
//          R_pointX[0]=0;R_pointY[1]=0;
//          L_point_num=0;
//          R_point_num=0;


          /* 提取部分使用的数据 */
 //         Get_Use_Image();
          /* 清除摄像头采集完成标志位  如果不清除，则不会再次采集数据 */

//         逐飞
//          compressImage(Image_Use,image_min);//图像压缩
//          //边线提取
//          findMaxDifference(image_min,MAX);
//          find_sideimage(image_min,MAX,ImageSide);//提取左右边线
//          find_upsideimage(image_min,UpdowmSide);//提取上边线
          //显示原始图像
          //TFT1.8动态显示摄像头图像
          //TFTSPI_Road(0, 0, LCDH, LCDW, (unsigned char *)Image_Use);

//        }


//              for(int i = 0; i <stepR; i++)
//              {
//                  TFTSPI_Draw_Dot((unsigned char)ipts1[i][0],(unsigned char)ipts1[i][1], u16GREEN);//画列数、行数、颜色
//              }
//            TFTSPI_Road(0, 0, 80, 120,(unsigned char *)image_min);
//            TFTSPI_BinRoadSide(ImageSide);          //左右边线和中线
//            TFTSPI_BinRoad_UpdownSide(UpdowmSide);  //上边线
//              TFTSPI_Draw_Line(80, 0, 80, 120, u16RED);                       //显示中线
//             TFTSPI_Draw_Line(0, ROAD_MAIN_ROW, 159, ROAD_MAIN_ROW, u16RED);//误差线
//            TFTSPI_Draw_Line(min_tag*STEP_COL, MAX[min_tag], min_tag*STEP_COL,80, u16BLUE);//最长线

//            sprintf(txt_main1, "E:%03d-R:%d", (int)g_sSteeringError,(int)ROAD_MAIN_ROW);
//            TFTSPI_P8X16Str(1, 6, txt_main1, u16ORANGE, u16BLACK);
//            sprintf(txt_main1, "T:%d Y:%d H:%d", (int)THRESHOLD,yuzhi,(int)high);
//            TFTSPI_P8X16Str(1, 7, txt_main1, u16ORANGE, u16BLACK);
//            sprintf(txt_main1, "RP%2.1f d%1.1f", RSpeed_PID.kp,RSpeed_PID.kd);//方向环pid
//            TFTSPI_P8X16Str(1, 8, txt_main1, u16ORANGE, u16RED);
//            sprintf(txt_main1, "Voltage: %05d;", Voltage);
//            TFTSPI_P8X16Str(1,9, txt_main1,u16RED,u16BLACK);       //电压显示

//       if (Camera_Flag == 2)
//       {
//          Get_Use_Image();     // 取出赛道及显示所需图像数据
//          Camera_Flag = 0;
          //compressImage(Image_Use,image_min);//图像压缩

          //Get_Bin_Image(3);    // 转换为01格式数据，0、1原图；2、3边沿提取  现在的循迹程序是按照3的条件下进行写的
          //Bin_Image_Filter();  // 滤波，三面被围的数据将被修改为同一数值
          //ImageGetSide(Bin_Image, ImageSide,ImageSide_last);     //提取赛道左右边线
          //UpdownSideGet(Bin_Image, UpdowmSide);   //提取赛道上下边线
          //GetRoadWide(ImageSide,Roadwide);        //计算赛道宽度


          //find_track_boundaries(Image_Use,ImageSide);//边缘检测
          //apply_gaussian_filter(Image_Use,Filtered_Image);//高斯滤波

          //sobel二值化
          //sobel_operator(Image_Use,Filtered_Image);
          //基于滑动窗口的方法进行边缘检测
          //sliding_window(Filtered_Image,ImageSide);
          //霍夫变换
          //hufu(Filtered_Image,ImageSide);
          //Canny边缘检测
          //detect_lane_edges(Image_Use,ImageSide);
          //ImageGetSide(Filtered_Image, ImageSide,ImageSide_last);     //提取赛道左右边线
          //提取边线
//          findMaxDifference(image_min,MAX);
//          find_sideimage(image_min,MAX,ImageSide);
        //  Camera_Flag = 0;     // 清除摄像头采集完成标志位  如果不清除，则不会再次采集数据
//          if(Show_TFT_Flag==1)
//          {
//              TFTSPI_Road(0, 0, LCDH, LCDW,Image_Use);

 //             TFTSPI_Road(0, 0, 80, 120,(unsigned char *)image_min);
//              TFTSPI_BinRoadSide(ImageSide);          //左右边线和中线
//              TFTSPI_Draw_Line(60, 0, 60, 79, u16RED);                       //显示中线
//              TFTSPI_Draw_Line(0, ROAD_MAIN_ROW, 119, ROAD_MAIN_ROW, u16RED);//误差线
//              TFTSPI_Draw_Line(min_tag*STEP_COL, MAX[min_tag], min_tag*STEP_COL,80, u16BLUE);//最长线
//
//              sprintf(txt_main, "E:%03d-R:%d", (int)g_sSteeringError,(int)ROAD_MAIN_ROW);
//              TFTSPI_P8X16Str(1, 6, txt_main, u16ORANGE, u16BLACK);
//              sprintf(txt_main, "T:%d Y:%d H:%d", (int)THRESHOLD,yuzhi,(int)high);
//              TFTSPI_P8X16Str(1, 7, txt_main, u16ORANGE, u16BLACK);
//              sprintf(txt_main, "RP%2.1f d%1.1f", RSpeed_PID.kp,RSpeed_PID.kd);//方向环pid
//              TFTSPI_P8X16Str(1, 8, txt_main, u16ORANGE, u16RED);
//              sprintf(txt_main, "Voltage: %05d;", Voltage);
//              TFTSPI_P8X16Str(1,9, txt_main,u16RED,u16BLACK);       //电压显示
//          }

          ///Bluetooth_work(UART0);
    //}
}

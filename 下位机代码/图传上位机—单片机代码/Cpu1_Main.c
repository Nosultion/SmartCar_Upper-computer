/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�TC264DA���İ�
����    д��chiusir
��E-mail��chiusir@163.com
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
�� ���źţ�    �ⲿ�жϵ�3�飺P15_1��
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
��ʱ��
������CCU6ģ��  ÿ��ģ��������������ʱ��  ������ʱ���ж�
�Ƽ�ʹ��CCU6ģ�飬STM����ϵͳʱ�ӻ�����ʱ��
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

// ��ʱ�� 5ms��50ms��־λ
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
    // ����CPU���ж�
    IfxCpu_enableInterrupts();

    // �رտ��Ź�
    IfxScuWdt_disableCpuWatchdog (IfxScuWdt_getCpuWatchdogPassword ());

    // �ȴ�CPU0 ��ʼ�����
    while(!IfxCpu_acquireMutex(&mutexCpu0InitIsOk));

    // ���к����жϵĲ��Զ�Ĭ����CPU0��ִ�У������Ҫ��CPU1��ο�����Bվ��Ƶ��
    // ����������Ƶ��ַ��https://space.bilibili.com/95313236




    while(1)//��ѭ��
    {
//      UART_PutChar(UART0, 100);  //֡ͷ
//      if (Camera_Flag == 2)
//      {
//          Camera_Flag = 0;
//
//          //��120x160�ڱ߿�
//         for (int j = 0; j < LCDH; j++)     //���ۿ�188
//         {
//             Image_Data[j][14] =70;
//             Image_Data[j][174] = 70;
//         }
//         //��ʾ��ɫ������Ҳ����������������߹���·�� ͬʱҲɾ����һ�δ�ŵ�·��
//          for (int i = 0; i < IMAGEH; i ++)          //���۸� 120
//          {
//              for (int j = 14; j < IMAGEW-14+1; j ++)     //���ۿ�188
//              {
//                  Image_Use_BLACK[i][j] = 0;
//                  //��ֵ��
//                  if (Image_Data[i][j] > (unsigned char)thres) //��ֵԽ����ʾ������Խ�࣬��ǳ��ͼ��Ҳ����ʾ����
//                      Bin_Image[i][j] = 255;
//                  else
//                      Bin_Image[i][j] = 0;
//
//                  //printf("%d",Bin_Image[i][j]);
//              }
//          }
//          //�Ϻ�����  ����������
//          //process_image(Image_Data);
//          //ͼ��ѹ��
//          compressImage(Bin_Image,image_min);//ͼ��ѹ��
//
//      }
//
//      UART_PutChar(UART0, 200);  //֡β
    }

          //�յ��������
//          L_pointX[0]=0;L_pointY[1]=0;
//          R_pointX[0]=0;R_pointY[1]=0;
//          L_point_num=0;
//          R_point_num=0;


          /* ��ȡ����ʹ�õ����� */
 //         Get_Use_Image();
          /* �������ͷ�ɼ���ɱ�־λ  �����������򲻻��ٴβɼ����� */

//         ���
//          compressImage(Image_Use,image_min);//ͼ��ѹ��
//          //������ȡ
//          findMaxDifference(image_min,MAX);
//          find_sideimage(image_min,MAX,ImageSide);//��ȡ���ұ���
//          find_upsideimage(image_min,UpdowmSide);//��ȡ�ϱ���
          //��ʾԭʼͼ��
          //TFT1.8��̬��ʾ����ͷͼ��
          //TFTSPI_Road(0, 0, LCDH, LCDW, (unsigned char *)Image_Use);

//        }


//              for(int i = 0; i <stepR; i++)
//              {
//                  TFTSPI_Draw_Dot((unsigned char)ipts1[i][0],(unsigned char)ipts1[i][1], u16GREEN);//����������������ɫ
//              }
//            TFTSPI_Road(0, 0, 80, 120,(unsigned char *)image_min);
//            TFTSPI_BinRoadSide(ImageSide);          //���ұ��ߺ�����
//            TFTSPI_BinRoad_UpdownSide(UpdowmSide);  //�ϱ���
//              TFTSPI_Draw_Line(80, 0, 80, 120, u16RED);                       //��ʾ����
//             TFTSPI_Draw_Line(0, ROAD_MAIN_ROW, 159, ROAD_MAIN_ROW, u16RED);//�����
//            TFTSPI_Draw_Line(min_tag*STEP_COL, MAX[min_tag], min_tag*STEP_COL,80, u16BLUE);//���

//            sprintf(txt_main1, "E:%03d-R:%d", (int)g_sSteeringError,(int)ROAD_MAIN_ROW);
//            TFTSPI_P8X16Str(1, 6, txt_main1, u16ORANGE, u16BLACK);
//            sprintf(txt_main1, "T:%d Y:%d H:%d", (int)THRESHOLD,yuzhi,(int)high);
//            TFTSPI_P8X16Str(1, 7, txt_main1, u16ORANGE, u16BLACK);
//            sprintf(txt_main1, "RP%2.1f d%1.1f", RSpeed_PID.kp,RSpeed_PID.kd);//����pid
//            TFTSPI_P8X16Str(1, 8, txt_main1, u16ORANGE, u16RED);
//            sprintf(txt_main1, "Voltage: %05d;", Voltage);
//            TFTSPI_P8X16Str(1,9, txt_main1,u16RED,u16BLACK);       //��ѹ��ʾ

//       if (Camera_Flag == 2)
//       {
//          Get_Use_Image();     // ȡ����������ʾ����ͼ������
//          Camera_Flag = 0;
          //compressImage(Image_Use,image_min);//ͼ��ѹ��

          //Get_Bin_Image(3);    // ת��Ϊ01��ʽ���ݣ�0��1ԭͼ��2��3������ȡ  ���ڵ�ѭ�������ǰ���3�������½���д��
          //Bin_Image_Filter();  // �˲������汻Χ�����ݽ����޸�Ϊͬһ��ֵ
          //ImageGetSide(Bin_Image, ImageSide,ImageSide_last);     //��ȡ�������ұ���
          //UpdownSideGet(Bin_Image, UpdowmSide);   //��ȡ�������±���
          //GetRoadWide(ImageSide,Roadwide);        //�����������


          //find_track_boundaries(Image_Use,ImageSide);//��Ե���
          //apply_gaussian_filter(Image_Use,Filtered_Image);//��˹�˲�

          //sobel��ֵ��
          //sobel_operator(Image_Use,Filtered_Image);
          //���ڻ������ڵķ������б�Ե���
          //sliding_window(Filtered_Image,ImageSide);
          //����任
          //hufu(Filtered_Image,ImageSide);
          //Canny��Ե���
          //detect_lane_edges(Image_Use,ImageSide);
          //ImageGetSide(Filtered_Image, ImageSide,ImageSide_last);     //��ȡ�������ұ���
          //��ȡ����
//          findMaxDifference(image_min,MAX);
//          find_sideimage(image_min,MAX,ImageSide);
        //  Camera_Flag = 0;     // �������ͷ�ɼ���ɱ�־λ  �����������򲻻��ٴβɼ�����
//          if(Show_TFT_Flag==1)
//          {
//              TFTSPI_Road(0, 0, LCDH, LCDW,Image_Use);

 //             TFTSPI_Road(0, 0, 80, 120,(unsigned char *)image_min);
//              TFTSPI_BinRoadSide(ImageSide);          //���ұ��ߺ�����
//              TFTSPI_Draw_Line(60, 0, 60, 79, u16RED);                       //��ʾ����
//              TFTSPI_Draw_Line(0, ROAD_MAIN_ROW, 119, ROAD_MAIN_ROW, u16RED);//�����
//              TFTSPI_Draw_Line(min_tag*STEP_COL, MAX[min_tag], min_tag*STEP_COL,80, u16BLUE);//���
//
//              sprintf(txt_main, "E:%03d-R:%d", (int)g_sSteeringError,(int)ROAD_MAIN_ROW);
//              TFTSPI_P8X16Str(1, 6, txt_main, u16ORANGE, u16BLACK);
//              sprintf(txt_main, "T:%d Y:%d H:%d", (int)THRESHOLD,yuzhi,(int)high);
//              TFTSPI_P8X16Str(1, 7, txt_main, u16ORANGE, u16BLACK);
//              sprintf(txt_main, "RP%2.1f d%1.1f", RSpeed_PID.kp,RSpeed_PID.kd);//����pid
//              TFTSPI_P8X16Str(1, 8, txt_main, u16ORANGE, u16RED);
//              sprintf(txt_main, "Voltage: %05d;", Voltage);
//              TFTSPI_P8X16Str(1,9, txt_main,u16RED,u16BLACK);       //��ѹ��ʾ
//          }

          ///Bluetooth_work(UART0);
    //}
}

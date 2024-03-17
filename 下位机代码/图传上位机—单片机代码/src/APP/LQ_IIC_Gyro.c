/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技TC264DA核心板
【编    写】ZYF/chiusir
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

使用例程的时候，建议采用没有空格的英文路径，
除了CIF为TC264DA独有外，其它的代码兼容TC264D
本库默认初始化了EMEM：512K，如果用户使用TC264D，注释掉EMEM_InitConfig()初始化函数。
工程下\Libraries\iLLD\TC26B\Tricore\Cpu\CStart\IfxCpu_CStart0.c第164行左右。
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include <IfxCpu_IntrinsicsTasking.h>
#include "LQ_IIC_Gyro.h"
#include <stdio.h>
#include "../Driver/LQ_SOFTI2C.h"
#include "../Driver/LQ_STM.h"
#include "LQ_OLED096.h"
#include "../User/LQ_AnoScope.h"
#include "LQ_TFT18.h"
#include "LQ_ICM20602.h"

#define OLED  0                 //选择屏幕0：TFT屏      1:OLED屏
#define MPU6050_ADDR  0x68  //IIC写入时的地址字节数据，+1为读取

unsigned char IIC_ICM20602 = 0;
unsigned char IIC_ICM20689 = 0;
unsigned char IIC_MPU9250 = 0;
unsigned char IIC_ICM42605 = 0;
unsigned char IIC_MPU6050 = 0;
unsigned char IIC_errorid = 0;

unsigned char Gyro_ID = 0;



/**
  * @brief   读取陀螺仪的设备ID
  *
  * @param   void
  *
  * @retur   void
  *
  * @note
  *
  * @see      Gyro_Chose();
  *
  * @date     2021/7/12 星期一
  */
unsigned char Gyro_Chose(void)
{
    Gyro_ID = MPU_Read_Byte(MPU6050_ADDR,WHO_AM_I);
    switch(Gyro_ID)
    {
        case 0x12:IIC_ICM20602 = 1;break;
        case 0x71:IIC_MPU9250 = 1;break;
        case 0x98:IIC_ICM20689 = 1;break;
        case 0x42:IIC_ICM42605 = 1;break;
        case 0x68:IIC_MPU6050 = 1;break;
        default :IIC_errorid = 1;return 0;
    }
    return Gyro_ID;
}


signed short  aacx,aacy,aacz;            //加速度传感器原始数据
signed short  gyrox,gyroy,gyroz;         //陀螺仪原始数据
short magx, magy, magz;
/**
  * @brief    读取MPU6050或者ICM20602原始数据 测试
  *
  * @param
  *
  * @return
  *
  * @note
  *
  * @see      Test_MPU6050();
  *
  * @date     2019/6/12 星期三
  */
void Test_IIC_Gyro(void)
{
    unsigned char res;
	char  txt[30];


	IIC_Init();                  //IIC初始化//IIC初始化
	delayms(100);
	res = Gyro_Chose();                //判断陀螺仪型号
	sprintf(txt,"ID:0X%2x",res);

#if OLED

	OLED_Init();                //LCD初始化
	delayms(100);
	OLED_CLS();                 //LCD清屏
	OLED_P6x8Str(70,2,txt);     // 字符串显示
    if(IIC_ICM20602)
        OLED_P8x16Str(0,0,"LQ 20602 Test");// 字符串显示
    if(IIC_MPU6050)
        OLED_P8x16Str(0,0,"LQ 6050 Test");// 字符串显示
    if(IIC_ICM42605)
        OLED_P8x16Str(0,0,"LQ 42605 Test");// 字符串显示
    if(IIC_MPU9250)
        OLED_P8x16Str(0,0,"LQ 9250 Test");// 字符串显示
    if(IIC_ICM20689)
        OLED_P8x16Str(0,0,"LQ 20689 Test");// 字符串显示
    if(IIC_errorid)
    {
        OLED_P8x16Str(0,0,"Gyro Test fail");// 字符串显示
        while(1);
    }

#else
	TFTSPI_Init(1);         //屏幕初始化为横屏
	TFTSPI_CLS(u16BLACK);   // 清屏
	if(IIC_MPU9250){
	    sprintf(txt,"0X%2x",res);
	    TFTSPI_P8X16Str(10,8,"ID:",u16RED,u16BLACK);
	    TFTSPI_P8X16Str(12,9,txt,u16RED,u16BLACK);// 字符串显示
	}else
	    TFTSPI_P8X16Str(9,9,txt,u16RED,u16BLACK);// 字符串显示
    if (IIC_ICM20602)
        TFTSPI_P8X16Str(2,0,"LQ 20602 Test",u16RED,u16BLACK);// 字符串显示
    if (IIC_MPU6050)
        TFTSPI_P8X16Str(2,0,"LQ 6050 Test",u16RED,u16BLACK);// 字符串显示
    if (IIC_ICM42605)
        TFTSPI_P8X16Str(2,0,"LQ 42605 Test",u16RED,u16BLACK);// 字符串显示
    if (IIC_MPU9250)
        TFTSPI_P8X16Str(2,0,"LQ 9250 Test",u16RED,u16BLACK);// 字符串显示
    if (IIC_ICM20689)
        TFTSPI_P8X16Str(2,0,"LQ 20689 Test",u16RED,u16BLACK);// 字符串显示
    if (IIC_errorid)
    {
        TFTSPI_P8X16Str(1,0,"Gyro Test fail",u16RED,u16BLACK);// 字符串显示
        while(1);
    }
#endif
	MPU6050_Init();
	while(1)
    {
	    MPU_Get_Raw_data(&aacx,&aacy,&aacz,&gyrox,&gyroy,&gyroz);   //得到加速度传感器数据
	    if (IIC_MPU9250)
	        MPU9250_Get_Magnetometer(&magx, &magy, &magz);

#if OLED
	    sprintf((char*)txt,"ax:%06d",aacx);
        OLED_P6x8Str(0,2,txt);
        sprintf((char*)txt,"ay:%06d",aacy);
        OLED_P6x8Str(0,3,txt);
        sprintf((char*)txt,"az:%06d",aacz);
        OLED_P6x8Str(0,4,txt);
        sprintf((char*)txt,"gx:%06d",gyrox);
        OLED_P6x8Str(0,5,txt);
        sprintf((char*)txt,"gy:%06d",gyroy);
        OLED_P6x8Str(0,6,txt);
        sprintf((char*)txt,"gz:%06d",gyroz);
        OLED_P6x8Str(0,7,txt);
        //ANO_DT_send_int16byte16(aacx,aacy,aacz,gyrox,gyroy,gyroz,gyroz,gyroz);

#else
	    sprintf((char*)txt,"ax:%06d",aacx);
	    TFTSPI_P8X16Str(0,1,txt,u16RED,u16BLACK);// 字符串显示
	    sprintf((char*)txt,"ay:%06d",aacy);
	    TFTSPI_P8X16Str(0,2,txt,u16RED,u16BLACK);// 字符串显示
	    sprintf((char*)txt,"az:%06d",aacz);
	    TFTSPI_P8X16Str(0,3,txt,u16RED,u16BLACK);// 字符串显示
	    sprintf((char*)txt,"gx:%06d",gyrox);
	    TFTSPI_P8X16Str(0,4,txt,u16RED,u16BLACK);// 字符串显示
	    sprintf((char*)txt,"gy:%06d",gyroy);
	    TFTSPI_P8X16Str(0,5,txt,u16RED,u16BLACK);// 字符串显示
	    sprintf((char*)txt,"gz:%06d",gyroz);
	    TFTSPI_P8X16Str(0,6,txt,u16RED,u16BLACK);// 字符串显示

	    if (IIC_MPU9250)
        {
            sprintf((char*)txt,"mx:%06d",magx);
            TFTSPI_P8X16Str(0,7,txt,u16RED,u16BLACK);
            sprintf((char*)txt,"my:%06d",magy);
            TFTSPI_P8X16Str(0,8,txt,u16RED,u16BLACK);
            sprintf((char*)txt,"mz:%06d",magz);
            TFTSPI_P8X16Str(0,9,txt,u16RED,u16BLACK);
        }
#endif
	    //ANO_DT_send_int16byte16(aacx,aacy,aacz,gyrox,gyroy,gyroz,gyroz,gyroz);
	    delayms(100);
    }

}

/**
  * @brief    不精确延时
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @see      delayms_mpu(100);
  *
  * @date     2019/4/22 星期一
*/
void delayms_mpu(uint16 ms)
{
	while(ms--)
	{
		uint16  i = 300;
		while(i--)
		{
			NOP(50);
		}
	}
}



/**
  * @brief    初始化MPU6050 或者 ICM20602
  *
  * @param    无
  *
  * @return   0：初始化成功   1：失败
  *
  * @note     使用前先初始化IIC接口
  *
  * @see      MPU6050_Init();
  *
  * @date     2019/6/12 星期三
  */
unsigned char MPU6050_Init(void)
{
    int  res;
    if (IIC_ICM42605)
    {
//        MPU_Write_Byte(MPU6050_ADDR, device_config_reg,bit_soft_reset_chip_config);//chip soft reset
//        delayms(10);
//
//            MPU_Write_Byte(MPU6050_ADDR,reg_bank_sel,0x01);//Change to bank 1
//            MPU_Write_Byte(MPU6050_ADDR,intf_config4,0x03);//4 wire spi mode  四线SPI  默认值0x03
//            MPU_Write_Byte(MPU6050_ADDR,reg_bank_sel,0x00);
//            MPU_Write_Byte(MPU6050_ADDR,fifo_config_reg,0x40);//Stream-to-FIFO Mode  陀螺仪Y轴陷波滤波器的频率选择
//
//            res = MPU_Read_Byte(MPU6050_ADDR,int_source0_reg);
//            MPU_Write_Byte(MPU6050_ADDR,int_source0_reg,0x00);
//            MPU_Write_Byte(MPU6050_ADDR,fifo_config2_reg,0x00);// watermark
//            MPU_Write_Byte(MPU6050_ADDR,fifo_config3_reg,0x02);// watermark
//            MPU_Write_Byte(MPU6050_ADDR,int_source0_reg, (unsigned char)res);
//            MPU_Write_Byte(MPU6050_ADDR,fifo_config1_reg,0x63);// Enable the accel and gyro to the FIFO
//
//            MPU_Write_Byte(MPU6050_ADDR,reg_bank_sel,0x00);
//            MPU_Write_Byte(MPU6050_ADDR,int_config_reg,0x36);
//
//            MPU_Write_Byte(MPU6050_ADDR,reg_bank_sel, 0x00);
//            res = (MPU_Read_Byte(MPU6050_ADDR,int_source0_reg)|bit_int_fifo_ths_int1_en);
//            MPU_Write_Byte(MPU6050_ADDR,int_source0_reg, (unsigned char)res);
//
//            MPU_Write_Byte(MPU6050_ADDR,reg_bank_sel, 0x00);
//            res = ((MPU_Read_Byte(MPU6050_ADDR,accel_config0_reg)&0x1F)|(bit_accel_ui_fs_sel_8g));//8g
//            MPU_Write_Byte(MPU6050_ADDR,accel_config0_reg, (unsigned char)res);
//
//            MPU_Write_Byte(MPU6050_ADDR,reg_bank_sel, 0x00);
//            res = ((MPU_Read_Byte(MPU6050_ADDR,accel_config0_reg)&0xF0)|bit_accel_odr_50hz);
//            MPU_Write_Byte(MPU6050_ADDR,accel_config0_reg, (unsigned char)res);
//
//            MPU_Write_Byte(MPU6050_ADDR,reg_bank_sel, 0x00);
//            res = ((MPU_Read_Byte(MPU6050_ADDR,gyro_config0_reg)&0x1F)|(bit_gyro_ui_fs_sel_1000dps));
//            MPU_Write_Byte(MPU6050_ADDR,gyro_config0_reg,(unsigned char)res);
//
//            MPU_Write_Byte(MPU6050_ADDR,reg_bank_sel, 0x00);
//            res = ((MPU_Read_Byte(MPU6050_ADDR,gyro_config0_reg)&0xF0)|bit_gyro_odr_50hz);
//            MPU_Write_Byte(MPU6050_ADDR,gyro_config0_reg, (unsigned char)res);
//
//            MPU_Write_Byte(MPU6050_ADDR,reg_bank_sel, 0x00);
//            res = MPU_Read_Byte(MPU6050_ADDR,pwr_mgmt0_reg)|(bit_accel_mode_ln); // Accel on in LNM
//            MPU_Write_Byte(MPU6050_ADDR,pwr_mgmt0_reg, (unsigned char)res);
//            delayms(1);
//
//            MPU_Write_Byte(MPU6050_ADDR,reg_bank_sel, 0x00);
//            res = MPU_Read_Byte(MPU6050_ADDR,pwr_mgmt0_reg)|(bit_gyro_mode_ln); // Gyro on in LNM
//            MPU_Write_Byte(MPU6050_ADDR,pwr_mgmt0_reg,(unsigned char)res);
            delayms(1);
            return 0;
    }
    else
    {
        res = MPU_Read_Byte(MPU6050_ADDR,WHO_AM_I);           //读取MPU6050的ID
        if(res != Gyro_ID)                                 //器件ID正确
            return 1;

        res = 0;
        res += MPU_Write_Byte(MPU6050_ADDR,MPU_PWR_MGMT1_REG,0X80);//复位MPU6050
        delayms_mpu(100);  //延时100ms
        res += MPU_Write_Byte(MPU6050_ADDR,MPU_PWR_MGMT1_REG,0X00);//唤醒MPU6050
        res += MPU_Set_Gyro_Fsr(3);					        	   //陀螺仪传感器,±2000dps
        res += MPU_Set_Accel_Fsr(1);					       	   //加速度传感器,±4g
        res += MPU_Set_Rate(1000);						       	   //设置采样率1000Hz
        res += MPU_Write_Byte(MPU6050_ADDR,MPU_CFG_REG,0x02);      //设置数字低通滤波器   98hz
        res += MPU_Write_Byte(MPU6050_ADDR,MPU_INT_EN_REG,0X00);   //关闭所有中断
        res += MPU_Write_Byte(MPU6050_ADDR,MPU_USER_CTRL_REG,0X00);//I2C主模式关闭
        res += MPU_Write_Byte(MPU6050_ADDR,MPU_PWR_MGMT1_REG,0X01);//设置CLKSEL,PLL X轴为参考
        res += MPU_Write_Byte(MPU6050_ADDR,MPU_PWR_MGMT2_REG,0X00);//加速度与陀螺仪都工作

        if(IIC_MPU9250)
        {
            MPU_Write_Byte(MPU6050_ADDR,MPU_FIFO_EN_REG,0X00);  //关闭FIFO
            MPU_Write_Byte(MPU6050_ADDR,MPU_INTBP_CFG_REG,0X82);//INT引脚低电平有效，开启bypass模式
            res = MPU_Read_Byte(AK8963_ADDR,MPU_MAG_WIA);   //读取MPU9250的ID
            if(res == AK8963_ID)                                //器件ID正确
            {
                MPU_Write_Byte(AK8963_ADDR,MPU_MAG_CNTL2_REG,0X01);//软件重置
                delayms_mpu(100);
                MPU_Write_Byte(AK8963_ADDR,MPU_MAG_CNTL1_REG,0X16);//设置为单次模式，输出16bit
            }
        }
        return 0;
    }
}

/**
  * @brief    设置陀螺仪测量范围
  *
  * @param    fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
  *
  * @return   0 ：设置成功
  *
  * @note     无
  *
  * @see      MPU_Set_Gyro_Fsr(3);		  //陀螺仪传感器,±2000dps
  *
  * @date     2019/6/12 星期三
  */
unsigned char MPU_Set_Gyro_Fsr(unsigned char fsr)
{
	return MPU_Write_Byte(MPU6050_ADDR,MPU_GYRO_CFG_REG,fsr<<3);
}

/**
  * @brief    设置加速度计测量范围
  *
  * @param    fsr:0,±2g;1,±4g;2,±8g;3,±16g
  *
  * @return   0：设置成功
  *
  * @note     无
  *
  * @see      MPU_Set_Accel_Fsr(1);		  //加速度传感器,±4g
  *
  * @date     2019/6/12 星期三
  */
unsigned char MPU_Set_Accel_Fsr(unsigned char fsr)
{
	return MPU_Write_Byte(MPU6050_ADDR,MPU_ACCEL_CFG_REG,fsr<<3);
}

/**
  * @brief    设置数字低通滤波
  *
  * @param    lpf:数字低通滤波频率(Hz)
  *
  * @return   0：设置成功
  *
  * @note     无
  *
  * @see      MPU_Set_LPF(100);
  *
  * @date     2019/6/12 星期三
  */
unsigned char MPU_Set_LPF(uint16 lpf)
{
	unsigned char  dat=0;
	if(lpf>=188)dat=1;
	else if(lpf>=98)dat=2;
	else if(lpf>=42)dat=3;
	else if(lpf>=20)dat=4;
	else if(lpf>=10)dat=5;
	else dat=6;
	return MPU_Write_Byte(MPU6050_ADDR,MPU_CFG_REG,dat);//设置数字低通滤波器
}

/**
  * @brief    设置采样率
  *
  * @param    rate:4~1000(Hz)
  *
  * @return   0：设置成功
  *
  * @note     无
  *
  * @see      MPU_Set_Rate(1000);       	   //设置采样率1000Hz
  *
  * @date     2019/6/12 星期三
  */
unsigned char MPU_Set_Rate(uint16 rate)
{
	unsigned char  dat;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	dat=1000/rate-1;
	MPU_Write_Byte(MPU6050_ADDR,MPU_SAMPLE_RATE_REG,dat);	    //设置数字低通滤波器
 	return MPU_Set_LPF(rate/2);	                                //自动设置LPF为采样率的一半
}

/**
  * @brief    获取温度值
  *
  * @param    无
  *
  * @return   温度值(扩大了100倍)
  *
  * @note     无
  *
  * @see      signed short temp = MPU_Get_Temperature();
  *
  * @date     2019/6/12 星期三
  */
short MPU_Get_Temperature(void)
{
    unsigned char  buf[2];
    short raw;
	float temp;
	MPU_Read_Len(MPU6050_ADDR,MPU_TEMP_OUTH_REG,2,buf);
    raw=((uint16)buf[0]<<8)|buf[1];
    temp=21+((double)raw)/333.87;
    return (short)temp*100;
}

/**
  * @brief    获取陀螺仪值
  *
  * @param    gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
  *
  * @return   0：读取成功
  *
  * @note     无
  *
  * @see      signed short data[3];
  * @see      MPU_Get_Gyroscope(&data[0], &data[1], &data[2]);
  *
  * @date     2019/6/12 星期三
  */
unsigned char MPU_Get_Gyroscope(signed short *gx,signed short *gy,signed short *gz)
{
    unsigned char  buf[6],res;
	res=MPU_Read_Len(MPU6050_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((uint16)buf[0]<<8)|buf[1];
		*gy=((uint16)buf[2]<<8)|buf[3];
		*gz=((uint16)buf[4]<<8)|buf[5];
	}
    return res;
}

/**
  * @brief    获取加速度值
  *
  * @param    ax,ay,az:陀螺仪x,y,z轴的原始读数(带符号)
  *
  * @return   0：读取成功
  *
  * @note     无
  *
  * @see      signed short data[3];
  * @see      MPU_Get_Accelerometer(&data[0], &data[1], &data[2]);
  *
  * @date     2019/6/12 星期三
  */
unsigned char MPU_Get_Accelerometer(signed short *ax,signed short *ay,signed short *az)
{
    unsigned char  buf[6],res;
	res=MPU_Read_Len(MPU6050_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((uint16)buf[0]<<8)|buf[1];
		*ay=((uint16)buf[2]<<8)|buf[3];
		*az=((uint16)buf[4]<<8)|buf[5];
	}
    return res;
}

/**
  * @brief    获取 加速度值 角速度值
  *
  * @param    ax,ay,az:陀螺仪x,y,z轴的原始读数(带符号)
  * @param    gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
  *
  * @return   0：读取成功
  *
  * @note     无
  *
  * @see      signed short data[6];
  * @see      ICM_Get_Raw_data(&data[0], &data[1], &data[2],&data[3], &data[4], &data[5]);
  *
  */
unsigned char MPU_Get_Raw_data(signed short *ax,signed short *ay,signed short *az,signed short *gx,signed short *gy,signed short *gz)
{
    if(IIC_ICM42605)
    {
        unsigned char  buf[14],res;
        res=MPU_Read_Len(MPU6050_ADDR,0X1F,12,buf);
        if(res==0)
        {
            *ax=((uint16)buf[0]<<8)|buf[1];
            *ay=((uint16)buf[2]<<8)|buf[3];
            *az=((uint16)buf[4]<<8)|buf[5];
            *gx=((uint16)buf[6]<<8)|buf[7];
            *gy=((uint16)buf[8]<<8)|buf[9];
            *gz=((uint16)buf[10]<<8)|buf[11];
        }
        return res;
    }
    else
    {
        unsigned char  buf[14],res;

        res=MPU_Read_Len(MPU6050_ADDR,MPU_ACCEL_XOUTH_REG,14,buf);
        if(res==0)
        {
            *ax=((uint16)buf[0]<<8)|buf[1];
            *ay=((uint16)buf[2]<<8)|buf[3];
            *az=((uint16)buf[4]<<8)|buf[5];
            *gx=((uint16)buf[8]<<8)|buf[9];
            *gy=((uint16)buf[10]<<8)|buf[11];
            *gz=((uint16)buf[12]<<8)|buf[13];
        }
        return res;
    }
}

/**
* @brief    获取磁力计值
*
* @param    mx,my,mz:磁力计仪x,y,z轴的原始读数(带符号)
*
* @return   0：读取成功
*
* @note     无
*
* @see      signed short data[3];
* @see      MPU9250_Get_Magnetometer(&data[0], &data[1], &data[2]);
*
* @date     2020/12/15 星期二
*/
unsigned char MPU9250_Get_Magnetometer(short *mx,short *my,short *mz)
{

  unsigned char  buf[6],res;
  res=MPU_Read_Len(AK8963_ADDR,MPU_MAG_XOUTL_REG,6,buf);
  if(res==0)
  {
    *mx=((uint16_t)buf[1]<<8)|buf[0];
    *my=((uint16_t)buf[3]<<8)|buf[2];
    *mz=((uint16_t)buf[5]<<8)|buf[4];
  }
  MPU_Read_Byte(AK8963_ADDR,MPU_MAG_ST2_REG);
  return res;
}

/**
  * @brief    IIC 连续读
  *
  * @param    addr:器件地址
  * @param    reg :要读取的寄存器地址
  * @param    len :要读取的长度
  * @param    buf :读取到的数据存储区
  *
  * @return   0 ：读取成功
  *
  * @note     底层驱动 移植时需要修改
  *
  * @see      unsigned char buf[14];
  * @see      MPU_Read_Len(MPU6050_ADDR,MPU_ACCEL_XOUTH_REG,14,buf);
  *
  * @date     2019/6/12 星期三
  */
unsigned char MPU_Read_Len(unsigned char addr,unsigned char reg,unsigned char len,unsigned char *buf)
{
    return IIC_ReadMultByteFromSlave(addr<<1, reg, len, buf);
}

/**
  * @brief    IIC 连续写
  *
  * @param    addr:器件地址
  * @param    reg :要写入的寄存器地址
  * @param    len :要写入的长度
  * @param    buf :写入到的数据存储区
  *
  * @return   0 ：写入成功
  *
  * @note     底层驱动 移植时需要修改
  *
  * @see      unsigned char buf[14];
  * @see      MPU9250_Write_Len(MPU9250_ADDR,MPU_ACCEL_XOUTH_REG,14,buf);
  *
  * @date     2021/7/1 星期四
  */
unsigned char MPU_Write_Len(unsigned char addr,unsigned char reg,unsigned char len,unsigned char *buf)
{
    return IIC_WriteMultByteToSlave(addr<<1, reg, len, buf);
}

/**
  * @brief    IIC 写一个寄存器
  *
  * @param    addr  :器件地址
  * @param    reg   :寄存器地址
  * @param    value :要写入的值
  *
  * @return   0 ：读取成功
  *
  * @note     底层驱动 移植时需要修改
  *
  * @see      MPU_Write_Byte(MPU6050_ADDR,MPU_SAMPLE_RATE_REG,1);
  *
  * @date     2019/6/12 星期三
  */
unsigned char MPU_Write_Byte(unsigned char addr,unsigned char reg,unsigned char value)
{
    return IIC_WriteByteToSlave(addr<<1, reg, value);
}

/**
  * @brief    IIC 读一个寄存器
  *
  * @param    addr  :器件地址
  * @param    reg   :寄存器地址
  *
  * @return   读取的值
  *
  * @note     底层驱动 移植时需要修改
  *
  * @see      MPU_Read_Byte(MPU6050_ADDR,WHO_AM_I);
  *
  * @date     2019/6/12 星期三
  */
unsigned char MPU_Read_Byte(unsigned char addr,unsigned char reg)
{
    unsigned char value[1];
    MPU_Read_Len(addr, reg, 1, value);
    return value[0];
}



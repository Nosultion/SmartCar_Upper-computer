/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�TC264DA���İ�
����    д��ZYF/chiusir
��E-mail  ��chiusir@163.com
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
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef __LQ_VL53_H_
#define __LQ_VL53_H_



#define VL53L0X_REG_IDENTIFICATION_MODEL_ID         0xc0
#define VL53L0X_REG_IDENTIFICATION_REVISION_ID      0xc2
#define VL53L0X_REG_PRE_RANGE_CONFIG_VCSEL_PERIOD   0x50
#define VL53L0X_REG_FINAL_RANGE_CONFIG_VCSEL_PERIOD 0x70
#define VL53L0X_REG_SYSRANGE_START                  0x00
#define VL53L0X_REG_RESULT_INTERRUPT_STATUS         0x13
#define VL53L0X_REG_RESULT_RANGE_STATUS             0x14
#define VL53_REG_DIS                                0x1E
#define VL53L0X_REG_I2C_SLAVE_DEVICE_ADDRESS        0x8a
#define VL53ADDR                                    0x29    //0x52   Ĭ�ϵ�ַ
#define VL53NEWADDR                                 0x30

/**
  * @brief    ����VL53
  *
  * @param    ��
  *
  * @return   ��
  *
  * @note     ��
  *
  * @example
  *
  * @date     2019/4/17 ������
  */
void Test_Vl53(void);



/**
  * @brief    VL53 д1���Ĵ���
  *
  * @param    dev��    ��ַ
  * @param    reg��    �Ĵ���
  * @param    data��   д������
  *
  * @return
  *
  * @note      ��ֲʱ��Ҫ�޸�����
  *
  * @see
  *
  * @date     2019/4/29 ����һ
  */
void VL53_Write_Byte(unsigned char dev, unsigned char reg, unsigned char data);



/**
  * @brief    VL53 ��n���Ĵ���
  *
  * @param    dev��    ��ַ
  * @param    reg��    �Ĵ���
  * @param    length;  ����
  * @param    data��   ָ���Ŷ�����
  *
  * @return
  *
  * @note     ��ֲʱ��Ҫ�޸�����
  *
  * @see
  *
  * @date     2019/4/29 ����һ
  */
void VL53_Read_nByte(unsigned char dev, unsigned char reg, unsigned char length, unsigned char* data);


#endif

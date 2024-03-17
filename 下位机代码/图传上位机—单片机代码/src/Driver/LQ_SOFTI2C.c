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

#include "LQ_SOFTI2C.h"


/*************************************************************************
*  �������ƣ�IIC��ʱ
*  ����˵����ADC��ʼ������
*  ����˵����us      ��  ��ʱʱ��
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  Ӧ�þ�����iic_delay(1);  //�򵥵���ʱ
*  �ڲ����� �޸�������Ե���IIC����
*************************************************************************/
void iic_delay()
{
	/* 200MHz ϵͳʱ���� ģ��IIC�ٶ�Ϊ 400Khz */
	
	unsigned char  i = 0;
	for(i = 0; i < 30; i++) //�޸�������Ե���IIC����
	{
		__asm("NOP"); /* delay */
	}
}

/*************************************************************************
*  �������ƣ�void IIC_Init(void)
*  ����˵����ģ��IIC��ʼ��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  Ӧ�þ�����IIC_Init();  //ģ��IIC��ʼ�� IIC�ܽ���LQ_SOFTI2C.h�ж���
*************************************************************************/
void IIC_Init(void)
{			
    IIC_SCL_INIT;
    IIC_SDA_INIT;
    IIC_SCL_H; 
    IIC_SDA_H;
}

/*************************************************************************
*  �������ƣ�void IIC_Start(void)
*  ����˵����ģ��IIC��ʼ�ź�
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  Ӧ�þ�����IIC_Start();
*************************************************************************/
void IIC_Start(void)
{
	SDA_OUT;   //sda����� 
	IIC_SDA_H;	
	IIC_SCL_H;
	iic_delay();
	iic_delay();
	iic_delay();
	iic_delay();
	iic_delay();
 	IIC_SDA_L; //START:when CLK is high,DATA change form high to low 
	iic_delay();
	iic_delay();
	iic_delay();
	IIC_SCL_L; //ǯסI2C���ߣ�׼�����ͻ�������� 
}


/*************************************************************************
*  �������ƣ�void IIC_Stop(void)
*  ����˵����ģ��IICֹͣ�ź�
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  Ӧ�þ�����IIC_Stop();
*************************************************************************/
void IIC_Stop(void)
{
	SDA_OUT; //sda�����
	IIC_SCL_L;
	IIC_SDA_L; //STOP:when CLK is high DATA change form low to high
    iic_delay();
	iic_delay();
	iic_delay();
	IIC_SCL_H; 
    iic_delay();
	iic_delay();
	iic_delay();
	IIC_SDA_H; //����I2C���߽����ź�
    iic_delay();							   	
}


/*************************************************************************
*  �������ƣ�unsigned char IIC_WaitAck(void)
*  ����˵����ģ��IIC�ȴ�Ӧ���ź�
*  ����˵������
*  �������أ�1������Ӧ��ʧ��    0������Ӧ��ɹ�
*  �޸�ʱ�䣺2020��3��10��
*  Ӧ�þ������ڲ����� ��ЧӦ�𣺴ӻ���9�� SCL=0 ʱ SDA ���ӻ�����,���� SCL = 1ʱ SDA��ȻΪ��
*************************************************************************/
unsigned char IIC_WaitAck(void)
{
	unsigned char  ucErrTime=0;
	SDA_IN; //SDA����Ϊ����  ���ӻ���һ���͵�ƽ��ΪӦ�� 
	IIC_SDA_H;iic_delay();	   
	IIC_SCL_H;iic_delay();	 
	while(IIC_SDA_READ)
	{
		ucErrTime++;
		if(ucErrTime>100)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL_L; //ʱ�����0 	   
	return 0;  
} 

/*************************************************************************
*  �������ƣ�void IIC_Ack(void)
*  ����˵����ģ��IIC����ACKӦ��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  Ӧ�þ������ڲ����� ����������һ���ֽ����ݺ�����������ACK֪ͨ�ӻ�һ���ֽ���������ȷ����
*************************************************************************/
void IIC_Ack(void)
{
	IIC_SCL_L;
	SDA_OUT;
	IIC_SDA_L;
	iic_delay();
	iic_delay();
	iic_delay();
	IIC_SCL_H;
	iic_delay();
	iic_delay();
	iic_delay();
	IIC_SCL_L;
}


/*************************************************************************
*  �������ƣ�void IIC_NAck(void)
*  ����˵����ģ��IIC������ACKӦ��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  Ӧ�þ������ڲ����� �������������һ���ֽ����ݺ�����������NACK֪ͨ�ӻ����ͽ������ͷ�SDA,�Ա���������ֹͣ�ź�
*************************************************************************/
void IIC_NAck(void)
{
	IIC_SCL_L;
	SDA_OUT;
	IIC_SDA_H;
	iic_delay();
	iic_delay();
	iic_delay();
	IIC_SCL_H;
	iic_delay();
	iic_delay();
	iic_delay();
	IIC_SCL_L;
}					 				     


/*************************************************************************
*  �������ƣ�void IIC_SendByte(unsigned char data_t)
*  ����˵����ģ��IIC����һ���ֽ�
*  ����˵����data   :  ���͵��ֽ�
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  Ӧ�þ�����IIC_SendByte(0x12);
*************************************************************************/
void IIC_SendByte(unsigned char data_t)
{                        
    unsigned char  t;
    SDA_OUT; 	    
    IIC_SCL_L; //����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {   
//        IIC_SDA_READ = data_t&0x80;	
        if(data_t&0x80)
        {
            IIC_SDA_H;
        }
        else 
        {
            IIC_SDA_L;
        }
        
        IIC_SCL_H;;
		iic_delay();
        data_t<<=1;
        iic_delay();
		iic_delay();
        IIC_SCL_L;	 
		iic_delay();		
    }
    iic_delay();
} 	 


/*************************************************************************
*  �������ƣ�unsigned char IIC_ReadByte(unsigned char ack)
*  ����˵����ģ��IIC��ȡһ���ֽ�
*  ����˵����ack=1 ʱ���������ݻ�û������ ack=0 ʱ����������ȫ���������
*  �������أ����յ����ֽ�
*  �޸�ʱ�䣺2020��3��10��
*  Ӧ�þ�����IC_ReadByte(0x12);
*************************************************************************/
unsigned char IIC_ReadByte(unsigned char ack)
{
	unsigned char  i,receive=0;
	SDA_IN; //SDA����Ϊ����ģʽ �ȴ����մӻ���������
    for(i=0;i<8;i++ )
	{
        IIC_SCL_L; 
		iic_delay();
		iic_delay();
		iic_delay();
        IIC_SCL_H;
        receive<<=1;
        if(IIC_SDA_READ)receive++; //�ӻ����͵ĵ�ƽ
        iic_delay();
    }					 
    if(ack)
        IIC_Ack(); //����ACK 
    else
        IIC_NAck(); //����nACK  
    return receive;
}

/*************************************************************************
*  �������ƣ�void ADC_init(void)
*  ����˵����ģ��IIC��ȡָ���豸 ָ���Ĵ�����һ��ֵ
*  ����˵����
  * @param    I2C_Addr  Ŀ���豸��ַ
  * @param    reg       Ŀ��Ĵ���
  * @param    buf       ��Ŷ����ֽ�
*  �������أ�1ʧ�� 0�ɹ�
*  �޸�ʱ�䣺2020��3��10��
*  Ӧ�þ�����IIC_ReadByteFromSlave(0xD0, 0x75, &data);   //�� IIC��ַΪ 0xD0������MPU6050���Ĵ���0x75
*************************************************************************/
unsigned char IIC_ReadByteFromSlave(unsigned char I2C_Addr,unsigned char reg,unsigned char *buf)
{
	IIC_Start();	
	IIC_SendByte(I2C_Addr);	 //���ʹӻ���ַ
	if(IIC_WaitAck()) //����ӻ�δӦ�������ݷ���ʧ��
	{
		IIC_Stop();
		return 1;
	}
	IIC_SendByte(reg); //���ͼĴ�����ַ
	IIC_WaitAck();	  
	
	IIC_Start();
	IIC_SendByte(I2C_Addr+1); //�������ģʽ			   
	IIC_WaitAck();
	*buf=IIC_ReadByte(0);	   
    IIC_Stop(); //����һ��ֹͣ����
	return 0;
}
 

/*************************************************************************
*  �������ƣ�void ADC_init(void)
*  ����˵����ģ��IICдָ���豸 ָ���Ĵ�����һ��ֵ
*  ����˵����
  * @param    I2C_Addr  Ŀ���豸��ַ
  * @param    reg       Ŀ��Ĵ���
  * @param    data      д�������
*  �������أ�1ʧ�� 0�ɹ�
*  �޸�ʱ�䣺2020��3��10��
*  Ӧ�þ�����IIC_ReadByteFromSlave(0xD0, 0X6B, 0X80);   //IIC��ַΪ 0xD0������MPU6050���Ĵ���0x6B д��0x80
*************************************************************************/
unsigned char IIC_WriteByteToSlave(unsigned char I2C_Addr,unsigned char reg,unsigned char data_t)
{
	IIC_Start();
	IIC_SendByte(I2C_Addr); //���ʹӻ���ַ
	if(IIC_WaitAck())
	{
		IIC_Stop();
		return 1; //�ӻ���ַд��ʧ��
	}
	IIC_SendByte(reg); //���ͼĴ�����ַ
    IIC_WaitAck();	  
	IIC_SendByte(data_t); 
	if(IIC_WaitAck())
	{
		IIC_Stop(); 
		return 1; //����д��ʧ��
	}
	IIC_Stop(); //����һ��ֹͣ����
    
    //return 1; //status == 0;
	return 0;
}

/*************************************************************************
*  �������ƣ�unsigned char IIC_ReadMultByteFromSlave(unsigned char dev, unsigned char reg, unsigned char length, unsigned char *data_t)
*  ����˵����ģ��IIC��ȡָ���豸 ָ���Ĵ�����n��ֵ
*  ����˵����
  * @param    dev       Ŀ���豸��ַ
  * @param    reg       Ŀ��Ĵ���
  * @param    length    ��ȡ����
  * @param    data      ��Ŷ�������
*  �������أ�1ʧ�� 0�ɹ�
*  �޸�ʱ�䣺2020��3��10��
*  Ӧ�þ�����IIC_ReadByteFromSlave(0xD0, 0X3B, 14, &data);   //�� 14���ֽ�
*************************************************************************/
unsigned char IIC_ReadMultByteFromSlave(unsigned char dev, unsigned char reg, unsigned char length, unsigned char *data_t)
{
    unsigned char  count = 0;
	unsigned char  temp;
	IIC_Start();
	IIC_SendByte(dev); //���ʹӻ���ַ
	if(IIC_WaitAck())
	{
		IIC_Stop(); 
		return 1; //�ӻ���ַд��ʧ��
	}
	IIC_SendByte(reg); //���ͼĴ�����ַ
    IIC_WaitAck();	  
	IIC_Start();
	IIC_SendByte(dev+1); //�������ģʽ	
	IIC_WaitAck();
    for(count=0;count<length;count++)
	{
		if(count!=(length-1))
            temp = IIC_ReadByte(1); //��ACK�Ķ�����
		else  
            temp = IIC_ReadByte(0); //���һ���ֽ�NACK
        
		data_t[count] = temp;
	}
    IIC_Stop(); //����һ��ֹͣ����
    //return count;
    return 0;
}

/*************************************************************************
*  �������ƣ�unsigned char IIC_WriteMultByteToSlave(unsigned char dev, unsigned char reg, unsigned char length, unsigned char* data_t)
*  ����˵����ģ��IICдָ���豸 ָ���Ĵ�����n��ֵ
*  ����˵����
  * @param    dev       Ŀ���豸��ַ
  * @param    reg       Ŀ��Ĵ���
  * @param    length    д�볤��
  * @param    data      ���д������
*  �������أ� 1ʧ�� 0�ɹ�
*  �޸�ʱ�䣺2020��3��10��
*  Ӧ�þ�����IIC_WriteMultByteToSlave(0xD0, 0X6B, 1, 0X80);   //��Ĵ���0x6Bд��0x80
*************************************************************************/
unsigned char IIC_WriteMultByteToSlave(unsigned char dev, unsigned char reg, unsigned char length, unsigned char* data_t)
{
    
 	unsigned char  count = 0;
	IIC_Start();
	IIC_SendByte(dev); //���ʹӻ���ַ
	if(IIC_WaitAck())
	{
		IIC_Stop();
		return 1; //�ӻ���ַд��ʧ��
	}
	IIC_SendByte(reg); //���ͼĴ�����ַ
    IIC_WaitAck();	  
	for(count=0;count<length;count++)
	{
		IIC_SendByte(data_t[count]); 
		if(IIC_WaitAck()) //ÿһ���ֽڶ�Ҫ�ȴӻ�Ӧ��
		{
			IIC_Stop();
			return 1; //����д��ʧ��
		}
	}
	IIC_Stop(); //����һ��ֹͣ����
    
	return 0;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
���������� int LQ_I2C_Write(unsigned char addr, unsigned char reg, unsigned int len, unsigned char *dat)
����  �ܡ� ģ��SPI��д���ݼ�����
* @param    addr   ���豸��ַ
* @param    reg    Ŀ��Ĵ���
* @param    len    д�볤��
* @param    *dat   ���д������
������ֵ�� 1ʧ�� 0�ɹ�
����  �ߡ� L Q
�������¡� 2021��4��3��
������汾�� V1.1
������������ LQ_I2C_Write(0x68, 0x38, 1, tmp)
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
int LQ_I2C_Write(unsigned char addr, unsigned char reg, unsigned int len, unsigned char *dat)
{
  unsigned int i = 0;

  IIC_Start();
  IIC_SendByte(addr << 1);
  IIC_WaitAck();

  IIC_SendByte(reg);
  IIC_WaitAck();

  for (i = 0; i < len; i++)
  {
      IIC_SendByte(dat[i]);
      IIC_WaitAck();
  }
  IIC_Stop();
  return 0;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
���������� int LQ_I2C_Read(unsigned char addr, unsigned char reg, unsigned int len, unsigned char *buf)
����  �ܡ� ģ��SPI��д���ݼ�����
* @param    addr   ���豸��ַ
* @param    reg    Ŀ��Ĵ���
* @param    len    д�볤��
* @param    *buf   ���д������
������ֵ�� 1ʧ�� 0�ɹ�
����  �ߡ� L Q
�������¡� 2021��4��3��
������汾�� V1.1
������������ LQ_I2C_Write(0x68, 0x38, 1, tmp)
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
int LQ_I2C_Read(unsigned char addr, unsigned char reg, unsigned int len, unsigned char *buf)
{
    IIC_Start();
    IIC_SendByte(addr << 1);
    IIC_WaitAck();

  IIC_SendByte(reg);
  IIC_WaitAck();
  iic_delay();
  IIC_Start();

  IIC_SendByte((addr << 1)+1);
  IIC_WaitAck();
  while (len)
  {
    if (len == 1)
      *buf = IIC_ReadByte(0);
    else
      *buf = IIC_ReadByte(1);
    buf++;
    len--;
  }
  IIC_Stop();
  return 0;
}


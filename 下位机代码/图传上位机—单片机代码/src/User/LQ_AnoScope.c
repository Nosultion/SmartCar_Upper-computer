/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ���Ƭ��ͨ�ú�����
����    д��chiusir
��E-mail  ��chiusir@163.com
������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
�������¡�2020��10��28��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://longqiu.taobao.com
------------------------------------------------
��dev.env.��
��Target �� 
��Crystal�� 
��SYS PLL�� 
________________________________________________________________
ʹ������4.3��λ��Э�飺ANO_TC�����ƴ�����վv4.34.exe
������յ����ݣ����ǲ���ʾ�������������ã�
��1�������½ǡ������ӡ���
��2�������Ͻǵ�2����ť���߼����롱������֡������λ��1��2��3�������رա�������ʾ��
��3�������Ͻǵ�������ť�����ݲ��Ρ�-->���½ǡ����á�-->�û�����-->ȷ����
       ���ز���ҳ��-->���½ǡ���ʼ��ʾ����ϣ�
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "../Driver/LQ_UART.h"

unsigned char data_to_send[50];


/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������void ANO_DT_send_int16(short data1,..., short data8 )
����  �ܡ�������λ���û�Э�飬ʹ��ʱռ��MCU��Դ�ϴ��ܳ�ʱ��ʹ��
������ֵ��short data1,..., short data8, �����͵�����
������ֵ��
������ֵ���� 
����  �ߡ�L Q
�������¡�2021��1��22�� 
������汾��V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void ANO_DT_send_int16byte16(short data1, short data2, short data3, short data4, short data5, short data6, short data7, short data8 )
{
  unsigned char  _cnt=0;
  unsigned char  sum = 0, i = 0;
  
  data_to_send[_cnt++] = 0xAA;      //����Э��֡ͷ  0xAAAA
  data_to_send[_cnt++] = 0xAA;
  data_to_send[_cnt++] = 0xF1;      //ʹ���û�Э��֡0xF1
  data_to_send[_cnt++] = 16;        //8��int16_t ���� 16���ֽ�
  
  data_to_send[_cnt++]=(unsigned short)(data1>>8);
  data_to_send[_cnt++]=(unsigned char)(data1);
  
  data_to_send[_cnt++]=(unsigned short)(data2>>8);
  data_to_send[_cnt++]=(unsigned char)(data2);
  
  data_to_send[_cnt++]=(unsigned short)(data3>>8);
  data_to_send[_cnt++]=(unsigned char)(data3);
  
  data_to_send[_cnt++]=(unsigned short)(data4>>8);
  data_to_send[_cnt++]=(unsigned char)(data4);
  
  data_to_send[_cnt++]=(unsigned short)(data5>>8);
  data_to_send[_cnt++]=(unsigned char)(data5);
  
  data_to_send[_cnt++]=(unsigned short)(data6>>8);
  data_to_send[_cnt++]=(unsigned char)(data6);
  
  data_to_send[_cnt++]=(unsigned short)(data7>>8);
  data_to_send[_cnt++]=(unsigned char)(data7);
  
  data_to_send[_cnt++]=(unsigned short)(data8>>8);
  data_to_send[_cnt++]=(unsigned char)(data8);
  
  sum = 0;
  for(i=0;i<_cnt;i++)
    sum += data_to_send[i];
  data_to_send[_cnt++]=sum;
  
  UART_PutBuff(UART0, data_to_send, _cnt);     //�����޸Ĳ�ͬ�Ĵ��ڷ�������;
}
void ANO_DT_send_int16(short data1, short data2, short data3, short data4, short data5, short data6, short data7, short data8 )
{
  unsigned char  _cnt=0;
  unsigned char  sum = 0, i = 0;
  
  data_to_send[_cnt++] = 0xAA;      //����Э��֡ͷ  0xAAAA
  data_to_send[_cnt++] = 0xAA;
  data_to_send[_cnt++] = 0xF1;      //ʹ���û�Э��֡0xF1
  data_to_send[_cnt++] = 16;        //8��int16_t ���� 16���ֽ�
  
  data_to_send[_cnt++]=(unsigned short)(data1>>8);
  data_to_send[_cnt++]=(unsigned char)(data1);
  
  data_to_send[_cnt++]=(unsigned short)(data2>>8);
  data_to_send[_cnt++]=(unsigned char)(data2);
  
  data_to_send[_cnt++]=(unsigned short)(data3>>8);
  data_to_send[_cnt++]=(unsigned char)(data3);
  
  data_to_send[_cnt++]=(unsigned short)(data4>>8);
  data_to_send[_cnt++]=(unsigned char)(data4);
  
  data_to_send[_cnt++]=(unsigned short)(data5>>8);
  data_to_send[_cnt++]=(unsigned char)(data5);
  
  data_to_send[_cnt++]=(unsigned short)(data6>>8);
  data_to_send[_cnt++]=(unsigned char)(data6);
  
  data_to_send[_cnt++]=(unsigned short)(data7>>8);
  data_to_send[_cnt++]=(unsigned char)(data7);
  
  data_to_send[_cnt++]=(unsigned short)(data8>>8);
  data_to_send[_cnt++]=(unsigned char)(data8);
  
  sum = 0;
  for(i=0;i<_cnt;i++)
    sum += data_to_send[i];
  data_to_send[_cnt++]=sum;
  
  UART_PutBuff(UART1, data_to_send, _cnt);     //�����޸Ĳ�ͬ�Ĵ��ڷ�������;
}
void ANO_DT_send_int16byte18(short data1, short data2, short data3, short data4, short data5, short data6, short data7, short data8, short data9 )
{
  unsigned char  _cnt=0;
  unsigned char  sum = 0, i = 0;
  
  data_to_send[_cnt++] = 0xAA;      //����Э��֡ͷ  0xAAAA
  data_to_send[_cnt++] = 0xAA;
  data_to_send[_cnt++] = 0xF1;      //ʹ���û�Э��֡0xF1
  data_to_send[_cnt++] = 18;        //9��int16_t ���� 18���ֽ�
  
  data_to_send[_cnt++]=(unsigned short)(data1>>8);
  data_to_send[_cnt++]=(unsigned char)(data1);
  
  data_to_send[_cnt++]=(unsigned short)(data2>>8);
  data_to_send[_cnt++]=(unsigned char)(data2);
  
  data_to_send[_cnt++]=(unsigned short)(data3>>8);
  data_to_send[_cnt++]=(unsigned char)(data3);
  
  data_to_send[_cnt++]=(unsigned short)(data4>>8);
  data_to_send[_cnt++]=(unsigned char)(data4);
  
  data_to_send[_cnt++]=(unsigned short)(data5>>8);
  data_to_send[_cnt++]=(unsigned char)(data5);
  
  data_to_send[_cnt++]=(unsigned short)(data6>>8);
  data_to_send[_cnt++]=(unsigned char)(data6);
  
  data_to_send[_cnt++]=(unsigned short)(data7>>8);
  data_to_send[_cnt++]=(unsigned char)(data7);
  
  data_to_send[_cnt++]=(unsigned short)(data8>>8);
  data_to_send[_cnt++]=(unsigned char)(data8);
  
  data_to_send[_cnt++]=(unsigned short)(data9>>8);
  data_to_send[_cnt++]=(unsigned char)(data9);
  
  sum = 0;
  for(i=0;i<_cnt;i++)
    sum += data_to_send[i];
  data_to_send[_cnt++]=sum;
  
  UART_PutBuff(UART0, data_to_send, _cnt);     //�����޸Ĳ�ͬ�Ĵ��ڷ�������;
}



#include "sht20.h"
#include "iic.h"
#include "delay.h"
#include "oled.h"
#include "stdio.h"

u8 tem_buffr[5] = {0};
u8 Rh_buffr[5] = {0};
u16 tem1 = 0,rh1 = 0;
float tem = 0;
float rh = 0;
char str[30];

void SHT20_Tem(void)//���¶�
{
	IIC_start();//��ʼ�ź�
	IIC_sendByte(0x80);//����������ַ
	while(IIC_waitAck())//�ȴ�Ӧ��
	{
		return ;
	}
	IIC_sendByte(0xF3);//���Ͳ����¶�����
	Delay_ms(80);
	IIC_start();//��ʼ�ź�
	IIC_sendByte(0x81);//���Ͷ�ȡ����
	while(IIC_waitAck())
	{
		IIC_start();//��ʼ�ź�
		IIC_sendByte(0x81);//���Ͷ�ȡ����
		Delay_us(100);
	}
	
	tem_buffr[0] = IIC_readByte(1);//��λ����
	tem_buffr[1] = IIC_readByte(1);//��λ����
	tem_buffr[2] = IIC_readByte(0);//У����
	
	IIC_stop();//ֹͣ�ź�
	/*���ݴ���*/
	tem_buffr[1] &= 0xFC;
	tem1 = ((tem_buffr[0]<<8) | tem_buffr[1]);
	tem =  175.72*(tem1/65536.0)-46.85;
	sprintf(str,"Tem: %.1f",tem);
	Display_String(30,2,str);
}


void SHT20_Rh(void)//��ʪ��
{
	IIC_start();//��ʼ�ź�
	IIC_sendByte(0x80);//����������ַ
	while(IIC_waitAck())//�ȴ�Ӧ��
	{
		return ;
	}
	IIC_sendByte(0xF5);//���Ͳ���ʪ������
	Delay_ms(25);
	IIC_start();//��ʼ�ź�
	IIC_sendByte(0x81);//���Ͷ�ȡ����
	
	while(IIC_waitAck())
	{
		IIC_start();//��ʼ�ź�
		IIC_sendByte(0x81);//���Ͷ�ȡ����
		Delay_us(100);
	}
	
	Rh_buffr[0] = IIC_readByte(1);//��λ����
	Rh_buffr[1] = IIC_readByte(1);//��λ����
	Rh_buffr[2] = IIC_readByte(0);//У����
	
	IIC_stop();//ֹͣ�ź�
	
	tem_buffr[1] &= 0xFC;
	rh1 = ((tem_buffr[0]<<8) | tem_buffr[1]);
	rh = -6 + 125*(rh1/65536.0);
	sprintf(str,"Hum: %.1f",rh);
	Display_String(30,4,str);
}









#ifndef __DS18B20_H
#define __DS18B20_H 

#include "sys.h" 
#include "delay.h"
#include "host_computer.h"

//IO��������
#define DS18B20_IO_IN()  {GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=8<<0;}
#define DS18B20_IO_OUT() {GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=3<<0;}
////IO��������											   
#define	DS18B20_DQ_OUT PAout(0) //���ݶ˿�	PA0
#define	DS18B20_DQ_IN  PAin(0)  //���ݶ˿�	PA0 
   	
#define ON  PAout(0)=1
#define OFF  PAout(0)=0

u8 DS18B20_Init(void);			//��ʼ��DS18B20
void DS18B20_Get_Temp_Hum(void);	//��ȡ�¶�
void AOSONG_Start(void);	
void DS18B20_Start(void);		//��ʼ�¶�ת��
void DS18B20_Write_Byte(u8 dat);//д��һ���ֽ�
u8 DS18B20_Read_Byte(void);		//����һ���ֽ�
u8 COM(void);	//��DS18B20��ȡһ���ֽ�
u8 DS18B20_Read_Bit(void);		//����һ��λ
u8 DS18B20_Check(void);			//����Ƿ����DS18B20
void DS18B20_Rst(void);			//��λDS18B2

#endif

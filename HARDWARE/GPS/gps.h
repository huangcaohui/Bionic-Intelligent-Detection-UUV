#ifndef __GPS_H
#define __GPS_H	 

#include "sys.h"
#include "usart2.h"	
#include "host_computer.h"
#include "delay.h" 								    								   
#include "stdio.h"	 
#include "stdarg.h"	 
#include "string.h"	 
#include "math.h"

//UTCʱ����Ϣ
__packed typedef struct  
{										    
 	u16 year;	//���
	u8 month;	//�·�
	u8 date;	//����
	u8 hour; 	//Сʱ
	u8 min; 	//����
	u8 sec; 	//����
}nmea_utc_time;

//NMEA 0183 Э����������ݴ�Žṹ��
__packed typedef struct  
{										    
	nmea_utc_time utc;			//UTCʱ��
	u32 latitude;				//γ�� ������1e6��,ʵ��Ҫ����1e6
	u8 nshemi;					//��γ/��γ,N:��γ;S:��γ				  
	u32 longitude;			    //���� ������1e6��,ʵ��Ҫ����1e6
	u8 ewhemi;					//����/����,E:����;W:����
	u8 gpssta;					//GPS״̬:0,δ��λ;1,�ǲ�ֶ�λ;2,��ֶ�λ;6,���ڹ���.

	u16 speed;					//��������,�Ŵ���1000��,ʵ�ʳ���10.��λ:0.001����/Сʱ	 
}nmea_msg;

 	////////////////////////////////////////////////////////////////////////////////////////////////////
//SkyTra S1216F8 ���ò����ʽṹ��
__packed typedef struct
{
	u16 sos;            //�������У��̶�Ϊ0XA0A1
	u16 PL;             //��Ч���ݳ���0X0004�� 
	u8 id;             //ID���̶�Ϊ0X05
	u8 com_port;       //COM�ڣ��̶�Ϊ0X00����COM1   
	u8 Baud_id;       //�����ʣ�0~8,4800,9600,19200,38400,57600,115200,230400,460800,921600��
	u8 Attributes;     //�������ݱ���λ�� ,0���浽SRAM��1���浽SRAM&FLASH��2��ʱ����
	u8 CS;             //У��ֵ
	u16 end;            //������:0X0D0A  
}SkyTra_baudrate;

//////////////////////////////////////////////////////////////////////////////////////////////////// 	
//SkyTra S1216F8 ���������Ϣ�ṹ��
__packed typedef struct
{
	u16 sos;            //�������У��̶�Ϊ0XA0A1
	u16 PL;             //��Ч���ݳ���0X0009�� 
	u8 id;             //ID���̶�Ϊ0X08
	u8 GGA;            //1~255��s��,0:disable
	u8 GSA;            //1~255��s��,0:disable
	u8 GSV;            //1~255��s��,0:disable
	u8 GLL;            //1~255��s��,0:disable
	u8 RMC;            //1~255��s��,0:disable
	u8 VTG;            //1~255��s��,0:disable
	u8 ZDA;            //1~255��s��,0:disable
	u8 Attributes;     //�������ݱ���λ�� ,0���浽SRAM��1���浽SRAM&FLASH��2��ʱ����
	u8 CS;             //У��ֵ
	u16 end;            //������:0X0D0A  
}SkyTra_outmsg;

//////////////////////////////////////////////////////////////////////////////////////////////////// 	
//SkyTra S1216F8 ����λ�ø����ʽṹ��
__packed typedef struct
{
	u16 sos;            //�������У��̶�Ϊ0XA0A1
	u16 PL;             //��Ч���ݳ���0X0003�� 
	u8 id;             //ID���̶�Ϊ0X0E
	u8 rate;           //ȡֵ��Χ:1, 2, 4, 5, 8, 10, 20, 25, 40, 50
	u8 Attributes;     //�������ݱ���λ�� ,0���浽SRAM��1���浽SRAM&FLASH��2��ʱ����
	u8 CS;             //У��ֵ
	u16 end;            //������:0X0D0A  
}SkyTra_PosRate;

//////////////////////////////////////////////////////////////////////////////////////////////////// 	
//SkyTra S1216F8 �����������(PPS)��Ƚṹ��
__packed typedef struct
{
	u16 sos;            //�������У��̶�Ϊ0XA0A1
	u16 PL;             //��Ч���ݳ���0X0007�� 
	u8 id;             //ID���̶�Ϊ0X65
	u8 Sub_ID;         //0X01
	u32 width;        //1~100000(us)
	u8 Attributes;     //�������ݱ���λ�� ,0���浽SRAM��1���浽SRAM&FLASH��2��ʱ����
	u8 CS;             //У��ֵ
	u16 end;            //������:0X0D0A 
}SkyTra_pps_width;

//////////////////////////////////////////////////////////////////////////////////////////////////// 	
//SkyTra S1216F8 ACK�ṹ��
__packed typedef struct
{
	u16 sos;            //�������У��̶�Ϊ0XA0A1
	u16 PL;             //��Ч���ݳ���0X0002�� 
	u8 id;             //ID���̶�Ϊ0X83
	u8 ACK_ID;         //ACK ID may further consist of message ID and message sub-ID which will become 3 bytes of ACK message
	u8 CS;             //У��ֵ
	u16 end;            //������ 
}SkyTra_ACK;

//////////////////////////////////////////////////////////////////////////////////////////////////// 	
//SkyTra S1216F8 NACK�ṹ��
__packed typedef struct
{
	u16 sos;            //�������У��̶�Ϊ0XA0A1
	u16 PL;             //��Ч���ݳ���0X0002�� 
	u8 id;             //ID���̶�Ϊ0X84
	u8 NACK_ID;         //ACK ID may further consist of message ID and message sub-ID which will become 3 bytes of ACK message
	u8 CS;             //У��ֵ
	u16 end;            //������ 
}SkyTra_NACK;

void gps_rxData_handel(void);
int NMEA_Str2num(u8 *buf,u8*dx);
void GPS_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GNRMC_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GNVTG_Analysis(nmea_msg *gpsx,u8 *buf);
u8 SkyTra_Cfg_Msg(u8 msgid,u8 uart1set);
u8 SkyTra_Cfg_Prt(u32 baud_id);
u8 SkyTra_Cfg_Tp(u32 width);
u8 SkyTra_Cfg_Rate(u8 Frep);
void SkyTra_Send_Date(u8* dbuf,u16 len);

#endif

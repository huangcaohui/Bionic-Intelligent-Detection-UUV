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

//UTC时间信息
__packed typedef struct  
{										    
 	u16 year;	//年份
	u8 month;	//月份
	u8 date;	//日期
	u8 hour; 	//小时
	u8 min; 	//分钟
	u8 sec; 	//秒钟
}nmea_utc_time;

//NMEA 0183 协议解析后数据存放结构体
__packed typedef struct  
{										    
	nmea_utc_time utc;			//UTC时间
	u32 latitude;				//纬度 分扩大1e6倍,实际要除以1e6
	u8 nshemi;					//北纬/南纬,N:北纬;S:南纬				  
	u32 longitude;			    //经度 分扩大1e6倍,实际要除以1e6
	u8 ewhemi;					//东经/西经,E:东经;W:西经
	u8 gpssta;					//GPS状态:0,未定位;1,非差分定位;2,差分定位;6,正在估算.

	u16 speed;					//地面速率,放大了1000倍,实际除以10.单位:0.001公里/小时	 
}nmea_msg;

 	////////////////////////////////////////////////////////////////////////////////////////////////////
//SkyTra S1216F8 配置波特率结构体
__packed typedef struct
{
	u16 sos;            //启动序列，固定为0XA0A1
	u16 PL;             //有效数据长度0X0004； 
	u8 id;             //ID，固定为0X05
	u8 com_port;       //COM口，固定为0X00，即COM1   
	u8 Baud_id;       //波特率（0~8,4800,9600,19200,38400,57600,115200,230400,460800,921600）
	u8 Attributes;     //配置数据保存位置 ,0保存到SRAM，1保存到SRAM&FLASH，2临时保存
	u8 CS;             //校验值
	u16 end;            //结束符:0X0D0A  
}SkyTra_baudrate;

//////////////////////////////////////////////////////////////////////////////////////////////////// 	
//SkyTra S1216F8 配置输出信息结构体
__packed typedef struct
{
	u16 sos;            //启动序列，固定为0XA0A1
	u16 PL;             //有效数据长度0X0009； 
	u8 id;             //ID，固定为0X08
	u8 GGA;            //1~255（s）,0:disable
	u8 GSA;            //1~255（s）,0:disable
	u8 GSV;            //1~255（s）,0:disable
	u8 GLL;            //1~255（s）,0:disable
	u8 RMC;            //1~255（s）,0:disable
	u8 VTG;            //1~255（s）,0:disable
	u8 ZDA;            //1~255（s）,0:disable
	u8 Attributes;     //配置数据保存位置 ,0保存到SRAM，1保存到SRAM&FLASH，2临时保存
	u8 CS;             //校验值
	u16 end;            //结束符:0X0D0A  
}SkyTra_outmsg;

//////////////////////////////////////////////////////////////////////////////////////////////////// 	
//SkyTra S1216F8 配置位置更新率结构体
__packed typedef struct
{
	u16 sos;            //启动序列，固定为0XA0A1
	u16 PL;             //有效数据长度0X0003； 
	u8 id;             //ID，固定为0X0E
	u8 rate;           //取值范围:1, 2, 4, 5, 8, 10, 20, 25, 40, 50
	u8 Attributes;     //配置数据保存位置 ,0保存到SRAM，1保存到SRAM&FLASH，2临时保存
	u8 CS;             //校验值
	u16 end;            //结束符:0X0D0A  
}SkyTra_PosRate;

//////////////////////////////////////////////////////////////////////////////////////////////////// 	
//SkyTra S1216F8 配置输出脉冲(PPS)宽度结构体
__packed typedef struct
{
	u16 sos;            //启动序列，固定为0XA0A1
	u16 PL;             //有效数据长度0X0007； 
	u8 id;             //ID，固定为0X65
	u8 Sub_ID;         //0X01
	u32 width;        //1~100000(us)
	u8 Attributes;     //配置数据保存位置 ,0保存到SRAM，1保存到SRAM&FLASH，2临时保存
	u8 CS;             //校验值
	u16 end;            //结束符:0X0D0A 
}SkyTra_pps_width;

//////////////////////////////////////////////////////////////////////////////////////////////////// 	
//SkyTra S1216F8 ACK结构体
__packed typedef struct
{
	u16 sos;            //启动序列，固定为0XA0A1
	u16 PL;             //有效数据长度0X0002； 
	u8 id;             //ID，固定为0X83
	u8 ACK_ID;         //ACK ID may further consist of message ID and message sub-ID which will become 3 bytes of ACK message
	u8 CS;             //校验值
	u16 end;            //结束符 
}SkyTra_ACK;

//////////////////////////////////////////////////////////////////////////////////////////////////// 	
//SkyTra S1216F8 NACK结构体
__packed typedef struct
{
	u16 sos;            //启动序列，固定为0XA0A1
	u16 PL;             //有效数据长度0X0002； 
	u8 id;             //ID，固定为0X84
	u8 NACK_ID;         //ACK ID may further consist of message ID and message sub-ID which will become 3 bytes of ACK message
	u8 CS;             //校验值
	u16 end;            //结束符 
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

#ifndef __DS18B20_H
#define __DS18B20_H 

#include "sys.h" 
#include "delay.h"
#include "host_computer.h"

//IO方向设置
#define DS18B20_IO_IN()  {GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=8<<0;}
#define DS18B20_IO_OUT() {GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=3<<0;}
////IO操作函数											   
#define	DS18B20_DQ_OUT PAout(0) //数据端口	PA0
#define	DS18B20_DQ_IN  PAin(0)  //数据端口	PA0 
   	
#define ON  PAout(0)=1
#define OFF  PAout(0)=0

u8 DS18B20_Init(void);			//初始化DS18B20
void DS18B20_Get_Temp_Hum(void);	//获取温度
void AOSONG_Start(void);	
void DS18B20_Start(void);		//开始温度转换
void DS18B20_Write_Byte(u8 dat);//写入一个字节
u8 DS18B20_Read_Byte(void);		//读出一个字节
u8 COM(void);	//从DS18B20读取一个字节
u8 DS18B20_Read_Bit(void);		//读出一个位
u8 DS18B20_Check(void);			//检测是否存在DS18B20
void DS18B20_Rst(void);			//复位DS18B2

#endif

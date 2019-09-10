#include "gps.h" 	   

const u32 BAUD_id[9]={4800,9600,19200,38400,57600,115200,230400,460800,921600};//模块支持波特率数组
nmea_msg gpsx;	//GPS信息
u8 gps_rx_buf[USART2_RX_LEN];	//GPS接收缓存区
u16 gps_rx_len;	//接收到的GPS数据长度
u16 i;

void gps_rxData_handel(void)
{
	if(usart2_rx_sta&0X8000)		//接收到一次数据了
	{
		gps_rx_len = usart2_rx_sta&0X7FFF;	//得到GPS数据长度
			
		for(i=0; i < gps_rx_len; ++i)
		{
			gps_rx_buf[i]=usart2_rx_buf[i];
		}
			
 		usart2_rx_sta=0;		   	//启动下一次接收
		gps_rx_buf[i]=0;			//自动添加结束符
		GPS_Analysis(&gpsx,(u8*)gps_rx_buf);//分析字符串
			
		locData.longitude = gpsx.longitude/1e6;
		locData.latitude = gpsx.latitude/1e6;
		sailData.speed = gpsx.speed/1e3;
			
		DMA1_Data_Transf(1);
 	}
}

//从buf里面得到第cx个逗号所在的位置
//返回值:0~0XFE,代表逗号所在位置的偏移.
//       0XFF,代表不存在第cx个逗号							  
u8 NMEA_Comma_Pos(u8 *buf,u8 cx)
{	 		    
	u8 *p=buf;
	while(cx)
	{		 
		if(*buf=='*'||*buf<' '||*buf>'z')return 0XFF;//遇到'*'或者非法字符,则不存在第cx个逗号
		if(*buf==',')cx--;
		buf++;
	}
	return buf-p;	 
}

//m^n函数
//返回值:m^n次方.
u32 NMEA_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}

//str转换为数字,以','或者'*'结束
//buf:数字存储区
//dx:小数点位数,返回给调用函数
//返回值:转换后的数值
int NMEA_Str2num(u8 *buf,u8*dx)
{
	u8 *p=buf;
	u32 ires=0,fres=0;
	u8 ilen=0,flen=0,i;
	u8 mask=0;
	int res;
	while(1) //得到整数和小数的长度
	{
		if(*p=='-'){mask|=0X02;p++;}//是负数
		if(*p==','||(*p=='*'))break;//遇到结束了
		if(*p=='.'){mask|=0X01;p++;}//遇到小数点了
		else if(*p>'9'||(*p<'0'))	//有非法字符
		{	
			ilen=0;
			flen=0;
			break;
		}	
		if(mask&0X01)flen++;
		else ilen++;
		p++;
	}
	if(mask&0X02)buf++;	//去掉负号
	for(i=0;i<ilen;i++)	//得到整数部分数据
	{  
		ires+=NMEA_Pow(10,ilen-1-i)*(buf[i]-'0');
	}
	if(flen>5)flen=5;	//最多取5位小数
	*dx=flen;	 		//小数点位数
	for(i=0;i<flen;i++)	//得到小数部分数据
	{  
		fres+=NMEA_Pow(10,flen-1-i)*(buf[ilen+1+i]-'0');
	} 
	res=ires*NMEA_Pow(10,flen)+fres;
	if(mask&0X02)res=-res;		   
	return res;
}	  							 

//分析GNRMC信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void NMEA_GNRMC_Analysis(nmea_msg *gpsx,u8 *buf)
{
	u8 *p1,dx;			 
	u8 posx;     
	u32 temp;	   
	float rs;  
	p1=(u8*)strstr((const char *)buf,"$GNRMC");//"$GNRMC",经常有&和GNRMC分开的情况,故只判断GPRMC.
	posx=NMEA_Comma_Pos(p1,1);								//得到UTC时间
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx)/NMEA_Pow(10,dx);	 	//得到UTC时间,去掉ms
		gpsx->utc.hour=temp/10000;
		gpsx->utc.min=(temp/100)%100;
		gpsx->utc.sec=temp%100;	 	 
	}	
	
	posx=NMEA_Comma_Pos(p1,3);								//得到纬度
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		
		gpsx->latitude=temp/NMEA_Pow(10,dx+2);	//得到°
		rs=temp%NMEA_Pow(10,dx+2);				//得到'	
		gpsx->latitude=gpsx->latitude*NMEA_Pow(10,6)+(rs*NMEA_Pow(10,6-dx))/60;//转换为° 
		
	}
	
	posx=NMEA_Comma_Pos(p1,4);								//南纬还是北纬 
	if(posx!=0XFF)gpsx->nshemi=*(p1+posx);	
	
 	posx=NMEA_Comma_Pos(p1,5);								//得到经度
	if(posx!=0XFF)
	{												  
		temp=NMEA_Str2num(p1+posx,&dx);		 	 
		gpsx->longitude=temp/NMEA_Pow(10,dx+2);	//得到°
		rs=temp%NMEA_Pow(10,dx+2);				//得到'		 
		gpsx->longitude=gpsx->longitude*NMEA_Pow(10,6)+(rs*NMEA_Pow(10,6-dx))/60;//转换为° 
	}
	
	posx=NMEA_Comma_Pos(p1,6);								//东经还是西经
	if(posx!=0XFF)gpsx->ewhemi=*(p1+posx);	
	
	posx=NMEA_Comma_Pos(p1,9);								//得到UTC日期
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		 				//得到UTC日期
		gpsx->utc.date=temp/10000;
		gpsx->utc.month=(temp/100)%100;
		gpsx->utc.year=2000+temp%100;	 	 
	} 
}

//分析GNVTG信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void NMEA_GNVTG_Analysis(nmea_msg *gpsx,u8 *buf)
{
	u8 *p1,dx;			 
	u8 posx;    
	p1=(u8*)strstr((const char *)buf,"$GNVTG");							 
	posx=NMEA_Comma_Pos(p1,7);								//得到地面速率
	if(posx!=0XFF)
	{
		gpsx->speed=NMEA_Str2num(p1+posx,&dx);
		if(dx<3)gpsx->speed*=NMEA_Pow(10,3-dx);	 	 		//确保扩大1000倍
	}
}  

//提取NMEA-0183信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void GPS_Analysis(nmea_msg *gpsx,u8 *buf)
{
	NMEA_GNRMC_Analysis(gpsx,buf);	//GPNMC解析
	NMEA_GNVTG_Analysis(gpsx,buf);	//GPNTG解析
}


///////////////////////////////////////////UBLOX 配置代码/////////////////////////////////////
////检查CFG配置执行情况
////返回值:0,ACK成功
////       1,接收超时错误
////       2,没有找到同步字符
////       3,接收到NACK应答
u8 SkyTra_Cfg_Ack_Check(void)
{			 
	u16 len=0,i;
	u8 rval=0;
	while((usart2_rx_sta&0X8000)==0 && len<100)//等待接收到应答   
	{
		len++;
		delay_ms(5);
	}		 
	if(len<100)   	//超时错误.
	{
		len=usart2_rx_sta&0X7FFF;	//此次接收到的数据长度 
		for(i=0;i<len;i++)
		{
			if(usart2_rx_buf[i]==0X83)break;
			else if(usart2_rx_buf[i]==0X84)
			{
				rval=3;
				break;
			}
		}
		if(i==len)rval=2;						//没有找到同步字符
	}else rval=1;								//接收超时错误
    usart2_rx_sta=0;							//清除接收
	return rval;  
}

//配置SkyTra_GPS/北斗模块波特率
//baud_id:0~8，对应波特率,4800/9600/19200/38400/57600/115200/230400/460800/921600	  
//返回值:0,执行成功;其他,执行失败(这里不会返回0了)
u8 SkyTra_Cfg_Prt(u32 baud_id)
{
	SkyTra_baudrate *cfg_prt=(SkyTra_baudrate *)usart2_tx_buf;
	cfg_prt->sos=0XA1A0;		//引导序列(小端模式)
	cfg_prt->PL=0X0400;			//有效数据长度(小端模式)
	cfg_prt->id=0X05;		    //配置波特率的ID 
	cfg_prt->com_port=0X00;			//操作串口1
	cfg_prt->Baud_id=baud_id;	 	////波特率对应编号
	cfg_prt->Attributes=1; 		  //保存到SRAM&FLASH
	cfg_prt->CS=cfg_prt->id^cfg_prt->com_port^cfg_prt->Baud_id^cfg_prt->Attributes;
	cfg_prt->end=0X0A0D;        //发送结束符(小端模式)
	SkyTra_Send_Date((u8*)cfg_prt,sizeof(SkyTra_baudrate));//发送数据给SkyTra   
	delay_ms(200);				//等待发送完成 
	Usart2_Init(BAUD_id[baud_id]);	//重新初始化串口2
	return SkyTra_Cfg_Ack_Check();//这里不会反回0,因为UBLOX发回来的应答在串口重新初始化的时候已经被丢弃了.
} 

//配置SkyTra_GPS模块的时钟脉冲宽度
//width:脉冲宽度1~100000(us)
//返回值:0,发送成功;其他,发送失败.
u8 SkyTra_Cfg_Tp(u32 width)
{
	u32 temp=width;
	SkyTra_pps_width *cfg_tp=(SkyTra_pps_width *)usart2_tx_buf;
	temp=(width>>24)|((width>>8)&0X0000FF00)|((width<<8)&0X00FF0000)|((width<<24)&0XFF000000);//小端模式
	cfg_tp->sos=0XA1A0;		    //cfg header(小端模式)
	cfg_tp->PL=0X0700;        //有效数据长度(小端模式)
	cfg_tp->id=0X65	;			    //cfg tp id
	cfg_tp->Sub_ID=0X01;			//数据区长度为20个字节.
	cfg_tp->width=temp;		  //脉冲宽度,us
	cfg_tp->Attributes=0X01;  //保存到SRAM&FLASH	
	cfg_tp->CS=cfg_tp->id^cfg_tp->Sub_ID^(cfg_tp->width>>24)^(cfg_tp->width>>16)&0XFF^(cfg_tp->width>>8)&0XFF^cfg_tp->width&0XFF^cfg_tp->Attributes;    	//用户延时为0ns
	cfg_tp->end=0X0A0D;       //发送结束符(小端模式)
	SkyTra_Send_Date((u8*)cfg_tp,sizeof(SkyTra_pps_width));//发送数据给NEO-6M  
	return SkyTra_Cfg_Ack_Check();
}

//配置SkyTraF8-BD的更新速率	 	    
//Frep:（取值范围:1,2,4,5,8,10,20）测量时间间隔，单位为Hz，最大不能大于20Hz
//返回值:0,发送成功;其他,发送失败.
u8 SkyTra_Cfg_Rate(u8 Frep)
{
	SkyTra_PosRate *cfg_rate=(SkyTra_PosRate *)usart2_tx_buf;
 	cfg_rate->sos=0XA1A0;	    //cfg header(小端模式)
	cfg_rate->PL=0X0300;			//有效数据长度(小端模式)
	cfg_rate->id=0X0E;	      //cfg rate id
	cfg_rate->rate=Frep;	 	  //更新速率
	cfg_rate->Attributes=0X01;	   	//保存到SRAM&FLASH	.
	cfg_rate->CS=cfg_rate->id^cfg_rate->rate^cfg_rate->Attributes;//脉冲间隔,us
	cfg_rate->end=0X0A0D;       //发送结束符(小端模式)
	SkyTra_Send_Date((u8*)cfg_rate,sizeof(SkyTra_PosRate));//发送数据给NEO-6M 
	return SkyTra_Cfg_Ack_Check();
}

//发送一批数据给SkyTraF8-BD，这里通过串口2发送
//dbuf：数据缓存首地址
//len：要发送的字节数
void SkyTra_Send_Date(u8* dbuf,u16 len)
{
	u16 j;
	for(j=0;j<len;j++)//循环发送数据
	{
		while((USART2->SR&0X40)==0);//循环发送,直到发送完毕   
		USART2->DR=dbuf[j];  
	}	
}

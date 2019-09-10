#include "gps.h" 	   

const u32 BAUD_id[9]={4800,9600,19200,38400,57600,115200,230400,460800,921600};//ģ��֧�ֲ���������
nmea_msg gpsx;	//GPS��Ϣ
u8 gps_rx_buf[USART2_RX_LEN];	//GPS���ջ�����
u16 gps_rx_len;	//���յ���GPS���ݳ���
u16 i;

void gps_rxData_handel(void)
{
	if(usart2_rx_sta&0X8000)		//���յ�һ��������
	{
		gps_rx_len = usart2_rx_sta&0X7FFF;	//�õ�GPS���ݳ���
			
		for(i=0; i < gps_rx_len; ++i)
		{
			gps_rx_buf[i]=usart2_rx_buf[i];
		}
			
 		usart2_rx_sta=0;		   	//������һ�ν���
		gps_rx_buf[i]=0;			//�Զ���ӽ�����
		GPS_Analysis(&gpsx,(u8*)gps_rx_buf);//�����ַ���
			
		locData.longitude = gpsx.longitude/1e6;
		locData.latitude = gpsx.latitude/1e6;
		sailData.speed = gpsx.speed/1e3;
			
		DMA1_Data_Transf(1);
 	}
}

//��buf����õ���cx���������ڵ�λ��
//����ֵ:0~0XFE,����������λ�õ�ƫ��.
//       0XFF,�������ڵ�cx������							  
u8 NMEA_Comma_Pos(u8 *buf,u8 cx)
{	 		    
	u8 *p=buf;
	while(cx)
	{		 
		if(*buf=='*'||*buf<' '||*buf>'z')return 0XFF;//����'*'���߷Ƿ��ַ�,�򲻴��ڵ�cx������
		if(*buf==',')cx--;
		buf++;
	}
	return buf-p;	 
}

//m^n����
//����ֵ:m^n�η�.
u32 NMEA_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}

//strת��Ϊ����,��','����'*'����
//buf:���ִ洢��
//dx:С����λ��,���ظ����ú���
//����ֵ:ת�������ֵ
int NMEA_Str2num(u8 *buf,u8*dx)
{
	u8 *p=buf;
	u32 ires=0,fres=0;
	u8 ilen=0,flen=0,i;
	u8 mask=0;
	int res;
	while(1) //�õ�������С���ĳ���
	{
		if(*p=='-'){mask|=0X02;p++;}//�Ǹ���
		if(*p==','||(*p=='*'))break;//����������
		if(*p=='.'){mask|=0X01;p++;}//����С������
		else if(*p>'9'||(*p<'0'))	//�зǷ��ַ�
		{	
			ilen=0;
			flen=0;
			break;
		}	
		if(mask&0X01)flen++;
		else ilen++;
		p++;
	}
	if(mask&0X02)buf++;	//ȥ������
	for(i=0;i<ilen;i++)	//�õ�������������
	{  
		ires+=NMEA_Pow(10,ilen-1-i)*(buf[i]-'0');
	}
	if(flen>5)flen=5;	//���ȡ5λС��
	*dx=flen;	 		//С����λ��
	for(i=0;i<flen;i++)	//�õ�С����������
	{  
		fres+=NMEA_Pow(10,flen-1-i)*(buf[ilen+1+i]-'0');
	} 
	res=ires*NMEA_Pow(10,flen)+fres;
	if(mask&0X02)res=-res;		   
	return res;
}	  							 

//����GNRMC��Ϣ
//gpsx:nmea��Ϣ�ṹ��
//buf:���յ���GPS���ݻ������׵�ַ
void NMEA_GNRMC_Analysis(nmea_msg *gpsx,u8 *buf)
{
	u8 *p1,dx;			 
	u8 posx;     
	u32 temp;	   
	float rs;  
	p1=(u8*)strstr((const char *)buf,"$GNRMC");//"$GNRMC",������&��GNRMC�ֿ������,��ֻ�ж�GPRMC.
	posx=NMEA_Comma_Pos(p1,1);								//�õ�UTCʱ��
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx)/NMEA_Pow(10,dx);	 	//�õ�UTCʱ��,ȥ��ms
		gpsx->utc.hour=temp/10000;
		gpsx->utc.min=(temp/100)%100;
		gpsx->utc.sec=temp%100;	 	 
	}	
	
	posx=NMEA_Comma_Pos(p1,3);								//�õ�γ��
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		
		gpsx->latitude=temp/NMEA_Pow(10,dx+2);	//�õ���
		rs=temp%NMEA_Pow(10,dx+2);				//�õ�'	
		gpsx->latitude=gpsx->latitude*NMEA_Pow(10,6)+(rs*NMEA_Pow(10,6-dx))/60;//ת��Ϊ�� 
		
	}
	
	posx=NMEA_Comma_Pos(p1,4);								//��γ���Ǳ�γ 
	if(posx!=0XFF)gpsx->nshemi=*(p1+posx);	
	
 	posx=NMEA_Comma_Pos(p1,5);								//�õ�����
	if(posx!=0XFF)
	{												  
		temp=NMEA_Str2num(p1+posx,&dx);		 	 
		gpsx->longitude=temp/NMEA_Pow(10,dx+2);	//�õ���
		rs=temp%NMEA_Pow(10,dx+2);				//�õ�'		 
		gpsx->longitude=gpsx->longitude*NMEA_Pow(10,6)+(rs*NMEA_Pow(10,6-dx))/60;//ת��Ϊ�� 
	}
	
	posx=NMEA_Comma_Pos(p1,6);								//������������
	if(posx!=0XFF)gpsx->ewhemi=*(p1+posx);	
	
	posx=NMEA_Comma_Pos(p1,9);								//�õ�UTC����
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		 				//�õ�UTC����
		gpsx->utc.date=temp/10000;
		gpsx->utc.month=(temp/100)%100;
		gpsx->utc.year=2000+temp%100;	 	 
	} 
}

//����GNVTG��Ϣ
//gpsx:nmea��Ϣ�ṹ��
//buf:���յ���GPS���ݻ������׵�ַ
void NMEA_GNVTG_Analysis(nmea_msg *gpsx,u8 *buf)
{
	u8 *p1,dx;			 
	u8 posx;    
	p1=(u8*)strstr((const char *)buf,"$GNVTG");							 
	posx=NMEA_Comma_Pos(p1,7);								//�õ���������
	if(posx!=0XFF)
	{
		gpsx->speed=NMEA_Str2num(p1+posx,&dx);
		if(dx<3)gpsx->speed*=NMEA_Pow(10,3-dx);	 	 		//ȷ������1000��
	}
}  

//��ȡNMEA-0183��Ϣ
//gpsx:nmea��Ϣ�ṹ��
//buf:���յ���GPS���ݻ������׵�ַ
void GPS_Analysis(nmea_msg *gpsx,u8 *buf)
{
	NMEA_GNRMC_Analysis(gpsx,buf);	//GPNMC����
	NMEA_GNVTG_Analysis(gpsx,buf);	//GPNTG����
}


///////////////////////////////////////////UBLOX ���ô���/////////////////////////////////////
////���CFG����ִ�����
////����ֵ:0,ACK�ɹ�
////       1,���ճ�ʱ����
////       2,û���ҵ�ͬ���ַ�
////       3,���յ�NACKӦ��
u8 SkyTra_Cfg_Ack_Check(void)
{			 
	u16 len=0,i;
	u8 rval=0;
	while((usart2_rx_sta&0X8000)==0 && len<100)//�ȴ����յ�Ӧ��   
	{
		len++;
		delay_ms(5);
	}		 
	if(len<100)   	//��ʱ����.
	{
		len=usart2_rx_sta&0X7FFF;	//�˴ν��յ������ݳ��� 
		for(i=0;i<len;i++)
		{
			if(usart2_rx_buf[i]==0X83)break;
			else if(usart2_rx_buf[i]==0X84)
			{
				rval=3;
				break;
			}
		}
		if(i==len)rval=2;						//û���ҵ�ͬ���ַ�
	}else rval=1;								//���ճ�ʱ����
    usart2_rx_sta=0;							//�������
	return rval;  
}

//����SkyTra_GPS/����ģ�鲨����
//baud_id:0~8����Ӧ������,4800/9600/19200/38400/57600/115200/230400/460800/921600	  
//����ֵ:0,ִ�гɹ�;����,ִ��ʧ��(���ﲻ�᷵��0��)
u8 SkyTra_Cfg_Prt(u32 baud_id)
{
	SkyTra_baudrate *cfg_prt=(SkyTra_baudrate *)usart2_tx_buf;
	cfg_prt->sos=0XA1A0;		//��������(С��ģʽ)
	cfg_prt->PL=0X0400;			//��Ч���ݳ���(С��ģʽ)
	cfg_prt->id=0X05;		    //���ò����ʵ�ID 
	cfg_prt->com_port=0X00;			//��������1
	cfg_prt->Baud_id=baud_id;	 	////�����ʶ�Ӧ���
	cfg_prt->Attributes=1; 		  //���浽SRAM&FLASH
	cfg_prt->CS=cfg_prt->id^cfg_prt->com_port^cfg_prt->Baud_id^cfg_prt->Attributes;
	cfg_prt->end=0X0A0D;        //���ͽ�����(С��ģʽ)
	SkyTra_Send_Date((u8*)cfg_prt,sizeof(SkyTra_baudrate));//�������ݸ�SkyTra   
	delay_ms(200);				//�ȴ�������� 
	Usart2_Init(BAUD_id[baud_id]);	//���³�ʼ������2
	return SkyTra_Cfg_Ack_Check();//���ﲻ�ᷴ��0,��ΪUBLOX��������Ӧ���ڴ������³�ʼ����ʱ���Ѿ���������.
} 

//����SkyTra_GPSģ���ʱ��������
//width:������1~100000(us)
//����ֵ:0,���ͳɹ�;����,����ʧ��.
u8 SkyTra_Cfg_Tp(u32 width)
{
	u32 temp=width;
	SkyTra_pps_width *cfg_tp=(SkyTra_pps_width *)usart2_tx_buf;
	temp=(width>>24)|((width>>8)&0X0000FF00)|((width<<8)&0X00FF0000)|((width<<24)&0XFF000000);//С��ģʽ
	cfg_tp->sos=0XA1A0;		    //cfg header(С��ģʽ)
	cfg_tp->PL=0X0700;        //��Ч���ݳ���(С��ģʽ)
	cfg_tp->id=0X65	;			    //cfg tp id
	cfg_tp->Sub_ID=0X01;			//����������Ϊ20���ֽ�.
	cfg_tp->width=temp;		  //������,us
	cfg_tp->Attributes=0X01;  //���浽SRAM&FLASH	
	cfg_tp->CS=cfg_tp->id^cfg_tp->Sub_ID^(cfg_tp->width>>24)^(cfg_tp->width>>16)&0XFF^(cfg_tp->width>>8)&0XFF^cfg_tp->width&0XFF^cfg_tp->Attributes;    	//�û���ʱΪ0ns
	cfg_tp->end=0X0A0D;       //���ͽ�����(С��ģʽ)
	SkyTra_Send_Date((u8*)cfg_tp,sizeof(SkyTra_pps_width));//�������ݸ�NEO-6M  
	return SkyTra_Cfg_Ack_Check();
}

//����SkyTraF8-BD�ĸ�������	 	    
//Frep:��ȡֵ��Χ:1,2,4,5,8,10,20������ʱ��������λΪHz������ܴ���20Hz
//����ֵ:0,���ͳɹ�;����,����ʧ��.
u8 SkyTra_Cfg_Rate(u8 Frep)
{
	SkyTra_PosRate *cfg_rate=(SkyTra_PosRate *)usart2_tx_buf;
 	cfg_rate->sos=0XA1A0;	    //cfg header(С��ģʽ)
	cfg_rate->PL=0X0300;			//��Ч���ݳ���(С��ģʽ)
	cfg_rate->id=0X0E;	      //cfg rate id
	cfg_rate->rate=Frep;	 	  //��������
	cfg_rate->Attributes=0X01;	   	//���浽SRAM&FLASH	.
	cfg_rate->CS=cfg_rate->id^cfg_rate->rate^cfg_rate->Attributes;//������,us
	cfg_rate->end=0X0A0D;       //���ͽ�����(С��ģʽ)
	SkyTra_Send_Date((u8*)cfg_rate,sizeof(SkyTra_PosRate));//�������ݸ�NEO-6M 
	return SkyTra_Cfg_Ack_Check();
}

//����һ�����ݸ�SkyTraF8-BD������ͨ������2����
//dbuf�����ݻ����׵�ַ
//len��Ҫ���͵��ֽ���
void SkyTra_Send_Date(u8* dbuf,u16 len)
{
	u16 j;
	for(j=0;j<len;j++)//ѭ����������
	{
		while((USART2->SR&0X40)==0);//ѭ������,ֱ���������   
		USART2->DR=dbuf[j];  
	}	
}

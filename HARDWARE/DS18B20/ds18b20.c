#include "ds18b20.h"

u8  U8comdata,U8temp,U8FLAG;

//初始化DS18B20的IO口 DQ 同时检测DS的存在
//返回1:不存在
//返回0:存在    	 
u8 DS18B20_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PORTA口时钟 
	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				//PORTA0 推挽输出
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
 	GPIO_SetBits(GPIOA, GPIO_Pin_0);    //输出1

	DS18B20_Rst();

	return DS18B20_Check();
}

//从ds18b20得到温度值
//精度：0.1C
//返回值：温度值 （-550~1250） 
u8 str[5];
float temp,humidity;
void DS18B20_Get_Temp_Hum(void)
{
	u16 chec;
   	AOSONG_Start();
	str[0]=COM();
	str[1]=COM();
	str[2]=COM();
	str[3]=COM();
	str[4]=COM();
	
	chec=(str[0]+str[1]+str[2]+str[3]);
	if(str[4]==(chec&0x00ff))
	{
		temp=(float)((str[2]&0x7f)<<8|str[3])/10;
		humidity=(float)(str[0]<<8|str[1])/10;
		
		envData.temperature = temp;
		envData.humidity = humidity;
		
		DMA1_Data_Transf(4);
	}
	
//	printf("[0]:%.2x     [1]:%.2x     [2]:%.2x     [3]:%.2x     [4]:%.2x      chec: %.2x    \r\n",str[2],str[3],str[2]&0x7f,(str[2]&0x7f)<<8,(str[2]&0x7f)<<8|str[3],chec&0x00ff);
//	printf("tem: %.2f    humidity: %.2f%% \r\n",temp,humidity);
//	
} 

//复位DS18B20
void DS18B20_Rst(void)	   
{                 
   	DS18B20_IO_OUT(); //SET PA0 OUTPUT
    DS18B20_DQ_OUT=0; //拉低DQ
    delay_us(850);    //拉低750us
    DS18B20_DQ_OUT=1; //DQ=1 
  	delay_us(15);     //15US
}

void AOSONG_Start(void)
{
		DS18B20_IO_OUT();
		OFF;
		delay_us(850);
		ON;
		DS18B20_IO_IN();
		delay_us(40);
		
	 //判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行	  
	   if(!DS18B20_DQ_IN)		 //T !	  
	   {
	   U8FLAG=2;
	 //判断从机是否发出 80us 的低电平响应信号是否结束	 
	   while((!DS18B20_DQ_IN)&&U8FLAG++);
	   U8FLAG=2;
	 //判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
	   while((DS18B20_DQ_IN)&&U8FLAG++);
		 }
}

//等待DS18B20的回应
//返回1:未检测到DS18B20的存在
//返回0:存在
u8 DS18B20_Check(void) 	   
{   
	u8 retry=0;
	DS18B20_IO_IN();//SET PA0 INPUT	 
    while (DS18B20_DQ_IN&&retry<90)
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=79)return 1;
	else retry=0;
    while (!DS18B20_DQ_IN&&retry<90)
	{
		retry++;
		delay_us(1);
	};
	if(retry>=240)return 1;	    
	return 0;
}

//从DS18B20读取一个位
//返回值：1/0
u8 DS18B20_Read_Bit(void) 			 // read one bit
{
    u8 data;
	DS18B20_IO_OUT();//SET PA0 OUTPUT
    DS18B20_DQ_OUT=0; 
	delay_us(2);
    DS18B20_DQ_OUT=1; 
	DS18B20_IO_IN();//SET PA0 INPUT
	delay_us(12);
	if(DS18B20_DQ_IN)data=1;
    else data=0;	 
    delay_us(50);           
    return data;
}

//从DS18B20读取一个字节
//返回值：读到的数据
u8 COM(void)
{
	u8 i;
          
	for(i=0;i<8;i++)	   
	  {
	 	   U8FLAG=2;	
	   	while((!DS18B20_DQ_IN)&&U8FLAG++);
			delay_us(10);
      delay_us(10);
      delay_us(10);
	  		U8temp=0;
	     if(DS18B20_DQ_IN)U8temp=1;
		    U8FLAG=2;
		 while((DS18B20_DQ_IN)&&U8FLAG++);
	   	//超时则跳出for循环		  
	   	 if(U8FLAG==1)break;
	   	//判断数据位是0还是1	 
	   	   
		// 如果高电平高过预定0高电平值则数据位为 1 
	   	 
		   U8comdata<<=1;
	   	   U8comdata|=U8temp;        //0
	     }//rof
	   return U8comdata;
	}

u8 DS18B20_Read_Byte(void)    // read one byte
{        
    u8 i,j,dat;
    dat=0;
	for (i=1;i<=8;i++) 
	{
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
    return dat;
}

//写一个字节到DS18B20
//dat：要写入的字节
void DS18B20_Write_Byte(u8 dat)     
 {             
    u8 j;
    u8 testb;
		DS18B20_IO_OUT();//SET PA0 OUTPUT;
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if (testb) 
        {
            DS18B20_DQ_OUT=0;// Write 1
            delay_us(2);                            
            DS18B20_DQ_OUT=1;
            delay_us(60);             
        }
        else 
        {
            DS18B20_DQ_OUT=0;// Write 0
            delay_us(60);             
            DS18B20_DQ_OUT=1;
            delay_us(2);                          
        }
    }
}
 
//开始温度转换
void DS18B20_Start(void)// ds1820 start convert
{   						               
    DS18B20_Rst();	   
	DS18B20_Check();	 
//    DS18B20_Write_Byte(0xcc);// skip rom
//    DS18B20_Write_Byte(0x44);// convert
}  

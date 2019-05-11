#include "include.h"

void IIC_Init(void)
{
  GPIO_Init(GPIOC, 15,GPO,0);//配置为GPIO功能
  GPIO_Init(GPIOC, 14,GPO,0);//配置为GPIO功能  
  SCL_OUT;
}


void IIC_Wait(void)
{
  uint8 i=0;
  for(i=0;i<100;i++)
  { 
    asm ("nop");
  }  
}

/*************************************************************************
* 北京龙邱智能科技 KV58智能车母板           
*
*  函数名称：void IIC_Star(void)
*  功能说明：启动函数
*  参数说明：无
*  函数返回：无
*  修改时间：2017年12月5日
*  备    注：
*************************************************************************/
void IIC_Star(void)
{
  SCL_OUT;
  SDA_OUT;
  IIC_Wait();
  SDA_HIGH;
  SCL_HIGH; 
  IIC_Wait();
  SDA_LOW;
  IIC_Wait();
  SCL_LOW; 
  IIC_Wait();
}
/*************************************************************************
* 北京龙邱智能科技 KV58智能车母板           
*
*  函数名称：void IIC_Stop(void)
*  功能说明：停止函数
*  参数说明：无
*  函数返回：无
*  修改时间：2017年12月5日
*  备    注：
*************************************************************************/
void IIC_Stop(void)
{
  SCL_OUT;
  SDA_OUT;
  IIC_Wait();
  SDA_LOW;
  IIC_Wait();
  SCL_HIGH; 
  IIC_Wait();
  SDA_HIGH;
  IIC_Wait();  
}
/*************************************************************************
* 北京龙邱智能科技 KV58智能车母板           
*
*  函数名称：uint8 IIC_SendByte(uint8 Data)
*  功能说明：IIC发送字节函数
*  参数说明：要发送的字节
*  函数返回：应答信号
*  修改时间：2017年12月5日
*  备    注：
*************************************************************************/
uint8 IIC_SendByte(uint8 Data)
{
  uint8 i;
  uint8 Ack;
  SDA_OUT;
  for( i=0; i<8; i++)
  {
    if(Data & 0x80) SDA_HIGH;
    else            SDA_LOW;    
    Data <<= 1;
    IIC_Wait();
    SCL_HIGH;      
    IIC_Wait();
    SCL_LOW;
    IIC_Wait();
  }
  SDA_HIGH;
  SDA_IN;
  IIC_Wait();
  
  SCL_HIGH;
  IIC_Wait();
  Ack = SDA_DATA;
  SCL_LOW;
  IIC_Wait();
  return Ack;
}
/*************************************************************** 

* 
* 函数名称：uint8 IIC_ReadByte(void) 
* 功能说明：IIC读取字节函数 
* 参数说明： 
* 函数返回：读取字节 
* 修改时间：2017年12月5日 
* 备 注： 
***************************************************************/ 
uint8 IIC_ReadByte(void) 
{ 
  uint8 i; 
  uint8_t byte; 
  SCL_OUT; 
  SDA_IN; //使能输入
  for( i=0; i<8; i++) 
  { 
    SCL_LOW;
    IIC_Wait(); 
    SCL_HIGH;
    IIC_Wait();
    byte = (byte<<1)|(SDA_DATA & 1);
  }
  SCL_LOW; 
  SDA_OUT;
  IIC_Wait(); 
  return byte; 
} 
/*************************************************************** 

* 
* 函数名称：static void IIC_Ack(void) 
* 功能说明：IIC有回复信号 
* 参数说明： 
* 函数返回：void 
* 修改时间：2017年12月5日 
* 备 注： 
***************************************************************/ 
static void IIC_Ack(void) 
{ 
  SCL_OUT; 
  SDA_OUT;
  SCL_LOW;
  SDA_LOW;
  IIC_Wait();
  SCL_HIGH;
  IIC_Wait();
  SCL_LOW;
  IIC_Wait();
} 
/*************************************************************** 

* 
* 函数名称：static void IIC_NAck(void) 
* 功能说明：IIC无回复信号 
* 参数说明： 
* 函数返回：void 
* 修改时间：2017年12月5日 
* 备 注： 
***************************************************************/ 
static void IIC_NAck(void) 
{ 
  SCL_OUT; 
  SDA_OUT;
  SCL_LOW;
  IIC_Wait();
  SDA_HIGH;
  IIC_Wait();
  SCL_HIGH;
  IIC_Wait();
  SCL_LOW;
  IIC_Wait();
} 

/*************************************************************************
* 北京龙邱智能科技 KV58智能车母板           
*
*  函数名称：void IIC_RegWrite(uint8 Device,uint8 Address,uint16 Data)
*  功能说明：向设备写数据
*  参数说明：要发送的字节
*  函数返回：应答信号
*  修改时间：2017年12月5日
*  备    注：
*************************************************************************/
void IIC_RegWrite(uint8 Device,uint8 Address,uint8 Data)
{
  uint8 i;
  uint8 Ack;
  
  for( i=0; i<3; i++)
  {
    IIC_Star();
    Ack = IIC_SendByte(Device);
    if( Ack == 1 )
    {
      continue;
    }
    
    Ack = IIC_SendByte(Address);
    if( Ack == 1 )
    {
      continue;
    }
    
    Ack = IIC_SendByte((uint8)Data);
    if( Ack == 1 )
    {
      continue;
    }
    
    IIC_Stop();
    if( Ack == 0 ) break;
  }
}
/*************************************************************** 

* 
* 函数名称：uint8_t SCB_RegRead(uint8_t Device,uint8_t Address,uint16_t *Data) 
* 功能说明：读取数据 
* 参数说明： 
* 函数返回：void 
* 修改时间：2017年12月5日 
* 备 注： 
***************************************************************/ 
uint8_t IIC_RegRead(uint8_t Device,uint8_t Address) 
{ 
  uint8 i; 
  uint8 Ack = 0;
  uint8_t Data;
  Device = Device<<1;
  IIC_Star();
  Ack += IIC_SendByte(Device);
  
  Ack += IIC_SendByte(Address);
  
  IIC_Star();
  Ack += IIC_SendByte(Device + 1);
  
  Data = IIC_ReadByte();
  IIC_NAck();
  
  IIC_Stop();
  
  return  Data;
} 
/***************************************************************  
* 
* 函数名称：int IIC_Probe(uint8_t chipAddr) 
* 功能说明：查询该地址的设备是否存在 
* 参数说明： 
* 函数返回：void 
* 修改时间：2017年12月5日 
* 备 注： 
***************************************************************/ 
int IIC_Probe(uint8_t chipAddr) 
{ 
  uint8_t err;
  err = 0;
  chipAddr <<= 1;
  
  IIC_Star();
  err = IIC_SendByte(chipAddr);
  IIC_Stop();
  
  return err;
}

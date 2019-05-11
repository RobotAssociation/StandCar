#include "include.h"

void IIC_Init(void)
{
  GPIO_Init(GPIOC, 15,GPO,0);//����ΪGPIO����
  GPIO_Init(GPIOC, 14,GPO,0);//����ΪGPIO����  
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
* �����������ܿƼ� KV58���ܳ�ĸ��           
*
*  �������ƣ�void IIC_Star(void)
*  ����˵������������
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2017��12��5��
*  ��    ע��
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
* �����������ܿƼ� KV58���ܳ�ĸ��           
*
*  �������ƣ�void IIC_Stop(void)
*  ����˵����ֹͣ����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2017��12��5��
*  ��    ע��
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
* �����������ܿƼ� KV58���ܳ�ĸ��           
*
*  �������ƣ�uint8 IIC_SendByte(uint8 Data)
*  ����˵����IIC�����ֽں���
*  ����˵����Ҫ���͵��ֽ�
*  �������أ�Ӧ���ź�
*  �޸�ʱ�䣺2017��12��5��
*  ��    ע��
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
* �������ƣ�uint8 IIC_ReadByte(void) 
* ����˵����IIC��ȡ�ֽں��� 
* ����˵���� 
* �������أ���ȡ�ֽ� 
* �޸�ʱ�䣺2017��12��5�� 
* �� ע�� 
***************************************************************/ 
uint8 IIC_ReadByte(void) 
{ 
  uint8 i; 
  uint8_t byte; 
  SCL_OUT; 
  SDA_IN; //ʹ������
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
* �������ƣ�static void IIC_Ack(void) 
* ����˵����IIC�лظ��ź� 
* ����˵���� 
* �������أ�void 
* �޸�ʱ�䣺2017��12��5�� 
* �� ע�� 
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
* �������ƣ�static void IIC_NAck(void) 
* ����˵����IIC�޻ظ��ź� 
* ����˵���� 
* �������أ�void 
* �޸�ʱ�䣺2017��12��5�� 
* �� ע�� 
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
* �����������ܿƼ� KV58���ܳ�ĸ��           
*
*  �������ƣ�void IIC_RegWrite(uint8 Device,uint8 Address,uint16 Data)
*  ����˵�������豸д����
*  ����˵����Ҫ���͵��ֽ�
*  �������أ�Ӧ���ź�
*  �޸�ʱ�䣺2017��12��5��
*  ��    ע��
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
* �������ƣ�uint8_t SCB_RegRead(uint8_t Device,uint8_t Address,uint16_t *Data) 
* ����˵������ȡ���� 
* ����˵���� 
* �������أ�void 
* �޸�ʱ�䣺2017��12��5�� 
* �� ע�� 
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
* �������ƣ�int IIC_Probe(uint8_t chipAddr) 
* ����˵������ѯ�õ�ַ���豸�Ƿ���� 
* ����˵���� 
* �������أ�void 
* �޸�ʱ�䣺2017��12��5�� 
* �� ע�� 
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

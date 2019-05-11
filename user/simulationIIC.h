#ifndef _SIMULATIONIIC_H
#define _SIMULATIONIIC_H

#include "include.h"

//IIC操作函数
void IIC_Init(void);
void IIC_Wait(void);
void IIC_Stop(void);
void IIC_Star(void);
uint8 IIC_SendByte(uint8 Data);
void IIC_RegWrite(uint8 Device,uint8 Address,uint8 Data);
uint8_t IIC_RegRead(uint8_t Device,uint8_t Address) ;
int IIC_Probe(uint8_t chipAddr);
#define SCL_OUT     DDRC15=1      //配置输出作为SCL_OUT
#define SDA_OUT     DDRC14=1      //配置作为输出作为SDA_OUT
#define SDA_IN      DDRC14=0      //配置作为输入作为SDA_IN

#define SCL_HIGH    PTC15_OUT=1   //配置输出高电平
#define SCL_LOW     PTC15_OUT=0   //配置输出低电平
#define SDA_HIGH    PTC14_OUT=1   //配置输出高电平
#define SDA_LOW     PTC14_OUT=0   //配置输出低电平
#define SDA_DATA    PTC14_IN      //读取引脚上的引脚状态

#endif
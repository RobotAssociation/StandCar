/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】龙邱KV58F24智能车VD母板
【编    写】管脚复用部分由CHIUSIR
【E-mail  】chiusir@163.com
【软件版本】V1.0
【最后更新】2017年12月15日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
------------------------------------------------
【dev.env.】IAR7.80.4
【Target  】MKV58F1M0VLQ24
【Crystal 】 50.000Mhz
【busclock】137.500MHz
【pllclock】275.000MHz
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#ifndef _LQI2C_H_
#define _LQI2C_H_

/**********************************  IIC(引脚复用) ***************************************/

#define I2C0_SCL    PTC14       // PTB0、PTB2、PTE19 PTE24 PTA12 PTC6 PTC14 PTD2
#define I2C0_SDA    PTC15       // PTB1、PTB3、PTE18 PTE25 PTA11 PTC7 PTC15 PTD3

#define I2C1_SCL    PTC14       // PTD8 PTE1、PTC10 PTA14 PTC14 
#define I2C1_SDA    PTC15       // PTD9 PTE0、PTC11 PTA13 PTC15

/**********************************  IIC(引脚复用) ***************************************/

//定义模块号
typedef enum I2Cn
{
    I2C_0  = 0,
    I2C_1  = 1
} I2Cn;

//定义读写选项
typedef enum MSmode
{
    write =   0x00,  /* Master write  */
    read =   0x01   /* Master read */
} MSmode;


#define I2C_DisableAck(I2Cn)        I2C_C1_REG(I2Cx[I2Cn]) |= I2C_C1_TXAK_MASK

//
#define I2C_RepeatedStart(I2Cn)     I2C_C1_REG(I2Cx[I2Cn]) |= I2C_C1_RSTA_MASK

//启动信号
#define I2C_Start(I2Cn)             I2C_C1_REG(I2Cx[I2Cn]) |= I2C_C1_TX_MASK+I2C_C1_MST_MASK;\
                                    //I2C_C1_REG(I2Cx[I2Cn]) |= I2C_C1_MST_MASK

//暂停信号
#define I2C_Stop(I2Cn)              I2C_C1_REG(I2Cx[I2Cn]) &= ~(I2C_C1_MST_MASK+I2C_C1_TX_MASK);\
                                    //I2C_C1_REG(I2Cx[I2Cn]) &= ~I2C_C1_TX_MASK

//进入接收模式(应答)
#define I2C_EnterRxMode(I2Cn)       I2C_C1_REG(I2Cx[I2Cn]) &= ~I2C_C1_TX_MASK;\
                                    I2C_C1_REG(I2Cx[I2Cn]) &= ~I2C_C1_TXAK_MASK
//进入接收模式(不应答)
#define I2C_PutinRxMode(I2Cn)       I2C_C1_REG(I2Cx[I2Cn]) &= ~I2C_C1_TX_MASK

//等待 I2C0_S
#define I2C_Wait(I2Cn)              while(( I2C_S_REG(I2Cx[I2Cn]) & I2C_S_IICIF_MASK)==0) {} \
                                    I2C_S_REG(I2Cx[I2Cn]) |= I2C_S_IICIF_MASK;

//写一个字节
#define I2C_write_byte(I2Cn,data)   I2C_D_REG(I2Cx[I2Cn]) = data


void I2C_Init(I2Cn A);                                        //初始化I2C
void I2C_WriteAddr(I2Cn, u8 SlaveID, u8 Addr, u8 Data);       //往地址里写入内容
void I2C_WriteAddr16(I2Cn i2cn, u8 SlaveID, u8 Addr, u16 Data);
void I2C_StartTransmission (I2Cn, u8 SlaveID, MSmode);        //启动传输
u8    I2C_ReadAddr(I2Cn, u8 SlaveID, u8 Addr);                 //读取地址里的内容


#endif
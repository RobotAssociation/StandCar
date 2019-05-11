/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】龙邱KV58F24智能车VD母板
【编    写】CHIUSIR
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
【busclock】120.000MHz
【pllclock】240.000MHz
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef __LQ_LED_H__
#define __LQ_LED_H__

#include "include.h"

#define BEE_ON PTA27_OUT=0
#define BEE_OFF PTA27_OUT=1

/*******************************************************************************
* Definitions
******************************************************************************/
//定义模块号
typedef enum
{
  LED0=0,
  LED1=1,
  LED2=2,   
  LED3=3,   
  LED4=4,
  LED5=5,
  LEDALL=6,//全部四个   
} LEDn_e;

typedef enum
{
  ON=0,  //亮
  OFF=1, //灭
  RVS=2, //反转  
}LEDs_e;



extern void LED_Init(void);
extern void LED_Ctrl(LEDn_e ledno, LEDs_e sta);
extern void Test_LED(void);

#endif 

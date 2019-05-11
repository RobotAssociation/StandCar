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
#ifndef __LQ_KEY_H__
#define __LQ_KEY_H__

#include "include.h"

/*******************************************************************************
* Definitions
******************************************************************************/
//定义模块号
typedef enum
{
    down=0,
    left=1,
    middle=2, 
    right=3, 
    up=4, 
} KEYn_e;
typedef enum
{
    LOW=0,  //DOWN
    HIGH=1, //UP  
    FAIL=0xff,
}KEYs_e;

extern void TestLED(void);
extern void KEY_Init(void);
extern u8 KEY_Read(KEYn_e keyno);
extern void  Test_GPIO_KEY(void);
extern void  Test_GPIO_EXINT(void);
#endif 

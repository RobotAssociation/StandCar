/*******************************************************************************
【平    台】龙邱KV58F24智能车VD母板
【编    写】CHIUSIR
【E-mail  】chiusir@163.com
【软件版本】V1.0
【最后更新】2018年3月28日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
------------------------------------------------
【dev.env.】IAR7.80.4及以上版本
【Target  】MKV58F1M0VLQ24
【Crystal 】 50.000Mhz
【busclock】137.500MHz
【pllclock】275.000MHz
------------------------------------------------
没有完美的代码，只有不断的奉献，大家一起努力；
赠人玫瑰手留余香，欢迎大家反馈bug，将开源进行到底；
=============================================================
接口定义：
摄像头          单片机接口    
VCC             3.3V 
GND             GND
Y0-7            PTD0-7共8位并口      
HREF            PTD13
VSY             PTD14
PCK             PTD12
-------------------------------------------------------------
TSL1401模块     单片机接口
VCC             5V
GND             GND
SI              PTE27
SCK             PTE28
ADC             ADC0_SE8
ADC             ADC0_SE4B

SI              PTE2
SCK             PTE3
ADC             ADC0DP0
ADC             ADC0_SE5B
===============================================================
多路电感模块    单片机接口
VCC             3.3V
GND             GND
ADC通道         管脚关系     
ADC0_SE10       PTE5
ADC0_DP2        PTE4
ADC0_SE4A       PTE6
ADC0_DP3        PTE11
ADC0_SE11       PTE12
ADC0_DP1        PTE16
ADC0_SE9        PTE17
ADC0_SE5A       PTE18
-------------------------------------------------------------
电源监控或者它用
ADC0_SE7A       HSADC0CH6  
-------------------------------------------------------------
九轴模块        单片机接口   FLEXCOMM8
VCC             3V3
GND             GND
SDA1            PTD8
SCL1            PTD9
//////////////////////////////////////////////////////////通用部分功能//////////
电机驱动        单片机接口   
VCC             3.3V
FTM0_CH0  PTC1，电机驱动接口
FTM0_CH1  PTC2，电机驱动接口
FTM0_CH2  PTC3，电机驱动接口
FTM0_CH3  PTC4，电机驱动接口
-------------------------------------------------------------
舵机接口        单片机接口
VADJ            可调电源
GND             GND
FTM3_CH6        PTC10，舵机接口
FTM3_CH7        PTC11，舵机接口
-------------------------------------------------------------
龙邱512编码器   单片机接口     
VCC             3V3
GND             GND      
LSB/A           PTA12   
DIR/B           PTA13
LSB/A           PTB18
DIR/B           PTB19
-------------------------------------------------------------
OLED模块        单片机接口  核心板用的是PTC16-19
VCC             3.3V        用户自行修改初始化和管脚定义
GND             GND
RST             PTC13
DC              PTC14
SDA             PTC15
CLK             PTC16
CS              PTC12
-------------------------------------------------------------
蓝牙/USBTTL     单片机接口   
GND             GND
TX              PTE24   UART4_TX 接蓝牙的RX
RX              PTE25   UART4_RX 接蓝牙的TX
-------------------------------------------------------------
LED接口定义：
LED1            PTA17 核心板
LED2            PTC0  核心板
LED3            PTD15 核心板   
LED4            PTE29 核心板
LEDD1           PTC18 母板
LEDD2           PTC19 母板
-------------------------------------------------------------
KEY接口定义：
KEY0            PTB20  
KEY1            PTB21
KEY2            PTB22   //可中断触发
-------------------------------------------------------------
修改历史：
20180208-中断处理函数在startup_MKV58F24.s中，名字必须对应才能触发中断；
20180327-已经修改神眼摄像头的采集模式为DMA，分辨率为188*120，100db HDR；
********************************************************************************/
#include "include.h"

//==========================================================================================  
//测试函数名称        测试内部模块及功能           智能车及应用开发               完成状况
//----------------------------------------------------------------------------------------- 
//Test_LED();         //测试GPIO输出口              LED显示及外设控制              已完成
//Test_OLED();       //测试模拟SPI功能              OLED模块功能                   已完成
//Test_GPIO_KEY();   //测试GPIO输入                 按键检测功能                   已完成 
//Test_GPIO_EXINT(); //测试GPIO输入及外部中断       按键、中断检测功能             已完成     
//Test_UART();       //测试UART及中断               蓝牙、USB转TTL调试及通信       已完成
//Test_ADC0();       //测试ADC采集功能              电磁传感器及电源电压监控       部分完成,HSADC不能通过
//Test_Servo();      //测试用FTM-PWM功能            数字舵机驱动控制               已完成
//Test_Motor();      //测试用FTM-PWM功能            电机驱动控制                   已完成
//Test_AB_Pulse_Cnt();//测试用FTM正交解码功能       编码器测速,电机舵机控制        已完成 
//Test_LPTMR_delay();//测试LPTMR功能                延时功能                       已完成
//Test_LPTMR_Counter();//测试LPTMR功能              计数功能                       已完成
//Test_DMA_Counter();//测试DMA计数功能              计数功能                       已完成
//TestOLED_TSL1401();//测试OLED和TSL1401功能        OLED显示屏及线阵摄像头         已完成
//TestOLED_MT9V034();//测试OLED和MT9V034功能        OLED显示屏及面阵摄像头动图     已完成
//Test_WDG();        //测试看门狗功能               程序监控                       已完成
//========================================================================================== 

void main(void)
{   
  PLL_Init(PLL235);             //设置内核及总线频率等
  KEY_Init();                   //按键及输入口初始化
  LED_Init();                   //LED初始化
  LCD_Init();                   //LCD初始化
//  TFTSPI_Init();                //TFT1.8吋SPI彩屏初始化
  UART_Init(UART_4,115200);     //串口初始化
  MT9V034_Init();               //摄像头初始化
  Servo_Init();                 //舵机初始化
  Motor_Init();                 //电机初始化
  PIDInit();
//  Init_LQ_9AX();
  ButterworthParameterInit();   //巴特沃斯低通滤波初始化
  FTM_AB_Init(FTM1);            //编码器初始化
  FTM_AB_Init(FTM2);            //编码器初始化
  MenuInit();                   //菜单初始化
  ADC0_Init();                  //ADC初始化
  PIT_Init(PIT0, 10);           //定时器0初始化
  PIT_Init(PIT1, 30);           //定时器1初始化
//  PIT_Init(PIT2, 5);            //定时器2初始化
  PIT_Init(PIT3, 70);            //定时器2初始化
//  GyroInit();






  LCD_CLS();                    //清屏
//  LCD_Show_LQLogo();          //显示龙邱LOGO
  LCD_P14x16Str(0,0,"16");      //字符串显示
  

 
//  time_delay_ms(500);           //延时
  
  
//#ifdef __USE_TFT18
//  TFTSPI_CLS(u16BLACK);       //蓝色屏幕
//#else    
//  LCD_CLS();                 //清屏
//#endif  
  Servo_Duty(servoMedian);
  time_delay_ms(5);        //延时
  EnableInterrupts          //中断使能
  LED_Ctrl(LEDALL, OFF);
//  -----------------------------------------------------------------------------------------
//    测试函数都是死循环，每次只能开启一个
//  -----------------------------------------------------------------------------------------
//  Test_ADC0();         //测试ADC采集功能               电磁传感器及电源电压监控
//  Test_UART();         //测试UART及中断                蓝牙、USB转TTL调试及通信
//  Test_LED();          //测试GPIO输出口                LED显示及外设控制
//  Test_OLED();         //O测试模拟SPI功能              OLED模块功能
//  Test_GPIO_KEY();     //测试GPIO输入                  按键检测功能
//  Test_GPIO_EXINT();   //测试GPIO输入及外部中断        按键、中断检测功能
//  Test_Servo();        //数字舵机测试
//  Test_Motor();        //直流电机驱动测试，            用龙邱全桥驱动板
//  Test_9AX();          //测试I2C及龙邱九轴
//  Test_MPU6050();      //测试I2C及6轴陀螺仪功能
//  Test_LQV034();       //OLED显示屏及面阵摄像头动图
//  Test_PIT();          //测试PIT定时中断功能
//  Test_AB_Pulse_Cnt(); //测试编码器正交解码功能
//  Test_LPTMR_delay();  //测试LPTMR延时功能
//  Test_LPTMR_Counter();//测试LPTMR计数功能
//  Test_DMA_Counter();  //测试DMA计数功能
//  TFTSPI_Test();       //测试龙邱TFT1.8吋SPI彩屏
//  Test_OLED();
//  -----------------------------------------------------------------------------------------
//  LCD_Show_LQLogo();

  while(1){
//    BEE_OFF;

//    VirtualOscilloscope(VirtualOscilloscopeData);
//    GyroAngleProcessing();
//    Servo_Duty(servoMedian);
//    Menu();

//    ReadGyro();

    LED_Ctrl(LED2, RVS);


    
  }
} 

/*********************************************************************************
* EOF
******************************************************************************/

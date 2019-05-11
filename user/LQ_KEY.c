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
【busclock】137.500MHz
【pllclock】275.000MHz
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "include.h"

void KEY_Init(void)
{
  GPIO_Init(GPIOC,7,GPI,1);
  GPIO_Init(GPIOC,8,GPI,1);
  GPIO_Init(GPIOC,9,GPI,1);  
  GPIO_Init(GPIOC,12,GPI,1);
  GPIO_Init(GPIOC,13,GPI,1);
}

u8 KEY_Read(KEYn_e keyno)
{
  switch(keyno) 
  {
  case down:
    return GPIO_Get(PTC7);
    break;
    
  case left:
    return GPIO_Get(PTC8);
    break;
    
  case middle:
    return GPIO_Get(PTC9);
    break;
    
  case right:
    return GPIO_Get(PTC12);
    break;
  
  case up:
    return GPIO_Get(PTC13);
    break;
    
  default:
    return 0XFF;    
  }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】CHIUSIR
【功能说明】测试按键及OLED显示
【软件版本】V1.0
【最后更新】2017年11月24日 
【函数名】
【返回值】无
【参数值】无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_GPIO_KEY(void)
{ 
  while (1)
  {  
    //测试按键      
    if(KEY_Read(up)==0)
      LCD_P6x8Str(13,3,(uint8_t*)"KEY0 Pressed!   ");
    else LCD_P6x8Str(13,3,(uint8_t*)"KEY0 NO Pressed!");
    if(KEY_Read(down)==0)
      LCD_P6x8Str(13,5,(uint8_t*)"KEY1 Pressed!   ");
    else LCD_P6x8Str(13,5,(uint8_t*)"KEY1 NO Pressed!");
    if(KEY_Read(middle)==0)
      LCD_P6x8Str(13,7,(uint8_t*)"KEY2 Pressed!   ");
    else LCD_P6x8Str(13,7,(uint8_t*)"KEY2 NO Pressed!");
    
    //LED闪烁
    LED_Ctrl(LED1, RVS);     
    
    //延时
    time_delay_ms(50);
  }
}

void PORTB_IRQHandler(void)
{
  int n;
  n=20;
  if((PORTB_ISFR & (1<<n)))
  {
      PORTB_ISFR |= (1<<n);
      /* 用户自行添加中断内程序 */
      LED_Ctrl(LED0, RVS); 
  }
  n=21;
  if((PORTB_ISFR & (1<<n)))
  {
      PORTB_ISFR |= (1<<n);
      /* 用户自行添加中断内程序 */
      LED_Ctrl(LED1, RVS); 
  }
  n=22;
  if((PORTB_ISFR & (1<<n)))
  {
      PORTB_ISFR |= (1<<n);
      /* 用户自行添加中断内程序 */
      LED_Ctrl(LED2, RVS); 
  }
}

void Test_GPIO_EXINT(void)
{  
  EXTI_Init(GPIOB,22,rising_down);   //K2
  EXTI_Init(GPIOB,21,falling_up);    //K1中断  
  EXTI_Init(GPIOB,20,falling_up);    //K0中断 
  for(;;)
  {  
    //systick中断延时
    time_delay_ms(5);
  }
}
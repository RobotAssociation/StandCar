/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨������KV58F24���ܳ�VDĸ��
����    д��CHIUSIR
��E-mail  ��chiusir@163.com
������汾��V1.0
�������¡�2017��12��15��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
------------------------------------------------
��dev.env.��IAR7.80.4
��Target  ��MKV58F1M0VLQ24
��Crystal �� 50.000Mhz
��busclock��137.500MHz
��pllclock��275.000MHz
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "include.h"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����  �ߡ�CHIUSIR
������˵������ʼ��LED����IO��
������汾��V1.0
�������¡�2017��11��24�� 
����������
������ֵ����
������ֵ����
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LED_Init(void)
{  
  //-----�˿ڳ�ʼ��----//
  GPIO_Init(GPIOB,8,GPO,1);
  GPIO_Init(GPIOB,9,GPO,1);
  GPIO_Init(GPIOB,10,GPO,1);
  GPIO_Init(GPIOB,11,GPO,1);
  //GPIO_Init(GPIOA,27,GPO,1);
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����  �ߡ�CHIUSIR
������˵��������IO����ߵ͵�ƽ
������汾��V1.0
�������¡�2017��11��24�� 
����������
������ֵ����
������ֵ��
LEDn_e ledno, ���
LEDs_e sta ״̬������
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LED_Ctrl(LEDn_e ledno, LEDs_e sta)
{
  switch(ledno) 
  {
  case LED0:
    if(sta==ON)       PTB11_OUT=0; //Turn on target LED1
    else if(sta==OFF) PTB11_OUT=1; //Turn off target LED1
    else if(sta==RVS) GPIO_Reverse (GPIOB, 11);//Toggle on target LED1
    break;
  case LED1:
    if(sta==ON)       PTB10_OUT=0; //Turn on target LED3
    else if(sta==OFF) PTB10_OUT=1; //Turn off target LED3
    else if(sta==RVS) GPIO_Reverse (GPIOB, 10);//Toggle on target LED3
    break;   
  case LED3:
    if(sta==ON)        PTB9_OUT=0; //Turn on target LED2
    else if(sta==OFF) PTB9_OUT=1; //Turn off target LED2
    else if(sta==RVS) GPIO_Reverse (GPIOB, 9);//Toggle on target LED2
    break;    
  case LED2:
    if(sta==ON)        PTB8_OUT=0; //Turn on target LED3
    else if(sta==OFF) PTB8_OUT=1; //Turn off target LED3
    else if(sta==RVS) GPIO_Reverse (GPIOB, 8);//Toggle on target LED3
    break; 
  case LEDALL:
    if(sta==ON) 
    {       
      PTB8_OUT=0;
      PTB9_OUT=0; //Turn on target LED1 
      PTB10_OUT=0; //Turn on target LED2 
      PTB11_OUT=0;//Turn on target LED3 
    }
    else if(sta==OFF)
    { 
      PTB8_OUT=1;
      PTB9_OUT=1;   //Turn off target LED1 
      PTB10_OUT=1;   //Turn off target LED2 
      PTB11_OUT=1;  //Turn off target LED3 
    }
    else if(sta==RVS)
    {       
      GPIO_Reverse (GPIOB, 8); //Toggle on target LED1
      GPIO_Reverse (GPIOB, 9); //Toggle on target LED2
      GPIO_Reverse (GPIOB, 10);  //Toggle on target LED3
      GPIO_Reverse (GPIOB, 11);
    }
    break;
  default:
    break;    
  }   
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����  �ߡ�CHIUSIR
������˵��������LED������ˮ��
������汾��V1.0
�������¡�2017��11��24�� 
����������
������ֵ����
������ֵ����
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_LED(void)
{
  u8 i=0;
  //Init output LED GPIO. //
  LED_Init();
  LED_Ctrl(LEDALL, OFF);    
  while (1)
  {          
    for(i=0;i<10;i++)
    {
      LED_Ctrl(LED1, RVS); 
      time_delay_ms(50*i);
      LED_Ctrl(LED1, OFF); 
      LED_Ctrl(LED2, RVS); 
      time_delay_ms(50*i);
      LED_Ctrl(LED2, OFF); 
      LED_Ctrl(LED3, RVS); 
      time_delay_ms(50*i);
      LED_Ctrl(LED3, OFF); 
      LED_Ctrl(LED0, RVS); 
      time_delay_ms(50*i);
      LED_Ctrl(LEDALL, OFF); 
      time_delay_ms(500);
    }
    LED_Ctrl(LEDALL, OFF); 
    time_delay_ms(500);
    LED_Ctrl(LEDALL, ON); 
    time_delay_ms(500);
  }
}


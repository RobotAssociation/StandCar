/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】龙邱KV58F24智能车VD母板
【编    写】CHIUSIR
【E-mail  】chiusir@163.com
【软件版本】V1.0
【最后更新】2017年12月18日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
------------------------------------------------
【dev.env.】IAR7.80.4
【Target  】MKV58F1M0VLQ24
【Crystal 】 50.000Mhz
【busclock】137.500MHz
【pllclock】275.000MHz

本程序尚在调试中，用户可以在次基础上修改--20171222
如果你能调试并共享给大家，相信赠人玫瑰手留余香，大家都会感谢你的！！！

QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "include.h"

u8 LQ9AX_DAT[19];

LQ9AXt ACC_X,ACC_Y,ACC_Z,GYRO_X,GYRO_Y,GYRO_Z,MAG_X,MAG_Y,MAG_Z;

void Test_9AX(void)
{
  u16 tem=0;
  float fv=0.01;
  char  txt[16]="X:";
  Init_LQ_9AX();
  while(1)
  {
    //九轴数据刷新
    Update9AX();
    if(ACC_X.MYBYTE.BYTEH > 0x7F)            //判断加速度X轴正负,此处为负值
    {
      tem= (~(ACC_X.MYWORD>>2) + 1)&0X3FFF;  //把补码数值转换为有效值
    }
    else                                    //正数情况处理
    {
      tem=(ACC_X.MYWORD>>2)&0X3FFF;          //转换为有效值
    }    
    sprintf(txt,"DJ0:%04d",tem);             //数值转换为字符串
    LCD_P8x16Str(10,0,(uint8*)txt);          //OLED屏显示转向数值
    
    //转换为加速度数值
    Cvt_14bit_Str(txt,ACC_X);                //加速度数值专为固定格式字符串，方便屏幕显示
    LCD_P6x8Str(10,4,(uint8*)txt);           //OLED屏显示数值
    
    //温度检测     
    if(LQ9AX_DAT[18]>0x7F)                   //温度位正数0--125°
    {
      LCD_P6x8Str(10,6,(uint8*)"low temp  ");    //OLED屏显示数值
    }
    else
    {
      sprintf(txt,"temp:+%03d ",LQ9AX_DAT[18]);  //转换为字符串
      LCD_P6x8Str(10,6,(uint8*)txt);             //OLED屏显示数值
    }
    time_delay_ms(100);
  }
}

void Init_LQ_9AX(void)
{
  I2C_Init(I2C_1);                       //初始化I2C1 
  
  ///////FXAS21002//////////////////////////////////////////////////////////////////////////////////////////
  // write 0000 0000 = 0x00 to CTRL_REG1 to place FXOS21002 in Standby
  // [7]: ZR_cond=0
  // [6]: RST=0
  // [5]: ST=0 self test disabled
  // [4-2]: DR[2-0]=000 for 800Hz
  // [1-0]: Active=0, Ready=0 for Standby mode
  I2C_WriteAddr(I2C_1, FXAS21002_I2C_ADDR, FXAS21002_CTRL_REG1, 0x00); 
  // write 0000 0000 = 0x00 to CTRL_REG0 to configure range and filters
  // [7-6]: BW[1-0]=00, LPF disabled
  // [5]: SPIW=0 4 wire SPI (irrelevant)
  // [4-3]: SEL[1-0]=00 for 10Hz HPF at 200Hz ODR
  // [2]: HPF_EN=0 disable HPF
  // [1-0]: FS[1-0]=00 for 1600dps (TBD CHANGE TO 2000dps when final trimmed parts available)
  I2C_WriteAddr(I2C_1, FXAS21002_I2C_ADDR, FXAS21002_CTRL_REG0, 0x00);     
  time_delay_ms(100);  
  // write 0000 0001 = 0x01 to CTRL_REG1 to configure 800Hz ODR and enter Active mode
  // [7]: ZR_cond=0
  // [6]: RST=0
  // [5]: ST=0 self test disabled
  // [4-2]: DR[2-0]=000 for 800Hz ODR
  // [1-0]: Active=1, Ready=0 for Active mode
  I2C_WriteAddr(I2C_1, FXAS21002_I2C_ADDR, FXAS21002_CTRL_REG1, 0x03);
  
  //////FXOS8700///////////////////////////////////////////////////////////////////////////////////////////
  time_delay_ms(100); 
  // write 0000 0000 = 0x00 to CTRL_REG1 to place FXOS8700 into standby
  // [7-1] = 0000 000
  // [0]: active=0    
  I2C_WriteAddr(I2C_1, FXOS8700_I2C_ADDR, FXOS8700_CTRL_REG1, 0x00);
  time_delay_ms(10);
  // write 0001 1111 = 0x1F to M_CTRL_REG1
  // [7]: m_acal=0: auto calibration disabled
  // [6]: m_rst=0: one-shot magnetic reset disabled
  // [5]: m_ost=0: one-shot magnetic measurement disabled
  // [4-2]: m_os=111=7: 8x oversampling (for 200Hz) to reduce magnetometer noise
  // [1-0]: m_hms=11=3: select hybrid mode with accel and magnetometer active
  I2C_WriteAddr(I2C_1, FXOS8700_I2C_ADDR, FXOS8700_M_CTRL_REG1, 0x1F);
  time_delay_ms(10);
  // write 0010 0000 = 0x20 to magnetometer control register 2
  // [7]: reserved
  // [6]: reserved
  // [5]: hyb_autoinc_mode=1 to map the magnetometer registers to follow the accelerometer registers
  // [4]: m_maxmin_dis=0 to retain default min/max latching even though not used
  // [3]: m_maxmin_dis_ths=0
  // [2]: m_maxmin_rst=0
  // [1-0]: m_rst_cnt=00 to enable magnetic reset each cycle
  I2C_WriteAddr(I2C_1, FXOS8700_I2C_ADDR, FXOS8700_M_CTRL_REG2, 0x20); 
  time_delay_ms(10);
  // [1-0]: fs=00 for 2g mode: 2048 counts / g = 8192 counts / g after 2 bit left shift
  I2C_WriteAddr(I2C_1, FXOS8700_I2C_ADDR, FXOS8700_XYZ_DATA_CFG, 0x00); 
  time_delay_ms(10);
  // write 0000 0010 = 0x02 to CTRL_REG2 to set MODS bits
  // [7]: st=0: self test disabled
  // [6]: rst=0: reset disabled
  // [5]: unused
  // [4-3]: smods=00
  // [2]: slpe=0: auto sleep disabled
  // [1-0]: mods=10 for high resolution (maximum over sampling)
  I2C_WriteAddr(I2C_1, FXOS8700_I2C_ADDR, FXOS8700_CTRL_REG2, 0x02); 
  time_delay_ms(10);
  // write 0000 0101 = 0x05 to accelerometer control register 1
  // [7-6]: aslp_rate=00
  // [5-3]: dr=000=0 for 800Hz data rate (when in hybrid mode)
  // [2]: lnoise=1 for low noise mode (since we're in 2g mode)
  // [1]: f_read=0 for normal 16 bit reads
  // [0]: active=1 to take the part out of standby and enable sampling
  
  I2C_WriteAddr(I2C_1, FXOS8700_I2C_ADDR, FXOS8700_CTRL_REG1, 0x05); 
  time_delay_ms(10); 
}

void Update9AX(void)
{      
  //I2C_WriteAddr(I2C_1, FXAS21002_I2C_ADDR, FX_CTRL_REG1, 0x00);
  //time_delay_ms(50);
  GYRO_X.MYBYTE.BYTEH=LQ9AX_DAT[0] = I2C_ReadAddr(I2C_1, FXAS21002_I2C_ADDR, 0x01);//高8位陀螺仪
  time_delay_ms(1);
  GYRO_X.MYBYTE.BYTEL=LQ9AX_DAT[1] = I2C_ReadAddr(I2C_1, FXAS21002_I2C_ADDR, 0x02);//低8位
  time_delay_ms(1);
  GYRO_Y.MYBYTE.BYTEH=LQ9AX_DAT[2] = I2C_ReadAddr(I2C_1, FXAS21002_I2C_ADDR, 0x03);
  time_delay_ms(1);
  GYRO_Y.MYBYTE.BYTEL=LQ9AX_DAT[3] = I2C_ReadAddr(I2C_1, FXAS21002_I2C_ADDR, 0x04);
  time_delay_ms(1);
  GYRO_Z.MYBYTE.BYTEH=LQ9AX_DAT[4] = I2C_ReadAddr(I2C_1, FXAS21002_I2C_ADDR, 0x05);
  time_delay_ms(1);
  GYRO_Z.MYBYTE.BYTEL=LQ9AX_DAT[5] = I2C_ReadAddr(I2C_1, FXAS21002_I2C_ADDR, 0x06);
  time_delay_ms(1);

  ACC_X.MYBYTE.BYTEH=LQ9AX_DAT[6] = I2C_ReadAddr(I2C_1,  FXOS8700_I2C_ADDR, 0x01);  //高8位加速度,
  time_delay_ms(1);
  ACC_X.MYBYTE.BYTEL=LQ9AX_DAT[7] = I2C_ReadAddr(I2C_1,  FXOS8700_I2C_ADDR, 0x02);  //低6位
  time_delay_ms(1);
  ACC_Y.MYBYTE.BYTEH=LQ9AX_DAT[8] = I2C_ReadAddr(I2C_1,  FXOS8700_I2C_ADDR, 0x03);
  time_delay_ms(1);
  ACC_Y.MYBYTE.BYTEL=LQ9AX_DAT[9] = I2C_ReadAddr(I2C_1,  FXOS8700_I2C_ADDR, 0x04);
  time_delay_ms(1);
  ACC_Z.MYBYTE.BYTEH=LQ9AX_DAT[10] = I2C_ReadAddr(I2C_1, FXOS8700_I2C_ADDR, 0x05);
  time_delay_ms(1);
  ACC_Z.MYBYTE.BYTEL=LQ9AX_DAT[11] = I2C_ReadAddr(I2C_1, FXOS8700_I2C_ADDR, 0x06);
  time_delay_ms(1);

  MAG_X.MYBYTE.BYTEH=LQ9AX_DAT[12] = I2C_ReadAddr(I2C_1, FXOS8700_I2C_ADDR, 0x33); //高8位地磁传感器
  time_delay_ms(1);
  MAG_X.MYBYTE.BYTEL=LQ9AX_DAT[13] = I2C_ReadAddr(I2C_1, FXOS8700_I2C_ADDR, 0x34); //低8位
  time_delay_ms(1);
  MAG_Y.MYBYTE.BYTEH=LQ9AX_DAT[14] = I2C_ReadAddr(I2C_1, FXOS8700_I2C_ADDR, 0x35);
  time_delay_ms(1);
  MAG_Y.MYBYTE.BYTEL=LQ9AX_DAT[15] = I2C_ReadAddr(I2C_1, FXOS8700_I2C_ADDR, 0x36);
  time_delay_ms(1);
  MAG_Z.MYBYTE.BYTEH=LQ9AX_DAT[16] = I2C_ReadAddr(I2C_1, FXOS8700_I2C_ADDR, 0x37);
  time_delay_ms(1);
  MAG_Z.MYBYTE.BYTEL=LQ9AX_DAT[17] = I2C_ReadAddr(I2C_1, FXOS8700_I2C_ADDR, 0x38);
  time_delay_ms(1);

  LQ9AX_DAT[18] = I2C_ReadAddr(I2C_1, FXOS8700_I2C_ADDR, 0x51);//温度传感器-40~125°
  time_delay_ms(1);
  //Data_Send_Senser();
  //time_delay_ms(100);
}

void UpdateAcceleration(){
  ACC_Y.MYBYTE.BYTEH=LQ9AX_DAT[8] = I2C_ReadAddr(I2C_1,  FXOS8700_I2C_ADDR, 0x03);
  LPTMR_delay_us(20);       //至少延时16us，否则爆炸
  ACC_Y.MYBYTE.BYTEL=LQ9AX_DAT[9] = I2C_ReadAddr(I2C_1,  FXOS8700_I2C_ADDR, 0x04);
}

void UpdateGyro(){
  LPTMR_delay_us(20);
  GYRO_X.MYBYTE.BYTEH=LQ9AX_DAT[0] = I2C_ReadAddr(I2C_1, FXAS21002_I2C_ADDR, 0x01);//高8位陀螺仪
  LPTMR_delay_us(20);
  GYRO_X.MYBYTE.BYTEL=LQ9AX_DAT[1] = I2C_ReadAddr(I2C_1, FXAS21002_I2C_ADDR, 0x02);//低8位
}

int ReadGyro(){
  int tem=0;
  UpdateAcceleration();
  if(ACC_Y.MYBYTE.BYTEH > 0x7F){
    tem= (~(ACC_Y.MYWORD>>2) + 1)&0X3FFF;
  }else{
    tem=(ACC_Y.MYWORD>>2)&0X3FFF;
  }

  VirtualOscilloscopeData[0] = tem;
//  VirtualOscilloscopeData[1] =  tem = angleFromAcceleration = ButtterworthLowPassFiltering(tem, &butterworthOfAcceleration, &Butter_10HZ_Parameter_Acce);

//  UpdateGyro();
//  angleFromGyro -= ((GYRO_X.MYWORD + gyroLast) * 0.01);
//  gyroLast = GYRO_X.MYWORD;
  angleFromGyro +=  0.155;
  VirtualOscilloscopeData[2] = angleFromGyro;
  VirtualOscilloscopeData[3] = GYRO_X.MYWORD;
  VirtualOscilloscope(VirtualOscilloscopeData);

  return tem;
}

void GyroInit(){
  angleFromGyro = ReadGyro();
}

void Cvt_14bit_Str(char str[],LQ9AXt V2)
{
  char characters[17]="0123456789ABCDEF";   
  u16 V1=1;
  float vv=0.01;
  uint8 tv=0; 
  
  V1=V2.MYWORD;  
  
  str[2] = '0';
  str[3] = 'x'; 
  tv=(u8)(V1>>12);
  str[4] = characters[tv]; 
  tv=(u8)((V1&0x0FFF)>>8);
  str[5] = characters[tv];  
  tv=(u8)((V1&0x00FF)>>4);    
  str[6] = characters[tv];        
  tv=(u8)(V1%16);    
  str[7] = characters[tv];
  str[8] = ' '; 
  
  if(V2.MYBYTE.BYTEH >0x7F)
  {
    str[9] = '-';      
    V1= (~(V2.MYWORD>>2) + 1)&0X3FFF;    
  }
  else
  {
    str[9] = '+'; 
    V1=(V2.MYWORD>>2)&0X3FFF;   
  }   
  
  vv=((((float)V1)/0xfff)*10000);
  //str[9] = '-';
  str[10] = characters[(u8)(V1/0xfff)];
  str[11] = '.';
  str[12] = characters[(u8)((((u16)vv)%10000)/1000)];  
  str[13] = characters[(u8)((((u16)vv)%1000)/100)];
  str[14] = characters[(u8)((((u16)vv)%100)/10)];
  str[15] = characters[(u8)(( (u16)vv)%10)];  
  str[16] = 'g';
  str[17] = '\0';
}  

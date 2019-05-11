#include "include.h"


 //************初始化L3G4200D*********************************
void Init_L3G4200D(void)
{
  I2C_Init(I2C_1);
  I2C_WriteAddr(I2C_1,L3G4200_Addr,CTRL_REG11, 0xE9);   //y,z axis disable  x axis enable
  I2C_WriteAddr(I2C_1,L3G4200_Addr,CTRL_REG22, 0x00);
  I2C_WriteAddr(I2C_1,L3G4200_Addr,CTRL_REG33, 0x08);
  //I2C_WriteAddr(I2C_1,L3G4200_Addr,CTRL_REG44, 0x30);	//+-2000ds
  //I2C_WriteAddr(I2C_1,L3G4200_Addr,CTRL_REG44, 0x10);	//+-500dps
  I2C_WriteAddr(I2C_1,L3G4200_Addr,CTRL_REG44, 0x00);	//+-250dps		
  I2C_WriteAddr(I2C_1,L3G4200_Addr,CTRL_REG55, 0x03);    //enable Low-Pass-filtered
}	
//******读取L3G4200D数据****************************************
int READ_L3G4200D(void)
{
  char  txt[16];
  unsigned char BUF[2];
  int T_X=0; 
  BUF[0]= I2C_ReadAddr(I2C_1, L3G4200_Addr, OUT_Y_L);
  time_delay_ms(1);
  BUF[1]= I2C_ReadAddr(I2C_1, L3G4200_Addr, OUT_Y_H);
  T_X = (BUF[1]<<8)|BUF[0];
     
  // BUF[2]=Single_Read(L3G4200_Addr,OUT_Y_L);
  // BUF[3]=Single_Read(L3G4200_Addr,OUT_Y_H);
  // T_Y=	(BUF[3]<<8)|BUF[2];
  sprintf(txt,"X:+%03d ",T_X);  //转换为字符串
  LCD_P6x8Str(10,6,(uint8*)txt);
    

  //  BUF[4]=Single_Read(L3G4200_Addr,OUT_Z_L);
  //  BUF[5]=Single_Read(L3G4200_Addr,OUT_Z_H);
  if(T_X > 32768)
  {
    T_X = T_X - 65535 ;   //理论上减65535 
  }
  return (T_X + 90); //  - 60  为陀螺仪的零点偏移量  43
}
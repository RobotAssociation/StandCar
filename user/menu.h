#ifndef __MENU_H__
#define __MENU_H__

void Menu();
void MenuInit();
void MenuAdd(u8 size,char text, unsigned char x, unsigned char y );
void MenuPageAdd(void (*aPage)(void));

/***************OLED菜单页函数*********************/
void OLEDMenuOfCameraImage();
void OLEDMenuOfGraphPID();
void OLEDMenuOfElectromagnetismPID();
void OLEDMenuOfMotorLeft();
void OLEDMenuOfMotorRight();
void OLEDMenuOfGyro();
void OLEDMenuOfADCshow();
void OLEDMenuOfADCMedia();
void OLEDMenuOfSpeedMeasure();
void OLEDMenuOfERECT();
void OLEDMenuOfGraphSpread();

/****************TFT1.8菜单页显示函数**************/
void TFTMenuOfMT9V034();


typedef struct menues{
  void (*page)(void);
  struct menues *next;
  struct menues *last;
}menu;

typedef struct{
  double value[5];
}menuValue;

typedef enum{
  OLED,
  TFT,
  TFTMIN,
}monitor;

extern monitor monitorSelection;

#endif 

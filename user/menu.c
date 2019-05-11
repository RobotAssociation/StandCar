#include "include.h"

#define NUMBER_OF_MENUS 7
#define NUMBER_OF_SPEED 2
#define NUMBER_OF_PID 4
#define NUMBER_OF_MOTOR 4
#define NUMBER_OF_ERECT 4
#define NUMBER_OF_ADC 5
#define NUMBER_OF_GRAPH_SPREAD 2
#define VARIATION_SPEED_PID 0.001
#define VARIATION_GRAPH_PID 1
#define VARIATION_Electromagnetism_PID 0.01
#define VARIATION_ERECT_PID 0.001
#define VARIATION_SPEED 10
#define VARIATION_ERECT 20
#define VARIATION_SERVO_MEDIA 10




short menuSelection = 0;
short menuSwitch = 0;
short temp;

menu *head = NULL, *menus;
monitor monitorSelection;
short menuPages = 0;
char txt[16];

/**************************
菜单初始化
将写好的菜单页使用MenuPageAdd();添加即可使用
菜单页的添加顺序就是菜单的顺序
**************************/
void MenuInit(){
  if(monitorSelection == OLED){
    //OLED的菜单页放这里
    MenuPageAdd(OLEDMenuOfGyro);
    //MenuPageAdd(OLEDMenuOfCameraImage);
    //MenuPageAdd(OLEDMenuOfGraphPID);
    MenuPageAdd(OLEDMenuOfElectromagnetismPID);
    MenuPageAdd(OLEDMenuOfMotorLeft);
    MenuPageAdd(OLEDMenuOfMotorRight);
//    MenuPageAdd(OLEDMenuOfERECT);

    MenuPageAdd(OLEDMenuOfADCshow);
    MenuPageAdd(OLEDMenuOfGraphSpread);
  }else if(monitorSelection == TFT){
    //TFT1.8的菜单页放这里
    MenuPageAdd(TFTMenuOfMT9V034);
    TFTSPI_CLS(u16WHITE);
  }
//  MenuPageAdd(MenuOfADCMedia);
//  MenuPageAdd(menuOfSpeedMeasure);
  menus = head;
}

void Menu(){

  if(!KEY_Read(left)){
      if(!KEY_Read(left)){
        menus = menus->last;
        LCD_CLS();
      }
    }else if(!KEY_Read(right)){
      if(!KEY_Read(right)){
        menus = menus->next;
        LCD_CLS();
      }
    }
    

    (*menus->page)();
}

void MenuPageAdd(void (*aPage)(void)) {
  menu *node = NULL;
  node = (menu *)malloc(sizeof(menu));
  if(!head){
    head = node;
  }

  menus = head;
  node->next = head;
  node->page = aPage;

  while(!(menus->next == head)){
    menus = menus->next;
  }
  node->last = menus;
  menus->next = node;
  head->last = node;
}


void TFTMenuOfMT9V034(){

  for (int i = GRAPH_HIGHT - 1; i > 0; i--){
    for (int j = 87 - 1; j > 7; j--){
      TFTSPI_Draw_Dot(2*j - 14, 2*i, Image_Use[i][j]);
      TFTSPI_Draw_Dot(2*j - 14+ 1, 2*i + 1, Image_Use[i][j]);
      TFTSPI_Draw_Dot(2*j - 14+ 1, 2*i, Image_Use[i][j]);
      TFTSPI_Draw_Dot(2*j - 14, 2*i + 1, Image_Use[i][j]);

    }
  }

}


void OLEDMenuOfGraphSpread(){
  if(!KEY_Read(middle)){
    if(!KEY_Read(middle)){
      menuSwitch++;
    }
  }

  menuSwitch = menuSwitch % NUMBER_OF_GRAPH_SPREAD;

  if(menuSwitch){
    UART_Put_Char(UART_4, 0x00);
    UART_Put_Char(UART_4, 0xFF);
    UART_Put_Char(UART_4, 0x01);
    UART_Put_Char(UART_4, 0x00);
    for(int i = 0; i < GRAPH_HIGHT - 1; i++){
      for(int j = 0; j < GRAPH_WIDTH - 1; j++){
        UART_Put_Char(UART_4, Image_Use[i][j]);
      }
    }
  }
}


void OLEDMenuOfCameraImage(){
  LCD_Show_Frame100();
  Draw_Road();
  LCD_P8x16Str(0,0,"Graph");
  sprintf(txt,"%03d",threshold);
  LCD_P6x8Str(100,1,(u8*)txt);
}


void OLEDMenuOfERECT(){
  if (!KEY_Read(middle)) {
    if (!KEY_Read(middle)) {
      menuSwitch++;
      LCD_CLS();
    }
  }

  if (!KEY_Read(up)) {
    if (!KEY_Read(up)) {
      if (menuSwitch == 0) {
        PIDErect.proportion += VARIATION_ERECT_PID;
      } else if (menuSwitch == 1) {
        PIDErect.integral += VARIATION_ERECT_PID;
      } else if (menuSwitch == 2) {
        PIDErect.derivative += VARIATION_ERECT_PID;
      } else if (menuSwitch == 3) {
        PIDErect.setPoint += VARIATION_ERECT;
      }
    }
  }else if(!KEY_Read(down)) {
    if (!KEY_Read(down)) {
      if (menuSwitch == 0) {
        PIDErect.proportion -= VARIATION_ERECT_PID;
      } else if (menuSwitch == 1) {
        PIDErect.integral -= VARIATION_ERECT_PID;
      } else if (menuSwitch == 2) {
        PIDErect.derivative -= VARIATION_ERECT_PID;
      } else if (menuSwitch == 3) {
        PIDErect.setPoint -= VARIATION_ERECT;
      }
    }
  }

  menuSwitch = menuSwitch % NUMBER_OF_ERECT;

  temp = (int) (PIDErect.proportion * (1/VARIATION_ERECT_PID));
  sprintf(txt, "P:%04d", temp);
  LCD_P8x16Str(10, 2, (u8*) txt);

  temp = (int) (PIDErect.integral * (1/VARIATION_ERECT_PID));
  sprintf(txt, "I:%04d", temp);
  LCD_P8x16Str(10, 4, (u8*) txt);

  temp = (int) (PIDErect.derivative * (1/VARIATION_ERECT_PID));
  sprintf(txt, "D:%04d", temp);
  LCD_P8x16Str(10, 6, (u8*) txt);


  sprintf(txt, "%04d", PIDErect.setPoint);
  LCD_P8x16Str(80, 4, (u8*) txt);

  sprintf(txt, "%04d", speedRightGet);
  LCD_P8x16Str(80, 6, (u8*) txt);

  LCD_P8x16Str(0, 0, "Erect");
  LCD_P8x16Str(70, 2, "Bary");
  if ((menuSwitch + 1) * 2 < 8) {
    LCD_P8x16Str(0, (menuSwitch + 1) * 2, ">");
  } else {
    LCD_P8x16Str(70, (menuSwitch + 1) * 2 - 4, ">");
  }


}


void OLEDMenuOfMotorRight(){
  if (!KEY_Read(middle)) {
    if (!KEY_Read(middle)) {
      menuSwitch++;
      LCD_CLS();
    }
  }
  menuSwitch = menuSwitch % NUMBER_OF_MOTOR;

  if (!KEY_Read(up)) {
    if (!KEY_Read(up)) {
      if (menuSwitch == 0) {
        PIDMotorRight.proportion += VARIATION_SPEED_PID;
      } else if (menuSwitch == 1) {
        PIDMotorRight.integral += VARIATION_SPEED_PID;
      } else if (menuSwitch == 2) {
        PIDMotorRight.derivative += VARIATION_SPEED_PID;
      } else if (menuSwitch == 3) {
        PIDMotorRight.setPoint += VARIATION_SPEED;
      }
    }
  }else if(!KEY_Read(down)) {
    if (!KEY_Read(down)) {
      if (menuSwitch == 0) {
        PIDMotorRight.proportion -= VARIATION_SPEED_PID;
      } else if (menuSwitch == 1) {
        PIDMotorRight.integral -= VARIATION_SPEED_PID;
      } else if (menuSwitch == 2) {
        PIDMotorRight.derivative -= VARIATION_SPEED_PID;
      } else if (menuSwitch == 3) {
        PIDMotorRight.setPoint -= VARIATION_SPEED;
      }
    }
  }

  temp = (int) (PIDMotorRight.proportion * (1/VARIATION_SPEED_PID));
  sprintf(txt, "P:%04d", temp);
  LCD_P8x16Str(10, 2, (u8*) txt);

  temp = (int) (PIDMotorRight.integral * (1/VARIATION_SPEED_PID));
  sprintf(txt, "I:%04d", temp);
  LCD_P8x16Str(10, 4, (u8*) txt);

  temp = (int) (PIDMotorRight.derivative * (1/VARIATION_SPEED_PID));
  sprintf(txt, "D:%04d", temp);
  LCD_P8x16Str(10, 6, (u8*) txt);


  sprintf(txt, "%04d", PIDMotorRight.setPoint);
  LCD_P8x16Str(80, 4, (u8*) txt);

  sprintf(txt, "%04d", speedRightGet);
  LCD_P8x16Str(80, 6, (u8*) txt);

  LCD_P8x16Str(0, 0, "Right Motor");
  LCD_P8x16Str(80, 2, "Speed");
  if ((menuSwitch + 1) * 2 < 8) {
    LCD_P8x16Str(0, (menuSwitch + 1) * 2, ">");
  } else {
    LCD_P8x16Str(70, (menuSwitch + 1) * 2 - 4, ">");
  }

}

void OLEDMenuOfMotorLeft(){
  if (!KEY_Read(middle)) {

    if (!KEY_Read(middle)) {
      menuSwitch++;
      LCD_CLS();
    }
  }
  menuSwitch = menuSwitch % NUMBER_OF_MOTOR;

  if (!KEY_Read(up)) {
    if (!KEY_Read(up)) {
      if (menuSwitch == 0) {
        PIDMotorLeft.proportion += VARIATION_SPEED_PID;
      } else if (menuSwitch == 1) {
        PIDMotorLeft.integral += VARIATION_SPEED_PID;
      } else if (menuSwitch == 2) {
        PIDMotorLeft.derivative += VARIATION_SPEED_PID;
      } else if (menuSwitch == 3) {
        PIDMotorLeft.setPoint += VARIATION_SPEED;
      }
    }
  } else if (!KEY_Read(down)) {
    if (!KEY_Read(down)) {
      if (menuSwitch == 0) {
        PIDMotorLeft.proportion -= VARIATION_SPEED_PID;
      } else if (menuSwitch == 1) {
        PIDMotorLeft.integral -= VARIATION_SPEED_PID;
      } else if (menuSwitch == 2) {
        PIDMotorLeft.derivative -= VARIATION_SPEED_PID;
      } else if (menuSwitch == 3) {
        PIDMotorLeft.setPoint -= VARIATION_SPEED;
      }
    }
  }

  temp = (int) (PIDMotorLeft.proportion * (1/VARIATION_SPEED_PID));
  sprintf(txt, "P:%04d", temp);
  LCD_P8x16Str(10, 2, (u8*) txt);

  temp = (int) (PIDMotorLeft.integral * (1/VARIATION_SPEED_PID));
  sprintf(txt, "I:%04d", temp);
  LCD_P8x16Str(10, 4, (u8*) txt);

  temp = (int) (PIDMotorLeft.derivative * (1/VARIATION_SPEED_PID));
  sprintf(txt, "D:%04d", temp);
  LCD_P8x16Str(10, 6, (u8*) txt);


  sprintf(txt, "%04d", PIDMotorLeft.setPoint);
  LCD_P8x16Str(80, 4, (u8*) txt);

  sprintf(txt, "%04d", speedLeftGet);
  LCD_P8x16Str(80, 6, (u8*) txt);

  LCD_P8x16Str(0, 0, "Left Motor");
  LCD_P8x16Str(80, 2, "Speed");
  if ((menuSwitch + 1) * 2 < 8) {
    LCD_P8x16Str(0, (menuSwitch + 1) * 2, ">");
  } else {
    LCD_P8x16Str(70, (menuSwitch + 1) * 2 - 4, ">");
  }

}

void OLEDMenuOfGraphPID(){
  if(!KEY_Read(middle)){
    if(!KEY_Read(middle)){
      menuSwitch++;
      LCD_CLS();
    }
  }
  menuSwitch = menuSwitch%NUMBER_OF_PID;

  if(!KEY_Read(up)){
    if(!KEY_Read(up)){
      if(menuSwitch == 0){
        PIDServoOfGraph.proportion += VARIATION_GRAPH_PID;
      }else if(menuSwitch == 1){
        PIDServoOfGraph.integral += VARIATION_GRAPH_PID;
      }else if(menuSwitch == 2){
        PIDServoOfGraph.derivative += VARIATION_GRAPH_PID;
      }else if(menuSwitch == 3){
        servoMedian += VARIATION_SERVO_MEDIA;
      }
    }
  }else if(!KEY_Read(down)){
    if(!KEY_Read(down)){
      if(menuSwitch == 0){
        PIDServoOfGraph.proportion -= VARIATION_GRAPH_PID;
      }else if(menuSwitch == 1){
        PIDServoOfGraph.integral -= VARIATION_GRAPH_PID;
      }else if(menuSwitch == 2){
        PIDServoOfGraph.derivative -= VARIATION_GRAPH_PID;
      }else if(menuSwitch == 3){
        servoMedian -= VARIATION_SERVO_MEDIA;
      }
    }
  }

  temp = (int)(PIDServoOfGraph.proportion * (1/VARIATION_GRAPH_PID));
  sprintf(txt,"P:%04d",temp);
  LCD_P8x16Str(20,2,(u8*)txt);

  temp = (int)(PIDServoOfGraph.integral * (1/VARIATION_GRAPH_PID));
  sprintf(txt,"I:%04d",temp);
  LCD_P8x16Str(20,4,(u8*)txt);

  temp = (int)(PIDServoOfGraph.derivative * (1/VARIATION_GRAPH_PID));
  sprintf(txt,"D:%04d",temp);
  LCD_P8x16Str(20,6,(u8*)txt);

  sprintf(txt, "MEDIAN");
  LCD_P8x16Str(80, 2, (u8*) txt);

  sprintf(txt, "%04d", servoMedian);
  LCD_P8x16Str(80, 4, (u8*) txt);

  sprintf(txt, "%04d", servo);
  LCD_P8x16Str(80, 6, (u8*) txt);


  LCD_P8x16Str(0,0,"Graph PID");
    
  if(menuSwitch == 3){
  LCD_P8x16Str(70,4,">");
  }else{
    LCD_P8x16Str(0,(menuSwitch+1)*2,"->");
  }


}

void OLEDMenuOfElectromagnetismPID(){
  if(!KEY_Read(middle)){
    if(!KEY_Read(middle)){
      menuSwitch++;
      LCD_CLS();
    }
  }
  menuSwitch = menuSwitch%NUMBER_OF_PID;

  if(!KEY_Read(up)){
    if(!KEY_Read(up)){
      if(menuSwitch == 0){
        PIDServoOfElectromagnetism.proportion += VARIATION_Electromagnetism_PID;
      }else if(menuSwitch == 1){
        PIDServoOfElectromagnetism.integral += VARIATION_Electromagnetism_PID;
      }else if(menuSwitch == 2){
        PIDServoOfElectromagnetism.derivative += VARIATION_Electromagnetism_PID;
      }else if(menuSwitch == 3){
        servoMedian += VARIATION_SERVO_MEDIA;
      }
    }
  }else if(!KEY_Read(down)){
    if(!KEY_Read(down)){
      if(menuSwitch == 0){
        PIDServoOfElectromagnetism.proportion -= VARIATION_Electromagnetism_PID;
      }else if(menuSwitch == 1){
        PIDServoOfElectromagnetism.integral -= VARIATION_Electromagnetism_PID;
      }else if(menuSwitch == 2){
        PIDServoOfElectromagnetism.derivative -= VARIATION_Electromagnetism_PID;
      }else if(menuSwitch == 3){
        servoMedian -= VARIATION_SERVO_MEDIA;
      }
    }
  }

  temp = (int)(PIDServoOfElectromagnetism.proportion * (1/VARIATION_Electromagnetism_PID));
  sprintf(txt,"P:%04d",temp);
  LCD_P8x16Str(20,2,(u8*)txt);

  temp = (int)(PIDServoOfElectromagnetism.integral * (1/VARIATION_Electromagnetism_PID));
  sprintf(txt,"I:%04d",temp);
  LCD_P8x16Str(20,4,(u8*)txt);

  temp = (int)(PIDServoOfElectromagnetism.derivative * (1/VARIATION_Electromagnetism_PID));
  sprintf(txt,"D:%04d",temp);
  LCD_P8x16Str(20,6,(u8*)txt);

  sprintf(txt, "MEDIAN");
  LCD_P8x16Str(80, 2, (u8*) txt);

  sprintf(txt, "%04d", servoMedian);
  LCD_P8x16Str(80, 4, (u8*) txt);

  sprintf(txt, "%04d", servo);
  LCD_P8x16Str(80, 6, (u8*) txt);


  LCD_P8x16Str(0,0,"Electromagnetism PID");

  if(menuSwitch == 3){
  LCD_P8x16Str(70,4,">");
  }else{
    LCD_P8x16Str(0,(menuSwitch+1)*2,"->");
  }


}


void OLEDMenuOfGyro(){
    temp=MPU_Get_Temperature();	//得到温度值
    uint16_t aacx,aacy,aacz,gyrox,gyroy,gyroz;
    MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
    MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
    sprintf((char*)txt,"ax:%06d",aacx);
    LCD_P6x8Str(0,0,txt);
    sprintf((char*)txt,"ay:%06d",aacy);
    LCD_P6x8Str(0,1,txt);
    sprintf((char*)txt,"az:%06d",aacz);
    LCD_P6x8Str(0,2,txt);
    sprintf((char*)txt,"gx:%06d",gyrox);
    LCD_P6x8Str(0,3,txt);
    sprintf((char*)txt,"gy:%06d",gyroy);
    LCD_P6x8Str(0,4,txt);
    sprintf((char*)txt,"gz:%06d",gyroz);
    LCD_P6x8Str(0,5,txt);
}



void OLEDMenuOfADCshow(){
//  ADC0_Ch_e ADCRemawp[] = {ADC0_SE5a, ADC0_SE9, ADC0_DP1, ADC0_SE11, ADC0_DP3, ADC0_SE4a, ADC0_SE10, ADC0_DP2};
  ADC0_Ch_e ADCRemawp[] = {ADC0_DP1, ADC0_SE5a, ADC0_DP2, ADC0_DP3,  ADC0_SE11, ADC0_SE9, ADC0_SE4a, ADC0_SE10};
//  if(!KEY_Read(Up)){
//    time_delay_ms(100);
//    if(!KEY_Read(Up)){
//      menuSwitch++;
//    }
//  }else if(!KEY_Read(Down)){
//    time_delay_ms(100);
//    if(!KEY_Read(Down)){
//      menuSwitch--;
//      if(menuSwitch<0){
//       menuSwitch = NUMBEROFADC-1;
//      }
//  }
//
//  menuSwitch = menuSwitch % NUMBEROFADC;


  if(!KEY_Read(up)){
    if(!KEY_Read(up)){
      menuSwitch += 1;
    }
  }else if(!KEY_Read(down)){
    if(!KEY_Read(down)){
      menuSwitch -= 1;
      if(menuSwitch<0){
        menuSwitch = NUMBER_OF_ADC;
      }
    }
  }

  LCD_CLS();

  sprintf(txt,"ADC%d:%d",(menuSwitch+0)%(NUMBER_OF_ADC),ADC0_Ave(ADCRemawp[(menuSwitch+0)%(NUMBER_OF_ADC)],ADC_16bit,10));  //2
  LCD_P8x16Str(0,0,(u8*)txt);

  sprintf(txt,"ADC%d:%d",(menuSwitch+1)%(NUMBER_OF_ADC),ADC0_Ave(ADCRemawp[(menuSwitch+1)%(NUMBER_OF_ADC)],ADC_16bit,10)); //0
  LCD_P8x16Str(0,2,(u8*)txt);

  sprintf(txt,"ADC%d:%d",(menuSwitch+2)%(NUMBER_OF_ADC),ADC0_Ave(ADCRemawp[(menuSwitch+2)%(NUMBER_OF_ADC)],ADC_16bit,10)); //7
  LCD_P8x16Str(0,4,(u8*)txt);

  sprintf(txt,"ADC%d:%d",(menuSwitch+3)%(NUMBER_OF_ADC),ADC0_Ave(ADCRemawp[(menuSwitch+3)%(NUMBER_OF_ADC)],ADC_16bit,10)); //4
  LCD_P8x16Str(0,6,(u8*)txt);


  time_delay_ms(10);

}

void OLEDMenuOfADCMedia(){
  ADC0_Ch_e ADCRemawp[] = {ADC0_DP1, ADC0_SE5a, ADC0_DP2, ADC0_DP3,  ADC0_SE11, ADC0_SE9, ADC0_SE4a, ADC0_SE10};
  sprintf(txt,"ADC:%d",(int)(ADC0_Ave(ADCRemawp[3],ADC_16bit,10)-ADC0_Ave(ADCRemawp[0],ADC_16bit,10)+ADC0_Ave(ADCRemawp[2],ADC_16bit,10)-ADC0_Ave(ADCRemawp[1],ADC_16bit,10))*10000
                           /(ADC0_Ave(ADCRemawp[3],ADC_16bit,10)+ADC0_Ave(ADCRemawp[0],ADC_16bit,10)+ADC0_Ave(ADCRemawp[2],ADC_16bit,10)+ADC0_Ave(ADCRemawp[1],ADC_16bit,10)));
  LCD_P8x16Str(0,3,(u8*)txt);
}


void MenuOfPrecisionAdjustment(){

}


void OLEDMenuOfSpeedMeasure(){

}


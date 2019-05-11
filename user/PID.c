#include "include.h"

PID PIDServoOfGraph,PIDServoOfElectromagnetism, PIDMotorLeft, PIDMotorRight, PIDErect;
int speedSet = 0;

void PIDInit(){
  PIDServoOfGraph.setPoint = 0;
  PIDServoOfGraph.proportion = 17;  //0.27
  PIDServoOfGraph.integral = 0;
  PIDServoOfGraph.derivative = 0;
  PIDServoOfGraph.isDeviation = false;

  PIDServoOfElectromagnetism.setPoint = 0;
  PIDServoOfElectromagnetism.proportion = 0.17;  //0.27
  PIDServoOfElectromagnetism.integral = 0;
  PIDServoOfElectromagnetism.derivative = 0;
  PIDServoOfElectromagnetism.isDeviation = false;

  PIDMotorLeft.setPoint = 0;
  PIDMotorLeft.proportion = 0.010;
  PIDMotorLeft.integral = 0.007;
  PIDMotorLeft.derivative = 0;
  PIDMotorLeft.isDeviation = true;

  PIDMotorRight.setPoint = 110;
  PIDMotorRight.proportion = 0.010;
  PIDMotorRight.integral = 0.007;
  PIDMotorRight.derivative = 0;
  PIDMotorRight.isDeviation = true;

  PIDErect.setPoint = 2100;
  PIDErect.proportion =0.8;
  PIDErect.integral = 0.000;
  PIDErect.derivative = 0;
  PIDErect.isDeviation = false;
}


//λ��ʽPID
int PositionalPID(int NextPoint, PID *aPID){
  int  currentError,proportionVariable, integralVariable, derivativeVariable;

  currentError = aPID->setPoint - NextPoint;
  aPID->sumError += currentError;

  proportionVariable = currentError;
  integralVariable = aPID->sumError;
  derivativeVariable = currentError - aPID->lastError;

  aPID->prevError = aPID->lastError;
  aPID->lastError = currentError;

  if(aPID->isDeviation){
    return (int)( aPID->proportion * proportionVariable
                + aPID->integral * integralVariable
                + aPID->derivative * derivativeVariable);
  }else{
    return (int)(aPID->setPoint
               + aPID->proportion * proportionVariable
               + aPID->integral * integralVariable
               + aPID->derivative * derivativeVariable);
  }
}

//����ʽPID
int IncrementalPID(int NextPoint, PID *aPID){
  int  currentError, proportionVariable, integralVariable, derivativeVariable;
  
  currentError = aPID->setPoint - NextPoint;
  aPID->sumError += currentError;

  proportionVariable = currentError - aPID->lastError;
  integralVariable = currentError;
  derivativeVariable = currentError - 2*aPID->lastError + aPID->prevError;

  aPID->prevError = aPID->lastError;
  aPID->lastError = currentError;
  
  
  if(aPID->isDeviation){
    return (int)( aPID->proportion * proportionVariable
                + aPID->integral * integralVariable
                + aPID->derivative * derivativeVariable);
  }else{
    return (int)(aPID->setPoint
               + aPID->proportion * proportionVariable
               + aPID->integral * integralVariable
               + aPID->derivative * derivativeVariable);
  }
}



//int PositionalPID(int NextPoint, PID aPID){
//    int  iError,dError;
//    char txt[16];
//
//    iError = aPID->SetPoint - NextPoint;       //ƫ��
//    aPID->SumError += iError;       //����
//    dError = iError - aPID->LastError;     //΢��
//    aPID->LastError = iError;
//
//    return (int)(aPID->SetPoint + aPID->Proportion * iError            //������
//           + aPID->Integral * aPID->SumError   //������
//           + aPID->Derivative * dError
//           );
//}
//
//
//void MotorLeftLocPIDCalc(int NextPoint){
//    int  iError,dError;
//    char txt[16];
//
//    iError = PIDMotorLeft.SetPoint - NextPoint;       //ƫ��
//    PIDMotorLeft.SumError += iError;       //����
//    dError = iError - PIDMotorLeft.LastError;     //΢��
//    PIDMotorLeft.LastError = iError;
//
//    speedLeft = (int)(PIDMotorLeft.SetPoint + PIDMotorLeft.Proportion * iError            //������
//           + PIDMotorLeft.Integral * PIDMotorLeft.SumError   //������
//           + PIDMotorLeft.Derivative * dError
//           );
//    Motor_Duty(MotL,speedLeft);
//
//}
//
//void MotorRightLocPIDCalc(int NextPoint){
//    int  iError,dError;
//    char txt[16];
//
//    iError = PIDMotorRight.SetPoint - NextPoint;       //ƫ��
//    PIDMotorRight.SumError += iError;       //����
//    dError = iError - PIDMotorRight.LastError;     //΢��
//    PIDMotorRight.LastError = iError;
//
//    speedRight = (int)( PIDMotorRight.SetPoint + PIDMotorRight.Proportion * iError            //������
//           + PIDMotorRight.Integral * PIDMotorRight.SumError   //������
//           + PIDMotorRight.Derivative * dError
//           );
//    Motor_Duty(MotR,speedRight);
//}
//
//
//void ErectLocPIDCalc(int NextPoint){
//    int  iError,dError;
//    char txt[16];
//
//    iError = PIDErect.SetPoint - NextPoint;       //ƫ��
//    PIDErect.SumError += iError;       //����
//    dError = iError - PIDErect.LastError;     //΢��
//    PIDErect.LastError = iError;
//
//    angle = (int)(PIDErect.Proportion * iError            //������
//           + PIDErect.Integral * PIDErect.SumError   //������
//           + PIDErect.Derivative * dError
//           );
//
//    PIDMotorLeft.SetPoint = angle;
//    PIDMotorRight.SetPoint = angle;
//}



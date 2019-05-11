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


//位置式PID
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

//增量式PID
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
//    iError = aPID->SetPoint - NextPoint;       //偏差
//    aPID->SumError += iError;       //积分
//    dError = iError - aPID->LastError;     //微分
//    aPID->LastError = iError;
//
//    return (int)(aPID->SetPoint + aPID->Proportion * iError            //比例项
//           + aPID->Integral * aPID->SumError   //积分项
//           + aPID->Derivative * dError
//           );
//}
//
//
//void MotorLeftLocPIDCalc(int NextPoint){
//    int  iError,dError;
//    char txt[16];
//
//    iError = PIDMotorLeft.SetPoint - NextPoint;       //偏差
//    PIDMotorLeft.SumError += iError;       //积分
//    dError = iError - PIDMotorLeft.LastError;     //微分
//    PIDMotorLeft.LastError = iError;
//
//    speedLeft = (int)(PIDMotorLeft.SetPoint + PIDMotorLeft.Proportion * iError            //比例项
//           + PIDMotorLeft.Integral * PIDMotorLeft.SumError   //积分项
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
//    iError = PIDMotorRight.SetPoint - NextPoint;       //偏差
//    PIDMotorRight.SumError += iError;       //积分
//    dError = iError - PIDMotorRight.LastError;     //微分
//    PIDMotorRight.LastError = iError;
//
//    speedRight = (int)( PIDMotorRight.SetPoint + PIDMotorRight.Proportion * iError            //比例项
//           + PIDMotorRight.Integral * PIDMotorRight.SumError   //积分项
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
//    iError = PIDErect.SetPoint - NextPoint;       //偏差
//    PIDErect.SumError += iError;       //积分
//    dError = iError - PIDErect.LastError;     //微分
//    PIDErect.LastError = iError;
//
//    angle = (int)(PIDErect.Proportion * iError            //比例项
//           + PIDErect.Integral * PIDErect.SumError   //积分项
//           + PIDErect.Derivative * dError
//           );
//
//    PIDMotorLeft.SetPoint = angle;
//    PIDMotorRight.SetPoint = angle;
//}



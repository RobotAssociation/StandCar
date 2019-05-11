#ifndef __PID_H__
#define __PID_H__

typedef struct{
    int  setPoint;              //设定目标 Desired Value
    long sumError;              //误差累计
    double  proportion;         //比例常数 Proportional Cons
    double  integral;           //积分常数 Integral Const
    double  derivative;         //微分常数 Derivative Const
    int lastError;              //Error[-1]
    int prevError;              //Error[-2]
    bool  isDeviation;              //返回值是否加上设定值     1为不加，   2为加上设定值
} PID;

void PIDInit(void);
int PositionalPID(int NextPoint, PID *aPID);
int IncrementalPID(int NextPoint, PID *aPID);
//void ServoLocPIDCalc(int NextPoint);
//void MotorLeftLocPIDCalc(int NextPoint);
//void MotorRightLocPIDCalc(int NextPoint);
//void ErectLocPIDCalc(int NextPoint);

extern PID PIDServoOfGraph,PIDServoOfElectromagnetism, PIDMotorLeft, PIDMotorRight, PIDErect;


#endif 

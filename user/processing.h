#ifndef _PROCESSING_H
#define _PROCESSING_H


extern int32 servoMedian, servo, angle, gyroLast;
extern float angleFromGyro, angleFromAcceleration;

typedef struct{
  int deviationNow;
  int deviationLast;
}deviation;


int DataFusion();

int GraphProcessing();
int GraphProcessingOfEdgeFluctuation();
void GraphProcessingOfLineEdgeFluctuation();
void GraphProcessingOfLineScanFromMedian(int i);
void GraphProcessingOfLineScanFromSettingPoint(int i, int m);
s8 GraphProcessingOfLineScanFromEdge(int i);
s8 GraphProcessingOfLineScanFromQuarters(int i);
int GraphProcessingOfLineWhitePointCounting(int i, int n, int m);
bool IsDisconnectRoad();


int ElectromagnetismProcessing();
int ElectromagnetismProcessingOfBasics();
bool ElectromagnetismProcessingOfLoseDataForStop();

void GyroAngleProcessing();

#endif

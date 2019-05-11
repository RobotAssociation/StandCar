#include "include.h"

/*****************************Graph*******************************/
#define LINE_INITIAL 8                    //从GRAPH_HIGHT - LINE_INITIAL行开始扫描(有近到远)
#define LINE_TERMINATION 10               //到M行结束
#define LINE_QUARTERS 24                  //行的四分之一点
#define LINE_EDGE_LEFT 4                  //左边界扫描终点
#define LINE_EDGE_RIGHT 92                //右边界扫描终点
#define HEI 1
#define BAI 0
/*****************************************************************/

int servoMedian = 5140, graphMedian = 0, servo = 0, angle = 0, gyroLast = 0;
float angleFromGyro = 0, angleFromAcceleration = 0;

/*****************************Graph*******************************/
s8
  edgeLeft[GRAPH_HIGHT],
  edgeRight[GRAPH_HIGHT],                //左右边界

  graphicMedian[GRAPH_HIGHT] = 47;       //每一行图像中值

bool
  isEdgeLeft[GRAPH_HIGHT],
  isEdgeRight[GRAPH_HIGHT];

deviation graphic;                      //基于中线的偏差

/*****************************************************************/


int DataFusion(){
  GetUseImage();                       //采集图像数据存放数组;
  threshold = GetOSTU(imageData);      //OSTU大津法 获取全局阈值
  GetBinarizationValue();              //二值化图像数据
  if(IsDisconnectRoad()){
    servo = servoMedian + PositionalPID(GraphProcessing(), &PIDServoOfGraph);
  }else{
    servo = servoMedian + PositionalPID(ElectromagnetismProcessing(), &PIDServoOfElectromagnetism);
  }
}

int GraphProcessing(){
  if(fieldOverFlag){

    graphMedian = GraphProcessingOfEdgeFluctuation() - 47;
    fieldOverFlag = 0;

  }
  return graphMedian;
}



int GraphProcessingOfEdgeFluctuation(){
  //    graph[GRAPH_HIGHT][LCHW]
//  GraphProcessingOfLineScanFromEdge(GRAPH_HIGHT - LINE_INITIAL);
  for(int i = GRAPH_HIGHT - LINE_INITIAL; i > LINE_TERMINATION; i--){
    isEdgeLeft[i] = false;
    isEdgeRight[i] = false;

    if(!graph[i][graphicMedian[i+1]]){
      if(i == GRAPH_HIGHT - LINE_INITIAL){
        GraphProcessingOfLineScanFromQuarters(i);
      }else{

      }
    }else{
      GraphProcessingOfLineScanFromSettingPoint(i, graphicMedian[i+1]);
      graphicMedian[i] = (edgeLeft[i] + edgeRight[i])/2;
      graph[i][graphicMedian[i]] = 0;
    }
  }

  for(int i = GRAPH_WIDTH; i > 0; i--){
    graph[27][i] = 0;
    graph[GRAPH_HIGHT - LINE_INITIAL][i] = 0;
    graph[LINE_TERMINATION][i] = 0;
  }
  return graphicMedian[27];
}

void GraphProcessingOfLineScanFromMedian(int i){
  for(int j = graphicMedian[i+1]; j>LINE_EDGE_LEFT; j--){
    if(!graph[i][j] && !graph[i][j-1] && graph[i][j+1] && graph[i][j+2]){
      isEdgeLeft[i] = true;
      edgeLeft[i] = j;
      break;
    }
  }

  for(int j = graphicMedian[i+1]; j < LINE_EDGE_RIGHT; j++){
    if(!graph[i][j] && !graph[i][j+1] && graph[i][j-1] && graph[j-2]){
      isEdgeRight[i] = true;
      edgeRight[i] = j;
      break;
    }
  }
}

/*
从特定点向两边扫描边界； i为指定行； m为特定点
如GraphProcessingOfLineScanFromSettingPoint(30, 40)
表示为从图像的第30行的第40个点开始向两边扫描突变
*/
void GraphProcessingOfLineScanFromSettingPoint(int i, int m){
  for(int j = m; j > LINE_EDGE_LEFT; j--){
    if(!graph[i][j] && !graph[i][j-1] && graph[i][j+1] && graph[i][j+2]){
      isEdgeLeft[i] = true;
      edgeLeft[i] = j;
      break;
    }
  }
  if(!isEdgeLeft[i]){
    edgeLeft[i] = 0;
  }

  for(int j = m; j < LINE_EDGE_RIGHT; j++){
    if(!graph[i][j] && !graph[i][j+1] && graph[i][j-1] && graph[j-2]){
      isEdgeRight[i] = true;
      edgeRight[i] = j;
      break;
    }
  }

  if(!isEdgeRight[i]){
    edgeRight[i] = GRAPH_WIDTH - 1;
  }
}

int GraphProcessingOfLineWhitePointCounting(int i, int n, int m){
  int count = 0;
  for(int j = n; j < m; j++){
    if(graph[i][j]){
      count++;
    }
  }
  return count;
}

s8 GraphProcessingOfLineScanFromQuarters(int i){
//  if(graph[i][LINE_EDGE_LEFT] && !graph[i][LINE_EDGE_RIGHT]){
//
//  }else if(!graph[i][LINE_EDGE_LEFT] && graph[i][LINE_EDGE_RIGHT]){
//
//  }else
  if(graph[i][LINE_QUARTERS]){
    GraphProcessingOfLineScanFromSettingPoint(i, LINE_QUARTERS);
  }else if(graph[i][LINE_QUARTERS*3]){
    GraphProcessingOfLineScanFromSettingPoint(i, LINE_QUARTERS*3);
  }

}

bool IsDisconnectRoad(){
  if(GraphProcessingOfLineWhitePointCounting(GRAPH_HIGHT - LINE_INITIAL - 20, 0, GRAPH_WIDTH-1) < 15 &&
     GraphProcessingOfLineWhitePointCounting(GRAPH_HIGHT - LINE_INITIAL - 21, 0, GRAPH_WIDTH-1) < 15 &&
     GraphProcessingOfLineWhitePointCounting(GRAPH_HIGHT - LINE_INITIAL - 22, 0, GRAPH_WIDTH-1) < 15 &&
     GraphProcessingOfLineWhitePointCounting(GRAPH_HIGHT - LINE_INITIAL - 23, 0, GRAPH_WIDTH-1) < 15 &&
     GraphProcessingOfLineWhitePointCounting(GRAPH_HIGHT - LINE_INITIAL - 25, 0, GRAPH_WIDTH-1) < 15){
    BEE_ON;
    return false;
  }else{
    BEE_OFF;
    return true;
  }

}


int ElectromagnetismProcessing(){
  return ElectromagnetismProcessingOfBasics();
}

int ElectromagnetismProcessingOfBasics(){
  ADC0_Ch_e ADCRemawp[] = {ADC0_DP1, ADC0_SE5a, ADC0_DP2, ADC0_DP3,  ADC0_SE11, ADC0_SE9, ADC0_SE4a, ADC0_SE10};
  return (int)(ADC0_Ave(ADCRemawp[3],ADC_16bit,10)-ADC0_Ave(ADCRemawp[0],ADC_16bit,10)+ADC0_Ave(ADCRemawp[2],ADC_16bit,10)-ADC0_Ave(ADCRemawp[1],ADC_16bit,10))*10000
             /(ADC0_Ave(ADCRemawp[3],ADC_16bit,10)+ADC0_Ave(ADCRemawp[0],ADC_16bit,10)+ADC0_Ave(ADCRemawp[2],ADC_16bit,10)+ADC0_Ave(ADCRemawp[1],ADC_16bit,10));
}

bool ElectromagnetismProcessingOfLoseDataForStop(){

}

void GyroAngleProcessing(){
  angle = angleFromGyro - PIDErect.setPoint;

   PIDMotorRight.setPoint = PIDMotorLeft.setPoint = -angle;



}




#ifndef _FILTER_H_
#define _FILTER_H_

typedef struct{
 float Input_Butter[3];
 float Output_Butter[3];
}butterworthData;

typedef struct{
  float a[3];
  float b[3];
}butterworthParameter;

//typedef struct{
//
//}kalmanParameter;

typedef struct{
  u8 count;
  int32 lastSum;
  int32 currentOutData;
  int32 lastOutData;
  int32 currentVelocity;
}gradientDescen;


extern butterworthParameter Butter_80HZ_Parameter_Acce, Butter_60HZ_Parameter_Acce, Butter_51HZ_Parameter_Acce,
                            Butter_30HZ_Parameter_Acce, Butter_20HZ_Parameter_Acce, Butter_15HZ_Parameter_Acce,
                            Butter_10HZ_Parameter_Acce, Butter_05HZ_Parameter_Acce, Butter_02HZ_Parameter_Acce,
                            Butter_01HZ_Parameter_Acce;

extern butterworthData butterworthOfAcceleration;

void ButterworthParameterInit();
float ButtterworthLowPassFiltering(float newInput, butterworthData *aButterworth, butterworthParameter *aParameter);
void SetCutoffFrequency(float sampleFrequent, float cutoffFrequent,butterworthParameter *LowPassFilter);

#endif

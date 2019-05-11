#include "include.h"

#define PI 3.141592653589793f
#define SAMPLEFREQUENCY 300

butterworthParameter Butter_80HZ_Parameter_Acce, Butter_60HZ_Parameter_Acce, Butter_51HZ_Parameter_Acce,
                     Butter_30HZ_Parameter_Acce, Butter_20HZ_Parameter_Acce, Butter_15HZ_Parameter_Acce,
                     Butter_10HZ_Parameter_Acce, Butter_05HZ_Parameter_Acce, Butter_02HZ_Parameter_Acce,
                     Butter_01HZ_Parameter_Acce;

butterworthData butterworthOfAcceleration;


void ButterworthParameterInit(){
  SetCutoffFrequency(SAMPLEFREQUENCY, 80,&Butter_80HZ_Parameter_Acce);
  SetCutoffFrequency(SAMPLEFREQUENCY, 60,&Butter_60HZ_Parameter_Acce);
  SetCutoffFrequency(SAMPLEFREQUENCY, 51,&Butter_51HZ_Parameter_Acce);
  SetCutoffFrequency(SAMPLEFREQUENCY, 30,&Butter_30HZ_Parameter_Acce);
  SetCutoffFrequency(SAMPLEFREQUENCY, 20,&Butter_20HZ_Parameter_Acce);
  SetCutoffFrequency(SAMPLEFREQUENCY, 15,&Butter_15HZ_Parameter_Acce);
  SetCutoffFrequency(SAMPLEFREQUENCY, 10,&Butter_10HZ_Parameter_Acce);
  SetCutoffFrequency(SAMPLEFREQUENCY, 05,&Butter_05HZ_Parameter_Acce);
  SetCutoffFrequency(SAMPLEFREQUENCY, 02,&Butter_02HZ_Parameter_Acce);
  SetCutoffFrequency(SAMPLEFREQUENCY, 01,&Butter_01HZ_Parameter_Acce);
}

//���װ�����˹ ��ͨ�˲���ֹƵ���趨    �ο� �ɶ���Ϣ���̴�ѧ ����WD��      �� CSDN����    https://blog.csdn.net/shengzhadon/article/details/46803401
//https://blog.csdn.net/zhwzhaowei/article/details/71037196

float ButtterworthLowPassFiltering(float newInput, butterworthData *aButterworth, butterworthParameter *aParameter){
  static int LPF_Cnt=0;
  aButterworth->Input_Butter[2] = newInput;
  if(LPF_Cnt>=100){
    //��ͨ�˲�
    aButterworth->Output_Butter[2] = aParameter->b[0] * aButterworth->Input_Butter[2]
                                    +aParameter->b[1] * aButterworth->Input_Butter[1]
                                    +aParameter->b[2] * aButterworth->Input_Butter[0]
                                    -aParameter->a[1] * aButterworth->Output_Butter[1]
                                    -aParameter->a[2] * aButterworth->Output_Butter[0];
  }else{
    //����Դ����
    aButterworth->Output_Butter[2]=aButterworth->Input_Butter[2];
    LPF_Cnt++;
  }

  //���ݸ���
  aButterworth->Input_Butter[0] = aButterworth->Input_Butter[1];
  aButterworth->Input_Butter[1] = aButterworth->Input_Butter[2];

  aButterworth->Output_Butter[0] = aButterworth->Output_Butter[1];
  aButterworth->Output_Butter[1] = aButterworth->Output_Butter[2];
  return aButterworth->Output_Butter[2];
}

//���װ�����˹ ��ͨ�˲���ֹƵ���趨    �ο� �ɶ���Ϣ���̴�ѧ ����WD��      ��    CSDN����  https://blog.csdn.net/sszhouplus/article/details/43113505
void SetCutoffFrequency(float sampleFrequent, float cutoffFrequent,butterworthParameter *LowPassFilter){
    float fr = sampleFrequent / cutoffFrequent;
    float ohm = tanf(PI / fr);
    float c = 1.0f + 2.0f * cosf(PI / 4.0f) * ohm + ohm * ohm;
    if (cutoffFrequent <= 0.0f) {
        // no filtering
        return;
    }
    LowPassFilter->b[0] = ohm * ohm / c;
    LowPassFilter->b[1] = 2.0f * LowPassFilter->b[0];
    LowPassFilter->b[2] = LowPassFilter->b[0];
    LowPassFilter->a[0]=1.0f;
    LowPassFilter->a[1] = 2.0f * (ohm * ohm - 1.0f) / c;
    LowPassFilter->a[2] = (1.0f - 2.0f * cosf(PI / 4.0f) * ohm + ohm * ohm) / c;
}

//float KalmanFiltering(float dataA, float dataB, kalmanParameter *aKalmanParameter){
//
//}

//�ݶ��½��� �ο� �ɶ���Ϣ���̴�ѧ ����WD��      ��    CSDN����  https://blog.csdn.net/nupt123456789/article/details/8281923
//AND https://blog.csdn.net/pengchengliu/article/details/80932232
void GradientDescen(int32 sequence[], u8 lenght, gradientDescen *aGradienDescen, u8 stepNumber, int32 inputData){

}

int LinearRCLowPassFiltering(){

}

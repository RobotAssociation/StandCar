#include "include.h"


int LimitingAmplitude(int value, int min, int max){
  if(value < min){
    value = min;
  }else if(value > max){
    value = max;
  }

  return value;
}

float Average(){

}

float Normalized(int value, int multiple, int min, int max){
  return (value-min)*multiple/(max-min);
}

void Convolution(kernel){

}

#include "GP2Y1010AU0F.h"
#ifndef Arduino_h
#include <Arduino.h>
#endif

//Init GPY2Y1010AU0F
GPY2Y1010AU0F::GPY2Y1010AU0F(int GPY2Y1010AU0F_LED, int GPY2Y1010AU0F_OUTPUT){
  pinMode(GPY2Y1010AU0F_LED,OUTPUT);
  this->GPY2Y1010AU0F_LED = GPY2Y1010AU0F_LED;
  this->GPY2Y1010AU0F_OUTPUT = GPY2Y1010AU0F_OUTPUT;
}

void GPY2Y1010AU0F::SAMPLING_TIMER(){
   digitalWrite(GPY2Y1010AU0F_LED,LOW);//Led on
   delayMicroseconds(SAMPLING_TIME);
   Adc_Read = analogRead(GPY2Y1010AU0F_OUTPUT);
   delayMicroseconds(PULSE_HIGH_TIME-SAMPLING_TIME);
   digitalWrite(GPY2Y1010AU0F_LED,HIGH);//Led off
   delayMicroseconds(PULSE_LOW_TIME);    
}

void GPY2Y1010AU0F::FOMULA_CALCULATOR(){
  SAMPLING_TIMER();
  float Vpd = Vref/1024; //Voltage per Digital Output
  Voltage_Read = Adc_Read * Vpd;
  DustDensity = (6.0/35) *Voltage_Read - 0.1;//(mg/m3)
  
  if(DustDensity <0){
    DustDensity = 0;  
  }
}

float GPY2Y1010AU0F::GET_VOLTAGE(){
    FOMULA_CALCULATOR();
    return Voltage_Read;   
}
float GPY2Y1010AU0F::GET_DUST_DENSITY(){
    FOMULA_CALCULATOR();
    return DustDensity;   
}
float GPY2Y1010AU0F::GET_ADC_READ(){
    FOMULA_CALCULATOR();
    return Adc_Read;   
}

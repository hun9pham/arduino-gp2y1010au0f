#pragma once

#ifndef _GP2Y1010AU0F_LIB_
#define _GP2Y1010AU0F_LIB_

#define SAMPLING_TIME 280
#define PULSE_LOW_TIME 9680 
#define PULSE_HIGH_TIME 320



//Connection
//extern const int GPY2Y1010AU0F_LED = 13; //V_Led
//extern const int GPY2Y1010AU0F_OUTPUT = A0; //Vo

class GPY2Y1010AU0F{
  private:
    int GPY2Y1010AU0F_LED; //Pin SENSOR LED
    int GPY2Y1010AU0F_OUTPUT; //Pin SENSOR OUTPUT   
    float Vref =5.0;//Voltage Reference
    float Adc_Read =0;//Read Analog Output of Sensor
    float Voltage_Read = 0; //Voltage Read from Sensor
    float DustDensity =0; //Density of Dust
    void SAMPLING_TIMER();
    void FOMULA_CALCULATOR();
  public:
    float GET_VOLTAGE();
    float GET_DUST_DENSITY();  
    float GET_ADC_READ();
    GPY2Y1010AU0F(int,int);
    
};

#endif

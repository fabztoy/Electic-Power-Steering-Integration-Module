#include <Arduino.h>
#include "speedometer.h"
#include "eps-parameters.h"

extern EpsConfig epsConfig;

uint8_t speedometer_pwm_pin = GPIO_NUM_32;
uint8_t speedometer_pwm_channel = 1;
uint16_t speedometer_pwm_freq = 50; // 50 Hz for servos
uint8_t speedometer_pwm_resolution = 16;

uint8_t speedometerMotorEscVoltage = 12;

uint16_t speedometerMotorMaxSpeed_rpm;

uint16_t speedometerPwmDutyAt1mS;
uint16_t speedometerPwmDutyAt2mS;


/*

*/
void speedometerInit(){

    // Motor for speedometer

    speedometerMotorMaxSpeed_rpm = 12 * epsConfig.P_speedometerConfig.P_speedometerMotorKV;

    float speedometerPwmCycleTime_ms = 1000 / speedometer_pwm_freq;

    uint16_t speedometerMaximumPwmDuty = 1;

    for (uint8_t bit_num=1;bit_num<speedometer_pwm_resolution;bit_num++) {
        speedometerMaximumPwmDuty<<=1;
        speedometerMaximumPwmDuty++;
    }

    float speedometerMsPerLSB = speedometerPwmCycleTime_ms/speedometerMaximumPwmDuty;

    speedometerPwmDutyAt1mS = static_cast<uint16_t>(1/speedometerMsPerLSB);
    speedometerPwmDutyAt2mS = 2 * speedometerPwmDutyAt1mS;

    // Hardware setup
    ledcSetup(speedometer_pwm_channel, speedometer_pwm_freq, speedometer_pwm_resolution);
    ledcAttachPin(speedometer_pwm_pin, speedometer_pwm_channel);

    speedometerUpdate(0);  

}


/*

*/
void speedometerUpdate(uint8_t speed_kph){

    // Calculate the desired speed for the motor
    uint16_t speedometerMotorSpeed = static_cast<uint16_t>(static_cast<float>(speed_kph) * epsConfig.P_speedometerConfig.P_speedometerSpeedConstant);

    uint16_t duty = ((static_cast<float>(speedometerMotorSpeed)/static_cast<float>(speedometerMotorMaxSpeed_rpm))*speedometerPwmDutyAt1mS)+speedometerPwmDutyAt1mS;

    ledcWrite(speedometer_pwm_channel, duty); 

}
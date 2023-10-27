#include "eps-speed-sensors.h"

#include <Arduino.h>

#include "driver/pcnt.h"

SpeedSensor* SpeedSensor::speedSensor_ = nullptr;

SpeedSensor* instance = nullptr;

portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;


/*

*/
void IRAM_ATTR samplingTimerISR() {
    
    portENTER_CRITICAL_ISR(&timerMux);

    instance->countResetVss();
    instance->countResetTach();

    portEXIT_CRITICAL_ISR(&timerMux);
}


/*

*/
void IRAM_ATTR vss_ISR() {
    portENTER_CRITICAL_ISR(&timerMux);
    instance->incCountVss();
    timerRestart(instance->vssTimeoutTimer);
    portEXIT_CRITICAL_ISR(&timerMux);
}


/*

*/
void IRAM_ATTR tach_ISR() {
    portENTER_CRITICAL_ISR(&timerMux);
    instance->incCountTach();
    timerRestart(instance->tachTimeoutTimer);
    portEXIT_CRITICAL_ISR(&timerMux);
}


/*
 * Static methods should be defined outside the class.
 */
SpeedSensor *SpeedSensor::GetInstance()
{
    /*
     * This is a safer way to create an instance. instance = new Singleton is
     * dangeruous in case two instance threads wants to access at the same time
     */
    if(speedSensor_ == nullptr){
        speedSensor_ = new SpeedSensor();
    }
    return speedSensor_;
}

SpeedSensor::SpeedSensor(){
    instance = this;

    this->countResetVss();
    this->countResetTach();

    //pinMode(GPIO_NUM_0, INPUT);

    // vehicle speed sensor
    attachInterrupt(GPIO_NUM_34, vss_ISR, FALLING);    
    
    // Engine Tachometer
    //attachInterrupt(GPIO_NUM_0, tach_ISR, FALLING);

    this->samplingTimer = timerBegin(0, 80, true);
    timerAttachInterrupt(this->samplingTimer, &samplingTimerISR, true);
    timerAlarmWrite(this->samplingTimer, 250000, true);
    timerAlarmEnable(this->samplingTimer);
}

uint16_t SpeedSensor::getCountVss(){
    return this->m_countVss;
}


void SpeedSensor::incCountVss(){
    this->m_countVss++;
}

void SpeedSensor::countResetVss(){
    this->m_countVss=0;
}

uint16_t SpeedSensor::getCountTach(){
    return this->m_countTach;
}


void SpeedSensor::incCountTach(){
    this->m_countTach++;
}

void SpeedSensor::countResetTach(){
    this->m_countTach=0;
}
#ifndef _EPS_SPEEDSENSOR_H_
#define _EPS_SPEEDSENSOR_H_

#include <Arduino.h>

class SpeedSensor{

    protected:
        SpeedSensor();
        static SpeedSensor* speedSensor_;

    public:
        SpeedSensor(SpeedSensor &other) = delete;
        void operator=(const SpeedSensor &) = delete;
        static SpeedSensor *GetInstance();

        uint16_t getCountVss();
        void incCountVss();
        hw_timer_t * vssTimeoutTimer;

        uint16_t getCountTach();
        void incCountTach();
        hw_timer_t * tachTimeoutTimer;

        hw_timer_t * samplingTimer;

        void countResetVss();
        void countResetTach();
    private:
        

        
        uint16_t m_countVss;
        
        uint16_t m_countTach;

};

#endif
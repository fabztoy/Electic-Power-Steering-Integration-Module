#ifndef _EPS_PARAMETERS_H_
#define _EPS_PARAMETERS_H_

#include <Arduino.h>


struct EpsTyreConfig {
    uint16_t P_tyreWidth;   // Tyre Width in mm
    uint8_t P_tyreAR;       // Tyre aspect Ratio in %
    uint8_t P_wheelSize_in; // Wheel size in Inches
};


struct EpsSpeedometerConfig {
    float P_speedometerSpeedConstant;           // speedometer speed constant in rpm/kph
    uint16_t P_speedometerMotorStepsPerRev;     // motor steps per revolution (360/StepAngle)
    uint16_t P_speedometerMotorCurrentRating;   // motor current rating in mA
    uint16_t P_speedometerMotorGearboxRatio;    // motor gear ratio in %
};


struct EpsSpeedLookupConfig {
    uint8_t P_speedLookupPt1_X;
    uint8_t P_speedLookupPt1_Y;
    uint8_t P_speedLookupPt2_X;
    uint8_t P_speedLookupPt2_Y;
    uint8_t P_speedLookupPt3_X;
    uint8_t P_speedLookupPt3_Y;
    uint8_t P_speedLookupPt4_X;
    uint8_t P_speedLookupPt4_Y;
    uint8_t P_speedLookupPt5_X;
    uint8_t P_speedLookupPt5_Y;
};


struct EpsConfig {
    EpsTyreConfig P_tyreConfig;
    uint8_t P_vssPulsesPerRev;  // vss pulses per revolutions
    uint8_t P_diffToVssRatio;   // vss to differential ratio in %
    EpsSpeedometerConfig P_speedometerConfig;
    EpsSpeedLookupConfig P_speedLookupConfig;
};


void epsParametersInit();

void printEpsConfigFile();
void deleteEpsConfigFile();
void updateEpsParameter(String pname, String pvalue);

#endif /* _EPS_PARAMETERS_H_ */
#include "eps-math.h"

#include <Arduino.h>

uint16_t tyreCircumference(uint16_t tyreWidth, uint8_t tyreAr, uint8_t wheelSize_in){

    float wheelSize = static_cast<float>(wheelSize_in) * 25.4;  // Convert inches to mm

    float tyreSidewallHeight = (tyreWidth * tyreAr) / 100.0;    // Calculate tyre sidewall height

    float tyreDiameter = wheelSize + 2 * tyreSidewallHeight;    // Calculate tyre diameter

    return static_cast<uint16_t>(PI * tyreDiameter);            // Return circumference

}
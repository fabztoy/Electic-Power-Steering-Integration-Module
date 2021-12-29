#include <Arduino.h>

#include "eps-cli.h"

void setup()
{
    setupEpsCli();
}

void loop()
{
    static uint64_t timeStamp = 0;

    manageEpsCli();
       
    if (millis() - timeStamp > 1000) {

        timeStamp = millis();
    }
}
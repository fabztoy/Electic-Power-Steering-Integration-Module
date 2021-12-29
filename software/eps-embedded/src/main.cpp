#include <Arduino.h>

#include "eps-cli.h"
#include "eps-parameters.h"


/*

*/
void setup()
{
    epsParametersInit();
    setupEpsCli();
}


/*

*/
void loop()
{
    static uint64_t timeStamp = 0;

    manageEpsCli();
       
    if (millis() - timeStamp > 1000) {

        timeStamp = millis();
    }
}
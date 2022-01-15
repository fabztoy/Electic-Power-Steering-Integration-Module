#include <Arduino.h>

#include "eps-cli.h"
#include "eps-can.h"
#include "eps-parameters.h"

#include "speedometer.h"

/*

*/
void setup()
{
    epsParametersInit();
    setupEpsCli();

    epsCanInit();
    speedometerInit();

    speedometerUpdate(50); // Set to 50km/h (530.8rpm) just for testing
}


/*

*/
void loop()
{
    static uint64_t timeStamp = 0;

    manageEpsCli();

    // Manage all inputs
    epsCanRxManage();
       
    if (millis() - timeStamp > 1000) {

        timeStamp = millis();
    }

    // Manage all outputs
    epsCanTxManage();

}
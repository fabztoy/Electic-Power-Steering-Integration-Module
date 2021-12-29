#include <Arduino.h>

void setup()
{
    Serial.begin(115200);
    Serial.println("Start");
    Serial.setTimeout(1);
    Serial.printf("> ");
}

void loop()
{
    static uint64_t timeStamp = 0;
       
    if (millis() - timeStamp > 1000) {

        timeStamp = millis();
    }
}
#include <Arduino.h>

#include "eps-cli.h"
#include "eps-can.h"
#include "eps-parameters.h"
#include "eps-logic.h"

#include "eps-speed-sensors.h"

#include "speedometer.h"
#include "frequencyInput.h"


SpeedSensor* vehicleSpeedSensor;

uint8_t ledPin = GPIO_NUM_2;

uint16_t io_freq_vss=0;

TaskHandle_t frequencyInputTaskHandle = NULL;

/*

*/
void setup()
{
    epsParametersInit();
    epsCliInit();
    //vehicleSpeedSensor = SpeedSensor::GetInstance();

    xTaskCreate(frequencyInputTask, "frequencyInputTask", 4096, NULL, 10, &frequencyInputTaskHandle);
    epsCanInit();

    // Yaris original Tyre size = 175/65R14


    pinMode(ledPin, OUTPUT);


    uint8_t pwm_pin = GPIO_NUM_27;
    uint16_t freq = 50; // 100 Hz
    uint16_t duty = 4095; // 2^13 / 2 = 4095 -> 50%

    // Set to a specific Frequency
    ledcAttachPin(pwm_pin,0);
    ledcWriteTone(0, freq);
    ledcWrite(0, duty);

    // Set Off
    //ledcDetachPin(pwm_pin);
    //digitalWrite (pwm_pin, LOW)

    speedometerInit();

    speedometerUpdate(50); // Set to 50km/h (530.8rpm) just for testing

}


/*

*/
void loop()
{
    static uint64_t timeStamp = 0;
    static uint64_t timeStamp_250 = 0;
    static uint64_t timestamp_10 = 0;

    epsCliManage();

    // Manage all inputs
    epsCanRxManage();

    epsMainLogic();
    
    if(millis() - timestamp_10 > 10){
        extern xQueueHandle freq_input_34_value_queue;
        if (uxQueueMessagesWaiting(freq_input_34_value_queue)){
            xQueueReceive(freq_input_34_value_queue, &io_freq_vss, portMAX_DELAY);
        }

    }

    if (millis() - timeStamp_250 > 250) {

        timeStamp_250 = millis();
        
        digitalWrite(2, !digitalRead(2));
        //Serial.println(vehicleSpeedSensor->getCount());
    }

    if (millis() - timeStamp > 250) {

        timeStamp = millis();
        Serial.println(io_freq_vss);
    }

    // Manage all outputs
    //epsCanTxManage();

}
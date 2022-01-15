#include <Arduino.h>

#include <ESP32CAN.h>
#include <CAN_config.h>

CAN_device_t CAN_cfg;               // CAN Config
const int rx_queue_size = 100;       // Receive Queue size


/*

*/
void epsCanInit(){
    CAN_cfg.speed = CAN_SPEED_500KBPS;
    CAN_cfg.tx_pin_id = GPIO_NUM_17;
    CAN_cfg.rx_pin_id = GPIO_NUM_16;
    CAN_cfg.rx_queue = xQueueCreate(rx_queue_size, sizeof(CAN_frame_t));
    // Init CAN Module
    ESP32Can.CANInit();
}


/*

*/
void epsCanRxManage(){
    CAN_frame_t rx_frame;

    // Receive next CAN frame from queue
    while(xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 0) == pdTRUE){

        if (rx_frame.FIR.B.FF == CAN_frame_std){
            Serial.printf("New standard frame");
        }else{
            Serial.printf("New extended frame");
        }

        if (rx_frame.FIR.B.RTR == CAN_RTR){
            Serial.printf(" RTR from 0x%08X, DLC %d\r\n", rx_frame.MsgID,  rx_frame.FIR.B.DLC);
        }else{
            Serial.printf(" from 0x%08X, DLC %d, Data ", rx_frame.MsgID,  rx_frame.FIR.B.DLC);
            for (int i = 0; i < rx_frame.FIR.B.DLC; i++) {
                Serial.printf("0x%02X ", rx_frame.data.u8[i]);
            }
            Serial.printf("\n");
        }
    }
}


/*

*/
void epsCanTxManage(){

    static unsigned long previousMillis = 0;   // will store last time a CAN Message was send
    unsigned long currentMillis = millis();

    // Send CAN Message
    if (currentMillis - previousMillis >= 1000) {
        previousMillis = currentMillis;
        CAN_frame_t tx_frame;
        tx_frame.FIR.B.FF = CAN_frame_ext;
        tx_frame.MsgID = 0x18FF01E7;
        tx_frame.FIR.B.DLC = 8;
        tx_frame.data.u8[0] = 0x00;
        tx_frame.data.u8[1] = 0x01;
        tx_frame.data.u8[2] = 0x02;
        tx_frame.data.u8[3] = 0x03;
        tx_frame.data.u8[4] = 0x04;
        tx_frame.data.u8[5] = 0x05;
        tx_frame.data.u8[6] = 0x06;
        tx_frame.data.u8[7] = 0x07;
        ESP32Can.CANWriteFrame(&tx_frame);
    }
}
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
            Serial.printf("std frame");
        }else{
            Serial.printf("extended frame");
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
    static unsigned long previousFor10ms = 0;    // Last time 10mS CAN Messages were sent
    static unsigned long previousFor20ms = 0;    // Last time 20mS CAN Messages were sent
    static unsigned long previousFor500ms = 0;   // Last time 500mS CAN Messages were sent

    unsigned long currentMillis = millis();

    // Send 10mS CAN Message
    // 0x0B0 and 0x0B2
    if (currentMillis - previousFor10ms >= 10) {
        previousFor10ms = currentMillis;
        CAN_frame_t tx_frame;

        // 0x0B0
        // [0] and [1] give a wheel speed
        // [2] and [3] give a wheel speed
        tx_frame.FIR.B.FF = CAN_frame_std;
        tx_frame.MsgID = 0x0B0;
        tx_frame.FIR.B.DLC = 6;
        tx_frame.data.u8[0] = 0x00; // wheelSpeed1[0]
        tx_frame.data.u8[1] = 0x0B; // wheelSpeed1[1]
        tx_frame.data.u8[2] = 0x00; // wheelSpeed2[0]
        tx_frame.data.u8[3] = 0x01; // wheelSpeed2[1]
        tx_frame.data.u8[4] = 0x00; // We need to find what to send
        tx_frame.data.u8[5] = 0x0B; // We need to find what to send
        tx_frame.data.u8[6] = 0xFF; // Not sent DLC = 6
        tx_frame.data.u8[7] = 0xFF; // Not sent DLC = 6
        ESP32Can.CANWriteFrame(&tx_frame);

        // 0x0B2
        // [0] and [1] give a wheel speed
        // [2] and [3] give a wheel speed
        tx_frame.FIR.B.FF = CAN_frame_std;
        tx_frame.MsgID = 0x0B2;
        tx_frame.FIR.B.DLC = 6;
        tx_frame.data.u8[0] = 0x00; // wheelSpeed3[0]
        tx_frame.data.u8[1] = 0x0B; // wheelSpeed3[1]
        tx_frame.data.u8[2] = 0x02; // wheelSpeed4[0]
        tx_frame.data.u8[3] = 0x01; // wheelSpeed4[1]
        tx_frame.data.u8[4] = 0x00; // We need to find what to send
        tx_frame.data.u8[5] = 0x0B; // We need to find what to send
        tx_frame.data.u8[6] = 0xFF; // Not sent DLC = 6
        tx_frame.data.u8[7] = 0xFF; // Not sent DLC = 6
        ESP32Can.CANWriteFrame(&tx_frame);

    }

    // Send 20mS CAN Message
    // 0x2C4
    if (currentMillis - previousFor20ms >= 20) {
        previousFor20ms = currentMillis;
        CAN_frame_t tx_frame;

        // 0x2C4, Engine Speed on [0] and [1]
        tx_frame.FIR.B.FF = CAN_frame_std;
        tx_frame.MsgID = 0x2C4;
        tx_frame.FIR.B.DLC = 8;
        tx_frame.data.u8[0] = 0x00; // engineSpeed[0]
        tx_frame.data.u8[1] = 0x01; // engineSpeed[1]
        tx_frame.data.u8[2] = 0x02; // We need to find what to send
        tx_frame.data.u8[3] = 0x03; // We need to find what to send
        tx_frame.data.u8[4] = 0x04; // We need to find what to send
        tx_frame.data.u8[5] = 0x05; // We need to find what to send
        tx_frame.data.u8[6] = 0x06; // We need to find what to send
        tx_frame.data.u8[7] = 0x07; // We need to find what to send
        ESP32Can.CANWriteFrame(&tx_frame);
    }

    // Send 500mS CAN Message
    // 0x610
    if (currentMillis - previousFor500ms >= 500) {
        previousFor500ms = currentMillis;
        CAN_frame_t tx_frame;

        // 0x610, Vehicle speed
        // [2] is vehicle speed (raw kph). Static bits on [3] and [4]
        tx_frame.FIR.B.FF = CAN_frame_std;
        tx_frame.MsgID = 0x610;
        tx_frame.FIR.B.DLC = 8;
        tx_frame.data.u8[0] = 0x00;
        tx_frame.data.u8[1] = 0x01;
        tx_frame.data.u8[2] = 0x02; // wheelBasedVehicleSpeed
        tx_frame.data.u8[3] = 0x03; // Static Data
        tx_frame.data.u8[4] = 0x04; // Static Data
        tx_frame.data.u8[5] = 0x05;
        tx_frame.data.u8[6] = 0x06;
        tx_frame.data.u8[7] = 0x07;
        ESP32Can.CANWriteFrame(&tx_frame);
    }
}

/*



*/
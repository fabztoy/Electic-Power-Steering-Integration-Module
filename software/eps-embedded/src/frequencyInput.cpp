

/*------------------------------------------------
 * Includes
------------------------------------------------*/
#include "frequencyInput.h"

#include <Arduino.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/periph_ctrl.h"

#include "driver/gpio.h"
#include "esp_attr.h"
#include "esp_log.h"


/*------------------------------------------------
 * Private Structs
------------------------------------------------*/

/* A sample structure to pass events from the
 * interrupt handler to the main program.
 */
typedef struct {
    uint8_t pin;
    uint32_t status; // information on the event type that caused the interrupt
    unsigned long timeStamp; // The time the event occured
} frequency_input_evt_t;


/*------------------------------------------------
 * Private declarations and defines
------------------------------------------------*/
xQueueHandle freq_input_34_evt_queue;   // A queue to handle pulse counter events
xQueueHandle freq_input_34_value_queue;   // A queue to handle pulse counter events

#define FREQ_INPUT_34_PULSES_PER_REV 2


/*------------------------------------------------
 * Private functions declarations
------------------------------------------------*/
static void IRAM_ATTR frequency_INPUT_34_intr();
int countRPM(int firstTime, int lastTime, int pulsePerRev);


/*------------------------------------------------
 * Private functions definitions
------------------------------------------------*/

/* frequency_INPUT_34_intr
 * Interrupt on pin 34
*/
static void IRAM_ATTR frequency_INPUT_34_intr(){
    unsigned long currentMillis = millis(); //Time at instant ISR was called

    //toggle != toggle;
  if(digitalRead(34) == HIGH){

    frequency_input_evt_t evt;
    portBASE_TYPE HPTaskAwoken = pdFALSE;

    evt.status = 2;
    evt.timeStamp = currentMillis; 


    xQueueSendFromISR(freq_input_34_evt_queue, &evt, &HPTaskAwoken);
    if (HPTaskAwoken == pdTRUE) {
        portYIELD_FROM_ISR();
    }
  }
    
}


/* countRPM
 * takes first timestamp and last timestamp and pulses per revolution 
 * Calculates RPM
 */
int countRPM(int firstTime, int lastTime, int pulsePerRev) {
  int timeDelta = (lastTime - firstTime); //lastTime - firstTime
  if (timeDelta <= 0){ // This means we've gotten something wrong
    return 0;
  }
  return ((60000 / pulsePerRev) / timeDelta);
}


/*------------------------------------------------
 * Public functions definitions
------------------------------------------------*/

/* frequencyInputTask
 * Task responsible for reading frequency inputs.
 * Needs to be started as a freertos task.
*/
void frequencyInputTask(void * arg)
{
  int lastStamp_34 = 0; //for previous time stamp for pin 47

  pinMode(34, INPUT); // io_freq_dut_tap_speed
  
  /* Initialize PCNT event queue and PCNT functions */
  freq_input_34_evt_queue = xQueueCreate(10, sizeof(frequency_input_evt_t));

  freq_input_34_value_queue = xQueueCreate(1, sizeof(int));

  attachInterrupt(34, frequency_INPUT_34_intr, CHANGE);

  int frequency_INPUT_34 = 0; // Fan 0 RPM

  frequency_input_evt_t evt_34;
  portBASE_TYPE res_34;

  for(;;){

    /* Wait for the event information passed from interrupt handler.
    * Once received, decode the event type and print it on the serial monitor.
    */
    res_34 = xQueueReceive(freq_input_34_evt_queue, &evt_34, 1000 / portTICK_PERIOD_MS);
    if (res_34 == pdTRUE) {

      if (evt_34.status==2){
        // status 2, pin interrupt.      


        if (lastStamp_34 == 0){
                lastStamp_34 = evt_34.timeStamp;
        }

        frequency_INPUT_34 = countRPM(lastStamp_34, evt_34.timeStamp, FREQ_INPUT_34_PULSES_PER_REV);
        if (frequency_INPUT_34 == -1){
            frequency_INPUT_34 = 0;
            continue;
        }
        lastStamp_34 = evt_34.timeStamp;

      }
    }else{
        /* No interrupt happened since the last check, need to check if there is a timeout.
        */
        unsigned long currentMillis = millis();

        // Timeout on 300 mS equivalent to 3.33Hz(signal) 1.66Hz(shaft) / 100 RPM
        if((lastStamp_34 + 300) < currentMillis){
            frequency_INPUT_34=0;
            lastStamp_34 = currentMillis;
        }
    }
  

    // send rpm value to other task using a queue.
    xQueueOverwrite(freq_input_34_value_queue, &frequency_INPUT_34);

  }
  
  /* Should never get there */
  detachInterrupt(34);

}

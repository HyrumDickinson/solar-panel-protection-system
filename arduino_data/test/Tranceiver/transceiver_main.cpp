// https://github.com/RalphBacon/nRF24L01-transceiver/blob/master/nRF24L01_Test_Node_1/nRF24L01_Test_Node_1.ino
#include <SPI.h>
#include <ArduinoJson.h>
#include "RF24.h"
#include "nRF24L01.h"

RF24 radio(7, 8);

byte pipeAddresses[][6] = {"1Node", "2Node"};  // there can only be 1 writing port at any given time, but there can be 5 reading ports
StaticJsonDocument<200> mainOut;                      // JSON from main should only contain a few variables, such as a shutdown boolean. Making it JSON for future expandability
/*
{
  "node": <node number>
  "shutdown": <boolean>
}
*/
StaticJsonDocument<200> data;                         // data received from the node

void setup() {
  Serial.begin(9600);
  while(!Serial) {}
  Serial.println("This is the main Tranceiver code");

  radio.begin();
  radio.setPALevel(RF24_PA_MIN);  // power level of the tranceiver...set to min to work with power from arduino
  radio.setDataRate(RF24_2MBPS);  // transfer data rate. 2 is fastest
  radio.setChannel(124);          // 0-127...124 is least likely frequency to be used by wifi/other stuff

  radio.openWritingPipe(pipeAddresses[0]);      // pipe/port 0 is always used by the writing pipe (only one) so we don't specify with a number
  radio.openReadingPipe(1, pipeAddresses[1]);   // the pipe number and the address of that pipe

}

void loop() {
  // Loops through all 60 panels (or more) to read
  for (int i = 0; i < 60; i++) {
    radio.stopListening();    // ALWAYS make sure we're not listening if we want to transmit
    
    mainOut["nodeNumber"] = i;
    //TODO: add something that changes the shutdown boolean
    
    if (!radio.write(&mainOut, 200)) {    //change data to JSON
      Serial.println("No acknowledgement of transmission. Check connection on other end"); // it didn't get anywhere
    }


    // Doesn't wait forever for a response. It times out after 1 second (arbitrary number)
    //TODO: just continue on to the next panel and note a failed response
    radio.startListening();   // listen for a response

    unsigned long long started_waiting_at = millis();

    if (millis() - started_waiting_at > 1000) {
      Serial.println("No response received - timeout!");
      return;                 // returns to end...instead just continue with code
    }

    radio.read(&data, 200);

    delay(1000);  // wait for 1 second
  }
}
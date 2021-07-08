// https://github.com/RalphBacon/nRF24L01-transceiver/blob/master/nRF24L01_Test_Node_1/nRF24L01_Test_Node_1.ino

#include <SPI.h>
#include "RF24.h"
#include "nRF24L01.h"
#include <OneWire.h>
#include <DallasTemperature.h>


/* CONSTANTS */
#define nodeNumber 0
#define ONE_WIRE_BUS 8
#define NUM_TEMP_SENSORS 6      //report calls for 6 temp sensors. If you use more/less, you only need to change it here

/* COMMUNICATION SETUP */
byte pipeAddresses[][6] = {"1Node", "2Node"};  // there can only be 1 writing port at any given time, but there can be 5 reading ports

/* JSON INSTANTIATION */
StaticJsonDocument<200> mainIn;                             /*{"node": <node number>, "shutdown": <boolean>}*/
StaticJsonDocument<200> data;                               // data received from the node
int T1 = data["T1"];
int T2 = data["T2"];
int T3 = data["T3"];
int T4 = data["T4"];
int T5 = data["T5"];
int T6 = data["T6"];
int V1 = data["V1"];
int V2 = data["V2"];
int V3 = data["V3"];
int C1 = data["C1"];
bool OVERHEAT = data["OVERHEAT"];
bool OVERVOLT = data["OVERVOLT"];
bool OVERCURR = data["OVERCURRENT"];

/* TEMPERATURE SETUP */
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress tempDeviceAddress;
float tempArray[NUM_TEMP_SENSORS];
int TEMP_THRESH = 90;   //! not making constant (#define). When in overheat, thresh is lower number. Allows for autonomous restart. Consult Kevin about this. number is arbitrary rn
bool overheat = false;
int tempThresholdsBroken; /* Reads how many temp sensors have detected an exceeded temp thresh (don't want a faulty sensor setting it off) 
- Lab report displays 6 temp sensors. Shut down at 3 or 4 */

/* SETUP TEMP SENSOR(s) */
void tempSensorSetup() {
  sensors.begin();

  //print the address of each sensor
  for (int i = 0; i < NUM_TEMP_SENSORS; i++) {
    //searches for data wires. Stores device addresses in "tempDeviceAddress" variable
    if (sensors.getAddress(tempDeviceAddress, i)) {
      Serial.print("Address ");
      Serial.print(i);
      Serial.print(": ");
      printAddress(tempDeviceAddress);
      Serial.println();

    } else {
      Serial.print("Could not find address for device at ");
      Serial.println(i);
    }
  }
}

void setup() {
    Serial.begin(9600);
    while(!Serial) {}
    Serial.println("This is the Node Tranceiver code");

    radio.begin();
    radio.setPALevel(RF24_PA_MIN);  // power level of the tranceiver...set to min to work with power from arduino
    radio.setDataRate(RF24_2MBPS);  // transfer data rate. 2 is fastest
    radio.setChannel(124);          // 0-127...124 is least likely frequency to be used by wifi/other stuff

    radio.openWritingPipe(pipeAddresses[0]);      // pipe/port 0 is always used by the writing pipe (only one) so we don't specify with a number
    radio.openReadingPipe(1, pipeAddresses[1]);   // the pipe number and the address of that pipe

}

void loop() {
    // start listening for the next command again
    radio.startListening();
    //TODO: remove this loop if we want autonomy w/o connection
    
    while (!radio.available()) {}

    radio.read(&mainIn, 200);
    radio.stopListening();
    if (mainIn["node"] == nodeNumber) {
        if (mainIn["shutdown"] == true) {
            //shutdown
        }

        if (!radio.write( &data, sizeof(unsigned char) )) {
            Serial.println("No acknowledgement of transmission - receiving radio device connected?");    
        }

    }
}

/* READS TEMPERATURE */
void readTemperatures() {
    //TODO: Convert to JSON (possibly continue as is, just write array values into JSON in loop)
    sensors.requestTemperatures(); // reads temps from all sensors
    tempThresholdsBroken = 0;   //! Resets to 0 to allow for overheat setting to change. May modify later
        
    // loop through each device & assign temperature value to "tempArray" array
    for (int i = 0; i < NUM_TEMP_SENSORS; i++) {
        if (sensors.getAddress(tempDeviceAddress, i)) {
            tempArray[i] = sensors.getTempF(tempDeviceAddress);  // reads temperature in degrees farenheit
            if (tempArray[i] >= TEMP_THRESH) {
                tempThresholdsBroken += 1;
            }
        }
    }
    if (tempThresholdsBroken >= 3) {    /* After initial overheat, other sensors may trigger as overheating due to TEMP_THRESH change. This is expected */
        overheat = true;
        TEMP_THRESH = 85;
    } else {     // does not wait for entire system cooldown. Again, going back to 1 or 2 faulty sensors ruining everything... this was an issue found in testing
        //TODO: find a way to get around faulty temperature sensors. Testing found that resetting the system worked...Maybe re-run tempSensorSetup occasionally
        overheat = false;
        TEMP_THRESH = 90;
    }

    jsonObject["T1"] = tempArray[0];
    jsonObject["T2"] = tempArray[1];
    jsonObject["T3"] = tempArray[2];
    jsonObject["T4"] = tempArray[3];
    jsonObject["T5"] = tempArray[4];
    jsonObject["T6"] = tempArray[5];
    jsonObject["OVERHEAT"] = overheat;
}
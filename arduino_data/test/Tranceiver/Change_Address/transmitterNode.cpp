/* LIBRARIES */
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <OneWire.h>
#include <DallasTemperature.h>

/* CONSTANTS */
#define ONE_WIRE_BUS 9
#define NUM_TEMP_SENSORS 3
#define NODENUMBER 3

/* TEMP SENSOR VARIABLES */
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress tempDeviceAddress;
float tempArray[NUM_TEMP_SENSORS];
int TEMP_THRESH = 90;   //! not making constant (#define). When in overheat, thresh is lower number. Allows for autonomous restart. Consult Kevin about this. number is arbitrary rn
bool overheat = false;
int tempThresholdsBroken; /* Reads how many temp sensors have detected an exceeded temp thresh (don't want a faulty sensor setting it off) 
- Lab report displays 6 temp sensors. Shut down at 3 or 4 */

/* TRANSCEIVER VARIABLES */
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00003";

struct Data_Package {
  int module = NODENUMBER;
  float T1 = 0.0;
  float T2 = 0.0;
  float T3 = 0.0;
  float T4 = 0.0;
  float T5 = 0.0;
  float T6 = 0.0;
  bool overTemp = false;
};
Data_Package data;

struct sendMessage {
    int node;
};
sendMessage message;


/*----------------SETUP----------------*/
void setup() {
    Serial.begin(9600);

    tempSensorSetup();
    radioSetup();
}


/*----------------TRANSCEIVER SETUP----------------*/
void radioSetup() {
    radio.begin();
    radio.openWritingPipe(address);
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_MIN);
    radio.stopListening();
}


/*----------------TEMP SENSOR SETUP----------------*/
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

/* PRINT THE HEX ADDRESS FOR TEMPERATURE SENSORS */
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) 
      Serial.print("0");
      Serial.print(deviceAddress[i], HEX);   
    }
}

/* READS TEMPERATURE */
void readTemperatures() {
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

    data.T1 = tempArray[0];
    data.T2 = tempArray[1];
    data.T3 = tempArray[2];
    data.T4 = tempArray[3];
    data.T5 = tempArray[4];
    data.T6 = tempArray[5];
    data.overTemp = overheat;
}


/*----------------LOOP----------------*/
void loop() {
    readTemperatures();

    if (overheat) { //calls cooldown function if more than 3 sensors detect an overheat
        Serial.println();
        Serial.println("************SYSTEM OVERHEAT************");
        Serial.println("3 or more temp sensors have exceeded the temperature threshold...waiting for cooldown");
        Serial.println("***************************************");
        //TODO: add some function to shut things down when in overheat
    }
    for (int i = 0; i < NUM_TEMP_SENSORS; i++) {
        if (tempArray[i] > TEMP_THRESH) {
            Serial.print("TEMP THRESH EXCEEDED AT SENSOR: ");
        } else {
            Serial.print("Temperature at sensor: ");
        }
        Serial.println(i);
        Serial.print("Farenheit: ");
        Serial.println(tempArray[i]);
    }

    Serial.println("-----------------------");
    
    radio.stopListening();
    radio.write(&data, sizeof(data));
    delay(500);
}
// IMPORTANT: this code works best with the MEGA as the transmitter and the UNO as the receiver
//https://howtomechatronics.com/projects/diy-arduino-rc-transmitter/
/* Testng has shown that sending data as a struct is the best method of transferring data.
    Sending a Char array or json string has proven to be too big, and adding more than 4 
    variables results in a garbled mess in said variables*/

/* LIBRARIES */
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <OneWire.h>
#include <DallasTemperature.h>

/* TEMPERATURE SETUP */
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress tempDeviceAddress;
float tempArray[NUM_TEMP_SENSORS];
int TEMP_THRESH = 90;   //! not making constant (#define). When in overheat, thresh is lower number. Allows for autonomous restart. Consult Kevin about this. number is arbitrary rn
bool overheat = false;
int tempThresholdsBroken; /* Reads how many temp sensors have detected an exceeded temp thresh (don't want a faulty sensor setting it off) 
- Lab report displays 6 temp sensors. Shut down at 3 or 4 */

/* TRANSCEIVER SETUP */
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
struct Data_Package {
  float temp1 = 12.7;
  float temp2 = 95.5;
  float temp3 = 112.2;
  float temp4 = 123.4;
  float temp5 = 115.3;
  float temp6 = 15.4;
  bool ok = true;
};
Data_Package data;

void setup() {
    Serial.begin(9600);

    radioSetup();
    tempSensorSetup();
}


void loop() {
    radio.write(&data, sizeof(data));
    delay(500);
}


/* SETUP TRANSCEIVER */
radioSetup() {
    radio.begin();
    radio.openWritingPipe(address);
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_MIN);
    radio.stopListening();
}

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

    data.temp1 = tempArray[0];
    data.temp2 = tempArray[1];
    data.temp3 = tempArray[2];
    data.temp4 = tempArray[3];
    data.temp5 = tempArray[4];
    data.temp6 = tempArray[5];
    data.ok = false;
}
/* LIBRARIES */
#include <OneWire.h>
#include <DallasTemperature.h>

/* CONSTANTS */
#define ONE_WIRE_BUS 9
#define NUM_TEMP_SENSORS 3

/* GENERAL VARIABLES */
int TEMP_THRESH = 90;       // When in overheat, thresh is lower number. Allows for autonomous restart. Val is arbitrary
int tempThresholdsBroken;   // Reads how many temp sensors have detected an exceeded temp thresh
bool overheat = false;

/* TEMP SENSOR VARIABLES */
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress tempDeviceAddress;
float tempArray[NUM_TEMP_SENSORS];
struct Data_Package {
  float T1 = 0.0;
  float T2 = 0.0;
  float T3 = 0.0;
  float T4 = 0.0;
  float T5 = 0.0;
  float T6 = 0.0;
};
Data_Package data;


/* Function: setup
 *    Initialises the system wide configuration and settings prior to start
*/
void setup() {
  Serial.begin(9600);
  while(!Serial) {}

  tempSensorSetup();
}


/* Function: tempSensorSetup
* initializes the temperature sensors (quantitiy determined by NUM_TEMP_SENSORS) and calls 'printTempSensorAddresses'
*/
void tempSensorSetup() {
  sensors.begin();

  //print the address of each sensor
  for (int i = 0; i < NUM_TEMP_SENSORS; i++) {
    //searches for data wires. Stores device addresses in "tempDeviceAddress" variable
    if (sensors.getAddress(tempDeviceAddress, i)) {
      Serial.print("Address ");
      Serial.print(i);
      Serial.print(": ");
      printTempSensorAddresses(tempDeviceAddress);
      Serial.println();

    } else {
      Serial.print("Could not find address for device at ");
      Serial.println(i);
    }
  }
}


/* Function: printTempSensorAddresses
* Prints the addresses of each detected temperature device for debugging purposes
*/
void printTempSensorAddresses(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) 
      Serial.print("0");
      Serial.print(deviceAddress[i], HEX);   
    }
}

void loop() {
    readTemperatures();
    Serial.println(data.T1);
    Serial.println(data.T2);
    Serial.println(data.T3);
}

/* Function: readTemperatures
* Cycles through each temp sensor and saves the readings to the 'data' struct. Also takes note of 
* an overheat (currently waits for 2 sensors to overheat. This number is arbitrary)
* - may make separate function for overheat readings (for simplification)
*/
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
    if (tempThresholdsBroken >= 2) {    /* After initial overheat, other sensors may trigger as overheating due to TEMP_THRESH change. This is expected */
        overheat = true;
        TEMP_THRESH = 85;
    } else {     // does not wait for entire system cooldown. Again, going back to 1 or 2 faulty sensors ruining everything... this was an issue found in testing
        //TODO: find a way to get around faulty temperature sensors. Testing found that resetting the system worked...Maybe re-run tempSensorSetup occasionally
        overheat = false;
        TEMP_THRESH = 90;
    }

    data.T1 = tempArray[0];   // updating readings in our data struct
    data.T2 = tempArray[1];
    data.T3 = tempArray[2];
    // data.overTemp = overheat;
}
/* Libararies */
#include <Arduino.h>
#include <SPI.h>
#include <Arduino_JSON.h>
#include <Ethernet.h>
#include <OneWire.h>
#include <DallasTemperature.h>

/* CONSTANTS */
#define ONE_WIRE_BUS 8
//TODO: find realistic values for these temperatures
#define TEMP_THRESH 85 // this threshold is an arbitrary number used for testing. Not actual field-use temp
#define TEMP_RESET 85  // system must cool down to this temp to resume functions. (currently arbitrary for testing)

/* TEMPERATURE SETUP */
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress tempDeviceAddress;
int numberOfDevices;
float temperatureReads[6];     //report calls for 6 temp sensors. If you use more/less, you only need to change it here
bool overheat = false;
int tempThresholdsBroken; /* Reads how many temp sensors have detected an exceeded temp thresh (don't want a faulty sensor setting it off) 
- Lab report displays 6 temp sensors. Shut down at 3 or 4 */

/* ETHERNET SETUP */
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x04};
IPAddress ip(128, 174, 186, 99);
IPAddress gateway(128, 174, 186, 1);
IPAddress subnet(255, 255, 254, 0);
EthernetServer server(23); // default is 23
boolean alreadyConnected = false;


void setup() {
  Serial.begin(9600);
  tempSensorSetup();
  ethernetSetup();
}


/* SETUP ETHERNET 
  By calling ethernet.begin(mac), it first tries to make a unique ip address. If that fails, it uses the fallback ip address (made above).
  begin(mac) keeps failing. fallback is always used for some reason. IT may give IP's
*/
void ethernetSetup() {
  Serial.println("Trying to get an IP address using DHCP");
  //! This way for sake of testing... delete next line and uncomment 'if' statement
  Ethernet.begin(mac, ip, gateway, subnet);
//  if (Ethernet.begin(mac) == 0) {
//    Serial.println("Failed to configure Ethernet using DHCP");
////    initialize the ethernet device not using DHCP:
//    Ethernet.begin(mac, ip, gateway, subnet);
//  }
  ip = Ethernet.localIP();
  //checking valid ip (not 0.0.0.0)
  if (ip[0] == 0 && ip[1] == 0 && ip[2] == 0 && ip[3] == 0) {
    Serial.print(Ethernet.localIP());
    Serial.println(" is not a valid IP Address. No connection made.");
    Serial.println("Stopping Program................");
    while (true) {}
  }
  // print your local IP address:
  Serial.print("My IP address: ");
  Serial.print(Ethernet.localIP());
  Serial.println();
  server.begin();
}


/* SETUP TEMP SENSOR(s) */
void tempSensorSetup() {
  sensors.begin();
  
  numberOfDevices = sensors.getDeviceCount();
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices found");

  //print the address of each sensor
  for (int i = 0; i < numberOfDevices; i++) {
    //searches for data wires. Stores device addresses in "tempDeviceAddress" variable
    if (sensors.getAddress(tempDeviceAddress, i)) {
      Serial.print("Address ");
      Serial.print(i);
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












//TODO: convert loop and other functinos to use/export JSON. Also listen for SHUTDOWN command
void loop() {
  // wait for a new client:
  EthernetClient client = server.available();

  if (client) { //makes sure client is connected
    readTemperatures();

    for (int i = 0; i < 6; i++) {
        if (temperatureReads[i] > TEMP_THRESH) {
            client.print("TEMP THRESH EXCEEDED AT SENSOR: ");
        } else {
            client.print("Temperature at sensor: ");
        }

        client.println(i);
        client.print("Farenheit: ");
        client.println(temperatureReads[i]);
    }

    if (overheat) {  //calls cooldown function if more than 3 sensors detect an overheat
        client.println();
        client.println("************SYSTEM OVERHEAT************");
        client.println("3 or more temp sensors have exceeded the temperature threshold...waiting for cooldown");
        client.println("***************************************");
    }
    client.println("-----------------------");
    //delay(1000);    //! Possibly use delay later. Code works with or without, just slower with delay
  }
}


/* READS TEMPERATURE */
void readTemperatures() {
    //TODO: Once converted to JSON, remove client/ethernet code
    sensors.requestTemperatures(); // reads temps from all sensors
    tempThresholdsBroken = 0;   //! Resets to 0 to allow for overheat setting to change. May modify later
        
    // loop through each device & assign temperature value to "temperatureReads" array
    for (int i = 0; i < numberOfDevices; i++) {
        if (sensors.getAddress(tempDeviceAddress, i)) {
            temperatureReads[i] = sensors.getTempF(tempDeviceAddress);  //reads temperature in degrees farenheit
            if (temperatureReads[i] >= TEMP_THRESH) {
                tempThresholdsBroken += 1;
            }
        }
    }
    if (tempThresholdsBroken >= 3) {
        overheat = true;
    } else {
        overheat = false;
    }
}
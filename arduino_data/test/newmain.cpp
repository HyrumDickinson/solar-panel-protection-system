/* Libararies */
#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <OneWire.h>
#include <DallasTemperature.h>

/* CONSTANTS */
#define ONE_WIRE_BUS 12
#define LEDPIN 11

/* TEMPERATURE SETUP */
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float farenheitTemp;

/* ETHERNET SETUP */
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x04};
IPAddress ip(128, 174, 186, 99);
IPAddress gateway(128, 174, 186, 1);
IPAddress subnet(255, 255, 254, 0);
EthernetServer server(23); // default is 23
boolean alreadyConnected = false;


void setup() {
  pinMode(LEDPIN, OUTPUT);
  Serial.begin(9600);
  while (!Serial) {}
  
  sensors.begin();
  ethernetSetup();
}

void loop() {
  // wait for a new client:
  EthernetClient client = server.available();
  
    if (client) {
        if (!alreadyConnected) {
        client.flush();
        Serial.println("New client recognized");
        client.println("--- Type command and hit Enter/Return ---"); 
        alreadyConnected = true;
        }

        client.println("Requesting Temps...");
        sensors.requestTemperatures();
        farenheitTemp = sensors.getTempFByIndex(0);

        if(farenheitTemp != DEVICE_DISCONNECTED_C) {
            client.print("Temperature for the device 1 (index 0) is: ");
            client.println(farenheitTemp);
            while (farenheitTemp > 85) {
                sensors.requestTemperatures();
                client.print("TEMPERATURE THRESH EXCEEDED: ");
                client.println(farenheitTemp);
                farenheitTemp = sensors.getTempFByIndex(0);
            }
        } else {
            client.println("Error: Could not read temperature data");
        }

  }
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
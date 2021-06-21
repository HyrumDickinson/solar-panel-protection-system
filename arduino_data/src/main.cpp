/* Libararies */
#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <MCP3428.h>

/* Constants */
#define CHANNEL_ONE 1     // Panel A
#define CHANNEL_TWO 2     // Panel B
#define CHANNEL_THREE 3   // Panel C
#define CHANNEL_FOUR 4    //! Current (where is panel D being read???)
#define NUM_CHANNELS 4 

/* MCP3428 Voltage and Current Sensor variables */
MCP3428 mcp3428;
channelArra9[NUM_CHANNELS] = {0.0, 0.0, 0.0, 0.0}; // accounts for all 4 channels
float voltageThreshold;   // max voltage desired
float currentThreshold;   // max current desired


/* ETHERNET Variables
 * The mac (media access controller) address is the hardware address for this unit.
 * This MUST be different in each arduino. */
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x04};
IPAddress ip(128, 174, 186, 99); 
IPAddress gateway(128, 174, 186, 1);
IPAddress subnet(255, 255, 254, 0);
EthernetServer server(23); // 23 is default
boolean gotMessage = false; // message from cleint

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  while (!Serial) {}

  while(!mcp3428.test()) {} // won't run if sensors don't work
  Serial.println("All sensors connected");
  ethernetSetup();
  Serial.println("Ethernet Connected");
}


void loop() {
  // wait for a new client:
  EthernetClient client = server.available();
  
  if (client) {
    if (!gotAMessage) {
      Serial.println("We have a new client");
      client.println("Hello, client!"); 
      gotAMessage = true;
    }
    // read the bytes incoming from the client:
    char thisChar = client.read();
    // Controlls an LED
    if (thisChar == 'l') {
      server.write("led ON");
      server.write("\n");
      digitalWrite(ledPin, HIGH);
    }
    if (thisChar == 'o') {
      server.write("led OFF");
      digitalWrite(ledPin, LOW);
    }
    // reads pot data
    if (thisChar == 'p') {
      while (true) {
        server.println(analogRead(readPin));
      }
    }

    if (thisChar == 'v')


  }
}

// Initializing the MCP3428
/* MCP3428 is a 4-channel anolog to digital converter (setup taken from old code)
   Channels: 1 = Panel A, 2 = Panel B, 3 = Panel C, 4 = Current.
*/
//? System in report designed for 4 pannel voltages (Refer to figure 4 in report)?? We only have room for 3 pannels + current
void setupMCP3428(){
  Serial.println("MCP3428 Test");
  
  while(!mcp3428.test()){
  Serial.println("ERROR: MCP3428 not found");
  //? Why delay for a second?
  delay(1000);
  }
  Serial.println("SUCCESS: MCP3428 found");
}

// Make this a sensible function later
void printVoltage() {
  //TODO: Check 'readADC' function. It includes a 'convertRaw()' and other steps that use random numbers. Find out what they are
  //TODO: wtf are these conversion numbers (57, 43.2, 14.3)?????
  float CHANNEL_ONE_VOLT = mcp3428.readADC(1) * 57;
  float CHANNEL_TWO_VOLT = mcp3428.readADC(2) * 43.2;
  float CHANNEL_THREE_VOLT = mcp3428.readADC(3) * 14.3;
  
  //TODO: Confirm 7-decimal accuracy
  Serial.println("ADC Voltages:");
  Serial.print("Channel A: ");
  Serial.println(CHANNEL_ONE_VOLT, 7);
  Serial.print("Channel B: ");
  Serial.println(CHANNEL_TWO_VOLT, 7);
  Serial.print("Channel C: ");
  Serial.println(CHANNEL_THREE_VOLT, 7);
}

//SETUP ETHERNET
/* By calling ethernet.begin(mac), it first tries to make a unique ip address. If that fails, it uses the fallback ip address (made above) */
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
  if (checkValidIP(ip) == false) {
    Serial.println("Stopping Program");
    while (true) {}
  }
  // print local IP address:
  Serial.print("My IP address: ");
  Serial.print(Ethernet.localIP());
  Serial.println();
  server.begin();
}

//verifying IP address
boolean checkValidIP(IPAddress address) {
  if (address[0] == 0 && address[1] == 0 && address[2] == 0 && address[3] == 0) {
    Serial.print(Ethernet.localIP());
    Serial.println(" is not a valid IP Address. No connection made.");
    return false;
  } else {
    return true;
  }
}

/* Communicates with user to determine max desired voltage. Modified for Ethernet connection) */
void setVoltThreshold() {
  char input = client.read();
  client.println("Enter MAX Voltage Threshold (0 to 72.9): ");
  while 
}

void setCurrentThreshold() {

}

void setTempThreshold() {
  
}


void checkVoltage() {

}

void checkCurrent() {

}

void checkTemp() {

}
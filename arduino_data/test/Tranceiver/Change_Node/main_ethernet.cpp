/*************************************************************************
 * Remote node - nRF24L01+ radio communications                          *
 *      Program acts as master and cycles through each node to           *
 *      send a data request command. This code runs regardless           *
 *      of connection through ethernet to ensure panel safety.           *
 *      
 *      Refer to (INSERT DOC HERE) for more info.                         *
 *      Author: Benjamin Olaivar                                         *
 *                                                                       *
 *        Last modified: 07/22/2021                                      *
 *                                                                       *
 *************************************************************************/

//https://medium.com/@benjamindavidfraser/arduino-nrf24l01-communications-947e1acb33fb

/* LIBRARIES */
#include <RF24.h> 
#include <SPI.h> 
#include <nRF24L01.h>
#include <Ethernet.h>
#include <ArduinoJson.h>

/* CONSTANTS */
#define CE_PIN 7        // set Chip-Enable (CE) and Chip-Select-Not (CSN) pins (for UNO)
#define CSN_PIN 8
#define NUM_NODES 2
#define CAPACITY 200    // the number of bytes required for our JSON (we use roughly 200)

/* GENERAL VARIABLES */
int node = 1;

/* JSON VARIABLES */
StaticJsonDocument<200> sendJson;       // initializing all the variables (as floats)
bool sent = true;          // indicates if a message has been sent
// sendJson["NODE"];
// sendJson["T1"];
// sendJson["T2"];
// sendJson["T3"];
// sendJson["T4"];
// sendJson["T5"];
// sendJson["T6"];

// sendJson["V1"];
// sendJson["V2"];
// sendJson["V3"];

// sendJson["C1"];

/* RADIO VARIABLES */
RF24 radio(CE_PIN,CSN_PIN);     // create RF24 radio object using selected CE and CSN pins

byte nodeAddress[5] = {'N','O','D', 0, 1};      // setup radio pipe address for remote sensor node

struct Command_Package {
    bool shutdown = false;
};
struct Data_Package {
  bool overheat = false;
  bool overVolt = false;
  bool overCurr = false;

  float V1 = 0.0;
  float V2 = 0.0;
  float V3 = 0.0;

  float C = 0.0;

  float T1 = 0.0;
  float T2 = 0.0;
  float T3 = 0.0;
//   float T4 = 0.0;
//   float T5 = 0.0;
//   float T6 = 0.0;
};
Command_Package sendCommand;    //TODO: add commands
Data_Package data;

/* ETHERNET VARIABLES */
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x04};
IPAddress ip(128, 174, 186, 99);
//IPAddress ip(192, 162, 1, 6);
IPAddress gateway(128, 174, 186, 1);
IPAddress subnet(255, 255, 254, 0);
EthernetServer server(23); // default is 23
boolean alreadyConnected = false;


/* Function: setup
*   calls various setup functions
*/
void setup() {
  Serial.begin(9600);
  while(!Serial) {}

  ethernetSetup();
  radioSetup();
}


/* Function: radioSetup
*   sets up transceiver settings. Allows for 5 retries before moving on to the next node.
*   NOTE: the enableAckPayload is crucial for this to work. It automatically switches between reading and listening modes.
*   The Acknowledgement payload is the response from each node filled with data.
*/
void radioSetup() {
    radio.begin();// begin radio object
  
    radio.setPALevel(RF24_PA_LOW);      // set power level of the radio
    radio.setDataRate(RF24_250KBPS);    // set RF datarate - lowest rate for longest range capability
   
    radio.setChannel(0x66);     // set radio channel to use - ensure all slaves match this
    radio.setRetries(5, 50);    // set time between retries and max no. of retries (time )
    
    radio.enableAckPayload();               // enable ackpayload - enables each slave to reply with data 
    radio.openWritingPipe(nodeAddress);     // setup write pipe to remote node - must match node listen address
}


/* Function: ethernetSetup 
*   By calling ethernet.begin(mac), it first tries to make a unique ip address. If that fails, it uses the fallback ip address (made above).
*   begin(mac) keeps failing. fallback is always used for some reason
*/
void ethernetSetup() {
  Serial.println("Trying to get an IP address using DHCP");

  Ethernet.begin(mac, ip, gateway, subnet);

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


/* Function: loop
 *    main loop program for the slave node - opens a new reading pipe when switching to a new node
 */
void loop() {
    EthernetClient client = server.available();

    Serial.print("Node ");
    Serial.print(node);
    Serial.println(":");
    radio.openWritingPipe(nodeAddress);
    readRadio();
    
    if (client) {
        //TODO: Test this with Hyrum
        // int len - client.available();
        // if (len > 0) {
        //     byte buffer[80];
        //     client.read(buffer, len);
        // }

        //TODO: end of test section

        char sendValue[CAPACITY];               // Serialize JSON Object to char array
        serializeJson(sendJson, sendValue);
        client.println(sendValue);
        // server.write(sendValue);
        Serial.println(sendValue);

    }
    changeNode();
    Ethernet.maintain();

    delay(1000);
}


/* Function: readRadio
*   Communicates to whichever node is currently being read from, and collects data through an acknowledgement response.
*   the acknowledgement response automatically switches between listening and writing modes, which proved better than manually doing so
*/
void readRadio() {
    Serial.println("[*] Attempting to transmit data to remote node.");
    
    // boolean to indicate if radio.write() tx was successful
    bool tx_sent;
    tx_sent = radio.write(&sendCommand, sizeof(sendCommand));
    // if tx success - receive and read smart-post ack reply
    if (tx_sent) {
        if (radio.isAckPayloadAvailable()) {
            // read acknowledgement payload and copy message to remoteNodeData 
            radio.read(&data, sizeof(data));
            sendJson["T1"] = data.T1;
            sendJson["T2"] = data.T2;
            sendJson["T3"] = data.T3;
            // sendJson["T4"] = data.T4;
            // sendJson["T5"] = data.T5;
            // sendJson["T6"] = data.T6;

            Serial.print("[+] Successfully received data from node.");
            Serial.print("  ---- Temp was: ");
            Serial.println(data.T1);
            if (data.T1 > 80) {
                sendCommand.shutdown = true;
            } else {
                sendCommand.shutdown = false;
            }
        }
        sent = true;
        sendJson["sent"] = true;
    } 
    else {
        Serial.println("[-] The transmission to the node failed.  ***********************************");

        //TODO: update json accordingly. May not want to change temperature variables though
        sent = false;
        sendJson["sent"] = false;
    }
    Serial.println("------------------------------------------");
}


/* Function: changeNode
*   Updates the nodeAddress to the next node by changing the last 2 variables of nodeAddress. (recall nodeAddress[5] = {'N','O','D', 0, 1};)
*   KEEP THE LAST 2 VARIABLES OF nodeAddress AS INTS FOR THIS FUNCTION TO WORK 
*   To change max # of nodes, just change the NUM_NODES variable
*/
void changeNode() {
    node = node + 1;
    if (node > NUM_NODES) {
        node = 1;
    }
    nodeAddress[3] = node / 10;
    nodeAddress[4] = node % 10;
    sendJson["NODE"] = node;
}
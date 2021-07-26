/*************************************************************************
 * Remote node - nRF24L01+ radio communications                          *
 *      Program acts as master and cycles through each node to           *
 *      send a data request command.                                     *
 *                                                                       *
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
#define CE_PIN 7    // set Chip-Enable (CE) and Chip-Select-Not (CSN) pins (for UNO)
#define CSN_PIN 8

/* JSON VARIABLES */
StaticJsonDocument<200> doc;

/* GENERAL VARIABLES */
bool confirmSent = false;

/* RADIO VARIABLES */
RF24 radio(CE_PIN,CSN_PIN);     // create RF24 radio object using selected CE and CSN pins

int node = 1;               // identifier for which node to read from

const byte nodeAddress[5] = {'N','O','D','E','1'};      // setup radio pipe address for remote sensor node
const byte nodeAddress2[5] = {'N','O','D','E','2'};

struct Command_Package {
    bool SHUTDOWN = false;
};
struct Data_Package {
  float T1 = 0.0;
  float T2 = 0.0;
  float T3 = 0.0;
};

Command_Package sendCommand;    //TODO: add commands
Data_Package data;

/* ETHERNET VARIABLES */
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x04};
IPAddress ip(128, 174, 186, 99);
IPAddress gateway(128, 174, 186, 1);
IPAddress subnet(255, 255, 254, 0);
EthernetServer server(23); // default is 23
boolean alreadyConnected = false;


/* Function: radioSetup
*   sets up transceiver settings. Allows for 5 retries before moving on to the next node
*/
void radioSetup() {
    
    radio.begin();// begin radio object
  
    radio.setPALevel(RF24_PA_LOW);      // set power level of the radio
    radio.setDataRate(RF24_250KBPS);    // set RF datarate - lowest rate for longest range capability
   
    radio.setChannel(0x66);     // set radio channel to use - ensure all slaves match this
    radio.setRetries(5, 10);    // set time between retries and max no. of retries (time )
    
    radio.enableAckPayload();               // enable ackpayload - enables each slave to reply with data 
    radio.openWritingPipe(nodeAddress);     // setup write pipe to remote node - must match node listen address
}


/* Function: ethernetSetup 
*  By calling ethernet.begin(mac), it first tries to make a unique ip address. If that fails, it uses the fallback ip address (made above).
*  begin(mac) keeps failing. fallback is always used for some reason
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
            //! remove
            confirmSent = true;
            // read ack payload and copy message to remoteNodeData 
            radio.read(&data, sizeof(data));
            Serial.print("[+] Successfully received data from node.");
            Serial.print("  ---- Temp was: ");
            Serial.println(data.T1);
            if (data.T1 > 80) {
                sendCommand.SHUTDOWN = true;
            }
        }
    } 
    else {
        Serial.println("[-] The transmission to the node failed.  ***********************************");
        //! remove
        confirmSent = false;
    }
    Serial.println("------------------------------------------");
}


/* Function: setup
*   calls various setup functions
*/
void setup() {
  Serial.begin(9600);

  ethernetSetup();
  radioSetup();
}


/* Function: loop
 *    main loop program for the slave node - opens a new reading pipe when switching to a new node
 */
void loop() {
    EthernetClient client = server.available();

    if (client) {
        if (node == 1) {
            Serial.println("Node 1: ");
            client.print("Node 1: ");
            radio.openWritingPipe(nodeAddress);
            node = 2;
        } else {
            Serial.println("Node 2: ");
            client.print("Node 2: ");
            radio.openWritingPipe(nodeAddress2);
            node = 1;
        }
        readRadio();
        if (confirmSent == true) {
            client.println(data.T1);
        } else {
            client.println("failed");
        }
        delay(1000);
    }

}

//https://medium.com/@benjamindavidfraser/arduino-nrf24l01-communications-947e1acb33fb

/* LIBRARIES */
#include <RF24.h> 
#include <SPI.h> 
#include <nRF24L01.h>

/* CONSTANTS */
#define CE_PIN 7    // set Chip-Enable (CE) and Chip-Select-Not (CSN) pins (for UNO)
#define CSN_PIN 8

#define buttonOut 4
#define buttonIn 3

/* RADIO VARIABLES */
RF24 radio(CE_PIN,CSN_PIN);     // create RF24 radio object using selected CE and CSN pins

int node = 1;               // identifier for which node to read from

const byte nodeAddress[5] = {'N','O','D','E','1'};      // setup radio pipe address for remote sensor node
const byte nodeAddress2[5] = {'N','O','D','E','2'};

//TODO:  add commands
struct Command_Package {
    bool SHUTDOWN = false;
};
Command_Package sendCommand;

struct Data_Package {
  float T1 = 0.0;
  float T2 = 0.0;
  float T3 = 0.0;
};
Data_Package data;


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


/* Function: readRadio
*   Communicates to whichever node is currently being read from, and collects data through an acknowledgement response.
*   the acknowledgement response automatically switches between listening and writing modes, which proved better than manually doing so
*/
void readRadio() {
    Serial.println("[*] Attempting to transmit data to remote node.");
    
    // boolean to indicate if radio.write() tx was successful
    bool tx_sent;
    tx_sent = radio.write( &sendCommand, sizeof(sendCommand));
    // if tx success - receive and read smart-post ack reply
    if (tx_sent) {
        if (radio.isAckPayloadAvailable()) {
            // read ack payload and copy message to remoteNodeData 
            radio.read(&data, sizeof(data));
            Serial.print("[+] Successfully received data from node.");
            Serial.print("  ---- Temp was: ");
            Serial.println(data.T1);
        }
    } 
    else {
        Serial.println("[-] The transmission to the node failed.  ------------------------------------------");
    }
    Serial.println("------------------------------------------");
}


/* Function: setup
*   calls various setup functions
*/
void setup() {
  Serial.begin(9600);
  pinMode(buttonOut, OUTPUT);
  digitalWrite(buttonOut, HIGH);
  pinMode(buttonIn, INPUT);

  radioSetup();
}


/* Function: loop
 *    main loop program for the slave node - opens a new reading pipe when switching to a new node
 */
void loop() {
    if (digitalRead(buttonIn) == HIGH) {
        Serial.println("button pressed");
    }
  
    if (node == 1) {
        Serial.println("Node 1: ");
        radio.openWritingPipe(nodeAddress);
        node = 2;
    } else {
        Serial.println("Node 2: ");
        radio.openWritingPipe(nodeAddress2);
        node = 1;
    }


    readRadio();
    delay(1000);
}

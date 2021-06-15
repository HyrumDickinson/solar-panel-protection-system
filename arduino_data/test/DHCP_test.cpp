/*
  This is main program (blablabla add more later)
*/

//LIBARARIES
#include <SPI.h>
#include <Ethernet.h>

//ETHERNET VARIABLES
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x04};
IPAddress ip(128, 174, 186, 99);
IPAddress gateway(128, 174, 186, 1);
IPAddress subnet(255, 255, 254, 0);
EthernetServer server(23); // default is 23
boolean alreadyConnected = false;

//TESTING PINS
int ledPin = 12;
int readPin = A10;
String commandString;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  while (!Serial) {}
  
  ethernetSetup();
}

void loop() {
  // wait for a new client:
  EthernetClient client = server.available();
  
  if (client) {
    if (!alreadyConnected) {
      client.flush();
      commandString = "";
      Serial.println("New client recognized");
      client.println("--- Type command and hit Enter/Return ---"); 
      alreadyConnected = true;
    }

    //waits for client to finish typing and adds each character to a command string
    while (client.available()) {
      char newChar = client.read();
      if (newChar == ',') {   // ',' is taking place of enter/return. Using enter/return adds extra character that messes up frequently
        server.print("Received command: ");
        server.println(commandString);
        processCommand(commandString);
      } else {
        commandString += newChar;
      }
    }  
  }
}


/* SETUP ETHERNET 
  By calling ethernet.begin(mac), it first tries to make a unique ip address. If that fails, it uses the fallback ip address (made above).
  begin(mac) keeps failing. fallback is always used for some reason
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


/* Command Processing Function
  takes string input from client and responds accordingly
  IN FUTURE WILL RESPOND TO GUI STARTUP AND WILL BE RAN 
*/
void processCommand(String command)
{
  server.print("Processing command ");
  server.println(command);
  
  if (command.indexOf("ledon") > -1){
    server.println("LED On command received"); 
    digitalWrite(ledPin, HIGH);   // sets the LED on
    server.println("LED was turned on");
    commandString = "";
    return;
  } 
  
  if (command.indexOf("setTempThresh") > -1){
    
    commandString = "";
    return;
  } 

  if (command.indexOf("readTemp") > -1){
    
    commandString = "";
    return;
  } 

  if (command.indexOf("setVoltageThresh") > -1){
    
    commandString = "";
    return;
  } 
  
  if (command.indexOf("readVoltage") > -1){
    
    commandString = "";
    return;
  }

  // wipes the command string
  commandString = "";
}
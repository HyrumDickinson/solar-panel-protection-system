/*
  This is main program (blablabla add more later)
  https://www.youtube.com/watch?v=vcOE2XAQHzY
  https://www.youtube.com/watch?v=jgKPmjQtJG4
*/

//LIBARARIES
#include <SPI.h>
#include <Ethernet.h>
// #include <MCP3428.h>

//CONSTATNS
#define CHANNEL_ONE 1     // Panel A
#define CHANNEL_TWO 2     // Panel B
#define CHANNEL_THREE 3   // Panel C
#define CHANNEL_FOUR 4    // Current
#define NUM_CHANNELS 4 

//ADC VARIABLES
float voltageThresh;
float currentThresh;


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
    digitalWrite(ledPin, HIGH);   // sets the LED on
    server.println("LED was turned on");
    commandString = "";
    server.println("-----------------");
    return;
  } 

  if (command.indexOf("ledoff") > -1) {
    digitalWrite(ledPin, LOW);
    commandString = "";
    server.println("-----------------");
    return;
  }

  if (command.indexOf("voltThresh") > -1) {
    int newIndex = command.indexOf("voltThresh") + 11;
    String newString = command.substring(newIndex);
    voltageTrhesh = newString.toInt();
    commandString = "";
    server.println("-----------------");
    return;
  }

  if (command.indexOf("gim thresh") > -1) {
    server.println(voltageThresh);
  }
  
  if (command.indexOf("readVoltage") > -1){
    server.println("Checking for sensor connection...");
    // while(!mcp3428.test()) {} //must check connection every time
    server.println("Sensor connected!");
    // printVoltage();
    server.println("pretend this is a valid search");
    commandString = "";
    return;
  }

  // wipes the command string
  commandString = "";
}

//void printVoltage() {
//  //TODO: Check 'readADC' function. It includes a 'convertRaw()' and other steps that use random numbers. Find out what they are
//  //TODO: wtf are these conversion numbers (57, 43.2, 14.3)?????
//  float CHANNEL_ONE_VOLT = mcp3428.readADC(1) * 57;
//  float CHANNEL_TWO_VOLT = mcp3428.readADC(2) * 43.2;
//  float CHANNEL_THREE_VOLT = mcp3428.readADC(3) * 14.3;
//  
//  //TODO: Confirm 7-decimal accuracy
//  Serial.println("ADC Voltages:");
//  Serial.print("Channel A: ");
//  Serial.println(CHANNEL_ONE_VOLT, 7);
//  Serial.print("Channel B: ");
//  Serial.println(CHANNEL_TWO_VOLT, 7);
//  Serial.print("Channel C: ");
//  Serial.println(CHANNEL_THREE_VOLT, 7);
//}


void setVoltThresh() {

}

void printVoltage() {
  server.print("Voltage thresh is : ");
  server.println(voltageThresh);
}
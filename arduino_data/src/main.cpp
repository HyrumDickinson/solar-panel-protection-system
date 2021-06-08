// https://www.youtube.com/watch?v=vcOE2XAQHzY
// https://www.youtube.com/watch?v=jgKPmjQtJG4

#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>


/* ETHERNET SETUP
 * 
 * The mac (media access controller) address is the hardware address for this unit.
 * This MUST be different in each arduino.
 */
// !testing client side via console. BASIC TESTING:
 /*
 *  *open serial monitor*
 *  
 *  Console:
 *  telnet <ip address from serial>
 *  
 *  *typing anything should return it in console*
 */
byte mac[] = {
  0x00, 0xAA, 0XBB, 0XCC, 0XDE, 0x02
}; // only modify last 3 variables: 0xBC, 0xDF, 0x03
IPAddress ip(172,16,235, 101); // IP address obtained from wifi settings. Last val (101) is arbitrary
IPAddress gateway(172,16,235, 1); // TODO: confirm router IP
IPAddress subnet(255, 255, 255, 0); // default
EthernetServer server(23); // 23 is default
boolean gotMessage = false; // message from cleint

void setup() {
  Serial.begin(9600);
  while(!Serial){}
}

// START ETHERNET CONNECTION
Serial.println("trying to get IP address using DHCP");
if (Ethernet.begin(mac) == 0) {
  Serial.println("Failed to config Ethernet using DHCP");
  // configs ethernet using our values instead
  Ethernet.begin(mac, ip, gateway, subnet);  
}
Serial.print("My IP address: ");
ip = Ethernet.localIP(); // creates ip that can be read
for (byte thisByte = 0; thisByte < 4, thisByte++) {
  Serial.print(ip[thisByte], DEC); // converts to decimal
  Serial.print(".");
}
serial.println();
server.begin(); // tells server to wait for connections from client


void loop() {
  // wait for client
  EthernetClient client = server.available();
  if !gotMessage) {
    Serial.println("Client found");
    client.println("Henlo"); // TODO: look into client side printing
    gotAMessage = true;  

    // read incoming bytes
    char thisChar = client.read();
    // echo back to client
    server.write(thisChar);
    Serial.print(thisChar);
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

// Prints the readings from the MCP3428
//Renamed to readVoltage() instead of printVoltage()
//TODO: do more than print. Integrate this into ETHERNET response. Review readADC function in MCP3428.cpp
void readVoltage() {
  float channel_A_volt = mcp3428.readADC(1)*57;
  float channel_B_volt = mcp3428.readADC(2)*43.2;
  float channel_C_volt = mcp3428.readADC(3)*14.3;
  
  Serial.println("ADC Voltages:");
  Serial.print("CH_A: ");
  Serial.println(channel_A_volt, 7);
  Serial.print("CH_B: ");
  Serial.println(channel_B_volt, 7);
  Serial.print("CH_C: ");
  Serial.println(channel_C_volt, 7);
  //? Why delay??
  delay(1000);
}


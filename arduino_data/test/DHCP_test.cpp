/*
 DHCP Chat  Server
 
 A simple server that distributes any incoming messages to all
 connected clients.  To use telnet to  your device's IP address and type.
 You can see the client's input in the serial monitor as well.
 Using an Arduino Wiznet Ethernet shield. 
 
 THis version attempts to get an IP address using DHCP
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 
 created 21 May 2011
 modified 9 Apr 2012
 by Tom Igoe
 Based on ChatServer example by David A. Mellis
 
 */
#include <SPI.h>
#include <Ethernet.h>

// The IP address will be dependent on your local network.
// Ip: 128.174.186.71
// Subnet: 255.255.254.0
// Gateway: 128.174.186.1
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x04};
IPAddress ip(128, 174, 186, 99);
IPAddress gateway(128, 174, 186, 1);
IPAddress subnet(255, 255, 254, 0);
int ledPin = 11;
int readPin = A10;

// telnet defaults to port 23
EthernetServer server(23);
boolean gotAMessage = false;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  while (!Serial) {;}

  // start the Ethernet connection:
  /* ?By calling ethernet.begin(mac), it first tries to make a unique ip address. If that fails, it uses the fallback ip address (made above).
     begin(mac) keeps failing. fallback is always used for some reason */
  Serial.println("Trying to get an IP address using DHCP");
  //! This way for sake of testing... delete next line and uncomment 'if' statement
  Ethernet.begin(mac, ip, gateway, subnet);
//  if (Ethernet.begin(mac) == 0) {
//    Serial.println("Failed to configure Ethernet using DHCP");
//    initialize the ethernet device not using DHCP:
//    Ethernet.begin(mac, ip, gateway, subnet);
//  }

  // print your local IP address:
  Serial.print("My IP address: ");
  ip = Ethernet.localIP();
  Serial.print(Ethernet.localIP());
  Serial.println();
  server.begin();
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

    if (thisChar == 'i') {
      server.println(69); 
    }
    
    // echo the bytes to the server as well:
    Serial.print(thisChar);
  }
}

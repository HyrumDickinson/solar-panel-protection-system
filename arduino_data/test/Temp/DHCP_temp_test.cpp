/*
  This is main program (blablabla add more later)
  https://www.youtube.com/watch?v=vcOE2XAQHzY
  https://www.youtube.com/watch?v=jgKPmjQtJG4
*/

//LIBARARIES
#include <SPI.h>
#include <Ethernet.h>
#include <DHT.h>
#include <DHT_U.h>

// #include <MCP3428.h>

//CONSTATNS
#define CHANNEL_ONE 1     // Panel A
#define CHANNEL_TWO 2     // Panel B
#define CHANNEL_THREE 3   // Panel C
#define CHANNEL_FOUR 4    // Current
#define NUM_CHANNELS 4
#define LEDPIN 12 
#define TEMP_PIN 2
#define DHTTYPE DHT11
DHT dht(TEMP_PIN, DHTTYPE);

/* TEMPERATURE variables */
float farenheitThresh;
float farenheitTemp;
float cleciusThresh;
float celciusTemp;


//ETHERNET VARIABLES
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
  dht.begin();
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
    delay(1000);
    farenheitTemp = dht.readTemperature(true);
    celciusTemp = dht.readTemperature();

    if (isnan(farenheitTemp) || isnan(celciusTemp)) {
        client.println(F("failed to read :("));
        return;
    }
    if (farenheitTemp > 81) {
        Serial.print("Temp threshold exceeded...shutting down");
        while (farenheitTemp > 81) {
            farenheitTemp = dht.readTemperature(true);
        }
    }

    client.print(F("Farenheit: "));
    client.println(farenheitTemp);
    client.print(F("Celcius: "));
    client.println(celciusTemp);
    client.println("----------------");
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
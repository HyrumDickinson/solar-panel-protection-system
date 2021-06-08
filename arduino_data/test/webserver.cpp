#include <SPI.h>
#include <Ethernet.h>

int led = 8;
String readString;

byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x04};
IPAddress ip(128, 174, 186, 99);
IPAddress gateway(128, 174, 186, 1);
IPAddress subnet(255, 255, 254, 0);
EthernetServer server(80);

void setup() {
    Serial.beginn(9600);
    pinMode(led, OUTPUT);
    Ethernet.begin(mac, ip, gateway, subnet);
    server.begin();
    Serial.print("Server is at ");
    Serial.println(Ethernet.localIP());
}

void loop() {
    EthernetClient client = server.available();
    if (client) {
        while (client.connected()) {
            if (client.available) {
                char c = client.read();
                Serial.println(c);

                if(readString.length() < 100) {
                    readString += c;
                }

                if (c = '\n') {
                    Serial.print(readString);
                    client.println("<HTTP/1.1 200 OK>");
                    client.println("<Connection-Type: text/html>");
                    client.println("<Connection: close>");
                    client.println("");

                    client.println("<!DOCTYPE html>");
                    client.println("<html>");
                    client.println("<head>");
                    client.println("<title>Webserver</title>");
                    client.println("</head>");
                    client.println("<body>");
                    client.println("<a href=\"/?button1on\"\><button>LED ON</button?</a>");
                    client.println("<a href=\"/?button2off\"\><button>LED OFF</button?</a>");
                    client.println("<body style=background-color:powderblue>");
                    
                    delay(1);
                    client.stop();

                    if (readString.indexOf("?button1on") > 0) {
                        digitalWrite(led, HIGH);
                    }
                    if (readString.indexOf("?button2on") > 0) {
                        digitalWrite(led, LOW);
                    }
                    readString = "";
                }
            }
        }
    }
}
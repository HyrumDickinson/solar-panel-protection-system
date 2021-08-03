import controlP5.*;
import processing.serial.*;  // including the serial object libarary

ControlP5 cp5;
Serial mySerial;
PFont font;


/* GENERAL VARIABLES */
JSONObject parsedJson;
String jsonPackage = null;    // string that stores serialized incoming json
int nodeDisplayed;
int nodeUpdated;
float temp1 = 0.0;  // test variable
float temp2 = 0.0;  // test variable
float temp3 = 0.0;  // test variable
boolean sent;
boolean confirmUpdate;

int newLine = 10;        // 10 is binary for 'return' or 'new line'.


void setup() {
  mySerial = new Serial(this, "COM4", 9600);
  size(1200, 600);  // siza deez app windows (x and y dimensions of application window)
  font = loadFont("SansSerif.plain-16.vlw");
  textFont(font);

  cp5 = new ControlP5(this);
  cp5.addButton("node1")    //"Node 1" is the name of the button
    .setPosition(100, 50)
    .setSize(100, 50);
  ;
  cp5.addButton("node2")    //"Node 1" is the name of the button
    .setPosition(100, 150)
    .setSize(100, 50)
    .setColorBackground(color(255, 0, 0));
  ;
  cp5.addButton("on")    //"Node 1" is the name of the button
    .setPosition(800, 50)
    .setSize(100, 50);
  ;
  cp5.addButton("off")    //"Node 1" is the name of the button
    .setPosition(800, 150)
    .setSize(100, 50)
    .setColorBackground(color(255, 0, 0));
  ;
}

void draw() {

  while (mySerial.available() > 0) {
    jsonPackage = mySerial.readStringUntil(newLine);    // reads until it receives the 'new line' (10) signal
  }

  if (jsonPackage != null) {
    String newJsonString = jsonPackage.substring(0, jsonPackage.length() - 2);
    println(newJsonString);
    try {
      background(255, 255, 255);
      fill(0, 0, 0);
      parsedJson = parseJSONObject(newJsonString);
      println(parsedJson);
      nodeUpdated = parsedJson.getInt("NODE");
      sent = parsedJson.getBoolean("SENT");
      print("NODE: ");
      println(nodeUpdated);
      text(nodeDisplayed, 600, 150);
    } 
    catch (Exception e) {
      println("failed parsing JSON");
    }
    try {
      confirmUpdate = true;
      temp1 = parsedJson.getFloat("T1");
      temp2 = parsedJson.getFloat("T2");
      temp3 = parsedJson.getFloat("T3");
    } catch (Exception e) {
      confirmUpdate = false;
      println("failed updating data");
    }
    
    if (sent == true && nodeUpdated == nodeDisplayed) {
        background(255, 255, 255);
        fill(0, 0, 0);
        text("Temp 1: ", 400, 90); 
        text(temp1, 500, 90);
        text("Temp 2: ", 400, 120);
        text(temp2, 500, 120);
        text("Temp 3: ", 400, 150); 
        text(temp3, 500, 150);
        text(nodeDisplayed, 600, 150);
      }
    
    println("---------------------------------------------");
    
  }
}

void on() {
  println("on");
  mySerial.write('o');
}

void off() {
  println("off");
  mySerial.write('f');
}

void node1() {
  nodeDisplayed = 1;
  println("switched to node 1");
}

void node2() {
  nodeDisplayed = 2;
  println("switched to node 2");
}

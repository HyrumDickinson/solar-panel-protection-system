/* LIBRARIES */
import controlP5.*;
import processing.serial.*;  // including the serial object libarary

/* INITIALIZATION */
ControlP5 cp5;
Serial mySerial;
PFont font;
data[] dataArray = new data[61];

/* GENERAL VARIABLES */
int newLine = 10;        // 10 is binary for 'return' or 'new line'.
float tempThresh = 10.0;
float voltThresh = 0.0;
float currThresh = 0.0;


/* JSON VARIABLES */
JSONObject parsedJson;
String jsonPackage = null;    // string that stores serialized incoming json
boolean sent;

/* NODE VARIABLES */
int nodeDisplayed;
int nodeUpdated;


void setup() {
  
  /* General setup of the window */
  mySerial = new Serial(this, "COM4", 9600);
  size(1600, 900);  // siza deez app windows (x and y dimensions of application window)
  font = loadFont("ArialMT-18.vlw");
  textFont(font);
  
  /* Creating array of data objects to record each node's data */
  for (int i = 0; i < 61; i++) {
    dataArray[i] = new data();
  }
  
  controlP5Setup();
}

/* Function: controlP5Setup()
*   initializes all the buttons and tabs displayed
*/
void controlP5Setup() {
  cp5 = new ControlP5(this);
  font = loadFont("ArialMT-19.vlw");
  ControlFont controlFont = new ControlFont(font);
  cp5.setFont(controlFont);
  
   cp5.addTab("default")
    .setLabel("Monitor")
    .setHeight(76)
    .setWidth(115)
    .setColorActive(color(232, 74, 39))
    .setColorBackground(color(19, 41, 75))
    .setColorForeground(color(30, 81, 150))
    ;
  cp5.addTab("Overview")
     .setHeight(76)
     .setWidth(115)
     .setColorActive(color(232, 74, 39))
     .setColorBackground(color(19, 41, 75))
     .setColorForeground(color(30, 81, 150))
     ;
  cp5.addTab("Settings")
    .setHeight(76)
    .setWidth(115)
    .setColorActive(color(232, 74, 39))
    .setColorBackground(color(19, 41, 75))
    .setColorForeground(color(30, 81, 150))
    ;
  
  cp5.getTab("default")
    .activateEvent(true)
    .setId(1)
  ;
  cp5.getTab("Overview")
    .activateEvent(true)
    .setId(2)
  ;
  cp5.getTab("Settings")
    .activateEvent(true)
    .setId(3)
  ;
  
  int xpos = 120;
  int ypos = 70;
  for (int i = 0; i < 60; i++) {
    
    if(i % 5 == 0) {
      xpos = 100;
      ypos += 48;
    }
    
    cp5.addButton("panel" + (i + 1))
      .setLabel("" + (i + 1))
      .setPosition(xpos, ypos)
      .setSize(75, 45)
      .setId(i + 1)
    ;
    xpos += 77;
  }
  
  cp5.addButton("enable")    //"enable" is the name of the button
    .setPosition(800, 800)
    .setSize(120, 50)
    .setColorBackground(color(23, 207, 23))
    .setColorForeground(color(46, 184, 46))
    .setColorActive(color(0, 200, 0))
  ;
  cp5.addButton("disable")    //"disable" is the name of the button
    .setPosition(1000, 800)
    .setSize(120, 50)
    .setColorBackground(color(230, 6, 6))
    .setColorForeground(color(195, 34, 34))
    .setColorActive(color(220, 0, 0))
  ;
  
   cp5.addButton("oof")    //"oof" is the name of the button
    .setPosition(1000, 800)
    .setSize(120, 50)
    .setColorBackground(color(230, 6, 6))
    .setColorForeground(color(195, 34, 34))
    .setColorActive(color(220, 0, 0))
  ;
  cp5.getController("oof").moveTo("Settings");
}


void draw() {
  background(232, 233, 234);
  stroke(color(19, 41, 75));    // The stroke is the OUTLINE of the shape/thing
  fill(color(19, 41, 75));      // the fill is the INSIDE COLOR of the shape/thing
  rect(0, 0, width, 75);        // Blue header rectangle

  updateData();

  if (cp5.getTab("default").isActive()) {
    updateDisplayData();
  }
}


void controlEvent(ControlEvent theControlEvent) {
  if (theControlEvent.isTab()) {
    println("event from tab : "+theControlEvent.getTab().getName()+" with id "+theControlEvent.getTab().getId());
  } else if (theControlEvent.getId() > 0 && theControlEvent.getId() <= 60) {
    nodeDisplayed = theControlEvent.getId();
    println("switched to node " + nodeDisplayed);
  }
}


/* Function updateDisplayData()
*   If on the monitor tab, this function displays the node data for the selected node
*/
void updateDisplayData() {
    if (nodeDisplayed > 0 && nodeDisplayed <= 60) {
        float[] temps = dataArray[nodeDisplayed].getTempsFarenheit();

        fill(0, 0, 0);
        text("Temp 1: ", 500, 150); 
        text(temps[0], 600, 150);
        text("Temp 2: ", 500, 200);
        text(temps[1], 600, 200);
        text("Temp 3: ", 500, 250); 
        text(temps[2], 600, 250);
        text(nodeDisplayed, 700, 250);
    }
}


/* Function updateDisplayData()
*   Takes in the new data and saves it to the data array
*/
void updateData() {
    while (mySerial.available() > 0) {
        jsonPackage = mySerial.readStringUntil(newLine);    // reads until it receives the 'new line' (10) signal
    }

    if (jsonPackage != null) {
        String newJsonString = jsonPackage.substring(0, jsonPackage.length() - 2);

        try {
            parsedJson = parseJSONObject(newJsonString);
            nodeUpdated = parsedJson.getInt("NODE");
            sent = parsedJson.getBoolean("SENT");
        } 
        catch (Exception e) {
            println("Failed parsing JSON");
        }

        if (sent == true) {          // If the jsonPackage was parsed correnctly, this works
            boolean overHeat = false;
            boolean overVolt = false;
            boolean overCurr = false;
            dataArray[nodeUpdated].setTemps(parsedJson.getFloat("T1"), parsedJson.getFloat("T2"), parsedJson.getFloat("T3"));
            // dataArray[nodeUpdated].setVoltage(parsedJson.getFloat("V"));
            // dataArray[nodeUpdated].setCurrent(parsedJson.getFloat("C"));
            
            if (overHeat || overVolt || overCurr) {
              cp5.getController("panel" + nodeUpdated).setColorBackground(color(213, 0, 50));
            }
            sent = false;
        } else {
          cp5.getController("panel" + nodeUpdated).setColorBackground(color(166, 168, 171));
        }
    
    }
}


void enable() {
  println("on");
  mySerial.write("{NODE:" + nodeDisplayed + ",SHUTDOWN:" + false + "}");
}
void disable() {
  println("off");
  mySerial.write("{NODE:" + nodeDisplayed + ",SHUTDOWN:" + true + "}");
}
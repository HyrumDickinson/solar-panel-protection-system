/*************************************************************************
 *                      PROTECTION SYSTEM GUI                            *
 *      This is the Graphical User Interface (gui) for the Solar Panel   *
 *      Protection System. This file should be stored with assisted      *
 *      classes: (INSERT CLASSES HERE). This is heavily reliant on the   *
 *      controlP5 library for buttons and gui development.               *
 *      This code is reliant on a serial connection to the MAIN NODE.    *
 *                                                                       *
 *      Code should be ran in PROCESSING IDE                             *
 *                                                                       *
 *      Refer to (INSERT DOC HERE) for more info.                        *
 *      Author: Benjamin Olaivar                                         *
 *                                                                       *
 *        Last modified: 08/10/2021                                      *
 *                                                                       *
 *************************************************************************/

/* LIBRARIES */
import controlP5.*;
import processing.serial.*;  // including the serial object libarary

/* CONTANTS */
final int NUMNODES = 60;
final float defaultOverheatTemp = 10.0;            // Default Temperature (Farenheit) that triggers overheat status. Can revert to in 'Settings' tab
final float defaultVoltageThreshold = 0.0;         // Default Voltage (in Volts)   that triggers overVoltage status. Can revert to in 'Settings' tab
final float defaultCurrentThreshold = 0.0;         // Default Current (in Amps)    that triggers overCurrent status. Can revert to in 'Settings' tab

color UOFI_BLUE = color(19, 41, 75);          // colors taken from official UIUC palette (https://www.uillinois.edu/OUR/brand/color_palettes)
color UOFI_BLUE_HOVER = color(30, 81, 150);
color UIC_RED = color(213, 0, 50);
color URBANA_ORANGE = color(232, 74, 39);
color COOL_GRAY_6 = color(166, 168, 171);


/* INITIALIZATION */
ControlP5 cp5;
Serial mySerial;
PFont font;
data[] dataArray = new data[NUMNODES];

/* GENERAL VARIABLES */
int activeTab;           // the current 
int newLine = 10;        // 10 is binary for 'return' or 'new line'.

float overheatTemp = defaultOverheatTemp;             // Modifiable Temperature (Farenheit) that triggers overheat status. Can be modified on 'Settings' tab
float voltageThreshold = defaultVoltageThreshold;     // Modifiable Voltage (in Volts)   that triggers overVoltage status. Can be modified on 'Settings' tab
float currentThreshold = defaultCurrentThreshold;     // Modifiable Current (in Amps)    that triggers overCurrent status. Can be modified on 'Settings' tab

/* JSON VARIABLES */
JSONObject parsedJson;
String jsonPackage = null;    // string that stores serialized incoming json
boolean sent;

/* NODE VARIABLES */
int nodeDisplayed;
int nodeUpdated;


void setup() {
  mySerial = new Serial(this, "COM4", 9600);
  
  /* General setup of the window */
  size(1600, 900);  // siza deez app windows (x and y dimensions of application window)
  font = loadFont("ArialMT-18.vlw");
  textFont(font);
  
  /* Creating array of data objects to record each node's data */
  for (int i = 0; i < NUMNODES; i++) {
    dataArray[i] = new data();
  }

  controlP5Setup();     // initiate all the buttons/tabs
}

/* Function: controlP5Setup()
*   initializes all the buttons and tabs displayed
*/
void controlP5Setup() {
  cp5 = new ControlP5(this);
  font = loadFont("ArialMT-19.vlw");
  ControlFont controlFont = new ControlFont(font);
  cp5.setFont(controlFont);
  
   cp5.addTab("default")      // The name of this tab is 'default'. You can change the display value with '.setLabel'
    .setLabel("Overview")     // The displayed text on the tab
    .setHeight(76)
    .setWidth(115)
    .setColorActive(URBANA_ORANGE)
    .setColorBackground(UOFI_BLUE)
    .setColorForeground(UOFI_BLUE_HOVER)
    .activateEvent(true)
    .setId(1)
    //.getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER)
    ;
  cp5.addTab("Monitor")      // I didn't change the 'label' (displayed text) because I'm satisifed with what's on here
     .setHeight(76)
     .setWidth(115)
     .setColorActive(URBANA_ORANGE)
     .setColorBackground(UOFI_BLUE)
     .setColorForeground(UOFI_BLUE_HOVER)
     .activateEvent(true)
     .setId(2)
     //.getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER)
     ;
  cp5.addTab("Settings")
    .setHeight(76)
    .setWidth(115)
    .setColorActive(URBANA_ORANGE)
    .setColorBackground(UOFI_BLUE)
    .setColorForeground(UOFI_BLUE_HOVER)
    .activateEvent(true)
    .setId(3)
    //.getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER)
    ;
  
  // Creating the array of panel buttons shown on the 'Monitor' tab
  int xpos = 120;
  int ypos = 120;
  int buttonsInRow = 5;     // creates a new line every x nodes. 
  for (int i = 0; i < NUMNODES; i++) {
    
    if(i % buttonsInRow == 0) {
      xpos = 100;
      ypos += 48;
    }
    
    cp5.addButton("panel" + (i + 1))
      .setLabel("" + (i + 1))
      .setPosition(xpos, ypos)
      .setSize(75, 45)
      .setId(i + 1)
      .moveTo("Monitor")
    ;
    xpos += 77;
  }

  cp5.addButton("enable")    // "enable" is the name of the button
    .setPosition(800, 800)
    .setSize(120, 50)
    .setColorBackground(color(23, 207, 23))
    .setColorForeground(color(46, 184, 46))
    .setColorActive(color(0, 200, 0))
    .moveTo("Monitor")
  ;
  cp5.addButton("disable")    // "disable" is the name of the button
    .setPosition(1000, 800)
    .setSize(120, 50)
    .setColorBackground(UIC_RED)
    .setColorForeground(color(195, 34, 34))
    .setColorActive(color(220, 0, 0))
    .moveTo("Monitor")
  ;
  
  
  // Creating the various buttons on 'Settings' tab
  cp5.addTextfield("changeOverheatThresh")
    .setLabel("Overheat Temperature Value (F)")
    .setPosition(1000, 500)
    .setSize(120, 50)
    .moveTo("Settings")
    .setColorCaptionLabel(color(0, 0, 0)) 
  ;
  
  
  
  
  
  
  
   cp5.addButton("defaultSettings")    // "defaultSettings" is the name of the button
    .setLabel("Set to Defaults")    // "Set to Defaults" is the text displayed
    .setPosition(1000, 800)
    .setSize(200, 50)
    .setColorBackground(UIC_RED)
    .setColorForeground(color(195, 34, 34))
    .setColorActive(color(220, 0, 0))
    .moveTo("Settings")
  ;
}

/* Function: draw()
*   Acts as the 'loop' function from arduino IDE. creates all the shapes/text on the screen
*/
void draw() {
    background(232, 233, 234);
    stroke(UOFI_BLUE);    // The stroke is the OUTLINE of the shape/thing
    fill(UOFI_BLUE);      // the fill is the INSIDE COLOR of the shape/thing
    rect(0, 0, width, 75);        // Blue header rectangle

    updateData();

    // Will only display the temperatures on the default ('Monitor') tab
    if (activeTab == 2) {
        
        
        updateDisplayData();
    } else if (activeTab == 1 || activeTab == 0) {
        
    }
}


/* Function: controlEvent()
*   Handles all events from controlP5 items, such as buttons and tabs. Button Id's 1-NUMNODES are dedicated to the solar panel buttons
*/
void controlEvent(ControlEvent theControlEvent) {
  if (theControlEvent.isTab()) {
    println("event from tab : "+theControlEvent.getTab().getName()+" with id "+theControlEvent.getTab().getId());
    activeTab = theControlEvent.getTab().getId();
  } else if (theControlEvent.isAssignableFrom(Textfield.class)) {
  
    
  } else if (theControlEvent.getId() > 0 && theControlEvent.getId() <= NUMNODES) {
    nodeDisplayed = theControlEvent.getId();
    println("switched to node " + nodeDisplayed);
  }
}


/* Function updateDisplayData()
*   If on the 'Monitor' tab, this function displays the node data for the selected node
*/
void updateDisplayData() {
    if (nodeDisplayed > 0 && nodeDisplayed <= NUMNODES) {
        fill(UOFI_BLUE);
        stroke(UOFI_BLUE);
        rect(1400, height / 3, 125, height / 3);
        float[] temps = dataArray[nodeDisplayed - 1].getTempsFarenheit();

        fill(0, 0, 0);
        text("Temp 1: ", 700, 150); 
        text(temps[0], 800, 150);
        text("Temp 2: ", 700, 200);
        text(temps[1], 800, 200);
        text("Temp 3: ", 700, 250); 
        text(temps[2], 800, 250);
        text(nodeDisplayed, 900, 250);
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
        
        try {
            if (sent == true) {          // If the jsonPackage was parsed correnctly, this works
              dataArray[nodeUpdated - 1].setTemps(parsedJson.getFloat("T1"), parsedJson.getFloat("T2"), parsedJson.getFloat("T3"));
              // dataArray[nodeUpdated - 1].setVoltage(parsedJson.getFloat("V"));
              // dataArray[nodeUpdated - 1].setCurrent(parsedJson.getFloat("C"));
              
              float[] tempArray = {parsedJson.getFloat("T1"), parsedJson.getFloat("T2"), parsedJson.getFloat("T3")};
  
              boolean overHeat = tempArray[0] >= overheatTemp || tempArray[1] >= overheatTemp || tempArray[2] >= overheatTemp;
              boolean overVolt = false;     //= parsedJson.getFloat("V") >= overVoltageThreshold;
              boolean overCurr = false;     //= parsedJson.getFloat("C") >= overCurrentThreshold;
              if (overHeat || overVolt || overCurr) {
                  cp5.getController("panel" + nodeUpdated).setColorBackground(UIC_RED);
              } else  {
                  cp5.getController("panel" + nodeUpdated).setColorBackground(UOFI_BLUE);
              }
              sent = false;
            } else {
                cp5.getController("panel" + nodeUpdated).setColorBackground(COOL_GRAY_6);
            }
        } catch (Exception e) {
            println("Failed updating Data");
        }
    
    }
}


/* Function: enable()
*   this is linked to the 'enable' button under the 'Monitor' tab. when the button is clicked, this function
*   is automatically called. This sends out a JSON command telling whcih node to activate
*/
void enable() {
  println("on");
  mySerial.write("{NODE:" + nodeDisplayed + ",SHUTDOWN:" + false + "}");
}
/* Function: disable()
*   this is linked to the 'disable' button under the 'Monitor' tab. when the button is clicked, this function
*   is automatically called. This sends out a JSON command telling whcih node to deactivate
*/
void disable() {
  println("off");
  mySerial.write("{NODE:" + nodeDisplayed + ",SHUTDOWN:" + true + "}");
}


/* Function: defaultSettings()
*   this is linked to the 'defaultSettings' button under the 'Settings' tab. when the button is clicked, this function
*   is automatically called. This sets the overheat, overVoltage, and overCurrent thresholds to their default values
*/
void defaultSettings() {
  println("reset settings to default");
  
  overheatTemp = defaultOverheatTemp;
  voltageThreshold = defaultVoltageThreshold;
  currentThreshold = defaultCurrentThreshold; 
}
/* Function: changeOverheatThresh(String inputValue)
*   this is linked to the 'defaultSettings' button under the 'Settings' tab. when the button is clicked, this function
*   is automatically called. This sets the overheat, overVoltage, and overCurrent thresholds to their default values
*/
void changeOverheatThresh(String inputValue) {
  println("changed oveheat threshold to " + inputValue);
  
  overheatTemp = Integer.valueOf(inputValue);
}

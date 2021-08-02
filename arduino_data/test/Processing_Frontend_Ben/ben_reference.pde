import processing.serial.*;  // including the serial object libarary
Button newButton;
JSONObject json;

/* SERIAL VARIABLES */
Serial mySerial;  // creating new serial object
String jsonPackage = null;    // string that stores serialized incoming json
int newLine = 10;        // 10 is binary for 'return' or 'new line'.
float temp1 = 0.0;  // test variable
float temp2 = 0.0;  // test variable
float temp3 = 0.0;  // test variable

/* BUTTON VARIABLES */

void setup() {
  size(1200, 600);  // siza deez app windows (x and y dimensions of application window)
  mySerial = new Serial(this, "COM4", 9600);
  newButton = new Button(100, 100, 100, 50, "Node 1", 0, 200, 200);
}

void draw() {
  newButton.update();
  newButton.render();
  
  if (newButton.isClicked() == true) {
    newButton.Color = color(213, 216, 220);
    delay(50);
  } else {
    newButton.Color = color(0, 200, 200);
  }
  while (mySerial.available() > 0) {
    jsonPackage = mySerial.readStringUntil(newLine);    // reads until it receives the 'new line' (10) signal
    if (jsonPackage != null) {
      String newJsonString = jsonPackage.substring(0, jsonPackage.length() - 2);
      println(newJsonString);
      try {
        json = parseJSONObject(newJsonString);
        float NODE = json.getFloat("NODE");
        print("NODE: ");
        println(NODE);
        background(255, 255, 255);
        fill(0, 102, 153);
        text("Temp 1: ", 400, 90); 
        text(temp1, 500, 90);
        text("Temp 2: ", 400, 120);
        text(temp2, 500, 120);
        text("Temp 3: ", 400, 150); 
        text(temp3, 500, 150);
        text(NODE, 600, 150);
        temp1 = json.getFloat("T1");
        temp2 = json.getFloat("T2");
        temp3 = json.getFloat("T3");
      } catch (Exception e) {
        println("failed... womp womp :(");
      }
      println("---------------------------------------------");
    }
    
  }
  
}
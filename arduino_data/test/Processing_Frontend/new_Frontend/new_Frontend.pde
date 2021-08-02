import processing.serial.*;  // including the serial object libarary
Button newButton;
JSONObject json;

/* SERIAL VARIABLES */
Serial mySerial;  // creating new serial object
String jsonPackage = null;    // string that stores serialized incoming json
int newLine = 10;        // 10 is binary for 'return' or 'new line'.
float temperature;  // test variable

/* BUTTON VARIABLES */

void setup() {
  size(1200, 600);  // siza deez app windows (x and y dimensions of application window)
  mySerial = new Serial(this, "COM3", 9600);
  newButton = new Button(100, 100, 100, 50, "Node 1", 0, 200, 200);
}

void draw() {
  newButton.update();
  newButton.render();
  text("Temp 1: ", 400, 90); 
  fill(0, 102, 153);
  text("Temp 2: ", 400, 120);
  fill(0, 102, 153);
  text("Temp 3: ", 400, 150); 
  
  
  if (newButton.isClicked() == true) {
    newButton.Color = color(213, 216, 220);
    delay(50);
  } else {
    newButton.Color = color(0, 200, 200);
  }
  while (mySerial.available() > 0) {
    jsonPackage = mySerial.readStringUntil(newLine);    // reads until it receives the 'new line' (10) signal
    String data = "{ \"id\": 0, \"species\": \"Panthera leo\", \"name\": \"Lion\"}";
    //try {
    //  json = parseJSONObject(data);
    //  String species = json.getString("species");
    //  println(species);
    //} catch (Exception e) {
    //  print("failed");
    //}
    //JSONObject json = parseJSONObject(jsonPackage);
    if (jsonPackage != null) {
      print(jsonPackage);
    }
  }
}

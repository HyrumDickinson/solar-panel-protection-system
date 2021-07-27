# This file contains a log of noteworthy code changes in any branch since 6-7-2021 and is embedded with a to do list.
btw you don't need a text editor for this file; GitHub already has a text editor and a great Markdown previewer built into the website,
just click that pencil button in the upper right corner when you open this file in GitHub online

### 6-2-2021
- [X] Hyrum Dickinson - ported code to this new repo & deleted old one
- [X] Hyrum Dickinson - Main branch - made changes to README
- [ ] this is what an empty box would look like

### 6-15-2021
- [X] **Hyrum Dickinson** - `python-2-version` - fixed syntax errors and made slight changes to imports for the GUI code. Previously, the code wouldn't run. Now, it runs responsively in a correctly set up python environment (see python-2-version README)
- [X] **Hyrum Dickinson** - `python-3-version` - made parallel changes as to python-2-version. When ran on python 3 instead of 2, the GUI continues to run extremely slowly
- [X] **Ben Olaivar** - `arduino_dev` - fixed analog pin in dhcp_test
- [X] **Ben Olaivar** - `arduino_dev` - ethernet connection is working
- [X] **Ben Olaivar** - `arduino_dev` - converted DHCP_test to command-based instead of letter based (allows for future expansion)
- [X] **Ben Olaivar** - `arduino_dev` - simplified ip confirmation and added space for future commands
- [ ] **Ben Olaivar** - `arduino_dev` - Try to get 'enter/return' to work instead of using ','
- [ ] **Ben Olaivar** - `arduino_dev` - Incorporate temp/voltage sensors

### 6-17-2021
- [X] **Hyrum Dickinson** - `arduino_dev` - added folder to arduino_data section with (unfinished) instructions on connecting arduino to wifi. 

### 6-18-2021
- [X] **Ben Olaivar** - `arduino_dev` - made placeholder functions for voltage/current sensor
- [X] **Ben Olaivar** - `arduino_dev` - made changes to main.cpp to account for voltage sensor test functions
- [X] **Ben Olaivar** - `arduino_dev` - fixed bug with DHCP_test that would cause infinite loop with multiple commands
- [X]Webserver? (no. Simply won't work well with both ethernet and wifi card)
- [ ]review conversion numbers

### 6-21-2021
- [X] **Ben Olaivar** - `arduino_dev` - additional thresh commands added to DHCP_test and main
- [X] **Ben Olaivar** - `arduino_dev` - successfully tested thresh functions
- [ ] **Ben Olaivar** - `arduino_dev` - may want to find a way for ethernet connection to be accessed anywhere (can currently only be accessed through local network)
- [X] **Ben Olaivar** - `arduino_dev` - need to order adapters for wifi card (currently having trouble uploading);

### 6-22-2021
- [X] **Hyrum Dickinson** - `main` - relabeled parts of the GUI, increased text size for a header, made some boxes and their text labels line up, and made other minor cosmetic GUI improvements.

### 6-23-2021
- [X] **Hyrum Dickinson** - `main` - resized GUI and moved some widgets so everything displays completely. A graph was partially cut off, beforehand.
- [X] **Hyrum Dickinson** - `main` - renamed some python files. All files that reference the renamed files were also updated.
- [X] **Hyrum Dickinson** - `main` - removed redundant imports of project files and python libraries. Several files were importing the same file or library more than once. Not any more. I also added comments to the import section of several files to show which libraries were already being imported from other file imports, to futureproof the program against coders unintentially adding redundant imports in the future. Since the cleanup, there is a noticable improvement in the time it takes our application to load.

### 6-25-21
- [X] **Ben Olaivar** - `arduino_dev` - Started temperature sensor code
- [X] **Ben Olaivar** - `arduino_dev` - successfully implemented threshold for single temp sensor
- [X] **Ben Olaivar** - `arduino_dev` - got adapters for wifi card...having difficulties
- [ ] **Ben Olaivar** - `arduino_dev` - wifi card needs network name and password to connect to network...university wifi has name, username, and password??
- [ ] **Ben Olaivar** - `arduino_dev` - ask seniors/IT for help
- [X] **Hyrum Dickinson** - `main` - added active button highlighting. Now, when an interactive element of the GUI is interacted with, the user will receive visual feedback letting them know that the GUI has received their input.

### 6-26-2021
- [X] **Hyrum Dickinson** - `main` - overhauled GUI color system backend to make it customizable and consistent. Now, each interactive element of the GUI has the same default background, same active background, same text color, etc. Each one can be changed for all interactive elements with a single line of code in Settings.py.

### 6-28-2021
- [X] **Ben Olaivar** - `arduino_dev` - added "temperature_ethernet.cpp" for ethernet development
- [X] **Ben Olaivar** - `arduino_dev` - successfully implimented multiple sensor readings for temperature sensors (add as many as you want...the libarary detects everything you connect)
- [X] **Ben Olaivar** - `arduino_dev` - connected temperature sensors to ethernet. Can now read data from TELNET server...no threshold implementation
- [X] **Ben Olaivar** - `arduino_dev` - removed DHT temp sensor files. They were only for testing and are no longer needed
- [X] **Ben Olaivar** - `arduino_dev` - implement temp thresholds into code
- [ ] **Ben Olaivar** - `arduino_dev` - convert data to JSON. Current implimintation will send one at a time. We want to push everything to the server at once

### 6-29-2021
- [X] **Ben Olaivar** - `arduino_dev` - successfully implimented array-based functionality in threshold_test.cpp
- [X] **Ben Olaivar** - `arduino_dev` - removed client dependency and print statements in "readTemperature" function
- [X] **Ben Olaivar** - `arduino_dev` - tweaked print statements on overheat
- [X] **Ben Olaivar** - `arduino_dev` - TEMP_THRESH can now be modified. If the system meets this value, it will change to a lower number. This allows for the system to cool down to a lower temperature before resuming functions
- [ ] **Ben Olaivar** - `arduino_dev` - Confirm JSON formatting before proceeding
- [ ] **Ben Olaivar** - `arduino_dev` - Find way to connect without being on same network (Consider options other than TELNET?). Current system is entirely dependent on ethernet connection. No wifi yet
- [ ] **Ben Olaivar** - `arduino_dev` - Speak to Kevin/IT about getting IP's for each arduino, as well as setting up wireless card (network name and password).

### 6-30-2021
- [X] **Hyrum Dickinson** - `main` - made connection status display even if no connection
- [ ] need to figure out how to delete old statuses instead of having them pile on top of each other
- [ ] need to figure out why two "disconnected" statuses pop up instead of one, starting with the second `sync` button click. It would make more sense to just display one per click.

### 7-01-2021
- [X] **Ben Olaivar** - `arduino_dev` - Finished JSON setup for temperature data reading
- [X] **Ben Olaivar** - `arduino_dev` - still not sure how I'm going to test the ADC without taking it off the board. May ask Kevin if I can rip it off the old PCB (Old PCB doesn't work anyways, right?)
- [X] **Ben Olaivar** - `arduino_dev` - Wifi module may only be able to connect to the NETGEAR wifi. After researching for solutions to login issue, nothing has come up. Either do netgear or setup a new system if you wanna connect wirelessly. As of now I'm going to focus on ethernet because we know it works
- [X] **Ben Olaivar** - `arduino_dev` - how is the arduino going to connect to the main board? Does the hardware team even know I'm doing this arduino thing? This arduino + ethernet shield is going to be doing 90% of the stuff the old board did. Only thing we need the old PCB for is the relays and ADC
- [ ] **Ben Olaivar** - `arduino_dev` - in general, talk to kevin. Need IP's for all boards, need confirmation of ADC removal, need confirmation of board hookup, need information on wireless commmunication. 
- [ ] **Ben Olaivar** - `arduino_dev` - found out the current board only connect to screw terminals A-C. D-F literally don't connect to anything and are just for show 

### 7-01-2021
- [X] **Ben Olaivar** - `arduino_dev` - IDEA IDEA IDEA: Instead of using a bunch of ethernet cables/a strong router/multiple routers, we use a bunch of ESP8266 (preferably NodeMCU) boards, which can create a mesh system. These boards may even be able to support the ENTIRE software w/o an arduino. Each node will send and receive to/from every other nearby node. If a node disconnects it won't affect child nodes (it's not a linear connection) because everything connects to everything. We can send JSON strings across everything and analyze them from there. https://randomnerdtutorials.com/esp-mesh-esp32-esp8266-painlessmesh/ : called painlessMesh
- [X] **Ben Olaivar** - `arduino_dev` - We'll also have to decide how we want the code to run. If a node doesn't connect, do we shut that panel down? I'd prefer finding a way for each node/panel to automate itself even if we can't read it. ALSO we'll want a box/node dedicated to uploading all the data. Example: https://github.com/gmag11/painlessMesh/blob/master/examples/mqttBridge/mqttBridge.ino
- [X] **Ben Olaivar** - `arduino_dev` - ESP8266 can support 4 stations for each access point, while the ESP32 can support 10 stations per access point
https://github.com/gmag11/painlessMesh/tree/master/examples

### 7-08-2021
- [X] **Ben Olaivar** - `arduino_dev` - DECIDED: no mesh system. Direct tranceiver communication between main box and nodes. The main box reads each node one at a time and sends the data to a desktop through TELNET. There are only research boxes, so this shouldn't be too demanding on the system. More reliable than the mesh system
- [X] **Ben Olaivar** - `arduino_dev` - added transceiver_node.cpp and transceiver_main.cpp. Still needs to be tested

### 7-11-2021
- [X] **Hyrum Dickinson** - `main` - Added powershell script to automate python environment creation.

### 7-22-2021
- [X] **Hyrum Dickinson** - `main` - Deleted dist and venv directories because we don't need them.
- [X] **Hyrum Dickinson** - `main` - Fixed issue where button text changed color while being clicked.
- [X] **Hyrum Dickinson** - `main` - Fixed issue where the graph header permanently changed color when graph selections were made.

### 7-23-2021
- [X] **Hyrum Dickinson** - `main` - Made all text, including active text, colored by the same local variable.
- [X] **Hyrum Dickinson** - `main` - Isolated graph background color from other local color variables.
- [X] **Hyrum Dickinson** - `main` - Made all entire widget backgrounds colored by same local variable.
- [X] **Hyrum Dickinson** - `main` - Updated color names to reflect their new functions.
- [X] **Hyrum Dickinson** - `main` - Created space between bottom buttons and bottom of data widget.
- [X] **Hyrum Dickinson** - `main` - Made space between buttons and edge of widget the same for all edge buttons.
- [X] **Hyrum Dickinson** - `main` - Increased width of sync button so that main background is the same thickness between widgets as it is around widgets.
- [X] **Hyrum Dickinson** - `main` - Thinned the outline around widgets.
- [X] **Hyrum Dickinson** - `main` - Made all widget backgrounds light blue.
- [X] **Hyrum Dickinson** - `main` - Made all widget outlines and text black.
- [X] **Hyrum Dickinson** - `main` - Made main background color dark blue.
- [X] **Hyrum Dickinson** - `main` - Reformatted connection status messages to compliment new light blue background.
- [X] **Hyrum Dickinson** - `main` - Set checkboxes to highlight larger area when pressed.
- [X] **Hyrum Dickinson** - `main` - Made graph fit with new aesthetic.
- [ ] **Hyrum Dickinson** - `main` - Need Kevin to choose main background color from four ready-to-go options. Once the color scheme is decided, the GUI look and feel overhaul will be complete. I still have some functionality quicks to fix, but the user interface of the GUI should stay the same from here on out - once Kevin's picked the color scheme.

### 7-26-2021
- [X] **Ben Olaivar** - `arduino_dev` - forgot to update WRITEME. What has been done so far is listed below
- [X] **Ben Olaivar** - `arduino_dev` - nrf24L01 transceiver has been chosen as best mode of communication. A main node by the door will send a data request each second to a new node, then receive and acknowledgement response containing all the data from the node.
- [X] **Ben Olaivar** - `arduino_dev` - The node end only has temperature sensor readings at this point. I still don't have acces to the voltage/current sensor, so reliably testing the code is difficult, although original code looks promising. This also goes for the relay control
- [X] **Ben Olaivar** - `arduino_dev` - The main node successfully works with ethernet sending data. It needs to be updated to receive commands and send them to their respective panel. Additionally, it needs to be updated to automatically send shutdown commands.
- [ ] **Ben Olaivar** - `arduino_dev` - Encorporate relay control and voltage/current sensor code on node size
- [ ] **Ben Olaivar** - `arduino_dev` - Encorporate commands on both node and main boxes. (node may only need shutdown command)


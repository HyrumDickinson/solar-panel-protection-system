# This file contains a log of noteworthy code changes in any branch since 6-7-2021 and is embedded with a to do list.
btw you don't need a text editor for this file; GitHub already has a text editor and a great Markdown previewer built into the website,
just click that pencil button in the upper right corner when you open this file in GitHub online

### 6-2-2021
- [X] Hyrum Dickinson - ported code to this new repo & deleted old one
- [X] Hyrum Dickinson - Main branch - made changes to README
- [ ] this is what an empty box would look like

### 6-15-2021
- [X] **Ben Olaivar** - `arduino_dev` - fixed analog pin in dhcp_test
- [X] **Ben Olaivar** - `arduino_dev` - ethernet connection is working
- [X] **Ben Olaivar** - `arduino_dev` - converted DHCP_test to command-based instead of letter based (allows for future expansion)
- [X] **Ben Olaivar** - `arduino_dev` - simplified ip confirmation and added space for future commands
- [ ] **Ben Olaivar** - `arduino_dev` - Try to get 'enter/return' to work instead of using ','
- [ ] **Ben Olaivar** - `arduino_dev` - Incorporate temp/voltage sensors

### 6-18-2021
- [X] **Ben Olaivar** - `arduino_dev` - made placeholder functions for voltage/current sensor
- [X] **Ben Olaivar** - `arduino_dev` - made changes to main.cpp to account for voltage sensor test functions
- [X] **Ben Olaivar** - `arduino_dev` - fixed bug with DHCP_test that would cause infinite loop with multiple commands
- [X]Webserver? (no. Simply won't work well with both ethernet and wifi card)
- [ ]review conversion numbers

### 6-18-2021
- [X] **Ben Olaivar** - `arduino_dev` - additional thresh commands added to DHCP_test and main
- [X] **Ben Olaivar** - `arduino_dev` - successfully tested thresh functions
- [ ] **Ben Olaivar** - `arduino_dev` - may want to find a way for ethernet connection to be accessed anywhere (can currently only be accessed through local network)
- [X] **Ben Olaivar** - `arduino_dev` - need to order adapters for wifi card (currently having trouble uploading);

### 6-25-21
- [X] **Ben Olaivar** - `arduino_dev` - Started temperature sensor code
- [X] **Ben Olaivar** - `arduino_dev` - successfully implemented threshold for single temp sensor
- [X] **Ben Olaivar** - `arduino_dev` - got adapters for wifi card...having difficulties
- [ ] **Ben Olaivar** - `arduino_dev` - wifi card needs network name and password to connect to network...university wifi has name, username, and password??
- [ ] **Ben Olaivar** - `arduino_dev` - ask seniors/IT for help

### 6-28-2021
- [X] **Ben Olaivar** - `arduino_dev` - added "temperature_ethernet.cpp" for ethernet development
- [X] **Ben Olaivar** - `arduino_dev` - successfully implimented multiple sensor readings for temperature sensors (add as many as you want...the libarary detects everything you connect)
- [X] **Ben Olaivar** - `arduino_dev` - connected temperature sensors to ethernet. Can now read data from TELNET server...no threshold implementation
- [X] **Ben Olaivar** - `arduino_dev` - removed DHT temp sensor files. They were only for testing and are no longer needed
- [X] **Ben Olaivar** - `arduino_dev` - implement temp thresholds into code
- [ ] **Ben Olaivar** - `arduino_dev` - convert data to JSON. Current implimintation will send one at a time. We want to push everything to the server at once

### 6-28-2021
- [X] **Ben Olaivar** - `arduino_dev` - successfully implimented array-based functionality in threshold_test.cpp
- [X] **Ben Olaivar** - `arduino_dev` - removed client dependency and print statements in "readTemperature" function
- [X] **Ben Olaivar** - `arduino_dev` - tweaked print statements on overheat
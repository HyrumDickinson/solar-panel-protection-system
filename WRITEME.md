# WRITEME

### 6-15-2021
- [X] **Hyrum Dickinson** - `python-2-version` - fixed syntax errors and made slight changes to imports for the GUI code. It now runs responsively in a correctly set up python environment (see python-2-version README)
- [X] **Hyrum Dickinson** - `python-3-version` - made parallel changes as to python-2-version. When ran on python 3 instead of 2, the GUI continues to run extremely slowly
- [X] **Ben Olaivar** - `arduino_dev` - fixed analog pin in dhcp_test
- [X] **Ben Olaivar** - `arduino_dev` - ethernet connection is working
- [X] **Ben Olaivar** - `arduino_dev` - converted DHCP_test to command-based instead of letter based (allows for future expansion)
- [X] **Ben Olaivar** - `arduino_dev` - simplified ip confirmation and added space for future commands
- [ ] Try to get 'enter/return' to work instead of using ','
- [ ] Incorporate temp/voltage sensors 

### 6-16-2021
- [X] **Hyrum Dickinson** - `python_2_version` - changed some GUI display labels per Kevin's request
- [ ] **Hyrum Dickinson** - `python_3_version` - still can't figure out why code won't run in python 3. Porting the code is starting to seem like a waste of time. I'll probably just leave it in the old version. 
- [ ] **Hyrum Dickinson** - `arduino_dev` - my desktop don't have onboard wifi and IT apparently doesn't want us to use routers. Will get a USB Wifi Adapter from ECE store tomorrow so I can practice connecting to arduino.

### 6-17-2021
- [X] **Hyrum Dickinson** - `arduino_dev` - added folder to arduino_data section with (unfinished) instructions on connecting arduino to wifi. 

### 6-18-2021
- [X] Ben Olaivar - made placeholder functions for voltage/current sensor
- [X] Ben Olaivar - made changes to main.cpp to account for voltage sensor test functions
- [X] Ben Olaivar - fixed bug with DHCP_test that would cause infinite loop with multiple commands
- [ ] Webserver?
- [ ] review conversion numbers

### 6-21-2021
- [X] Ben Olaivar - additional thresh commands added to DHCP_test and main
- [X] Ben Olaivar - successfully tested thresh functions
- [ ] may want to find a way for ethernet connection to be accessed anywhere (can currently only be accessed through local network)
- [ ] need to order adapters for wifi card (currently having trouble uploading);

### 6-23-2021
- [X] **Hyrum Dickinson** - `main` - fixed issue where parts of GUI weren't displaying & improved cosmetics
- [X] **Hyrum Dickinson** - `main` - renamed some python files to make them more self-explanatory. All files that reference the renamed files were also updated.
- [X] **Hyrum Dickinson** - `main` - removed redundant imports. Several files were importing the same file or library more than once. not any more
# This file contains a log of noteworthy code changes in any branch since 6-7-2021 and is embedded with a to do list.
btw you don't need a text editor for this file; GitHub already has a text editor and a great Markdown previewer built into the website,
just click that pencil button in the upper right corner when you open this file in GitHub online

### 6-2-2021
- [X] Hyrum Dickinson - ported code to this new repo & deleted old one
- [X] Hyrum Dickinson - Main branch - made changes to README
- [ ] this is what an empty box would look like

### 6-15-2021
- [X] Ben Olaivar - fixed analog pin in dhcp_test
- [X] Ben Olaivar - ethernet connection is working
- [X] Ben Olaivar - converted DHCP_test to command-based instead of letter based (allows for future expansion)
- [X] Ben Olaivar - simplified ip confirmation and added space for future commands
- [ ] Try to get 'enter/return' to work instead of using ','
- [ ] Incorporate temp/voltage sensors

### 6-18-2021
- [X] Ben Olaivar - made placeholder functions for voltage/current sensor
- [X] Ben Olaivar - made changes to main.cpp to account for voltage sensor test functions
- [X] Ben Olaivar - fixed bug with DHCP_test that would cause infinite loop with multiple commands
- [ ] Webserver?
- [ ] review conversion numbers

### 6-18-2021
- [X] Ben Olaivar - additional thresh commands added to DHCP_test and main
- [X] Ben Olaivar - successfully tested thresh functions
- [ ] may want to find a way for ethernet connection to be accessed anywhere (can currently only be accessed through local network)
- [ ] need to order adapters for wifi card (currently having trouble uploading);
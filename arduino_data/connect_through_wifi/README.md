### How to connect Arduino Uno Wifi Rev 2 to Windows 10 desktop through ad.uillinois.edu wifi 

#### Install WifiNINA library
Use your local arduino app's library manager

#### Check that WifiNINA firmware is up to date on Arduino board
Run `CheckWiFiNINAFirmwareVersion.ino`

#### If necessary, update firmware
Follow instructions on <https://www.arduino.cc/en/Tutorial/WiFiNINAFirmwareUpdater>  
This may take a while

#### Enable connection to WPA2-Enterprise encrpyted wifi network (i.e. IllinoisNet)
Create your own arduino_secrets.h file using arduino_secrets_template.h (see <https://www.andreagrandi.it/2020/12/16/how-to-safely-store-arduino-secrets/> or arduino forum for guidance)  
Run `ConnectToWPA2EnterpriseEncryptedWifiNetwork.ino`


To be continued...


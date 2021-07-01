================================================================================================
GENERAL
================================================================================================

All PCB's were made using EAGLE 9.5.0
All PCB's were ordered through PCBWay.
Latest Gerber files were zipped and uploaded in each project folder.
For the BOM including parts to populate the PCB's, see the Documentation folder on Box.

================================================================================================
PCBWAY ORDER INFORMATION
================================================================================================

Gerber Files: smartInterface.zip
Board Type: Single Pieces
Different Design in Panel: 1
Size: 100 mm x 100 mm
Quantity: 10
Number of Layers: 4
Layer Stack:
 - L1: smartInterface.top
 - L2: smartInterface.ly2
 - L3: smartInterface.ly3
 - L4: smartInterface.bot
Material: FR-4
FR4-TG: TG 150-160
Thickness: 1.6
Min Track/Spacing: 6/6 mil
Min Hole Size: 0.3 mm
Solder Mask: Red
Silkscreen: White
Gold Fingers: No
Surface Finish: HASL with lead
Via Process: Tenting Vias
Finished Copper: 1 oz Cu
Inner Copper: 1 oz

Gerber Files: switchBoard.zip
Board Type: Single Pieces
Different Design in Panel: 1
Size: 25mm x 100mm
Quantity: 10
Number of Layers: 2
Layer Stack:
 - L1: switchBoard.top
 - L2: switchBoard.bot
Material: FR-4
FR4-TG: TG 150-160
Thickness: 1.6
Min Track/Spacing: 6/6 mil
Min Hole Size: 0.3 mm
Solder Mask: Red
Silkscreen: White
Gold Fingers: No
Surface Finish: HASL with lead
Via Process: Tenting Vias
Finished Copper: 1 oz Cu
Inner Copper: 1 oz

================================================================================================
BURNING THE BOOTLOADER - DO THIS BEFORE MOUNTING THE ATMEGA328P ON THE PCB
================================================================================================
To use the firmware that was originally written for the system, the Arduino bootloader should be burned onto the ATmega328P.

Here are resources to burn the bootloader:
https://www.arduino.cc/en/Tutorial/ArduinoToBreadboard
https://www.arduino.cc/en/Main/Standalone

================================================================================================
FLASHING FIRMWARE
================================================================================================
After burning the bootloader into the ATmega328P, the chip can be placed on the PCB.
The PCB was designed so that the microcontroller could be programmed without removing the chip.
Near the Ethernet connector is a 6 pin header that is used for programming.

The system was designed to be flashed with an FTDI Basic Breakout board from Sparkfun Electronics:
https://www.sparkfun.com/products/9716

Connect this FTDI board to the 6 pin header, and connector a USB cable from the FTDI board to your computer.
You can then use the Arduino IDE to flash any desired Arduino firmware to the system.




# Solar Panel Protection System

## Project overview
This code represents the software portion of an ongoing effort to build a tool that monitors solar energy systems and keeps panels from overheating.

This project is funded by the University of Illinois Department of Electrical & Computer Engineering and is based out of the Center for Electric Machinery & Electromechanics. The project lies under the direction of Arijit Banerjee, Assistant Professor in ECE. The software portion of the project is undergoing construction by undergraduate student workers Hyrum Dickinson and Benjamin Olaivar, supervised by Kevin Colravy, Research Engineer at CEME. Only the software portion of the solar panel protection system project is hosted on this repo.

The original solar panel protection system was created by then-undergraduate students Dillon Vadgama, Douglas Lee, and Sachin Reddy for their fall 2019 senior design project. Dubbed "Smart Interface for ECEB Research Solar Panels," the project won the Texas Instruments Best Project Award. Now, researchers from the University of Illinois are attempting to improve on their prototype to create a robust, scalable tool that can monitor entire solar energy systems and reliably keep individual panels from overheating.

This branch is primarily focused on the development of the onboard arduino code. This code will encompass everything from ethernet communication to sensor data reading. The majority of work done in this branch will be done under the 'arduino_data' directory, the 'test' files. Files in this directory are organized using platformio, which allows for convenient access/storage of libraries, test code, and other files. 

## Repository setup
### `main`
The latest stable, deployable version of the code is stored in the `main` branch.
### `python-2-version` and `python-3-version`
Until code is fully ported to Python 3, these branches are the `main` branches of the code in their respective language.
### `original`
It is important to note that the original code for this project is not our own. It was written by Dillon Vadgama, Douglas Lee, and Sachin Reddy as part of a senior design project. The exact original code, including the original README, is stored in this branch. 

## System tools  
### Software  
Microprocessor code - Arduino Language  
Original GUI - Python 2 & Tkinter  
New GUI - Processing 3  
README & WRITEME files - Markdown  
### Hardware (not included in repo)  
Arduino Uno with Wifi  
Custom PCB Arduino Shield  

## Old Python GUI
### Dependencies
- matplotlib  
- tkinter (Python 3) or Tkinter (Python 2)  
- sqlite3  
- pillow  
- datetime  
The original code was written to for PIL. PIL is now unsupported - its own website recommends against its use. Pillow is a maintained backwords-compatible fork of PIL.
### Run
run Main.py in your preferred Python environment
### Building
source venv/bin/activate  
python2 Setup.py py2app -A  

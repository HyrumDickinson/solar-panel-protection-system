# Solar Panel Protection System


## Overview
This branch is primarily focused on the development of the onboard arduino code. This code will encompass everything from ethernet communication to sensor data reading. The majority of work done in this branch will be done under the 'arduino_data' directory, the 'test' files. Files in this directory are organized using platformio, which allows for convenient access/storage of libraries, test code, and other files. 

## Branches

### `main`
The latest stable, deployable version of the code is stored in the `main` branch.
### `python-2-version` and `python-3-version`
Until code is fully ported to Python 3, these branches are the `main` branches the code in their respective language.
### `original`
It is important to note that the original code for this project is not our own. It was written by Dillon Vadgama, Douglas Lee, and Sachin Reddy as part of a senior design project. The exact original code, including the original README, is stored in this branch. 
### All others
All other repos exist for code editing purposes and are changed or deleted at the discretion of Hyrum Dickinson or Benjamin Olaivar.

## Languages

Microprocessor code - Arduino Language
Original GUI - Python 2 & Tkinter
New GUI - Processing 3
README & WRITEME files - Markdown

## Dependencies

- matplotlib
- tkinter (Python 3) or Tkinter (Python 2)
- sqlite3
- pillow
- datetime

The original code was written to for PIL. PIL is now unsupported - its own website recommends against its use. Pillow is a maintained backwords-compatible fork of PIL.

## Run



## Building

source venv/bin/activate
python2 Setup.py py2app -A

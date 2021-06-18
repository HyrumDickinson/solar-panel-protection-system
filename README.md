# Solar Panel Protection System


## Overview
This code represents the software portion of an ongoing effort to build a tool that monitors solar energy systems and keeps panels from overheating.

This project is funded by the University of Illinois Department of Electrical & Computer Engineering and is based out of the Center for Electric Machinery & Electromechanics. The project lies under the direction of Arijit Banerjee, Assistant Professor in ECE. The software is undergoing construction by undergraduate student workers Hyrum Dickinson and Benjamin Olaivar, supervised by Kevin Colravy, Research Engineer at CEME.

The original solar panel protection system was created by then-undergraduate students Dillon Vadgama, Douglas Lee, and Sachin Reddy for their senior design project in the fall of 2019. Dubbed "Smart Interface for ECEB Research Solar Panels," their project won the Texas Instruments Best Project Award. Now, researchers from the University of Illinois are attempting to improve on the original prototype to create a robust, scalable tool that can monitor entire solar energy systems and reliably keep individual panels from overheating.

## Branches

### `main`
The latest stable, deployable version of the code is stored in the `main` branch. Currently, Python 2 is the primary programming language of this project.

### `python-3-version`
Python 2 is an old langauge. Efforts are underway to port the GUI code to Python 3; however, there has been little success. 
Until code is fully ported to Python 3, these branches are the `main` branches the code in their respective language. Currently, the `main` 
Python 2 version runs very responsively in a python 2 anaconda environment, while Python-3-version remains extremely slow. The `python_3_version` branch may eventually merge into `main`, but only if it can be fixed to run as responsibly as the legacy Python 2 version of the GUI code.

### `original`
It is important to note that the original code for this project is not my own. It was written by Dillon Vadgama, Douglas Lee, and 
Sachin Reddy as part of their original design project. The exact original code, including the original README, is stored in this branch. 
Under no circumstances should anything in the `original` branch be edited.

The `original` branch is an exact copy of the files in <https://github.com/sachinreddy1/SolarPanelMonitor>. There may be instances 
over the course of this project where viewing his git commit history, rather than only his final project files, is useful.

#### All others
All other repos exist for code editing purposes and are changed or deleted at the discretion of Hyrum Dickinson or Benjamin Olaivar.

## Languages

- `main` : Python 2
- `python-3-version` : Python 3
- `original` : Python 2

All README and WRITEME files are written in Markdown.

## Dependencies

Create a new anaconda python 2 environment and install the following packages in order:
- pillow
- matplotlib

There were significant import issues with the code previously and with my anaconda installation, particularly with the Tkinter package. For whatever reason, 
the code worked when I completely reinstalled anaconda and did exactly what I instructed above, even though the same code would not work on my existing 
anaconda python 2 environment with all the `original` branch's recommended dependecies installed. 

The original code was written to run with PIL, not pillow. However, PIL is currently an unsupported package whose own website recommends against its use. 
Pillow is a maintained fork of PIL. It should be backwards compatible.

## Run

Run SolarPanelMonitor.py in your preferred python environment.

## Building

source venv/bin/activate  
python2 Setup.py py2app -A

**For questions about `Solar-Panel-Protection-System`, please contact Hyrum Dickinson at hyrumsd2@illinois.edu.**


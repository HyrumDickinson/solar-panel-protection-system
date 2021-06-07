# Solar Panel Protection System


## Overview
This code represents the software portion of an ongoing effort to build a tool that monitors solar energy systems and keeps panels from overheating.

This project is funded by the University of Illinois Department of Electrical & Computer Engineering and is based out of the Center for Electric Machinery & Electromechanics. The project lies under the direction of Arijit Banerjee, Assistant Professor in ECE. The software is undergoing construction by undergraduate student workers Hyrum Dickinson and Benjamin Olaivar, supervised by Kevin Colravy, Research Engineer at CEME.

The original solar panel protection system was created by then-undergraduate students Dillon Vadgama, Douglas Lee, and Sachin Reddy for their senior design project in the fall of 2019. Dubbed "Smart Interface for ECEB Research Solar Panels," their project won the Texas Instruments Best Project Award. Now, researchers from the University of Illinois are attempting to improve on the original prototype to create a robust, scalable tool that can monitor entire solar energy systems and reliably keep individual panels from overheating.

## Branches

### `main`
The latest stable, deployable version of the code is stored in the `main` branch.
### `python-2-version` and `python-3-version`
Until code is fully ported to Python 3, these branches are the `main` branches the code in their respective language.
### `original`
It is important to note that the original code for this project is not my own. It was written by Dillon Vadgama, Douglas Lee, and Sachin Reddy as part of their original design project. The exact original code, including the original README, is stored in this branch. Under no circumstances should anything in the `original` branch be edited.

The `original` branch is an exact copy of the files in <https://github.com/sachinreddy1/SolarPanelMonitor>. There may be instances over the course of this project where viewing his git commit history, rather than only his final project files, is useful.
#### All others
All other repos exist for code editing purposes and are changed or deleted at the discretion of Hyrum Dickinson or Benjamin Olaivar.

## Languages

- `main` : Python 3
- `python-3-version` : Python 3
- `python-2-version` : Python 2
- `original` : Python 2

All README files are written in Markdown.

## Dependencies

- matplotlib
- tkinter (Python 3) or Tkinter (Python 2)
- sqlite3
- pillow
- datetime

The original code was written to run with PIL, not pillow. However, PIL is currently an unsupported package whose own website recommends against its use. Pillow is a maintained fork of PIL, and should be backwards compatible.

## Run

Run SolarPanelMonitor.py in your preferred python environment.

## Building

source venv/bin/activate
python2 Setup.py py2app -A

**For questions about this README, the repository, or the code in it, please contact Hyrum Dickinson at hyrumsd2@illinois.edu.**

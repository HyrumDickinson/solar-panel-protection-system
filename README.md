# Solar Panel Monitor

Must use python2 and pip2 for installing packages.

## Dependencies

Create a new anaconda python 2 environment and install the following packages in the listed order:
- pillow
- matplotlib

There were significant import issues with the code previously and with my anaconda installation, particularly with the Tkinter package. For whatever reason, the code worked when I completely reinstalled anaconda and did exactly what I instructed above, even though the same code would not work on my existing anaconda python 2 environment with all the `original` branch's recommended dependecies installed. 

## Run
python2 SolarPanelMonitor.py

## Building
source venv/bin/activate
python2 Setup.py py2app -A

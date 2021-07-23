# AUTOMATED APPLICATION SETUP - THIS SCRIPT WILL GET SOLAR_PANEL_PROTECTION_SYSTEM RUNNING ON YOUR LOCAL COMPUTER

# If you have not already, please install Anaconda, Powershell, and Git. 
# This script runs in Anaconda Powershell Prompt, or it can run in regular Powershell if you have it set up to recognize conda commands.
# You can run the commands verbatim, one line at a time, or you can make a copy of this file on your computer and then tell your computer to run it in Anaconda Powershell Prompt.

git clone https://github.com/HyrumDickinson/Solar_Panel_Protection_System
# if you are not an co-owner of this repository, do not run the line above. 
# Instead, use your account at github.com to fork the URL, and then git clone the new URL created by your fork
conda create --name python2_solarpanels python=2
conda activate python2_solarpanels
conda install pillow
# type Y when prompted
conda install matplotlib
# type Y when prompted
python Main.py
# after running this command, the application should load onto your screen. 

# Disclaimer: This script has only been tested in Windows 10.
# Disclaimer: Tf you do not have a compatible hardware system setup to run in tandem with the GUI, this application will lack most of its functionality.

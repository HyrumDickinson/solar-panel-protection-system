import json
from Connector import * # this line imports the following: 
	 # import socket
     # import threading
     # from Connection import * # imports Settings
from Monitor import * # this line imports the following:
     # from PIL import Image, ImageTk
     # from Graph import *
      # import matplotlib
      # matplotlib.use("TkAgg")
      # from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2Tk
      # from matplotlib.figure import Figure
      # import matplotlib.animation as animation
      # from matplotlib import style
      # import Tkinter as tk
      # import sqlite3
      # from Settings import * # imports default trip points and color definitions
      # style.use("ggplot")
      # import matplotlib.dates as mdates
      # import numpy as np
      # import datetime
      # import time

BUFFER_SIZE = 1024

class Application:
	def __init__ (self):
		self.lastData = None
		self.lastIP = None

		self.command = None
		self.Monitor = None
		self.c = Connector()

	# ----------------- #

	def commands(self):
		self.conn = sqlite3.connect('solarPanel.db')
		cursor = self.conn.cursor()	
		
		# Check if table exists.
		cursor.execute(''' SELECT count(name) FROM sqlite_master WHERE type='table' AND name='voltages' ''')
		if cursor.fetchone()[0] == 0:
			cursor.execute("""CREATE TABLE voltages (
			timeRecorded integer,
			ip text,
			voltage_1 real,
			voltage_2 real,
			voltage_3 real,
			current_1 real,
			temperature_1 real,
			temperature_2 real,
			temperature_3 real,
			temperature_4 real,
			temperature_5 real,
			temperature_6 real
			)""")	

		while True:
			if self.lastData and self.command != 'sync' and self.command != 'quit':
				try:
					packet = json.loads(self.lastData)

					# Switch relay configuration if trip point reached
					currConnection = self.c.findConnection(self.lastIP)
					if currConnection != -1 and self.c.connections[currConnection].configSwitch != packet["S"] and packet["X"] != 0:
						self.Monitor.updateCheckbox(packet["S"])

					if packet["X"] != 0 and self.c.connections[currConnection].manualSwitch == 0:
						self.c.connections[currConnection].currentAck = 1
						
						if packet["X"] == 1:
							self.Monitor.widgetFrames[currConnection][2]['fg'] = RED
							self.Monitor.widgetFrames[currConnection][2]['text'] = 'Status: Over-Voltage'
						if packet["X"] == 2:
							self.Monitor.widgetFrames[currConnection][2]['fg'] = RED
							self.Monitor.widgetFrames[currConnection][2]['text'] = 'Status: Over-Current'
						if packet["X"] == 3:
							self.Monitor.widgetFrames[currConnection][2]['fg'] = RED
							self.Monitor.widgetFrames[currConnection][2]['text'] = 'Status: Over-Heating'

					if self.c.connections[currConnection].manualSwitch == 1:
						self.Monitor.updateCheckbox(packet["S"])

						if packet["X"] == 1:
							self.Monitor.widgetFrames[currConnection][2]['fg'] = RED
							self.Monitor.widgetFrames[currConnection][2]['text'] = 'Status: Over-Voltage'
						if packet["X"] == 2:
							self.Monitor.widgetFrames[currConnection][2]['fg'] = RED
							self.Monitor.widgetFrames[currConnection][2]['text'] = 'Status: Over-Current'
						if packet["X"] == 3:
							self.Monitor.widgetFrames[currConnection][2]['fg'] = RED
							self.Monitor.widgetFrames[currConnection][2]['text'] = 'Status: Over-Heating'

					cursor.execute("INSERT INTO voltages VALUES (:timeRecorded, :ip, :voltage_1, :voltage_2, :voltage_3, :current_1, :temperature_1, :temperature_2, :temperature_3, :temperature_4, :temperature_5, :temperature_6)", 
					{
					'timeRecorded': time.time(), 
					'ip': self.lastIP,
					'voltage_1': packet["V1"], 
					'voltage_2': packet["V2"], 
					'voltage_3': packet["V3"], 
					'current_1': packet["C1"],
					'temperature_1': packet["T1"],
					'temperature_2': packet["T2"],
					'temperature_3': packet["T3"],
					'temperature_4': packet["T4"],
					'temperature_5': packet["T5"],
					'temperature_6': packet["T6"]
					})
					self.conn.commit()
					self.lastData = None
					self.lastIP = None
				except ValueError as e:
					pass

			if self.command != None:
				if self.command == 'quit':
					self.conn.close()
					self.Monitor.root.quit()
					for i in self.c.connections:
						i.socket.close()
					return					
				if self.command == 'sync':
					self.c.clear()
					self.Monitor.clearWidgets()
					self.Monitor.updateWidgets()
					self.c.connect()
					self.Monitor.updateWidgets()
					for i in range(0, len(self.c.connections)):
						self.Monitor.updateCheckbox(i)

				self.command = None

	# ----------------- #

	def receiver(self):
		self.c.connect()
		self.Monitor.updateWidgets()

		while True:
			for i in self.c.connections:
				if i.isConnected and self.command != 'sync' and self.command != 'quit':
					# SEND
					data = {}
					data['V'] = i.voltageValue
					data['C'] = i.currentValue
					data['T'] = i.temperatureValue
					data['S'] = i.configSwitch
					data['M'] = i.manualSwitch
					data['ACK'] = i.currentAck
					i.socket.send(json.dumps(data))
					# RECEIVE
					try:
						self.lastData = i.socket.recv(BUFFER_SIZE)
						self.lastIP = i.ip
					except:
						print("Timed out.")
				elif not i.isConnected and self.command != 'sync' and self.command != 'quit':
					print("No connection")
			if self.command == 'quit':
				return

	# ----------------- #

	def inputting(self, command):
		self.command = command

	def TripPointInputting(self, voltageValue, currentValue, temperatureValue, i):
		if len(self.c.connections) == 0:
			return

		try:
			self.c.connections[i].voltageValue = max(min(float(voltageValue), MAX_VOLTAGE_TRIP_POINT), MIN_VOLTAGE_TRIP_POINT)
			self.c.connections[i].currentValue = max(min(float(currentValue), MAX_CURRENT_TRIP_POINT), MIN_CURRENT_TRIP_POINT)
			self.c.connections[i].temperatureValue = max(min(float(temperatureValue), MAX_TEMPERATURE_TRIP_POINT), MIN_TEMPERATURE_TRIP_POINT)
		except:
			pass

		self.Monitor.updateEntries()

	def configSwitchInputting(self, connection, i):
		if len(self.c.connections) == 0:
			return
		self.c.connections[connection].configSwitch = i

	def manualSwitchInputting(self, i):
		if len(self.c.connections) == 0:
			return

		self.Monitor.updateStatus()
		if self.c.connections[i].manualSwitch == 0:
			self.c.connections[i].manualSwitch = 1
			self.Monitor.toggleManualSwitchButton['text'] = 'OFF'	#
		else:
			self.c.connections[i].manualSwitch = 0
			self.Monitor.toggleManualSwitchButton['text'] = 'ON'	#

	def formatSelect(self, input):
		ret = ""
		for i in input:
			ret += str(i) + "\n"
		return ret

	# ----------------- #

	def run(self, Monitor):
		t1 = threading.Thread(target=self.receiver, args=())
		t2 = threading.Thread(target=self.commands, args=())
		self.Monitor = Monitor
		self.Monitor.runSetup()
		t1.start()
		t2.start()
		self.Monitor.run()
		t1.join()
		t2.join()

if __name__ == "__main__":
	a = Application()
	m = Monitor(a)
	a.run(m)

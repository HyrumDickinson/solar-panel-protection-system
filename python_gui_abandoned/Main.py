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
		print("Main.commands (thread t2) began running")
		self.conn = sqlite3.connect('solarPanel.db') # opens connection to SQLite database
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

		while True: # this always evaluates to true and loops forever
			if self.lastData and self.command != 'sync' and self.command != 'quit':
				try:
					packet = json.loads(self.lastData)

					# Switch relay configuration if trip point reached
					currConnection = self.c.findConnection(self.lastIP)
					if currConnection != -1 and self.c.connection.configSwitch != packet["S"] and packet["X"] != 0:
						self.Monitor.updateCheckbox(packet["S"])

					if packet["X"] != 0 and self.c.connection.manualSwitch == 0:
						self.c.connection.currentAck = 1
						
						if packet["X"] == 1:
							self.Monitor.updateStatus[2]['fg'] = RED
							self.Monitor.updateStatus[2]['text'] = 'Status: Over-Voltage'
						if packet["X"] == 2:
							self.Monitor.updateStatus[2]['fg'] = RED
							self.Monitor.updateStatus[2]['text'] = 'Status: Over-Current'
						if packet["X"] == 3:
							self.Monitor.updateStatus[2]['fg'] = RED
							self.Monitor.updateStatus[2]['text'] = 'Status: Over-Heating'

					if self.c.connection.manualSwitch == 1:
						self.Monitor.updateCheckbox(packet["S"])

						if packet["X"] == 1:
							self.Monitor.updateStatus[2]['fg'] = RED
							self.Monitor.updateStatus[2]['text'] = 'Status: Over-Voltage'
						if packet["X"] == 2:
							self.Monitor.updateStatus[2]['fg'] = RED
							self.Monitor.updateStatus[2]['text'] = 'Status: Over-Current'
						if packet["X"] == 3:
							self.Monitor.updateStatus[2]['fg'] = RED
							self.Monitor.updateStatus[2]['text'] = 'Status: Over-Heating'

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
					self.c.connection.socket.close()
					return					
				if self.command == 'sync':
					self.c.clear()
					self.c.connect()

					self.Monitor.statusWidget.destroy() 
					
					self.Monitor.updateStatus()
					# self.Monitor.clearStatus() 
					for i in range(0, 4):
						self.Monitor.updateCheckbox(i)

				self.command = None

	# ----------------- #

	def receiver(self):
		print("Main.receiver (thread t1) began running")
		self.c.connect() # this command is definately running. therefore the connection problem must be with the command itself, not the way it is executed
		# self.Monitor.updateStatus()
		# counter = 0 # * debugger line
		while True: # ! this function attempts to send and receive data from the arduino as fast as possible. perhaps we should implement a delay?
			# counter+=1 # * debugger line
			# print("there is " + str(self.c.connection.isConnection) + " a connection (send/receive #" + str(counter) + ")") # * debugger line
			
			if self.c.connection.isConnected and self.command != 'sync' and self.command != 'quit':
				# SEND
				data = {}
				data['V'] = self.c.connection.voltageValue
				data['C'] = self.c.connection.currentValue
				data['T'] = self.c.connection.temperatureValue
				data['S'] = self.c.connection.configSwitch
				data['M'] = self.c.connection.manualSwitch
				data['ACK'] = self.c.connection.currentAck
				self.c.connection.socket.send(json.dumps(data))
				# RECEIVE
				try:
					self.lastData = self.c.connection.socket.recv(BUFFER_SIZE)
					self.lastIP = self.c.connection.ip
				except:

					print("Timed out.")
				print("THERE IS A CONNECTION")
			elif not self.c.connection.isConnected and self.command != 'sync' and self.command != 'quit':
				# print("No connection")
				# print("Timed out.")
				print("THERE IS NO CONNECTION")
				return

			if self.command == 'quit':
				return

	# ----------------- #

	def inputting(self, command):
		self.command = command

	def TripPointInputting(self, voltageValue, currentValue, temperatureValue, i):
		if self.c.connection.isConnected == False:
			return

		try:
			self.c.connection.voltageValue = max(min(float(voltageValue), MAX_VOLTAGE_TRIP_POINT), MIN_VOLTAGE_TRIP_POINT)
			self.c.connection.currentValue = max(min(float(currentValue), MAX_CURRENT_TRIP_POINT), MIN_CURRENT_TRIP_POINT)
			self.c.connection.temperatureValue = max(min(float(temperatureValue), MAX_TEMPERATURE_TRIP_POINT), MIN_TEMPERATURE_TRIP_POINT)
		except:
			pass

		self.Monitor.updateEntries()

	def configSwitchInputting(self, connection, i):
		if self.c.connection.isConnected == False:
			return
		self.c.connection.configSwitch = i

	def manualSwitchInputting(self, i):
		if self.c.connection.isConnected == False:
			return

		self.Monitor.updateStatus()
		if self.c.connection.manualSwitch == 0:
			self.c.connection.manualSwitch = 1
			self.Monitor.toggleManualSwitchButton['text'] = 'OFF'
		else:
			self.c.connection.manualSwitch = 0
			self.Monitor.toggleManualSwitchButton['text'] = 'ON'

	def formatSelect(self, input):
		ret = ""
		for i in input:
			ret += str(i) + "\n"
		return ret

	# ----------------- #

	def run(self, Monitor):
		print("Application.run() about to run")
		t1 = threading.Thread(target=self.receiver)
		print("t1 = threading.Thread(target=self.receiver) ran")
		t2 = threading.Thread(target=self.commands)
		print("t2 = threading.Thread(target=self.commands) ran")
		self.Monitor = Monitor
		print("Application.Monitor = Monitor ran")
		self.Monitor.runSetup()
		print("Application.Monitor.runSetup() ran")
		t1.start()
		print("t1.start() ran")
		t2.start()
		print("t2.start() ran")
		self.Monitor.run()
		print("Application.Monitor.run()")
		t1.join()
		print("t1.join() ran")
		t2.join()
		print("t2.join() ran")

if __name__ == "__main__":
	print("application about to run")
	a = Application()
	print("a = Application() ran")
	m = Monitor(a)
	print("m = Monitor(a) ran")
	print("a.run(m) about to run")
	a.run(m)
	print("a.run(m) ran")


# from Tkinter import tkk
from PIL import Image, ImageTk 
from Graph import * # this line imports the following:
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

HEIGHT = 600
WIDTH = 750

class Monitor():
	def __init__ (self, application):
		self.root = tk.Tk()
		self.root.protocol("WM_DELETE_WINDOW", self.close_window)
		self.root.winfo_toplevel().title("Solar Panel Monitor")
		self.root.geometry('{}x{}'.format(WIDTH, HEIGHT))
		self.root.configure(bg=MAIN_BACKGROUND_COLOR)
		self.root.resizable(0,0)

		self.application = application
		self.graph = None
		self.widgetFrames = []
		self.selected = 0
		self.vars = []

	# ------------- #

	def setupFrames(self):
		# Spacing
		spacingFrame = tk.Frame(self.root, bg=MAIN_BACKGROUND_COLOR, width=25)
		spacingFrame.pack(side="left")

		# Connection frame
		self.connFrame = tk.Frame(self.root, bg=FIFTY_SHADES_OF_GRAY, highlightbackground=TEXT_COLOR, highlightcolor=TEXT_COLOR, highlightthickness=2)
		self.connFrame.pack(side="left", fill="y", pady="25")

		# Data frame
		self.dataFrame = tk.Frame(self.root, bg=DATA_WIDGET_COLOR, highlightbackground=TEXT_COLOR, highlightcolor=TEXT_COLOR, highlightthickness=2)
		self.dataFrame.pack(side="right", fill="both", padx="25", pady="25")

	def setupTripPoints(self):
		TripPointFrame = tk.Frame(self.topFrame, bg=DATA_WIDGET_COLOR)
		TripPointFrame.pack(side="left")

		# TripPoint Title
		TripPointFrame_1 = tk.Frame(TripPointFrame, bg=DATA_WIDGET_COLOR)
		TripPointFrame_1.pack(side="top", padx=40, pady=2.5, fill="x")
		TripPointTitle = tk.Label(TripPointFrame_1, text="Trip points:", bg=DATA_WIDGET_COLOR, font='Helvetica_Neue 15 bold')
		TripPointTitle.pack(side="left")

		# Voltage Trip Point Label and Entry
		TripPointFrame_2 = tk.Frame(TripPointFrame, bg=DATA_WIDGET_COLOR)
		TripPointFrame_2.pack(side="top", pady=2.5, fill="x")
		voltageEntryTripPoint = tk.Label(TripPointFrame_2, text="Max voltage: ", padx=43, bg=DATA_WIDGET_COLOR)
		voltageEntryTripPoint.pack(side="left")
		self.voltageEntry = tk.Entry(TripPointFrame_2, font=40, width=5)
		self.voltageEntry.insert(0, DEFAULT_VOLTAGE_TRIP_POINT)
		self.voltageEntry.bind('<FocusIn>', lambda event, i=0: self.on_entry_click(event, i))
		self.voltageEntry.bind('<FocusOut>', lambda event, i=0: self.on_focusout(event, i))
		self.voltageEntry.config(fg=TEXT_COLOR)
		self.voltageEntry.pack(side="left")
		voltageUnit = tk.Label(TripPointFrame_2, text="V", bg=DATA_WIDGET_COLOR, font='Helvetica_Neue 12 bold italic')
		voltageUnit.pack(side="left", padx=15)

		# Current Trip Point Label and Entry
		self.TripPointFrame_3 = tk.Frame(TripPointFrame, bg=DATA_WIDGET_COLOR)
		self.TripPointFrame_3.pack(side="top", pady=2.5, fill="x")
		currentEntryTripPoint = tk.Label(self.TripPointFrame_3, text="Max current: ", padx=43, bg=DATA_WIDGET_COLOR)
		currentEntryTripPoint.pack(side="left")
		self.currentEntry = tk.Entry(self.TripPointFrame_3, font=40, width=5)
		self.currentEntry.insert(0, DEFAULT_CURRENT_TRIP_POINT)
		self.currentEntry.bind('<FocusIn>', lambda event, i=1: self.on_entry_click(event, i))
		self.currentEntry.bind('<FocusOut>', lambda event, i=1: self.on_focusout(event, i))
		self.currentEntry.config(fg=TEXT_COLOR)
		self.currentEntry.pack(side="left", padx=1)
		currentUnit = tk.Label(self.TripPointFrame_3, text="A", bg=DATA_WIDGET_COLOR, font='Helvetica_Neue 12 bold italic')
		currentUnit.pack(side="left", padx=15)

		# Temperature Trip Point Label and Entry
		TripPointFrame_4 = tk.Frame(TripPointFrame, bg=DATA_WIDGET_COLOR)
		TripPointFrame_4.pack(side="top", padx=43, pady=2.5, fill="both")
		temperatureEntryTripPoint = tk.Label(TripPointFrame_4, text="Max temperature: ", bg=DATA_WIDGET_COLOR)
		temperatureEntryTripPoint.pack(side="left")
		self.temperatureEntry = tk.Entry(TripPointFrame_4, font=40, width=5)
		self.temperatureEntry.insert(0, DEFAULT_TEMPERATURE_TRIP_POINT)
		self.temperatureEntry.bind('<FocusIn>', lambda event, i=2: self.on_entry_click(event, i))
		self.temperatureEntry.bind('<FocusOut>', lambda event, i=2: self.on_focusout(event, i))
		self.temperatureEntry.config(fg=TEXT_COLOR)
		self.temperatureEntry.pack(side="left", padx=15)
		temperatureUnit = tk.Label(TripPointFrame_4, text="C", bg=DATA_WIDGET_COLOR, font='Helvetica_Neue 12 bold italic')
		temperatureUnit.pack(side="left")

	def setupCheckboxes(self):
		self.configFrame = tk.Frame(self.topFrame, bg=DATA_WIDGET_COLOR)
		self.configFrame.pack(side="right", padx=10)

		configFrame = tk.Frame(self.configFrame, bg=DATA_WIDGET_COLOR)
		configFrame.pack(side="bottom", pady=2.5)

		configFrame1 = tk.Frame(configFrame, bg=DATA_WIDGET_COLOR)
		configFrame1.pack(side="left", pady=2.5)
		configFrame2 = tk.Frame(configFrame, bg=DATA_WIDGET_COLOR)
		configFrame2.pack(side="left", pady=2.5)

		# Output Configuration
		var4 = tk.IntVar()
		checkboxD = tk.Checkbutton(configFrame2, text="AD", background=DATA_WIDGET_COLOR, activebackground=DATA_WIDGET_COLOR, variable=var4, command=lambda: self.updateCheckbox(3))
		checkboxD.pack(side="bottom", pady=2.5)
		var3 = tk.IntVar()
		checkboxC = tk.Checkbutton(configFrame2, text="BC", background=DATA_WIDGET_COLOR, activebackground=DATA_WIDGET_COLOR, variable=var3, command=lambda: self.updateCheckbox(2))
		checkboxC.pack(side="bottom", pady=2.5)
		var2 = tk.IntVar()
		checkboxB = tk.Checkbutton(configFrame2, text="CD", background=DATA_WIDGET_COLOR, activebackground=DATA_WIDGET_COLOR, variable=var2, command=lambda: self.updateCheckbox(1))
		checkboxB.pack(side="bottom", pady=2.5)
		var1 = tk.IntVar()
		checkboxA = tk.Checkbutton(configFrame2, text="XX", background=DATA_WIDGET_COLOR, activebackground=DATA_WIDGET_COLOR, variable=var1, command=lambda: self.updateCheckbox(0))
		checkboxA.pack(side="bottom", pady=2.5)
		configLabel = tk.Label(configFrame1, text="Config Switch: ", bg=DATA_WIDGET_COLOR)
		configLabel.pack(side="bottom")
		self.vars = [var1, var2, var3, var4]

	def setupSyncButton(self):
		# SYNC Label
		self.syncFrame = tk.Frame(self.connFrame, bg=CONNECTOR_WIDGET_COLOR)
		self.syncFrame.pack(side="bottom", fill="x")		

	def setupButtons(self):

		# the two lines below add a sync button. However, it's an ugly one, and furthurmore there's already
		# a sync button in the form of sync.png

		# self.syncButton = tk.Button(self.syncFrame, text='sync', highlightbackground=CONNECTOR_WIDGET_COLOR, font=20, command=lambda: self.application.inputting('sync'))
		# self.syncButton.pack(side="bottom", padx=40)

		TripPointEntryButton = tk.Button(self.TripPointFrame_3, text="OK", background=INACTIVE_BUTTON_COLOR, activebackground=ACTIVE_BUTTON_COLOR, font=40, command=lambda: self.application.TripPointInputting(self.voltageEntry.get(), self.currentEntry.get(), self.temperatureEntry.get(), self.selected))
		TripPointEntryButton.pack(side="left", padx=30)

		manualConfigFrame = tk.Frame(self.configFrame, bg=DATA_WIDGET_COLOR)
		manualConfigFrame.pack(side="top", pady=2.5)
		self.toggleManualSwitchButton = tk.Button(manualConfigFrame, text="ON", background=INACTIVE_BUTTON_COLOR, activebackground=ACTIVE_BUTTON_COLOR, font=40, command=lambda: self.application.manualSwitchInputting(self.selected))
		self.toggleManualSwitchButton.pack(side="right", pady=5, padx=5)
		manualConfigLabel = tk.Label(manualConfigFrame, text="Manual Switch: ", bg=DATA_WIDGET_COLOR)
		manualConfigLabel.pack(side="right")

	def setupQueryButtons(self):
		queryButtonFrame = tk.Frame(self.dataFrame, bg=DATA_WIDGET_COLOR)
		queryButtonFrame.pack(side="bottom")
		v1Button = tk.Button(queryButtonFrame, text="V1", background=INACTIVE_BUTTON_COLOR, activebackground=ACTIVE_BUTTON_COLOR, font=30, command=lambda: self.graph.setField('voltage_1'))
		v1Button.pack(side="left")
		v2Button = tk.Button(queryButtonFrame, text="V2", background=INACTIVE_BUTTON_COLOR, activebackground=ACTIVE_BUTTON_COLOR, font=30, command=lambda: self.graph.setField('voltage_2'))
		v2Button.pack(side="left")
		v3Button = tk.Button(queryButtonFrame, text="V3", background=INACTIVE_BUTTON_COLOR, activebackground=ACTIVE_BUTTON_COLOR, font=30, command=lambda: self.graph.setField('voltage_3'))
		v3Button.pack(side="left")
		c1Button = tk.Button(queryButtonFrame, text="C1", background=INACTIVE_BUTTON_COLOR, activebackground=ACTIVE_BUTTON_COLOR, font=30, command=lambda: self.graph.setField('current_1'))
		c1Button.pack(side="left")
		t1Button = tk.Button(queryButtonFrame, text="T1", background=INACTIVE_BUTTON_COLOR, activebackground=ACTIVE_BUTTON_COLOR, font=30, command=lambda: self.graph.setField('temperature_1'))
		t1Button.pack(side="left")
		t2Button = tk.Button(queryButtonFrame, text="T2", background=INACTIVE_BUTTON_COLOR, activebackground=ACTIVE_BUTTON_COLOR, font=30, command=lambda: self.graph.setField('temperature_2'))
		t2Button.pack(side="left")
		t3Button = tk.Button(queryButtonFrame, text="T3", background=INACTIVE_BUTTON_COLOR, activebackground=ACTIVE_BUTTON_COLOR, font=30, command=lambda: self.graph.setField('temperature_3'))
		t3Button.pack(side="left")
		t4Button = tk.Button(queryButtonFrame, text="T4", background=INACTIVE_BUTTON_COLOR, activebackground=ACTIVE_BUTTON_COLOR, font=30, command=lambda: self.graph.setField('temperature_4'))
		t4Button.pack(side="left")
		t5Button = tk.Button(queryButtonFrame, text="T5", background=INACTIVE_BUTTON_COLOR, activebackground=ACTIVE_BUTTON_COLOR, font=30, command=lambda: self.graph.setField('temperature_5'))
		t5Button.pack(side="left")
		t6Button = tk.Button(queryButtonFrame, text="T6", background=INACTIVE_BUTTON_COLOR, activebackground=ACTIVE_BUTTON_COLOR, font=30, command=lambda: self.graph.setField('temperature_6'))
		t6Button.pack(side="left")

	def setup(self):			
		self.setupFrames()
		self.topFrame = tk.Frame(self.dataFrame, bg=DATA_WIDGET_COLOR)
		self.topFrame.pack(side="top", fill="x", pady=10)
		# self.topFrame = tk.Frame(self.connFrame, bg=CONNECTOR_WIDGET_COLOR)
		# self.topFrame.pack(side="bottom", fill="x", pady=10)
		self.setupTripPoints()
		self.setupCheckboxes()
		self.setupSyncButton()
		self.setupQueryButtons()

	def updateWidgets(self):
		connLength = len(self.application.c.connections)

		if connLength == 0:
			BACKGROUND = FIFTY_SHADES_OF_GRAY

			# Frame for Widget
			widgetFrame = tk.Frame(self.connFrame, bg=BACKGROUND)
			widgetFrame.pack(side="top", fill="x")

			# IP Status
			ipStatus = tk.Label(widgetFrame, text="Status: Not Connected", bg=BACKGROUND, font='TkDefaultFont 8')
			ipStatus.pack(side="bottom", fill="x", padx=5, pady=5)
			ipStatus.config(fg=RED)

		else :
			for i in range(0, connLength):
				BACKGROUND = CONNECTOR_WIDGET_COLOR if i == 0 else FIFTY_SHADES_OF_GRAY

				# Frame for Widget
				widgetFrame = tk.Frame(self.connFrame, bg=BACKGROUND)
				widgetFrame.pack(side="top", fill="x")

				# IP Label
				ipLabel = tk.Label(widgetFrame, text='IP: ' + self.application.c.connections[i].ip, bg=BACKGROUND, font='TkDefaultFont 10')
				ipLabel.pack(side="top", fill="x", padx=10, pady=5)
				ipLabel.config(fg=GREEN)
			
				# IP Status
				ipStatus = tk.Label(widgetFrame, text="Status: Connected", bg=BACKGROUND, font='TkDefaultFont 10')
				ipStatus.pack(side="bottom", fill="x", padx=10, pady=5)
				ipStatus.config(fg=ACTIVE_BUTTON_COLOR)

				self.widgetFrames.append([widgetFrame, ipLabel, ipStatus])
				index = len(self.widgetFrames) - 1
				widgetFrame.bind('<Button-1>', lambda event, i=index: self.frameInteraction(event, i, BACKGROUND))
				ipLabel.bind('<Button-1>', lambda event, i=index: self.labelInteraction(event, i))
				ipStatus.bind('<Button-1>', lambda event, i=index: self.labelInteraction(event, i))
				widgetFrame.bind("<Enter>", lambda event, i=index: self.frameInteraction(event, i, BACKGROUND))
				widgetFrame.bind("<Leave>", lambda event, i=index: self.frameInteraction(event, i, BACKGROUND))

	# ------------- #

	def switchConnections(self, i):
		# Change checkbox
		self.updateCheckbox(self.application.c.connections[i].configSwitch)

		# Change Trip Point values
		self.voltageEntry.delete(0, "end")
		self.voltageEntry.insert(0, self.application.c.connections[i].voltageValue)
		self.voltageEntry.config(fg=TEXT_COLOR)
		self.currentEntry.delete(0, "end")
		self.currentEntry.insert(0, self.application.c.connections[i].currentValue)
		self.currentEntry.config(fg=TEXT_COLOR)
		self.temperatureEntry.delete(0, "end")
		self.temperatureEntry.insert(0, self.application.c.connections[i].temperatureValue)
		self.temperatureEntry.config(fg=TEXT_COLOR)

	def updateCheckbox(self, i):
		for var in self.vars:
			var.set(0)
		for j in range(0, len(self.vars)):
			if j == i:
				self.vars[j].set(1)
		self.application.configSwitchInputting(self.selected, i)
		self.updateStatus()

	def labelInteraction(self, event, index):
		color = CONNECTOR_WIDGET_COLOR

		if event.type == 4 :	# Clicked
			self.clearWidgetColors()
			color = CONNECTOR_WIDGET_COLOR	
			self.selected = index
			self.switchConnections(self.selected)
			self.graph.a.clear()

		for i in self.widgetFrames[index]: i.configure(bg=color)

	def frameInteraction(self, event, index, bg):
		color = CONNECTOR_WIDGET_COLOR

		if event.type == 4 :	# Clicked
			self.clearWidgetColors()
			color = CONNECTOR_WIDGET_COLOR	
			self.selected = index
			self.switchConnections(self.selected)
			self.graph.a.clear()

		if event.type == 7 :	# Entered
			color = CONNECTOR_WIDGET_COLOR if self.selected == index else FIFTY_SHADES_OF_GRAY

		if event.type == 8 :	# Exited
			color = CONNECTOR_WIDGET_COLOR if self.selected == index else FIFTY_SHADES_OF_GRAY

		for i in self.widgetFrames[index]: i.configure(bg=color)

	def clearWidgetColors(self):
		for i in self.widgetFrames:
			for j in i:
				j.configure(bg=FIFTY_SHADES_OF_GRAY)

	# ------------- #

	def on_entry_click(self, event, i):
		if i == 0:
			self.voltageEntry.delete(0, "end")
			self.voltageEntry.insert(0, '')
			self.voltageEntry.config(bg=INACTIVE_BUTTON_COLOR, fg=TEXT_COLOR)
		if i == 1:
			self.currentEntry.delete(0, "end")
			self.currentEntry.insert(0, '')
			self.currentEntry.config(bg=INACTIVE_BUTTON_COLOR, fg=TEXT_COLOR)
		if i == 2:
			self.temperatureEntry.delete(0, "end")
			self.temperatureEntry.insert(0, '')
			self.temperatureEntry.config(bg=INACTIVE_BUTTON_COLOR, fg=TEXT_COLOR)

	def on_focusout(self, event, i):
		if len(self.application.c.connections) == 0:
			if i == 0:
				if self.voltageEntry.get() == '':
					self.voltageEntry.insert(0, DEFAULT_VOLTAGE_TRIP_POINT)
					self.voltageEntry.config(bg=INACTIVE_BUTTON_COLOR, fg=TEXT_COLOR)
			if i == 1:
				if self.currentEntry.get() == '':
					self.currentEntry.insert(0, DEFAULT_CURRENT_TRIP_POINT)
					self.currentEntry.config(bg=INACTIVE_BUTTON_COLOR, fg=TEXT_COLOR)
			if i == 2:
				if self.temperatureEntry.get() == '':
					self.temperatureEntry.insert(0, DEFAULT_TEMPERATURE_TRIP_POINT)
					self.temperatureEntry.config(bg=INACTIVE_BUTTON_COLOR, fg=TEXT_COLOR)
		else:
			if i == 0:
				if self.voltageEntry.get() == '':
					self.voltageEntry.insert(0, self.application.c.connections[self.selected].voltageValue)
					self.voltageEntry.config(fg=TEXT_COLOR)
			if i == 1:
				if self.currentEntry.get() == '':
					self.currentEntry.insert(0, self.application.c.connections[self.selected].currentValue)
					self.currentEntry.config(fg=TEXT_COLOR)
			if i == 2:
				if self.temperatureEntry.get() == '':
					self.temperatureEntry.insert(0, self.application.c.connections[self.selected].temperatureValue)
					self.temperatureEntry.config(fg=TEXT_COLOR)

	def updateEntries(self):
		self.voltageEntry.delete(0, "end")
		self.voltageEntry.insert(0, self.application.c.connections[self.selected].voltageValue)
		self.voltageEntry.config(fg=TEXT_COLOR)

		self.currentEntry.delete(0, "end")
		self.currentEntry.insert(0, self.application.c.connections[self.selected].currentValue)
		self.currentEntry.config(fg=TEXT_COLOR)

		self.temperatureEntry.delete(0, "end")
		self.temperatureEntry.insert(0, self.application.c.connections[self.selected].temperatureValue)
		self.temperatureEntry.config(fg=TEXT_COLOR)

	def updateStatus(self):
		if len(self.application.c.connections) == 0:
			return

		self.widgetFrames[self.selected][2]['fg'] = ACTIVE_BUTTON_COLOR
		self.widgetFrames[self.selected][2]['text'] = 'Status: Connected'
		self.application.c.connections[self.selected].currentAck = 0

	# ------------- #

	def clearWidgets(self):
		for i in self.widgetFrames:
			for j in i:
				j.destroy()

		self.widgetFrames = []
		self.selected = 0

	def runSetup(self):
		self.setup()
		self.updateCheckbox(0)

	def close_window(self):
		self.application.command = 'quit'

	# ------------- #

	def run(self):
		self.setupButtons()

		# Button Images
		sync_button = Image.open('sync.png')
		sync_image_for_button = ImageTk.PhotoImage(sync_button)
		self.syncButton = tk.Button(self.syncFrame, image=sync_image_for_button, bg=INACTIVE_BUTTON_COLOR, activebackground=ACTIVE_BUTTON_COLOR, command=lambda: self.application.inputting('sync'))
		self.syncButton.config(width="120", height="20")
		self.syncButton.pack(side="bottom")
		self.syncButton.config(image=sync_image_for_button) 

		self.graph = Graph(self)
		self.graph.run()
		ani = animation.FuncAnimation(self.graph.f, self.graph.animate, interval=1000)
		self.root.mainloop()


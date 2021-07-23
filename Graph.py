import matplotlib
matplotlib.use('TkAgg')
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2Tk
from matplotlib.figure import Figure
import matplotlib.animation as animation
from matplotlib import style
import Tkinter as tk
# from Tkinter import tkk
import sqlite3
from Settings import * # imports default trip points and color definitions
style.use('ggplot')

import matplotlib.dates as mdates
import numpy as np
import datetime
import time

units = ['V', 'V', 'V', 'A', 'C', 'C', 'C', 'C', 'C', 'C']
title = ['Voltage_AD vs. Time (Minutes)', 'Voltage_BC vs. Time (Minutes)', 'Voltage_CD vs. Time (Minutes)', 'Current vs. Time (Minutes)', 'Temperature #1 vs. Time (Minutes)', 'Temperature #2 vs. Time (Minutes)',
		 'Temperature #3 vs. Time (Minutes)', 'Temperature #4 vs. Time (Minutes)', 'Temperature #5 vs. Time (Minutes)', 'Temperature #6 vs. Time (Minutes)']

class Graph:
	def __init__ (self, Monitor):
		self.Monitor = Monitor
		self.f = Figure(figsize=(5,5), dpi=100, facecolor=WIDGET_BACKGROUND_COLOR)
		self.a = self.f.add_subplot(111)
		self.a.set_facecolor(INACTIVE_BUTTON_COLOR)
		# self.a.set_edgecolor(TEXT_COLOR)
		# self.a.tick_params(axis='x', color=TEXT_COLOR, edge_color=TEXT_COLOR, grid_color=TEXT_COLOR)
		# self.a.tick_params(axis='y', color=TEXT_COLOR, grid_color=TEXT_COLOR)
		self.a.tick_params(grid_color=TEXT_COLOR, grid_linewidth=0.4)
		self.a.spines['top'].set_visible(False)			# The spines are, by default, white. I don't want a white edge on my grid. Removing these two spines lets the grid edges underneath show.
		self.a.spines['left'].set_visible(False)		# For some reason, the bottom and right edges of the grid are cut off. I've spent way too much time trying to make this grid black already.
		self.a.spines['bottom'].set_color(TEXT_COLOR)	# Since I can't figure out how to keep the bottom right edge of the graph from being cut off, I'm just coloring those borders (spines) the
		self.a.spines['bottom'].set_linewidth(0.4)		# same TEXT_COLOR color with the same linewidth as the rest of the graph so it looks as if the whole graph is displaying. And for the record,
		self.a.spines['right'].set_color(TEXT_COLOR)	# in case any coder is reading this and decrying my sense of ethics in merely making the graph LOOK fixed instead of actually fixing it, the
		self.a.spines['right'].set_linewidth(0.4)		# only part of the graph that is cut off is the very edge. No data will be lost by my allowing that sliver of the frame to remain unviewable.
		self.field = 'voltage_1'
		self.f.suptitle(title[self.getFieldIndex()], fontsize=10, color=TEXT_COLOR)

	def run(self):
		self.setupValueFrame()
		canvas = FigureCanvasTkAgg(self.f, self.Monitor.dataFrame)
		canvas.draw()
		canvas.get_tk_widget().pack(side='bottom', fill='x', pady=5)
		canvas._tkcanvas.pack(side='bottom', fill='x')


	def animate(self, i):
		if len(self.Monitor.application.c.connections) > 0:
			dataList = self.getData(self.Monitor.application.c.connections[self.Monitor.selected].ip)
			xList = []
			yList = []
			for i in dataList:
				x = i[0]
				y = i[1]
				xList.append(datetime.datetime.fromtimestamp(x))
				yList.append(y)

			self.a.clear()
			self.a.plot_date(xList, yList, color=TEXT_COLOR, fmt='-r')
			self.a.xaxis.set_major_formatter(mdates.DateFormatter('%M:%S'))
			# self.a.tick_params(axis='both', which='major', labelsize=8, color=GREEN)
			self.getLastData(self.Monitor.application.c.connections[self.Monitor.selected].ip)

	def getData(self, ip):
		conn = sqlite3.connect('solarPanel.db')
		cursor = conn.cursor()	

		if self.field == 'voltage_2':
			cursor.execute('SELECT timeRecorded, voltage_2, voltage_3 FROM voltages WHERE (:ip)',
			{
				'ip': ip,
			})
			values = cursor.fetchall()
			retValues = []
			for value in values:
				retValues.append((value[0], value[1]-value[2]))
			return retValues

		cursor.execute('SELECT timeRecorded, ({0}) FROM voltages WHERE (:ip)'.format(self.field),
		{
			'ip': ip,
		})
		values = cursor.fetchall()
		conn.commit()
		return values

	def getLastData(self, ip):
		fields = ['voltage_1', 'voltage_2', 'voltage_3', 'current_1', 'temperature_1', 
		'temperature_2', 'temperature_3', 'temperature_4', 'temperature_5', 'temperature_6']
		values = [self.v1Value, self.v2Value, self.v3Value, self.c1Value, self.t1Value, 
		self.t2Value, self.t3Value, self.t4Value, self.t5Value, self.t6Value]
		labels = ['V_ad: ', 'V_bc: ', 'V_cd: ', 'I: ','T1: ', 'T2: ', 'T3: ', 'T4: ', 'T5: ', 'T6: ']
		units = ['V', 'V', 'V', 'A', 'C', 'C', 'C', 'C', 'C', 'C']
		voltageVals = [0, 0, 0, 0]

		c = 0
		conn = sqlite3.connect('solarPanel.db')
		cursor = conn.cursor()	
		for field in fields:
			if field == 'voltage_2':
				cursor.execute('SELECT voltage_2, voltage_3 FROM voltages WHERE (:ip) ORDER BY timeRecorded DESC LIMIT 1',
				{
					'ip': ip,
				})
				val = cursor.fetchone()
				if val:
					values[c]['text'] = labels[c] + str(round(val[0] - val[1], 2)) + units[c]
					voltageVals[2] = val[0] - val[1]
			else:	
				cursor.execute('SELECT ({0}) FROM voltages WHERE (:ip) ORDER BY timeRecorded DESC LIMIT 1'.format(field),
				{
					'ip': ip,
				})
				val = cursor.fetchone()
				if val:
					values[c]['text'] = labels[c] + str(round(val[0], 2)) + units[c]
					if field == 'voltage_1':
						voltageVals[3] = val[0]
					if field == 'voltage_3':
						voltageVals[1] = val[0]

			conn.commit()
			c += 1

		currentConfig = self.Monitor.application.c.connections[self.Monitor.selected].configSwitch
		cursor.execute('SELECT current_1 FROM voltages WHERE (:ip) ORDER BY timeRecorded DESC LIMIT 1',
		{
			'ip': ip,
		})
		lastCurrent = cursor.fetchone()
		if lastCurrent and len(voltageVals):
			self.pValue['text'] = 'P: ' + str(round(voltageVals[currentConfig] * lastCurrent[0], 2)) + 'W'


	# ------------- #

	# This frame shows the numerical values of each variable

	def setupValueFrame(self):
		valueFrame = tk.Frame(self.Monitor.dataFrame, bg=WIDGET_BACKGROUND_COLOR)
		valueFrame.pack(side='top')

		self.pValue = tk.Label(valueFrame, text='P: XX', fg=TEXT_COLOR, bg=WIDGET_BACKGROUND_COLOR, font='Helvetica_Neue 11 bold')
		self.pValue.pack(side='left', padx=5)
		self.v1Value = tk.Label(valueFrame, text='V_ad: XX', fg=TEXT_COLOR, bg=WIDGET_BACKGROUND_COLOR, font='Helvetica_Neue 11 bold')
		self.v1Value.pack(side='left', padx=5)
		self.v2Value = tk.Label(valueFrame, text='V_bc: XX', fg=TEXT_COLOR, bg=WIDGET_BACKGROUND_COLOR, font='Helvetica_Neue 11 bold')
		self.v2Value.pack(side='left', padx=5)
		self.v3Value = tk.Label(valueFrame, text='V_cd: XX', fg=TEXT_COLOR, bg=WIDGET_BACKGROUND_COLOR, font='Helvetica_Neue 11 bold')
		self.v3Value.pack(side='left', padx=5)
		self.c1Value = tk.Label(valueFrame, text='I: XX', fg=TEXT_COLOR, bg=WIDGET_BACKGROUND_COLOR, font='Helvetica_Neue 11 bold')
		self.c1Value.pack(side='left', padx=5)

		temperatureFrame = tk.Frame(self.Monitor.dataFrame, bg=WIDGET_BACKGROUND_COLOR)
		temperatureFrame.pack(side='top', padx=5)

		self.t1Value = tk.Label(temperatureFrame, text='T1: XX', fg=TEXT_COLOR, bg=WIDGET_BACKGROUND_COLOR, font='Helvetica_Neue 11 bold')
		self.t1Value.pack(side='left', padx=5)
		self.t2Value = tk.Label(temperatureFrame, text='T2: XX', fg=TEXT_COLOR, bg=WIDGET_BACKGROUND_COLOR, font='Helvetica_Neue 11 bold')
		self.t2Value.pack(side='left', padx=5)
		self.t3Value = tk.Label(temperatureFrame, text='T3: XX', fg=TEXT_COLOR, bg=WIDGET_BACKGROUND_COLOR, font='Helvetica_Neue 11 bold')
		self.t3Value.pack(side='left', padx=5)
		self.t4Value = tk.Label(temperatureFrame, text='T4: XX', fg=TEXT_COLOR, bg=WIDGET_BACKGROUND_COLOR, font='Helvetica_Neue 11 bold')
		self.t4Value.pack(side='left', padx=5)
		self.t5Value = tk.Label(temperatureFrame, text='T5: XX', fg=TEXT_COLOR, bg=WIDGET_BACKGROUND_COLOR, font='Helvetica_Neue 11 bold')
		self.t5Value.pack(side='left', padx=5)
		self.t6Value = tk.Label(temperatureFrame, text='T6: XX', fg=TEXT_COLOR, bg=WIDGET_BACKGROUND_COLOR, font='Helvetica_Neue 11 bold')
		self.t6Value.pack(side='left', padx=5)

	# ------------- #

	def setField(self, field):
		self.field = field
		self.f.suptitle(title[self.getFieldIndex()], fontsize=10)

	def getFieldIndex(self):
		if self.field == 'voltage_1':
			return 0
		elif self.field == 'voltage_2':
			return 1
		elif self.field == 'voltage_3':
			return 2
		elif self.field == 'current_1':
			return 3
		elif self.field == 'temperature_1':
			return 4
		elif self.field == 'temperature_2':
			return 5
		elif self.field == 'temperature_3':
			return 6
		elif self.field == 'temperature_4':
			return 7
		elif self.field == 'temperature_5':
			return 8
		elif self.field == 'temperature_6':
			return 9
		else:
			return 0

from Settings import * # imports default trip points and color definitions

# Main class for connections
class Connection:
	def __init__ (self, socket, ip, port, isConnected):
		# Connection values
		self.socket = socket
		self.ip = ip
		self.port = port 
		self.isConnected = isConnected

		# Values to configure
		self.voltageValue = DEFAULT_VOLTAGE_TRIP_POINT
		self.currentValue = DEFAULT_CURRENT_TRIP_POINT
		self.temperatureValue = DEFAULT_TEMPERATURE_TRIP_POINT
		self.configSwitch = 0
		self.manualSwitch = 0

		self.currentAck = 0

	def setSocket(self, socket):
		self.socket = socket
	def getSocket(self):
		return self.socket
	def setIp(self, ip):
		self.ip = ip
	def getIp(self):
		return self.ip
	def setPort(self, port):
		self.port = port
	def getPort(self):
		return self.port
	def setIss(self, isConnected):
		self.isConnected = isConnected
	def getIsConnected(self):
		return self.isConnected

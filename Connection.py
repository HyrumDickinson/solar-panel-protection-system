from Settings import * # imports default trip points and color definitions

# Main class for connections
class Connection:
	def __init__ (self, socket, ip, port, connected):
		# Connection values
		self.socket = socket
		self.ip = ip
		self.port = port
		self.connected = connected

		# Values to configure
		self.voltageValue = DEFAULT_VOLTAGE_TRIP_POINT
		self.currentValue = DEFAULT_CURRENT_TRIP_POINT
		self.temperatureValue = DEFAULT_TEMPERATURE_TRIP_POINT
		self.configSwitch = 0
		self.manualSwitch = 0

		self.currentAck = 0

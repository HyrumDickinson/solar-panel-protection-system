import socket
import threading
from Connection import * # imports Settings

TCP_PORT = 23
INIT_CONNECTION = 0
NUM_CONNECTIONS = 100
TIMEOUT = 5

# Main class for IP Scanner
class Connector:
   def __init__ (self):
      self.threads = []
      # Default IP address
      self.ip = "128.174.168.99" # change this to your device's local ip address
      self.isConnected = False

      # this used to be an array of connections. I don't want an array of connections. I want one single connection. An ethernet connection to an arduino.
      # self.connections = []
      self.connection = Connection(socket, self.ip, TCP_PORT, False)

   # originally, scan() had an extra argument i that added the last digits to a common default router ip address. 
   # the connect() function tried scanning 100 times for ip addresses with the hardcoded beginning,
   # ending anywhere between 1 and 100. This was silly. The python program only needs to connect to one arduino.
   # That arduino is the device that will take care of connecting to the 60 solar panel arduinos and relaying their information 
   # through ethernet to the desktop that runs this python interface.
   # scan() has been changed to only search for one hardcoded ip address - the ip address of that arduino.

   def scan(self):
      print("scan about to run")
      s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
      s.settimeout(TIMEOUT)
      try:
         s.connect((self.ip, TCP_PORT))
         self.connection = (s, self.ip, TCP_PORT, True)
         print("scan() DONE.")
      except:
         pass
      print("scan ran")

   # ------------- #

   def connect(self):
      self.threads = []
      self.connection = Connection(socket, self.ip, TCP_PORT, True)
      
      # try:
      #    self.ip = self.ip
      #    print("connect() DONE a.")
      # except:
      #    pass

      self.threads.append(threading.Thread(target=self.scan))
      for t in self.threads:
         t.start()
      for t in self.threads:
         t.join()
      print("connect() DONE")

   # ------------- #

   def findConnection(self, ip):
      for idx, i in enumerate(self.connection):
         if i.ip == ip:
            return idx
      return -1

   def clear(self):
      self.connection.socket.close()

      self.connection = Connection(socket, self.ip, TCP_PORT, False)
      self.threads = []

   # ------------- #

   # def get_ip_address(self):
   #    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
   #    s.connect("8.8.8.8", 80)
   #  return s.getsockname()[0]

print("Connector file read")


if __name__ == "__main__":
   print("Connector about to run")
   c = Connector()
   print(self.ip)
   c.connect()

   # print(len(c.connections))
   print("there is " + str(1) + " connection")
   # for i in c.connections:
   #    print (i.ip)
   #    print (i.isConnected)
   #    i.socket.close()
   print(c.connection.ip)
   print(c.connection.isConnected)
   c.connection.socket.close()
   print("Connector ran") # * debugger function

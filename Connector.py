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
      self.connections = []
      # Default IP address
      self.ip = "128.174.168.99" # change this to your device's local ip address
      self.isConnected = False

   # originally, this function had an extra argument i that added the last digits to a common default router ip address. 
   # the connect() function tried scanning 100 times for ip addresses with the hardcoded beginning,
   # ending anywhere between 1 and 100. However, this had the limitation of only searching for ip addresses
   # that began in "192.168.1". Since searching for all possible ip addresses is impractical, and portability
   # is not a priority for this application, I chose to hardcode the ip address of the physical device
   # this program will be connecting to. Now, the scan will only run once per connect attempt, and search
   # specifically for the hardcoded ip address. 

   def scan(self):
      print("scan about to run")
      s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
      s.settimeout(TIMEOUT)
      try:
         s.connect((self.ip, TCP_PORT))
         self.connections.append(Connection(s, self.ip, TCP_PORT, True)) 
      except:
         pass
      print("scan ran")

   # ------------- #

   def connect(self):
      self.threads = []
      self.connections = []
      
      try:
         self.ip = self.get_ip_address().rpartition('.')[0] + "."
      except:
         pass

      self.threads.append(threading.Thread(target=self.scan))
      for t in self.threads:
         t.start()
      for t in self.threads:
         t.join()
      print ("DONE.")

   # ------------- #

   def findConnection(self, ip):
      for idx, i in enumerate(self.connections):
         if i.ip == ip:
            return idx
      return -1

   def clear(self):
      for i in self.connections:
         i.socket.close()

      self.connections = []
      self.threads = []

   # ------------- #

   def get_ip_address(self):
      s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
      s.connect(("8.8.8.8", 80))
      return s.getsockname()[0]

print("Connector file read")


if __name__ == "__main__":
   print("Connector about to run")
   c = Connector()
   print(c.get_ip_address().rpartition('.')[0] + ".")
   c.connect()

   print(len(c.connections))
   for i in c.connections:
      print (i.ip)
      print (i.isConnected)
      i.socket.close()

   if len(c.connections) == 0:
      c.isConnected == False

   print("Connector ran")

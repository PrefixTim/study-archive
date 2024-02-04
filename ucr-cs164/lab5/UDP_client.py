'''
	udp socket client
	Silver Moon
'''

import socket	#for sockets
import sys	#for exit

# create dgram udp socket
try:
	s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
except socket.error as err:
	print('Failed to create socket. Error: ' + str(err))
	sys.exit()

host = 'localhost'
port = 3452

while(1) :
	msg = input('Enter message to send : ')
	
	try :
		#Set the whole string
		s.sendto(msg.encode(), (host, port))
		
		# receive data from client (data, addr)
		d = s.recvfrom(1024)
		reply = d[0].decode()
		addr = d[1]
		
		print ('Server reply : ' + reply)
	
	except socket.error as err:
		print('Error: ' + str(err))
		sys.exit()
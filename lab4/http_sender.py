# Socket client example in python

import socket  # for sockets
import sys  # for exit

try:
    # create an AF_INET, STREAM socket (TCP)
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
except socket.error as err:
    print('Failed to create socket. Error: ' + str(err))
    sys.exit()

print('Socket Created')

host = 'www.google.com'

try:
    remote_ip = socket.gethostbyname(host)

except socket.gaierror:
    # could not resolve
    print('Hostname could not be resolved. Exiting')
    sys.exit()

print('Ip address of ' + host + ' is ' + remote_ip)

# Connect to remote server
s.connect((remote_ip, 80))

print('Socket Connected to ' + host + ' on ip ' + remote_ip)

# Send some data to remote server
message = b"GET / HTTP/1.1\r\n\r\n"

try:
    # Set the whole string
    s.sendall(message)
except socket.error:
    # Send failed
    print('Send failed')
    sys.exit()

print('Message send successfully')

# Now receive data
reply = s.recv(4096)

print(reply.decode())

s.close()

import socket
import sys
from threading import Lock
from threading import Thread
import queue
import random
import time

HOST = ''
PORT = 1452 + 2000# + random.randint(0, 1000)

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
print(f'Socket created on Port={PORT}')

try:
    s.bind((HOST, PORT))
except socket.error as err:
    print('Failed to create socket. Error: ' + str(err))
    sys.exit()
print('Socket bind complete')

message_q = queue.Queue()
addresses = set()
lock = Lock()

while True:
    data, addr = s.recvfrom(1024)
    print('Connected with ' + addr[0] + ':' + str(addr[1]) + ' Data: ' + data.decode())
    with lock:
        addresses.add(addr)
    print(addresses)
    data = data.decode().strip()
    if data.startswith('!q'):
        with lock:
            addresses.remove(addr)
        s.sendto('Removed\n'.encode(), addr)
    elif data.startswith('!sendall '):
        msg = data.replace('!sendall ', '') + "\n"
        with lock:
            for addr in addresses:
                try:
                    s.sendto(msg.encode(), addr)
                except socket.error as _:
                    addresses.remove(addr) # doesn;t work =(
    else:
        s.sendto(('OK\t' + data + "\n").encode(), addr)

s.close()

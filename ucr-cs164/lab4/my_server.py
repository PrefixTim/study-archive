import socket
import sys
import threading
import queue
import random
import time

HOST = ''
PORT = 1452 + 2000# + random.randint(0, 1000)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print(f'Socket created on Port={PORT}')

try:
    s.bind((HOST, PORT))
except socket.error as err:
    print('Failed to create socket. Error: ' + str(err))
    sys.exit()
print('Socket bind complete')

s.listen(10)
print('Socket now listening')

message_q = queue.Queue()
connections = dict() # thread unsafe nned to add lock

def clientthread(index):
    connections[index].send(b'Welcome to the server. Type something and hit enter\n') 
    while True:
        data = connections[index].recv(1024).decode().strip()
        reply = 'OK\t'
        print(data)
        if data.startswith('!q'):
            break
        elif data.startswith('!sendall '):
            message_q.put(data.replace('!sendall ', ''))
        else:
            reply += data

        connections[index].sendall((reply + '\n').encode())

    connections[index].close()
    del connections[index]

def send2all():
    while True:
        while message_q.empty():
            time.sleep(0.1)
        msg = message_q.get()
        for index in connections:
            connections[index].sendall((msg + '\n').encode())

threading.Thread(target=send2all, args=()).start()

while True:
    conn, addr = s.accept()
    print('Connected with ' + addr[0] + ':' + str(addr[1]))
    index = hash(conn)
    connections[index] = conn
    threading.Thread(target=clientthread, args=(index,)).start()

s.close()

from RDT3 import *

HOST = ''
PORT = 8888
ADDR = (HOST, PORT)

s = newsock(ADDR)
print("Created Socket and bind\n")
seq = False
while True:
    data, seq = recieve(s, error=0.9, seq=seq)
    print(data, "\n\n")
    if data == b'':
        break

s.close()

from RDT3 import *

HOST='localhost'
PORT = 8888
ADDR = (HOST, PORT)

s = newsock()
seq = False
for i in range(10):
    msg= 'Message #'+str(i)

    seq = sendto(s, msg.encode(), ADDR, error=0.75, seq=seq)

sendto(s, b'', ADDR, seq=seq)
s.close()
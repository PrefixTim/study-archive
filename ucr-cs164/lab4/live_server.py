import socket
import sys

HOST = ''  # Symbolic name meaning all available interfaces
PORT = 8888  # Arbitrary non-privileged port

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print('Socket created')

try:
    s.bind((HOST, PORT))
except socket.error as err:
    print('Failed to create socket. Error: ' + str(err))
    sys.exit()

print('Socket bind complete')

s.listen(10)
print('Socket now listening')

# now keep talking with the client
while 1:
    # wait to accept a connection - blocking call
    conn, addr = s.accept()
    print('Connected with ' + addr[0] + ':' + str(addr[1]))

    data = conn.recv(1024)
    reply = 'OK...' + data.decode()
    if not data:
        break

    conn.sendall(reply.encode())
conn.close()
s.close()

import socket
import sys
import threading

HOST = ''  # Symbolic name meaning all available interfaces
PORT = 8888  # Arbitrary non-privileged port

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print('Socket created')

# Bind socket to local host and port
try:
    s.bind((HOST, PORT))
except socket.error as err:
    print('Failed to create socket. Error: ' + str(err))
    sys.exit()

print('Socket bind complete')

# Start listening on socket
s.listen(10)
print('Socket now listening')


# Function for handling connections. This will be used to create threads
def clientthread(conn):
    # Sending message to connected client
    conn.send(b'Welcome to the server. Type something and hit enter\n')  # send only takes string

    # infinite loop so that function do not terminate and thread do not end.
    while True:

        # Receiving from client
        data = conn.recv(1024)
        reply = 'OK...' + data.decode()
        if not data:
            break

        conn.sendall(reply.encode())

    # came out of loop
    conn.close()


# now keep talking with the client
while 1:
    # wait to accept a connection - blocking call
    conn, addr = s.accept()
    print('Connected with ' + addr[0] + ':' + str(addr[1]))

    # start new thread takes 1st argument as a function name to be run, second is the tuple of arguments to the function.
    threading.Thread(target=clientthread, args=(conn,)).start()

s.close()

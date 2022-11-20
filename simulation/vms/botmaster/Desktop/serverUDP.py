import socket
import sys
import base64

HOST = "111.11.11.10"
PORT = 65321
BUFSIZE = 1024

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind((HOST, PORT))
while 1:
    data, addr = s.recvfrom(BUFSIZE)
    print(data)
    print ('server received from %r\n', addr)   # %%r  (data.decode())
#    s.sendto(data, addr)

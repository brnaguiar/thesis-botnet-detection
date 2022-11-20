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

def gen_pktlen_binomial():
    n = 4096     # ~ 300k
    size = 15000 # ~ 300k
    prob = 0.15  # estes valores experimentei para ver se davam um grafico parecido...
    y = np.random.binomial(n, prob, size=size)           # gen binomial random distribution
    y = np.interp(y, (np.amin(y), np.amax(y)), (2, 50))  # scale to 2-50 characteres... #50
    #return y
    return [round(x) for x in y]                                        #

def gen_pktlen_normal():
    mu = 0
    sigma = 0.5
    n = 200000
    y = np.random.normal(mu, sigma, n)
    #return [round(x) for x in y]
    y = np.interp(y, (np.amin(y), np.amax(y)), (2, 20))  # scale to 2-50 characteres...
    return [round(x) for x in y] 

def gen_negative_binomial():  #????
    maxv = 70
    minv = 2
    n = 7#8
    p = 0.50#0.35 
    r = np.random.negative_binomial(n, p, size=1000)  #5555
    r = r % (maxv - minv + 1) + minv
    return r

def gen_beta_negative_binomial():
    minv = 1
    maxv = 60 
    a = 1.5; b = 1.7; n = 3;
    p = np.random.beta(a, b, size=1000)
    r = np.random.negative_binomial(n, p)
    return r % (maxv - minv +1) + minv

def gen_pps():
    exp = 1+np.random.exponential(scale=1000, size=5000) # generate random exponential dist
    exp = np.interp(exp, (np.amin(exp), np.amax(exp)), (1, 150)) # scale to max 150pps
    exp = np.round(exp)  # float to int
    exp1 = exp[exp < 13] # for
    exp2 = exp[exp >=13] # more accentuated
    expx = [exp1, exp2]  # exp dist.
    picks = [] # pps
    for k in range(5000):
        idx = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1] # weighted random pick 
        pick = random.choice(expx[idx[random.randint(0, 19)]]) #pick a pps
        picks.append(pick) 
    return  np.array(picks) 


HOST = "127.0.0.1"      #111.11.11.10111.11.11.10111.11.11.10111.11.11.10
PORT = 4440             # The port used by the server
file2send = "../../../files/textfile900k"  ##  ./../../.

# GEN PACKET SIZES
pick_pkt_lens = gen_negative_binomial()
print(len(pick_pkt_lens ))

# FILE TO MEM
file_ = open(file2send, "r")
txt_mem = []
end_of_file = False
while end_of_file == False:
    txt = file_.read(pick_pkt_lens[random.randint(2, 60)])
    if txt != '':
        txt_mem.append(txt)
    if txt == '':
        end_of_file = True

# GEN PPS SIZES
    pps = gen_beta_negative_binomial()

n_120_150 = random.randint(1, 3)
n_60_100 = random.randint(4, 8)
#n_30_50 = random.randint(30, 50)
n_12_30 = 120 - n_120_150 - n_60_100 #- n_30_50 

pps_30_50 = [random.randint(20, 80) for i in range(n_60_100)]
#pps_30_50 = [random.randint(34, 104) for i in range(n_30_50)]
pps_12_20 = [random.randint(8, 15) for i in range(n_12_30)]

# DIVIDE PPS INTO HIGH AND LOW...
low_pps =  [random.randint(2, 5) for i in range(1900)]#   list(pps[(pps <= 7)])
high_pps =  pps_30_50  + pps_12_20#    list(pps[(pps > 7)])

group_pps = []

while(len(high_pps) > 0):
    pick_high_sample = high_pps.pop(random.randint(0, len(high_pps)-1))
    n_low_samples = 0
    if pick_high_sample <= 20: 
        n_low_samples = random.randint(1, 4)
    elif pick_high_sample > 20 and pick_high_sample < 60:
        n_low_samples = random.randint(5, 10)
    else:
        n_low_samples = random.randint(7, 15)
    pps = [pick_high_sample]
    print(pps)
    for i in range(n_low_samples):
        pps.append(low_pps[random.randint(0, len(low_pps)-1)])  #   dont pop... pick only.
    random.shuffle(pps) # shufle pps
    print(pps)
    group_pps.append(pps)

random.shuffle(group_pps)
print(len(group_pps))

group_txt = []
for pps_array in group_pps:
    aux = []
    for pps in pps_array:
        aux.append(
            [txt_mem[random.randint(0, len(txt_mem)-1)].encode(encoding='ascii') for i in range(int(pps))]
        )
    group_txt.append(aux)

#print(group_pps[0])
print(group_txt[0])

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
    for pps_group in group_txt:
        for pps in pps_group:
            for txt in pps:
                s.sendall(txt)
                time.sleep(0.018)
                #print("len: ", len(txt.decode()))
            print("PPS: ", len(pps))
            time.sleep(random.uniform(1.5, 10))
        ts = random.uniform(15, 30)
        print(ts)
        time.sleep(ts)

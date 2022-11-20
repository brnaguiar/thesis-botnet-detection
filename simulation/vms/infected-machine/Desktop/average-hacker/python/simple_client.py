import base64
import socket
import numpy as np
import random
import time
from collections import deque
import string
HOST = "111.11.11.10"   # "127.0.0.1"      #111.11.11.10
PORT = 65321#4440             # The port used by the server
file2send = "../files/textfile300k"

## GEN PACKET SIZES
#pick_pkt_lens = gen_pktsizes()
#
# FILE TO MEM
#file_ = open(file2send, "r")
#txt_mem = []
#end_of_file = False
#while end_of_file == False:
#    txt = file_.read(pick_pkt_lens.pop(random.randint(2, 50))) 
#    if txt != '':
#        txt_mem.append(txt)
#    if txt == '':
#        end_of_file = True
#a = ''
#while(len(a) < 50):
#    a = txt_mem.pop()
#print(len(a))

# 50 = ~ 60/80 pps
# 1 - 6: < 10 pps safe
# 7 - 8/9: < 12.5/15pps safe
# 10 - 17 -> 12.5/25 pps safe 
# 18 - 30 -> 25 / 40 pps
#sendtxts = [deque([txt_mem.pop(random.randint(0, len(txt_mem)-1))
#                for i in range(random.randint(18, 30))]) for i in range(10)]
#
#for s in sendtxts:
#    print(len(s))

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    i = 0
    string_input = ''.join(random.choice(string.ascii_lowercase) for x in range(1))
    print(string_input)
    s.sendall(str.encode(string_input))
#    for dq in sendtxts:
#        while(len(dq) > 0):
#            s.sendall(str.encode(dq.pop()))
#            time.sleep(0.020)
#        print("len packets: ", len(dq))
#        time.sleep(10)



import numpy as np
import argparse

def beta_binomial(a, b, n):
    p = np.random.beta(a, b, size=None)
    r = np.random.binomial(n, p)
    return r

def beta_negative_binomial(a, b, n, min=1, max=150):
    p = np.random.beta(a, b, size=None)
    r = np.random.negative_binomial(n, p)
    return r % (max - min + 1) + min

def negative_binomial(n, p, min=2, max=70):
    #print(p)
    r = np.random.negative_binomial(n, p, size=None)
    r = r % (max - min + 1) + min
    return r

def empirical_prob(pps_or_pktlen):
    global pps_arr
    global pktlen_arr
    if pps_or_pktlen == "pps":
        return np.random.choice(list(range(np.size(pps_arr))), size=None, replace=True, p=pps_arr)
    if pps_or_pktlen == "silence":
        return np.random.choice(list(range(np.size(silence_arr))), size=None, replace=True, p=silence_arr)
    if pps_or_pktlen == "low_sil":
        return np.random.choice([6, 10, 420, 700, 800, 850, 900], size=None, p=[0.10, 0.10, 0.20, 0.10, 0.20, 0.10, 0.20]) #1 1111repor 3, 6...    
    else:
        return np.random.choice(list(range(np.size(pktlen_arr))), size=None, replace=True, p=pktlen_arr) 


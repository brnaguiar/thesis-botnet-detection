import numpy as np
import argparse

def beta_binomial(a, b, n):
    p = np.random.beta(a, b, size=None)
    r = np.random.binomial(n, p)
    return r

def beta_negative_binomial(a, b, n, min=1, max=150):
    #print(min)
    p = np.random.beta(a, b, size=None)
    r = np.random.negative_binomial(n, p)
    return r % (max - min + 1) + min

def negative_binomial(n, p, min=2, max=70):
    r = np.random.negative_binomial(n, p, size=None)
    return r % (max - min + 1) + min

def empirical_prob(filename):
    arr = np.load("dist_weights/" + filename + ".npy")
    return np.random.choice(list(range(np.size(arr))), size=None, Replace=True, p=arr)

parser = argparse.ArgumentParser()
parser.add_argument('-m', '--method', nargs=1, required=True, help='function method')
parser.add_argument('-p', '--params', nargs='*', required=True, help="function parameters")
args = parser.parse_args()

dist_method = args.method[0]
params_values = [float(a) for a in args.params]

## PPS
if dist_method == "beta_negative_binomial":
    print(beta_negative_binomial(params_values[0], params_values[1], params_values[2]))
    exit(0)

## average packet length
if dist_method == "negative_binomial":
    print(negative_binomial(params_values[0], params_values[1]))
    exit(0)

if dist_method == "beta_binomial":
    print(beta_binomial(params_values[0], params_values[1], params_values[2]))

import numpy as np
import math

def FFT(s):
    Fs = 10000 # sample rate
    Ts = 1.0/Fs; # sampling interval
    ts = np.arange(0,1,Ts) # time vector
    y = s # the data to make the fft from
    n = len(y) # length of the signal
    k = np.arange(n)
    T = n/Fs
    frq = k/T # two sides frequency range
    frq = frq[range(int(n/2))] # one side frequency range
    Y = np.fft.fft(y)/n # fft computing and normalization
    Y = Y[range(int(n/2))]


n = 1024
l1 = []
l2 = []
l3 = []

pi = np.pi
def sine(x):
    return np.sin(x)


for i in range(n):
    l1.append(sine(pi*i + pi))
    l2.append(2*sine(2*pi*i + pi/2))
    l3.append(1.5*sine(4*pi*i + pi/4))

s1 = np.array(l1)
s2 = np.array(l2)
s3 = np.array(l3)
ss = s1 + s2 + s3
t = np.linspace(0,1,n)

print("00000000000000000")
for s in ss:
    print(f"{s},")


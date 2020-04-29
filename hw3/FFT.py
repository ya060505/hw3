import matplotlib.pyplot as plt

import numpy as np

import serial

import time


Fs = 100.0;  # sampling rate

Ts = 10.0/Fs; # sampling interval

t = np.arange(0,10,Ts) # time vector; create Fs samples between 0 and 1.0 sec.

x = np.arange(0,10,Ts) # signal vector; create Fs samples

y = np.arange(0,10,Ts) # signal vector; create Fs samples

z = np.arange(0,10,Ts) # signal vector; create Fs samples

tilt = np.arange(0,10,Ts) # signal vector; create Fs samples


serdev = '/dev/ttyACM0'

s = serial.Serial(serdev, 115200)

for i in range(0, int(Fs)):

    line=s.readline() 

    x[i] = float(line)

    line=s.readline() 

    y[i] = float(line)

    line=s.readline() 

    z[i] = float(line)

    line=s.readline() 

    tilt[i] = float(line)
    


fig, ax = plt.subplots(2, 1)

ax[0].plot(t,x, color="red", linewidth=2.5, linestyle="-", label="x")

ax[0].plot(t,y, color="blue", linewidth=2.5, linestyle="-", label="y")

ax[0].plot(t,z, color="green", linewidth=2.5, linestyle="-", label="z")

ax[0].legend(loc='lower left', frameon=True)

ax[0].set_xlabel('Time')

ax[0].set_ylabel('Acc Vector')

ax[1].stem(t,tilt)

ax[1].set_xlabel('Time')

ax[1].set_ylabel('Tilt')

plt.show()

s.close()
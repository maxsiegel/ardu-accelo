import time

import matplotlib.pyplot as plt
import serial
from mpl_toolkits.mplot3d import Axes3D

ser = serial.Serial('/dev/ttyACM0', 9600)

fig = plt.figure()
# fig.show()

ax = fig.add_subplot(111, projection='3d')
plt.ion()
plt.show()
n = 1

xs = []
ys =[]
zs=[]

while True:


    try:
        data = ser.readline()
        print(data)
        d = eval(data)
        # print(d['accel'])       #
        x, y, z = d['accel']
        xs.append(float(x))
        ys.append(float(y))
        zs.append(float(z))

        ax.scatter(xs, ys, zs)
        if n == 1:
            plt.show()
        else:
            plt.pause(.1)
        time.sleep(.01)
        print(n)
        n+=1

    except Exception as err:
        print(err)

import datetime
import select
import socket
import sys
import time
from os.path import join

timestamp = datetime.datetime.now().isoformat().replace('-', '_').replace(':', '_').replace('.', '_')

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(('', 7400))

t = 1;

filename = join('/home/maxs/temp/ardu-test-files/', timestamp)

with open(filename, 'wt') as f:

    while True:
        data = s.recvfrom(1024)

        # print data

        f.write(str(data[0]) + '\n')

        t += 1
        time.sleep(.01)
        if not t % 10:
            print('--------------------------------------------------------------------------------')
            print('receiving data...' + str(t))

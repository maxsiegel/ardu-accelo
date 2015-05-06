import datetime
import select
import socket
import sys
import time
from os.path import join

timestamp = datetime.datetime.now().isoformat().replace('-', '_').replace(':', '_').replace('.', '_')

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# s.settimeout(2)
s.bind(('', 7400))

t = 1;

with open(join('/home/maxs/temp/ardu-test-files/', timestamp), 'wb'):
    while True:
        data = s.recvfrom(1024)
        # d.append(data)
        if data[0] != '\n':
            if data[0] != '\r':
                print data[0]
        # print(data.decode('utf-8'))
        t += 1
        time.sleep(.01)
        # print 'time: ' + str(t)

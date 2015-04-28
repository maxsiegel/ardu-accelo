import select
import socket
import sys

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# s.settimeout(2)

s.connect(('192.168.12.46', 7))


s.send('\n')

d = []
while True:
    data = s.recv(4096)
    # d.append(data)
    print data
    # print(data.decode('utf-8'))

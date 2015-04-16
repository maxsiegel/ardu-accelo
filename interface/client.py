## copied mostly from http://stackoverflow.com/questions/14802608/running-game-engine-while-reading-data-wirelessly-in-blender

#
# Client Program
# Make sure the server is being run in Blender

import json
import socket
import time

print 'INFO: Creating Socket'
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# ip_addr = raw_input('IP: ')
# port_addr = raw_input('PORT: ')

# Type 'localhost' in the IP field
# Type '8081' in the PORT field

def connect():

    ip_addr = 'localhost'
    port_addr = '8081'

    print 'INFO: Connecting to server'
    s.settimeout(5) # number of bad attempts before closing
    s.connect((ip_addr, int(port_addr)))

    # Listen for welcome
    data = s.recv(100)
    print data
    print ''

def send_to_blender(message):
    # while 1:                    #
    # message = raw_input('CLIENT: ')
    # # if message=='end' or message=='End' or message=='END':
    #     print ''
    #     print 'SHUTTING DOWN CLIENT, SERVER STILL RUNNING'
    #     s.send(message)
    #     break
    # elif message=='end server' or message=='End server' or message=='End Server':
    #     print ''
    #     print 'SHUTTING DOWN SERVER'
    #     s.send(message)
    #     break
    # else:
    s.send(json.dumps(message))
    data = s.recv(10000)
    print data

def quit():
    print 'INFO: Closing socket'
    s.close()
    print 'INFO: Quitting'

# connect()
# while True:

#     send_to_blender('hello')

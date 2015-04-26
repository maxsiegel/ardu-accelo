## copied mostly from http://stackoverflow.com/questions/14802608/running-game-engine-while-reading-data-wirelessly-in-blender

# Server Program
# Make sure the client is being run on the data generation computer



import json
import socket
import sys

import bge

print('INFO: Starting up')


# Create a TCP/IP socket to listen on
print('INFO: Creating TCP/IP Socket')
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Prevent from 'ADDRESS ALREADY IN USE' upon restart
print('INFO: Housekeeping...')
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

# Bind the socket to port 8081 on all interfaces
server_address = ('localhost', 8081)
print('INFO: Binding and starting up on %s port %s' % server_address)
server.bind(server_address)
print('INFO: Server bound')

def send_welcome(cont):
    cont.send('SERVER: Welcome'.encode('utf8'))

# Listen for connectons for 5 seconds
server.listen(5)

# Connection is the SOCKET OBJECT for the connection
# Client_address is the connected peer(the client)
connection, client_address = server.accept()
print('INFO: Connection from', connection.getpeername())
print('INFO: Sending welcome msg')
send_welcome(connection)



def main():
    SERVER_LOOP = True
    print('test')
    # import bge

    # cont = bge.logic.getCurrentController()
    # owner = cont.owner


    # while SERVER_LOOP:

        # Receive data
    try:
        data = connection.recv(10000)

    # Unless there's an error
    except OSError:
        print(connection)

    # Decode the data into usable lists
    if type(data) != type(''):
        data = data.decode()

    # If we want to end the client stream but keep the server running
    # if data=='end' or data=='End' or data=='END':
    #     print('INFO: Closing connection with ',connection.getpeername())
    #     connection.shutdown(socket.SHUT_RD | socket.SHUT_WR)

    #     connection.close()
    #     connection, client_address = server.accept()
    #     print('INFO: Connection from', connection.getpeername())
    #     print('INFO: Sending welcome msg')
    #     send_welcome(connection)


    # If we want to stop running the server
    # elif data=='end server' or data=='End server' or data=='End Server':

    #     print('SERVER SHUT DOWN')
    #     SERVER_LOOP = False

    # Display when data is loaded back on the client side


    bad = True
    while bad:
        try:
            data = eval(json.loads(data))
            # print(data)
            #!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            # print(bge.logic.getCurrentScene())
            # for o in bge.logic.getCurrentScene():
                # if o.name == 'Cube':
                    # box = o
            box = bge.logic.getCurrentController().owner
            # print(box)
            motion_actuators  = [act for act in box.actuators if act.name == 'Motion']
            motion_actuator = motion_actuators[0]

            instantaneous_velocity = data['accel']
            motion_actuator.linV = instantaneous_velocity
            motion_actuator.useLocalLinV = False
            box.reinstancePhysicsMesh()

            print(motion_actuator.linV)
            bad = False
        except:
            print("malformed data")
            # print(data)
    #!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


        # owner['test'] = data
        # ('CLIENT: %s' % data)
        # message = data
        # print(data)

        # print('SERVER: %s' % data)
# import bge

# connection.close()
# def main():
#     a = bge.logic.getCurrentController()
#     print(a)

import serial

import client


def main():
    ser = serial.Serial('/dev/ttyACM0', 9600)

    # should send a python dictionary

    client.connect()
    while True:
        data = ser.readline()
        print(data)
        client.send_to_blender(data)
        # dat = eval(d)

if __name__ == '__main__':
    main()

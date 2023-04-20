# Python script to SEND data to Arduino script over the serial port
    # https://www.hackster.io/ansh2919/serial-communication-between-python-and-arduino-e7cce0

import serial
import time

arduino = serial.Serial(port='COM4', baudrate=115200, timeout=.1)

def write_read(x):
    arduino.write(bytes(x, 'utf-8'))
    time.sleep(0.05)
    data = arduino.readline()
    return data


while True:
    num = input("Enter a number: ")
    value = write_read(num)
    print(value)
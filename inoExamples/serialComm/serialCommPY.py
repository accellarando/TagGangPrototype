# Python script to SEND data to Arduino script over the serial port
    # https://www.hackster.io/ansh2919/serial-communication-between-python-and-arduino-e7cce0

import serial
import time

# Arduino Uno (COM9) connected to left-side USB
ser = serial.Serial(port='COM9', baudrate=9600, timeout=.1)

def write_read(x):
    if not ser.isOpen():
        ser.open()
        #print('com is open', ser.isOpen())
  
    ser.write(bytes(x, 'utf-8'))
    time.sleep(0.05)
    data = ser.readline()
    return data

while True:
    num = input("Enter a number: ")
    value = write_read(num)
    print(value)
    ser.close()  # Close the serial port
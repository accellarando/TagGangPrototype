# Python script to SEND data to Arduino script over the serial port
    # https://www.hackster.io/ansh2919/serial-communication-between-python-and-arduino-e7cce0

import serial
import time

def increment_input():
    while True:
        inputNum = int(input("Enter a number: "))

        #print(inputNum)
        #print(str(inputNum).encode()) # prints b'3' (bytes object containing the UTF-8 encoded string "3")

        incrementNum = (int(send_receive_arduino(inputNum)))
        
        print("Incremented number: ", incrementNum)

# args = int
def send_receive_arduino(x):
    ser = serial.Serial('COM9', 9600)
    #time.sleep(1.5)
    # x data is of int type, must convert int to string then to bytes before sending
    # with encode() as this sends as a byte string == byte stream
    ser.write(str(x).encode())
    
    # wait for modified data from arduino to be sent back
    # decode() only if reading data that is sent from the Arduino to the Python code
    # (if the data is sent in a non-ASCII format, such as bytes or a string of bytes)
    # to be interpreted correctly in python

    response = ser.read().decode().strip()
    #.decode().strip()

    # do not need a Serial .read..() function if the arduino prints the final output
    # only need it if you want to capture or process the output from the Arduino 
    # in your Python script

    

    # must close port when done using, then return response data from arduino if .read()
    ser.close()
    return int(response)


# Call the increment_input function to start the program
increment_input()
# Python script to SEND data to Arduino script over the serial port
    # https://www.hackster.io/ansh2919/serial-communication-between-python-and-arduino-e7cce0

import serial

def send_receive_from_arduino(x):
    # Establish connection to Arduino Uno
    ser = serial.Serial('COM9', 9600)
    # Encode and send data to Arduino
    ser.write(str(x).encode())
    # Wait for Arduino to process data and send response
    arduino_data = ser.readline().decode().strip()
    # Close connection to Arduino
    ser.close()
    # Return response from Arduino
    return arduino_data

def increment_input():
    # Ask user for input and convert to integer
    num = int(input("Enter a number: "))
    #print(num)
    # Add 1 to input using Arduino
    send_receive_from_arduino(num)
    # Print the incremented number
    #print("Incremented number: ", incremented_num)

# Call the increment_input function to start the program
increment_input()

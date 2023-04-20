# move_stepper.py 
E Kinect will send information to computer
E Computer translates that information to a stepper motor command, e.g. "-3" or "24"
R Then, this program calls a script to send the stepper motor command to the arduino over serial:
	./stepper_control.py -3
R The Arduino picks up this serial command
R Arduino sends serial command to stepper driver
R Stepper motor moves to desired positio
# Python script to send the stepper motor command that gives the absolute position(main.c, d_pos) to the Arudino script "serialMotorDrive.ino"
# over serial communication

#Python pseudocode for moving the stepper motor
# https://github.com/accellarando/TagGang/blob/main/docs/meetings/20230415.txt
# https://github.com/accellarando/TagGangPrototype/blob/main/shellOutput.txt\
# https://github.com/accellarando/TagGangPrototype/blob/main/main2.c
# https://www.youtube.com/watch?v=7spK_BkMJys
# https://howtomechatronics.com/tutorials/arduino/stepper-motors-and-arduino-the-ultimate-guide/
# https://docs.google.com/document/d/15zFXHQggSIYDtgaQV2rhGvOE4UlRhYPvJxaJMdB6EUI/edit?usp=sharing

# serial comm between py and ino
    # https://www.hackster.io/ansh2919/serial-communication-between-python-and-arduino-e7cce0
# Importing Libraries
import serial
import time
arduino = serial.Serial(port='COM4', baudrate=115200, timeout=.1)
def write_read(x):
    arduino.write(bytes(x, 'utf-8'))
    time.sleep(0.05)
    data = arduino.readline()
    return data
while True:
    num = input("Enter a number: ") # Taking input from user
    value = write_read(num)
    print(value) # printing the value




#!/usr/bin/python3

import os #and whatever other libraries you'll need

#This function should read the arg from the command line and return a number
def get_cmd_arg():
    return 0

#This function should connect to arduino and send the number of steps we want to move
def send_to_arduino(steps):


if __name__ == "__main__":
    steps = get_cmd_arg()
    send_to_arduino(steps)

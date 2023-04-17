#!/usr/bin/python3

#Python pseudocode for moving the stepper motor
# https://github.com/accellarando/TagGang/blob/main/docs/meetings/20230415.txt
# https://github.com/accellarando/TagGangPrototype/blob/main/shellOutput.txt\
# https://github.com/accellarando/TagGangPrototype/blob/main/main2.c
# https://www.youtube.com/watch?v=7spK_BkMJys
# https://howtomechatronics.com/tutorials/arduino/stepper-motors-and-arduino-the-ultimate-guide/
# https://docs.google.com/document/d/15zFXHQggSIYDtgaQV2rhGvOE4UlRhYPvJxaJMdB6EUI/edit?usp=sharing

import os #and whatever other libraries you'll need

#This function should read the arg from the command line and return a number
def get_cmd_arg():
    return 0

#This function should connect to arduino and send the number of steps we want to move
def send_to_arduino(steps):


if __name__ == "__main__":
    steps = get_cmd_arg()
    send_to_arduino(steps)

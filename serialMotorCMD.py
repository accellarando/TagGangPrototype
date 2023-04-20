# WAS move_stepper.py 

#Python pseudocode for moving the stepper motor
# https://github.com/accellarando/TagGang/blob/main/docs/meetings/20230415.txt
# https://github.com/accellarando/TagGangPrototype/blob/main/shellOutput.txt\
# https://github.com/accellarando/TagGangPrototype/blob/main/main2.c
# https://www.youtube.com/watch?v=7spK_BkMJys
# https://howtomechatronics.com/tutorials/arduino/stepper-motors-and-arduino-the-ultimate-guide/
# https://docs.google.com/document/d/15zFXHQggSIYDtgaQV2rhGvOE4UlRhYPvJxaJMdB6EUI/edit?usp=sharing

# serial comm between py and ino
    # https://www.hackster.io/ansh2919/serial-communication-between-python-and-arduino-e7cce0

# C to Python
    # https://www.digitalocean.com/community/tutorials/how-to-use-subprocess-to-run-external-programs-in-python-3
    # https://stackoverflow.com/questions/42993038/using-subprocess-call-to-read-output-of-c-executable-print-statement-realtime
    
#!/usr/bin/python3

# main.c script updates the int variable, d_pos (private to the move_stepper function), in real time:
# using Python's subprocess module to run the C script as a subprocess and capture its output.
# This output is the step value needed to step the motor in Arduino, which is done through
# serial connections between the Python and Arudino scripts.

# Importing libraries
import subprocess
import serial # PySerial library to open a serial connection

# Function: read the C script as a subprocess to capture and send step value from a specific output line
# through another function
def read_c_output():
     # Run main.c as a subprocess and redirect its output to a pipe
     # File is located in the current working directory as serialMotorCMD.py
     p = subprocess.Popen(["./main.c"], stdout=subprocess.PIPE)

     # Read and parse the output of main.c, line by line, to find the move_stepper function's output line
     for line in iter(p.stdout.readline, b''):
          # Decode the bytes returned by the subprocess' output to a string and strip any white spaces at the start or end of the line
          line = line.decode().strip()
          # Extract the int, d_pos, from the move_stepper function's output line into the int, steps
          # Assumes line format: "Step motor by %d\n", where %d is replaced by d_pos
          if "Step motor by" in line:
               steps = int(line.split()[-1])
               # Call the send_to_arduino function with the extracted d_pos value (int steps) as its arg
               send_to_arduino(steps)
               # FIXME do we need to break?

# Function: send step value from Python to Arduino via serial communication
def send_to_arduino(steps):
     # Open and configure serial connection (change as needed) using the PySerial library
     # FIXME what baud rate to use? - Arduino default = 9600
     ser = serial.Serial(port = 'COM9', baudrate = 9600, timeout = 1) # Windows port
     #ser = serial.Serial(port = '/dev/ttyACM0', baudrate = 115200, timeout = 1) # Linux port

     # Convert the steps value to a byte string and send it to the Arduino
     ser.write(str(steps).encode())

     # Close the serial port
     ser.close()

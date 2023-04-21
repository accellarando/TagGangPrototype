// serial comm between python and arduino
  // https://www.hackster.io/ansh2919/serial-communication-between-python-and-arduino-e7cce0

// https://github.com/accellarando/TagGang/blob/main/docs/meetings/20230415.txt
  // Computer translates that information to a stepper motor command, e.g. "-3" or "24"
  // .py script to send the stepper motor command to the arduino over serial -> e.g. (stepper control)./move_stepper.py -3
  // ! Arduino picks up this serial command
  // ! Arduino sends serial command to stepper driver
  // ! Stepper motor moves to desired position

#include <AccelStepper.h>

// Define the stepper motor connections
#define STEPS_PER_REV 200 // Number of steps per revolution for the stepper motor
#define EN 3              // Enable connected to Nucleo CN5 3, Arduino pin D2 (digital)
#define STEP 5            // Step connected to Nucleo CN5 6, Arduino pin D5 (digital)
#define DIR 7             // Direction connected to Nucleo CN5 8, Arduino pin D7 (digital)

// !! Math already done in move_stepper() main2.c, no need to modify position values in define

// Private variables
int steps;
String input;

AccelStepper stepper(AccelStepper::DRIVER, STEP, DIR); // Instance of the AccelStepper class for the stepper motor

void setup() {
  // Configure the serial connection
  Serial.begin(9600); // Set baud rate for serial port [FIXME DO WE NEED]
  //Serial.setTimeout(1); // Set max of 1 ms wait for serial data, doc: default 1000 ms 

  // Configure the stepper motor
  stepper.setMaxSpeed(1000); // Set max speed value for the stepper (steps/sec), doc: <1000 steps unreliable
  stepper.setAcceleration(500); // Set acceleration (steps per second per second), doc: >0.0 acc

  // Enable the motor driver (Enable = Active LOW, can ignore function as it is by default)
  //stepper.enableOutputs();
}

void loop() {
  // Check if there is data available to read from the serial port
  // FIXME if OR while?
  if (Serial.available() > 0) {
    input = Serial.readStringUntil('\n'); // Read incoming byte stream from serialMotorDriver.py as a string and decode it
    steps = input.toInt(); // Convert the data string to an integer

    // Move the stepper motor by the incoming-data-specified number of steps (non-blocking)
    // move() accounts for positions in both directions relative to current position
    stepper.move(steps);

    // Wait until the stepper motor has reached its target position
    // distanceToGo() is distance from the current position to the target position in steps in both directions
    while (stepper.distanceToGo() ! = 0){
      stepper.run();
    }
  }
}

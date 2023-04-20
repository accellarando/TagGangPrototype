// serial comm between python and arduino
  // https://www.hackster.io/ansh2919/serial-communication-between-python-and-arduino-e7cce0

// https://github.com/accellarando/TagGang/blob/main/docs/meetings/20230415.txt
  // Computer translates that information to a stepper motor command, e.g. "-3" or "24"
  // .py script to send the stepper motor command to the arduino over serial -> e.g. (stepper control)./move_stepper.py -3
  // ! Arduino picks up this serial command
  // ! Arduino sends serial command to stepper driver
  // ! Stepper motor moves to desired position
/*
int x;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1);
}

void loop() {
  while (!Serial.available());
  x = Serial.readString().toInt();
  Serial.print(x + 1);
}
*/

#include <AccelStepper.h>

// Define the pins connected to the stepper motor driver
#define STEP 5 // Connect step pin to A5
#define DIR 4 // Connect direction pin to A4
//int STEP = 5
//pinMode(STEP, OUTPUT);

// ?? Microstepping pins

#define STEPS_PER_REV 200 // Define the number of steps per revolution for the stepper motor

// !! Math already done in move_stepper() main2.c, no need to modify position values in define

AccelStepper stepper(AccelStepper::DRIVER, STEP, DIR); // Instance of the AccelStepper class for the stepper motor

void setup() {
  // Configure the serial port
  // !! https://www.google.com/search?q=how+to+figure+out+what+baud+rate+to+use+arduino&rlz=1C1CHBF_enUS773US773&oq=how+to+figureout+what+baud+rate+to+use+ar&aqs=chrome.1.69i57j33i10i160l3.10089j0j7&sourceid=chrome&ie=UTF-8
  Serial.begin(115200); // !! Set baud rate for serial port FIXME
  //Serial.setTimeout(1); // Set max of 1 ms wait for serial data, doc: default 1000 ms 

  // Configure the stepper motor
  stepper.setMaxSpeed(1000); // Set max speed value for the stepper (steps/sec), doc: <1000 steps unreliable
  stepper.setAcceleration(1000); // Set acceleration (steps per second per second), doc: >0.0 acc
}

void loop() {
  // Serial communication between serialMotorCMD.py script 

  // Read the step value (d_pos) from the serialMotorCMD.py script via serial communication
  int stepVal = 0; // !! put outside
  if (Serial.available() > 0) { // !! Math already done in move_stepper() main2.c
    stepVal = Serial.parseInt();
  }

  // !! SETUP: restart to default position
  while (stepper1.currentPosition() != 0) { // loop executes until both motors reach position 0
  stepper1.run();
  }

  // ?? PSUEDO: create hand-gesture to start tracking

  // Using step values sent from serialMotorCMD.py
  int targetPos = stepVal * STEPS_PER_REV / 360; // Convert the step value to a target position for the stepper motor
  // Move the stepper motor to the target position
  stepper.moveTo(targetPos);
  stepper.run();
}

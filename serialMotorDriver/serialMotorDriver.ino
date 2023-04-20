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

// Define the stepper motor connections
#define STEPS_PER_REV 200 // Number of steps per revolution for the stepper motor
#define STEP 5 // Connect step pin to A5
#define DIR 4 // Connect direction pin to A4
//int STEP = 5
//pinMode(STEP, OUTPUT);
// ?? Microstepping pins

// !! Math already done in move_stepper() main2.c, no need to modify position values in define

// Private variables
int steps;
String input;

AccelStepper stepper(AccelStepper::DRIVER, STEP, DIR); // Instance of the AccelStepper class for the stepper motor

void setup() {
  // Configure the serial connection
  // !! https://www.google.com/search?q=how+to+figure+out+what+baud+rate+to+use+arduino&rlz=1C1CHBF_enUS773US773&oq=how+to+figureout+what+baud+rate+to+use+ar&aqs=chrome.1.69i57j33i10i160l3.10089j0j7&sourceid=chrome&ie=UTF-8
  Serial.begin(115200); // !! Set baud rate for serial port FIXME DO WE NEED?
  //Serial.setTimeout(1); // Set max of 1 ms wait for serial data, doc: default 1000 ms 

  // Configure the stepper motor
  stepper.setMaxSpeed(1000); // Set max speed value for the stepper (steps/sec), doc: <1000 steps unreliable
  stepper.setAcceleration(500); // Set acceleration (steps per second per second), doc: >0.0 acc
}

/**
 * Serial communication between serialMotorCMD.py script
 */
void loop() {
  // Check if there is data available to read from the serial port
  if (Serial.available() > 0) {
    input = Serial.readStringUntil('\n'); // Read incoming data from serialMotorDriver.py as a string (see subprocess1.txt for expl.)
    steps = input.toInt(); // Convert the data string to an integer
    //stepVal = Serial.parseInt();

    // Move the stepper motor by the incoming-data-specified number of steps (non-blocking)
    stepper.move(steps); // move() accounts for positions in both directions relative to current position
    //stepper.moveTo(steps); // long absolute

    // Wait until the stepper motor has reached its target position
    // distanceToGo() is distance from the current position to the target position in steps in both directions
    while (stepper.distanceToGo() ! = 0){
      stepper.run();
    }
  }

  // !! SETUP: restart to default position

  // ?? OUTSIDE OR INSIDE IF COND
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

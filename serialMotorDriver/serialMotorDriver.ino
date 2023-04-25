// serial comm between python and arduino
  // https://www.hackster.io/ansh2919/serial-communication-between-python-and-arduino-e7cce0

// https://github.com/accellarando/TagGang/blob/main/docs/meetings/20230415.txt
  // Computer translates that information to a stepper motor command, e.g. "-3" or "24"
  // .py script to send the stepper motor command to the arduino over serial -> e.g. (stepper control)./move_stepper.py -3
  // ! Arduino picks up this serial command
  // ! Arduino sends serial command to stepper driver
  // ! Stepper motor moves to desired position

#include <Stepper.h>

#define STEPS_PER_REV 200 // Number of steps per revolution for the stepper motor

// Define the stepper motor connections
#define STEP_PIN 6
#define DIR_PIN 7
#define ENABLE_PIN 2
#define CONTROL_PIN 5
#define RESET_PIN 8

//VREF PWM pins
#define VREFA_PIN 3
#define VREFB_PIN 9

// Private variables
int steps;
String input;

Stepper stepper(200, DIR_PIN, STEP_PIN);

void setup(){
  //Initialize pins
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(VREFA_PIN, OUTPUT);
  pinMode(VREFB_PIN, OUTPUT);

  //Set up VREF pins
  analogWrite(VREFA_PIN, 50);
  analogWrite(VREFB_PIN, 50);

  //don't forget this part!!
  digitalWrite(RESET_PIN, HIGH);
  digitalWrite(CONTROL_PIN, LOW);

  stepper.setSpeed(1000);

  // Configure the serial connection, set baud rate
  Serial.begin(115200); 
}

void loop() {
  // Check if there is data available to read from the serial port
  if (Serial.available()) {
    input = Serial.readStringUntil('\n'); // Read incoming byte stream from serialMotorDriver.py as a string and decode it
    steps = input.toInt(); // Convert the data string to an integer

    // Move the stepper motor by the incoming-data-specified number of steps 
    // move() accounts for positions in both directions relative to current position
    digitalWrite(ENABLE_PIN, HIGH);
    stepper.step(steps*8); //8 clock signals = 1 step
    digitalWrite(ENABLE_PIN, LOW);
  }
}

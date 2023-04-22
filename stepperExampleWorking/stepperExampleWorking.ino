#include <AccelStepper.h>
#include <MultiStepper.h>

#define STEP_PIN 6
#define DIR_PIN 7
#define ENABLE_PIN 2
#define CONTROL_PIN 5

#define VREFA_PIN 3
#define VREFB_PIN 9

AccelStepper stepper(AccelStepper::FULL2WIRE, STEP_PIN, DIR_PIN);

void setup(){
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(VREFA_PIN, OUTPUT);
  pinMode(VREFB_PIN, OUTPUT);

  analogWrite(VREFA_PIN, 40);
  analogWrite(VREFB_PIN, 40);
  digitalWrite(ENABLE_PIN, HIGH);
  
}

void loop(){
  stepper.moveTo(-stepper.currentPosition());
  stepper.run();
  delay(500);
}

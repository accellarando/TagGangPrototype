#include <Stepper.h>

#define STEP_PIN 6
#define DIR_PIN 7
#define ENABLE_PIN 2
#define CONTROL_PIN 5
#define RESET_PIN 8

#define VREFA_PIN 3
#define VREFB_PIN 9

byte directionPin = 7;
int numberOfSteps = 100;
byte ledPin = 13;
int pulseWidthMicros = 20;  // microseconds
int millisbetweenSteps = 250;

Stepper stepper(200, DIR_PIN, STEP_PIN);

void setup(){
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(VREFA_PIN, OUTPUT);
  pinMode(VREFB_PIN, OUTPUT);

  analogWrite(VREFA_PIN, 50);
  analogWrite(VREFB_PIN, 50);

  digitalWrite(ENABLE_PIN, HIGH);
  digitalWrite(RESET_PIN, HIGH);

  stepper.setSpeed(200);
}

void loop(){
  stepper.step(200);
  delay(1000);
  stepper.step(-200);
  delay(1000);
}

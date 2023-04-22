#define STEP_PIN 6
#define DIR_PIN 7
#define ENABLE_PIN 2
#define CONTROL_PIN 5

#define VREFA_PIN 3
#define VREFB_PIN 9

/*   
 *   Basic example code for controlling a stepper without library
 *      
 *   by Dejan, https://howtomechatronics.com
 */

void setup() {
  // Sets the two pins as Outputs

  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(VREFA_PIN, OUTPUT);
  pinMode(VREFB_PIN, OUTPUT);

  analogWrite(VREFA_PIN, 40);
  analogWrite(VREFB_PIN, 40);
  digitalWrite(ENABLE_PIN, HIGH);
}
void loop() {
  digitalWrite(DIR_PIN,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < 800; x++) {
    digitalWrite(STEP_PIN,HIGH); 
    delayMicroseconds(700);    // by changing this time delay between the steps we can change the rotation speed
    digitalWrite(STEP_PIN,LOW); 
    delayMicroseconds(700); 
  }
  delay(1000); // One second delay
  
  digitalWrite(DIR_PIN,LOW); //Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < 1600; x++) {
    digitalWrite(STEP_PIN,HIGH);
    delayMicroseconds(500);
    digitalWrite(STEP_PIN,LOW);
    delayMicroseconds(500);
  }
  delay(1000);
}

// serial comm between python and arduino
  // https://www.hackster.io/ansh2919/serial-communication-between-python-and-arduino-e7cce0

// https://github.com/accellarando/TagGang/blob/main/docs/meetings/20230415.txt
  // Computer translates that information to a stepper motor command, e.g. "-3" or "24"
  // .py script to send the stepper motor command to the arduino over serial -> e.g. (stepper control)./move_stepper.py -3
  // ! Arduino picks up this serial command
  // ! Arduino sends serial command to stepper driver
  // ! Stepper motor moves to desired position

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

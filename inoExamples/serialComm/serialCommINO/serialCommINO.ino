 // Arduino script to RECEIVE incoming data from Python script over the serial port
  // https://www.hackster.io/ansh2919/serial-communication-between-python-and-arduino-e7cce0


void setup() {
  // Start serial communication with baud rate 9600
  Serial.begin(9600);
}

void loop() {
  while (Serial.available() > 0){
    // if only encoded int sent with no other characters, reads only
    // the int value until the end of the line and ignores non-numericals
    // (more efficent)
    int num = Serial.parseInt();

    // reads entire string until it encounters a termination character,
    // which may include non-numeric characters
    //String str = Serial.readString();
    //str.trim();
    //Serial.print(str);
    //int num = str.toInt();

    num++;
    // send incremented number back to python
    Serial.println(num);
  }
}


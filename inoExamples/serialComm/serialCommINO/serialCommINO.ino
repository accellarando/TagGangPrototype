 // Arduino script to RECEIVE incoming data from Python script over the serial port
  // https://www.hackster.io/ansh2919/serial-communication-between-python-and-arduino-e7cce0


void setup() {
  // Start serial communication with baud rate 9600
  Serial.begin(9600);
}

void loop() {
  // Wait for data from Python
  while (!Serial.available()) {
    // Do nothing until data is received
  }

  if (Serial.available() > 0){
    // Read data from Python
    String input = Serial.readStringUntil('\n');
    // Convert input to integer and increment by 1
    int incremented_num = input.toInt() + 1;
    // Send incremented number back to Python
    Serial.println(incremented_num);
  }
}

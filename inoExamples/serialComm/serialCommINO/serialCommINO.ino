 // Arduino script to RECEIVE incoming data from Python script over the serial port
  // https://www.hackster.io/ansh2919/serial-communication-between-python-and-arduino-e7cce0

int num;

void setup() {
  // Start serial communication with baud rate 9600
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0){
    // Read data from Python

    int received_value = Serial.parseInt();
    received_value++;
    Serial.println(received_value);

    //String input = Serial.readStringUntil('\n');
    //input.trim();
    //num = input.toInt() + 1;
    // Send incremented number back to Python
    //Serial.println(num);
  }
}


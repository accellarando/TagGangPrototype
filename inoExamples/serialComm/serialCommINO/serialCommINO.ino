 // Arduino script to RECEIVE incoming data from Python script over the serial port
  // https://www.hackster.io/ansh2919/serial-communication-between-python-and-arduino-e7cce0

int x;

void setup() {
  // Arduino IDE: Tool < Serial Monitor
  Serial.begin(9600);
  //Serial.setTimeout(1); // remove?
  Serial.println("ARDUINO READY (1)");
}

void loop() {
  while (!Serial.available());
  x = Serial.readString().toInt();
  Serial.print(x + 1);
}
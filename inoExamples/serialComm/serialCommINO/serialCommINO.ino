 // Arduino script to RECEIVE incoming data from Python script over the serial port
  // https://www.hackster.io/ansh2919/serial-communication-between-python-and-arduino-e7cce0

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
#define pin 2

String command = "";

void setup() {
  pinMode(pin, INPUT);
}

void loop() {
  bool re = digitalRead(pin);
  if (re) {
    command += "1";
  }
}

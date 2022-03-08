int leds[9] = {13,12,11,10,9,8,7,6,5};

void setup()
{
  for (int i = 0; i < 9; i++) {
  	pinMode(leds[i], OUTPUT);
  }
}

void loop()
{
  for (int i = 0; i < 9; i++) {
  	digitalWrite(leds[i], HIGH);
    delay(300);
  }
  for (int i = 9; i > 0; i--) {
  	digitalWrite(leds[i], LOW);
    delay(300);
  }
}
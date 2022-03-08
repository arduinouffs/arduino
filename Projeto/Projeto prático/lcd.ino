#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 6, 7, 8, 9, 10, 11, 12, 13);

byte smiley[8] = {
  B00000,
  B10001,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
};

void setup() {
    lcd.begin(16, 2);
    lcd.clear();
    lcd.print("Hello World!!");
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.print("Hello World!!");
}
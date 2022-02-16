#include <LiquidCrystal.h>

LiquidCrystal lcd(2,4,10,11,12,13);

void setup() {
  pinMode(5, OUTPUT);
  analogWrite(5, 0);

  lcd.begin(16,2);
  lcd.blink();
}

void loop() {

  lcd.clear();
  lcd.home();

  //lcd.setCursor(15,0);
  //lcd.rightToLeft();

  //lcd.setCursor(0,0);
  //lcd.leftToRight();
  
  //lcd.autoscroll();
  //lcd.setCursor(8,0);
  
  for (int c = 0; c < 10; c++) {
      lcd.write(byte(48 + c));
      delay(1000);
  }
}

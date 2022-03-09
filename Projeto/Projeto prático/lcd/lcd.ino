#include <LiquidCrystal.h>

//                rs  e   d4  d5  d6  d7
LiquidCrystal lcd(4,  5,  6,  7,  8,  9);

void setup() {
  lcd.begin(16,2);
  lcd.clear();
  lcd.print("Hello world!!!");
  lcd.setCursor(0, 1); // coluna linha
  lcd.print("UFFS");
}

void loop() {
  delay(99999999);
}

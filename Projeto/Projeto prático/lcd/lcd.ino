#include <LiquidCrystal.h>

//                rs  e   d4  d5   d6   d7
LiquidCrystal lcd(5,  8,  9,  A4,  A5,  4);

byte smile[8] = {
  B11100,
  B10100,
  B11100,
//  B00000,
//  B00000,
//  B00000,
//  B00000,
//  B00000
};

void setup() {
  lcd.createChar(0, smile);
  lcd.begin(16,2);
  lcd.print("Hello world!!!");
  lcd.setCursor(0, 1); // coluna linha
  lcd.print("UFFS");
  delay(1000);
  lcd.clear();
  lcd.write(byte(0));
}

void loop() {
  delay(99999999);
}

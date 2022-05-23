#include <LiquidCrystal_I2C.h>

// caso precise, defina cabeçalho de função aqui

byte grau[8] = {
  B11100,
  B10100,
  B11100,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

const LiquidCrystal_I2C lcd(0x27, 16, 2);

void inicialize_lcd() {
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, grau);
  att_lcd(true);
}

void att_lcd(bool force) {
  static bool mode = true;
  if ((millis() % 10000) == 0 || force) {
//  if (millis() > timeDisplay) {
    if (mode) {
      lcd.clear();
      lcd.print("IP local:");
      lcd.setCursor(0,1);
      lcd.print(Ethernet.localIP());
    } else {
      lcd.clear();
      lcd.print("Temperatura: ");
      lcd.print(int(dht.readTemperature()));
      lcd.write((byte)0);
      lcd.setCursor(0, 1);
      lcd.print("Umidade:     ");
      lcd.print(int(dht.readHumidity()));
      lcd.print("%");
    }
    mode = !mode;
//    timeDisplay = millis() + 10000;
  }
}

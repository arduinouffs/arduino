#include <LiquidCrystal_I2C.h>

const LiquidCrystal_I2C lcd(0x27, 16, 2);

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

unsigned long tempoDisplay = 0;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, grau);
}

void loop() {
  if ((millis() % 1000) == 0) {
//      float temperatura = dht.readTemperature();
//      float umidade = dht.readHumidity();
      
      lcd.clear();
      lcd.print("Temperatura: ");
//      lcd.print(int(temperatura));
      lcd.write((byte)0);
      lcd.setCursor(0, 1);
      lcd.print("Umidade:     ");
//      lcd.print(int(umidade));
      lcd.print("%");
      
      tempoDisplay = millis() + 5000;
  }
}

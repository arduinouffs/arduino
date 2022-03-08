#include "DHT.h"
 
#define DHTPIN A1
#define DHTTYPE DHT11
#define LedRed 2
#define LedBlu 3

DHT dht(DHTPIN, DHTTYPE);
 
void setup() 
{
  Serial.begin(9600);
  dht.begin();
  pinMode(LedRed, OUTPUT);
  pinMode(LedBlu, OUTPUT);
}
 
void loop() 
{
  // A leitura da temperatura e umidade pode levar 250ms!
  // O atraso do sensor pode chegar a 2 segundos.
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  // testa se retorno é valido, caso contrário algo está errado.
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");
  } else {
    if (h > 80) {
      digitalWrite(LedBlu, 1);
    } else {
      digitalWrite(LedBlu, 0);
    }

    if (t > 34) {
      digitalWrite(LedRed, 1);
    } else {
      digitalWrite(LedRed, 0)
    }

    Serial.print("Umidade: ");
    Serial.print(h);
    Serial.print("%        ");
    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.println("°C");
  }
}
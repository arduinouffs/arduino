#include <DHT.h>

// caso precise, defina cabeçalho de função 

#define DHTPIN A0 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT

const DHT dht(DHTPIN, DHTTYPE);

void inicialize_dht() {
  dht.begin();
}

float read_temperature() {
  return dht.readTemperature();
}

float read_humidity() {
  return dht.readHumidity();
}

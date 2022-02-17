#include <SPI.h>
#include <SD.h>
#include <MQ135.h>
#include <DHT.h>

#define PIN_MQ135 A5 // MQ135 Analog Input Pin
#define DHTPIN 2 // DHT Digital Input Pin
#define DHTTYPE DHT11 // DHT11 or DHT22, depends on your 

File myFile;
MQ135 mq135_sensor(PIN_MQ135);
DHT dht(DHTPIN, DHTTYPE);

float temperature, humidity; // Temp and Humid floats, will be measured by the DHT

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

//  // re-open the file for reading:
//  myFile = SD.open("data.txt");
//  if (myFile) {
//    Serial.println("data.txt:");
//
//    // read from the file until there's nothing else in it:
//    while (myFile.available()) {
//      Serial.write(myFile.read());
//    }
//    // close the file:
//    myFile.close();
//  } else {
//    // if the file didn't open, print an error:
//    Serial.println("error opening data.txt");
//  }
  dht.begin();
}

void loop() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    delay(20000);
    return;
  }
  float rzero = mq135_sensor.getRZero();
  float correctedRZero = mq135_sensor.getCorrectedRZero(temperature, humidity);
  float resistance = mq135_sensor.getResistance();
  float ppm = mq135_sensor.getPPM();
  float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity);
  
  myFile = SD.open("data.txt", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.println("Escrevendo em data.txt...");
    
    myFile.print("MQ135 RZero: ");
    myFile.print(rzero);
    myFile.print("\t Corrected RZero: ");
    myFile.print(correctedRZero);
    myFile.print("\t Resistance: ");
    myFile.print(resistance);
    myFile.print("\t PPM: ");
    myFile.print(ppm);
    myFile.print("ppm");
    myFile.print("\t Corrected PPM: ");
    myFile.print(correctedPPM);
    myFile.println("ppm");

    Serial.print("MQ135 RZero: ");
    Serial.print(rzero);
    Serial.print("\t Corrected RZero: ");
    Serial.print(correctedRZero);
    Serial.print("\t Resistance: ");
    Serial.print(resistance);
    Serial.print("\t PPM: ");
    Serial.print(ppm);
    Serial.print("ppm");
    Serial.print("\t Corrected PPM: ");
    Serial.print(correctedPPM);
    Serial.println("ppm");
    
    // close the file:
    myFile.close();
    Serial.println("Escrito em data.txt com sucesso.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening data.txt");
  }

  delay(20000);
}

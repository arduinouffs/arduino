#include <MQ7.h>
#include <MQUnifiedsensor.h> //mq9
#include <MQ2.h>
#include <MQ135.h>
#include <DHT.h>

#define PIN_MQ135 A3 // MQ135 Analog Input Pin
#define pin  A5
#define A_PIN 2
#define VOLTAGE 5
#define         Board                   ("Arduino UNO")
#define         Pin                     (A4)  //Analog input 4 of your arduino
#define         Type                    ("MQ-9") //MQ9
#define         Voltage_Resolution      (5)
#define         ADC_Bit_Resolution      (10) // For arduino UNO/MEGA/NANO
#define         RatioMQ9CleanAir        (9.6) //RS / R0 = 60 ppm 
#define dhtpin A2
#define dhttype DHT11

MQ7 mq7(A_PIN, VOLTAGE);
MQUnifiedsensor MQ9(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin, Type);
MQ2 mq2(pin);
MQ135 mq135_sensor(PIN_MQ135);
DHT dht(dhtpin, dhttype);

void setup() {
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial connection
  }
  mq7.calibrate();    // calculates R0
  MQ9.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ9.init();
  mq2.begin();
  float calcR0 = 0;
  for(int i = 1; i<=10; i ++)
  {
    MQ9.update(); // Update data, the arduino will be read the voltage on the analog pin
    calcR0 += MQ9.calibrate(RatioMQ9CleanAir);
    Serial.print(".");
  }
  MQ9.setR0(calcR0/10);
  if(isinf(calcR0)) {Serial.println("MQ-9: Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(calcR0 == 0){Serial.println("MQ-9: Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply"); while(1);}
}

void loop() {
  Serial.println("\n\n\n\n\n**************************************MQ-7(CO2)***********************************");
  Serial.print("PPM = "); Serial.println(mq7.readPpm());
  
  Serial.println("\n********************************MQ-9(CO2, GLP, CH2)*******************************");
  MQ9.update(); // Update data, the arduino will be read the voltage on the analog pin
  /*
  Exponential regression:
  GAS     | a      | b
  LPG     | 1000.5 | -2.186
  CH4     | 4269.6 | -2.648
  CO      | 599.65 | -2.244
  */
  MQ9.setA(1000.5); MQ9.setB(-2.186); // Configurate the ecuation values to get LPG concentration
  float LPG = MQ9.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup
  MQ9.setA(4269.6); MQ9.setB(-2.648); // Configurate the ecuation values to get LPG concentration
  float CH4 = MQ9.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup
  MQ9.setA(599.65); MQ9.setB(-2.244); // Configurate the ecuation values to get LPG concentration
  float CO = MQ9.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup
  Serial.print("\tGLP\t|\tCH4\t|\tCO2\t\n");
  Serial.print("\t");Serial.print(LPG);Serial.print("\t");Serial.print("|");Serial.print("\t");Serial.print(CH4);Serial.print("\t");Serial.print("|");Serial.print("\t");Serial.println(CO);
  
  Serial.println("\n********************************MQ-2(GLP, CO2, Smoke)*******************************");
  float lpg = mq2.readLPG();
  float co = mq2.readCO();
  float smoke = mq2.readSmoke();
  Serial.print("\tGLP\t|\tCO2\t|\tSmoke\t\n");
  Serial.print("\t");Serial.print(lpg);Serial.print("\t");Serial.print("|");Serial.print("\t");Serial.print(co);Serial.print("\t");Serial.print("|");Serial.print("\t");Serial.println(smoke);
  
  Serial.println("\n*******************************MQ-135(GLP, CO2, Smoke)******************************");
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println(F("Failed to read from DHT sensor!"));
  }
  float rzero = mq135_sensor.getRZero();
  float correctedRZero = mq135_sensor.getCorrectedRZero(temperature, humidity);
  float resistance = mq135_sensor.getResistance();
  float ppm = mq135_sensor.getPPM();
  float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity);
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
  
  delay(10000);
}

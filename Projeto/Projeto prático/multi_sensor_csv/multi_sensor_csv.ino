#include <MQ7.h>
#include <MQUnifiedsensor.h> //mq9
#include <MQ2.h>
#include <MQ135.h>
#include <DHT.h>

#define MQ135_PIN A3 // MQ135 Analog Input MQ2_PIN
#define MQ2_PIN  A5
#define MQ7_PIN 2
#define VOLTAGE 5
#define         Board                   ("Arduino UNO")
#define         MQ9_PIN                     (A4)  //Analog input 4 of your arduino
#define         Type                    ("MQ-9") //MQ9
#define         Voltage_Resolution      (5)
#define         ADC_Bit_Resolution      (10) // For arduino UNO/MEGA/NANO
#define         RatioMQ9CleanAir        (9.6) //RS / R0 = 60 ppm 
#define dhtpin A2
#define dhttype DHT11
#define pin2 4 // DSM501A
#define pin1 3 // DSM501A

unsigned long sampletime_ms = 5000;//sampe 1s ;
unsigned long duration1;
unsigned long duration2;
unsigned long lowpulseoccupancy1 = 0;
unsigned long lowpulseoccupancy2 = 0;
float ratio1 = 0;
float ratio2 = 0;
float concentration1 = 0;
float concentration2 = 0;
int count = 1;

MQ7 mq7(MQ7_PIN, VOLTAGE);
MQUnifiedsensor MQ9(Board, Voltage_Resolution, ADC_Bit_Resolution, MQ9_PIN, Type);
MQ2 mq2(MQ2_PIN);
MQ135 mq135_sensor(MQ135_PIN);
DHT dht(dhtpin, dhttype);

void setup() {
  Serial.begin(9600);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
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
    MQ9.update(); // Update data, the arduino will be read the voltage on the analog MQ2_PIN
    calcR0 += MQ9.calibrate(RatioMQ9CleanAir);
    Serial.print(".");
  }
  MQ9.setR0(calcR0/10);
  if(isinf(calcR0)) {Serial.println("MQ-9: Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(calcR0 == 0){Serial.println("MQ-9: Warning: Conection issue founded, R0 is zero (Analog MQ2_PIN with short circuit to ground) please check your wiring and supply"); while(1);}
  dht.begin();
  Serial.print("MQ-7: CO2,MQ-9: CO2,MQ-9: GLP,MQ-9: CH4,MQ-2: GLP, MQ-2: CO2,MQ-2: Fumaça,MQ-135: rzero,MQ-135: correctedRZero,MQ-135: resistance,MQ-135: ppm,MQ-135: correctedPPM,DSM501A: PM2.5,DSM501A: PM10,DSM501A: Consideração,DHT11: Temperatura,DHT11: Umidade\n");
}

void loop() {
//  Serial.println("\n\n\n\n\n**************************************MQ-7(CO2)***********************************");
//  Serial.print("PPM = "); Serial.println(mq7.readPpm());/
  Serial.print(mq7.readPpm()); Serial.print(",");
  
//  Serial.println("\n********************************MQ-9(CO2, GLP, CH4)*******************************");/
  MQ9.update(); // Update data, the arduino will be read the voltage on the analog MQ2_PIN
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
//  Serial.print("\tGLP\t|\tCH4\t|\tCO2\t\n");/
//  Serial.print("\t");Serial.print(LPG);Serial.print("\t");Serial.print("|");Serial.print("\t");Serial.print(CH4);Serial.print("\t");Serial.print("|");Serial.print("\t");Serial.println(CO);/
  Serial.print(CO);Serial.print(",");Serial.print(LPG);Serial.print(",");Serial.print(CH4);Serial.print(",");
  
//  Serial.println("\n********************************MQ-2(GLP, CO2, Smoke)*******************************");
  float lpg = mq2.readLPG();
  float co = mq2.readCO();
  float smoke = mq2.readSmoke();
//  Serial.print("\tGLP\t|\tCO2\t|\tSmoke\t\n");/
//  Serial.print("\t");Serial.print(lpg);Serial.print("\t");Serial.print("|");Serial.print("\t");Serial.print(co);Serial.print("\t");Serial.print("|");Serial.print("\t");Serial.println(smoke);/
  Serial.print(lpg);Serial.print(",");Serial.print(co);Serial.print(",");Serial.print(smoke);Serial.print(",");
  
//  Serial.println("\n*******************************MQ-135(GLP, CO2, Smoke)******************************");
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
//  if (isnan(temperature) || isnan(humidity)) {
//    Serial.println(F("Failed to read from DHT sensor!"));
//  }
  float rzero = mq135_sensor.getRZero();
  float correctedRZero = mq135_sensor.getCorrectedRZero(temperature, humidity);
  float resistance = mq135_sensor.getResistance();
  float ppm = mq135_sensor.getPPM();
  float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity);
//  Serial.print("MQ135 RZero: ");
//  Serial.print(rzero);
//  Serial.print("\t Corrected RZero: ");
//  Serial.print(correctedRZero);
//  Serial.print("\t Resistance: ");
//  Serial.print(resistance);
//  Serial.print("\t PPM: ");
//  Serial.print(ppm);
//  Serial.print("ppm");
//  Serial.print("\t Corrected PPM: ");
//  Serial.print(correctedPPM);
//  Serial.println("ppm");
  Serial.print(rzero);Serial.print(",");Serial.print(correctedRZero);Serial.print(",");Serial.print(resistance);Serial.print(",");Serial.print(ppm);Serial.print(",");Serial.print(correctedPPM);Serial.print(",");

//  Serial.println("\n***************************************DSM501A**************************************");
  duration1 = pulseIn(pin1, LOW);
  duration2 = pulseIn(pin2, LOW);
  lowpulseoccupancy1 = lowpulseoccupancy1+duration1;
  lowpulseoccupancy2 = lowpulseoccupancy2+duration2;

  ratio1 = lowpulseoccupancy1/(sampletime_ms*10.0);  // Integer percentage 0=>100
  concentration1 = 1.1*pow(ratio1,3)-3.8*pow(ratio1,2)+520*ratio1+0.62; // using spec sheet curve

  ratio2 = lowpulseoccupancy2/(sampletime_ms*10.0);  // Integer percentage 0=>100
  concentration2 = 1.1*pow(ratio2,3)-3.8*pow(ratio2,2)+520*ratio2+0.62; // 

//  Serial.print("concentration1(PM2.5) = ");
//  Serial.print(concentration1);
//  Serial.print(" pcs/0.01cf  -  ");
//
//  Serial.print("concentration2(PM10) = ");
//  Serial.print(concentration2);
//  Serial.print(" pcs/0.01cf  -  ");
  Serial.print(concentration1);Serial.print(",");Serial.print(concentration2);Serial.print(",");

  
  if (concentration1 < 1000) {
   Serial.print("Clean");
  }
  
  if (concentration1 > 1000 && concentration1 < 10000) {
   Serial.print("GOOD");
  }
  
  if (concentration1 > 10000 && concentration1 < 20000) {      
   Serial.print("ACCEPTABLE");
  }
    if (concentration1 > 20000 && concentration1 < 50000) {
   Serial.print("HEAVY");
}

  if (concentration1 > 50000 ) {   
   Serial.print("HAZARD");        
  } 
  Serial.print(",");

  lowpulseoccupancy1 = 0;
  lowpulseoccupancy2 = 0;

  //  Serial.println("\n***************************************DHT11**************************************");
  Serial.print(dht.readTemperature()); Serial.print(","); Serial.print(dht.readHumidity());
  
  Serial.println();
  Serial.print("Contagem "); Serial.println(count);
  count++;
//  delay(30000);
}

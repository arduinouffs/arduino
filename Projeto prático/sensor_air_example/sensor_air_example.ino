
// #include <LiquidCrystal.h>
//LiquidCrystal //    lcd(8, 9, 4, 5, 6, 7);
// nome: dsm501a pm2.5
int pin2 = 4; // D3
int pin1 = 3; // D2
unsigned long duration1;
unsigned long duration2;

unsigned long starttime;
unsigned long sampletime_ms = 3000;//sampe 1s ;
unsigned long lowpulseoccupancy1 = 0;
unsigned long lowpulseoccupancy2 = 0;
float ratio1 = 0;
float ratio2 = 0;
float concentration1 = 0;
float concentration2 = 0;

void setup() {
  Serial.begin(9600);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  starttime = millis();//get the current time;
//  //    lcd.begin(16, 2);
}

void loop() {
  duration1 = pulseIn(pin1, LOW);
  duration2 = pulseIn(pin2, LOW);
  lowpulseoccupancy1 = lowpulseoccupancy1+duration1;
  lowpulseoccupancy2 = lowpulseoccupancy2+duration2;


  if ((millis()-starttime) > sampletime_ms)//if the sampel time == 30s
  {
    ratio1 = lowpulseoccupancy1/(sampletime_ms*10.0);  // Integer percentage 0=>100
    concentration1 = 1.1*pow(ratio1,3)-3.8*pow(ratio1,2)+520*ratio1+0.62; // using spec sheet curve

    ratio2 = lowpulseoccupancy2/(sampletime_ms*10.0);  // Integer percentage 0=>100
    concentration2 = 1.1*pow(ratio2,3)-3.8*pow(ratio2,2)+520*ratio2+0.62; // 

//    //    lcd.setCursor(0, 0);
    //    lcd.print("PM10 ");
    //    lcd.setCursor(6, 0);
    //    lcd.print(concentration1,3);
       
    Serial.print("concentration1 = ");
    Serial.print(concentration1);
    Serial.print(" pcs/0.01cf  -  ");

    Serial.print("concentration2 = ");
    Serial.print(concentration2);
    Serial.print(" pcs/0.01cf  -  ");

    
    if (concentration1 < 1000) {
     Serial.print("Clean");
     Serial.print(concentration1);
    }
    
    if (concentration1 > 1000 && concentration1 < 10000) {
     Serial.print("GOOD");
     Serial.print(concentration1);
    }
    
    if (concentration1 > 10000 && concentration1 < 20000) {      
     Serial.print("ACCEPTABLE");
     Serial.print(concentration1);
    }
      if (concentration1 > 20000 && concentration1 < 50000) {
     Serial.print("HEAVY");
     Serial.print(concentration1);
  }

    if (concentration1 > 50000 ) {   
     Serial.print("HAZARD");
     Serial.print(concentration1);        
    } 

    Serial.println();
      
    lowpulseoccupancy1 = 0;
    lowpulseoccupancy2 = 0;
    starttime = millis();
  }
}

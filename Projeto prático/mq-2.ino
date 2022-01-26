 #include <MQ2.h>
  
  int pin = A0;
  
  MQ2 mq2(pin);
  
  void setup(){
    Serial.begin(9600);
    mq2.begin();
  }

  void loop () {
    float lpg = mq2.readLPG();
    float co = mq2.readCO();
    float smoke = mq2.readSmoke();

    Serial.print("Gas: ");
    Serial.print(lpg);
    Serial.print(" Co2: ");
    Serial.print(co);
    Serial.print(" Fumaça: ");
    Serial.print(smoke);
    Serial.print('\n');
    
    delay(1000);
  }

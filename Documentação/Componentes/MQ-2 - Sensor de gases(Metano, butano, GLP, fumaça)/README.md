
# MQ-2 Sensor de gases
### Introdução
Este sensor é capaz de ler a presença dos gases metano, butano, GLP e fumaça.
### Montagem
Há diferentes variantes deste mesmo sensor, por isso deve-se tomar cuidado com a pinagem. Neste caso foi utilizado a versão de 4 pinos soldada em uma placa: 
![dht11](dht11.png)
|Pino|Função|
|-|-|
|GND|GND|
|DOUT|Dado digital|
|AOUT|Dado analógico|
|VCC|VCC|
### Consumo de corrente e voltagem
Ele trabalha com uma voltagem na faixa de 3-5V, consumindo uma corrente de no máximo 150mA.
### Consumo de memória flash e SRAM no Arduino Uno
O Scketch padrão para teste do sensor fornecido pela bibliteca [<MQ2.h>](https://github.com/labay11/MQ-2-sensor-library) consome 5842 bytes da memória flash, equivalente a 18%% da capacidade total. Variáveis no Scketch consomem 398 bytes de memória SRAM, equivalente a 19% da capacidade total.
### Bibliotecas utilizadas
Para funcionamento do sensor, será necessária instalação da biblioteca [<MQ2.h>](https://github.com/labay11/MQ-2-sensor-library) (ou [<MQUnifiedsensor.h>](https://www.arduino.cc/reference/en/libraries/mqunifiedsensor/)) na Arduino IDE. O Scketch a seguir é um exemplo de como utilizá-la, este que é fornecido pela própria biblioteca na seção de exemplos:
```
<pre lang="cpp"><code>
#include <MQ2.h>

//change this with the pin that you use
int pin = A0;
float lpg, co, smoke;

MQ2 mq2(pin);

void setup(){
  Serial.begin(9600);
  
  // calibrate the device
  mq2.begin();
}

void loop(){
  
  /*
   * read the values from the sensor, it returns
   * an array which contains 3 values.
   * 0 : LPG in ppm
   * 1 : CO in ppm
   * 2 : SMOKE in ppm
   */
  float* values= mq2.read(true); //set it false if you don't want to print the values to the Serial
  
  // lpg = values[0];
  lpg = mq2.readLPG();
  // co = values[1];
  co = mq2.readCO();
  // smoke = values[2];
  smoke = mq2.readSmoke();
  
  delay(1000);
}
</code></pre>
```
> Busque por "[MQ-2 datasheet](https://www.google.com/search?q=MQ-2+datasheet)" para mais informações sobre este componente.


# MQ-7 Sensor de CO²
### Introdução
Este sensor é capaz de ler níveis da presença do Dióxido de Carbono.
### Montagem
Há diferentes variantes deste mesmo sensor, por isso deve-se tomar cuidado com a pinagem. Neste caso foi utilizado a versão de 4 pinos soldada em uma placa: 
![mq7](mq7.png)
|Pino|Função|
|-|-|
|VCC|VCC|
|GND|GND|
|D0|Dado digital|
|A0|Dado analógico|
### Consumo de corrente e voltagem
Ele trabalha com uma voltagem na faixa de 3-5V, consumindo uma corrente de no máximo 150mA.
### Consumo de memória flash e SRAM no Arduino Uno
O Scketch padrão para teste do sensor fornecido pela bibliteca [<MQ7.h>](https://www.arduino.cc/reference/en/libraries/mq7sensor/) consome 4730 bytes da memória flash, equivalente a 14% da capacidade total. Variáveis no Scketch consomem 250 bytes de memória SRAM, equivalente a 12% da capacidade total.
### Bibliotecas utilizadas
Para funcionamento do sensor, será necessária instalação da biblioteca [<MQ7.h>](https://www.arduino.cc/reference/en/libraries/mq7sensor/) na Arduino IDE, ou pode ser usada no lugar a biblioteca [<MQUnifiedsensor.h>](https://www.arduino.cc/reference/en/libraries/mqunifiedsensor/) . O Scketch a seguir é um exemplo de como utilizá-la, este que é fornecido pela própria biblioteca na seção de exemplos:
```
/*	
	MQ7_Example.ino

	Example sketch for MQ7 carbon monoxide detector.
		- connect analog input 
		- set A_PIN to the relevant pin
		- connect device ground to GND pin 
		- device VC to 5.0 volts

	Created by Fergus Baker
	22 August 2020
	Released into the public domain.
*/


#include "MQ7.h"

#define A_PIN 7
#define VOLTAGE 5

// init MQ7 device
MQ7 mq7(A_PIN, VOLTAGE);

void setup() {
	Serial.begin(9600);
	while (!Serial) {
		;	// wait for serial connection
	}

	Serial.println(""); 	// blank new line

	Serial.println("Calibrating MQ7");
	mq7.calibrate();		// calculates R0
	Serial.println("Calibration done!");
}
 
void loop() {
	Serial.print("PPM = "); Serial.println(mq7.readPpm());

	Serial.println(""); 	// blank new line
	delay(1000);
}
```
> Busque por "[mq-7 datasheet](https://www.google.com/search?q=mq-7+datasheet)" para mais informações sobre este componente.

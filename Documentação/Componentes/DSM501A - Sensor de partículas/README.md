# DSM501A PM2.5 e PM10 sensor ótico de partículas
### Introdução
Este sensor é capaz de ler a quantidade partículas no ar indicando o nível de poluição e a qualidade do ar, medindo partículas (*Particulate matter* ou Material particulado) de tamanho 2.5μm e 10μm.
### Montagem
Leia o [artigo](https://create.arduino.cc/projecthub/mircemk/arduino-air-quality-monitor-with-dsm501a-sensor-b4f8fc) para um compreensão mais detalhada caso sinta necessidade. 
![dsm501a](dsm501a.png)
Da esquerda para a direita, em pé olhando pelo lado de trás(as cores dos fios podem mudar, esqueça-as):
|Pino|Função|
|-|-|
|Fio 0|Controle (controle de sensibilidade para Vout1)|
|Fio 1|Vout2 (PM10)|
|Fio 2|VCC|
|Fio 3|Vout1 (PM2.5)|
|Fio 4|GND|
### Consumo de corrente e voltagem
Ele trabalha com uma voltagem de 5V, consumindo uma corrente de no máximo 90mA.
### Consumo de memória flash e SRAM no Arduino Uno
O Scketch utilizado no teste consumiu -- de memória flash equivalendo a -% da capacidade total e -- de SRAM equivalendo a -% da capacidade total.
### Bibliotecas utilizadas
Até o momento, não foi encontrada uma biblioteca para gerenciar este sensor, então os cálculos de conversão são feitos diretamente em código:
```
-
```
> Busque por "[DSM501A datasheet](https://www.google.com/search?q=DSM501A+datasheet)" para mais informações sobre este componente.

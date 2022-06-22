

# Explicações sobre o projeto
### Introdução
Este projeto de monitoramento e controle do clima do ambiente, tem por objetivo economizar energia com base em limites aceitáveis de temperatura e umidade no ambiente do datacenter da UFFS, e também detectar substâncias estranhas no ar que possam indicar contaminantes provenientes de incêndio ou não: Gases tóxicos e/ou corrosivos, fuligem ou CO².
### Hardware
Para montagem deste projeto seja em protoboard, seja soldado, siga o passo-a-passo conectando os cabos exatamente como na imagem. Caso alguma configuração no código seja alterada, conexões devem obrigatoriamente serem revistas!
Placa Ethernet vai encaixada sob o Arduino Uno, as conexões dos componentes são feitas no [Shield Ethernet](https://github.com/arduinouffs/arduino/tree/main/Documenta%C3%A7%C3%A3o/Componentes/Shield%20Ethernet%20W5100) ao invés do Arduino. Os cabos brancos representados são as conexões reservadas para que a placa funcione juntamente ao Arduino. Pino 4 não está sendo usado pela placa para cartão SD.
![Arduino](Projeto_bb.png)
### Software
* O primeiro passo será [ler o controle](../Infravermelho/Leitura) se o modelo de ar condicionado for outro a não ser o Komecco já instanciado nos códigos desse projeto. Para A/C's já demandados, copie as marcas de tempo já disponíveis no arquivo [Strings_controles.txt](https://github.com/arduinouffs/arduino/blob/main/Strings_controles.txt ).
* Segundo passo é inserir as marcas de tempo no código e subir a compilação para o Arduino.
* Terceiro passo é montar o projeto onde for mais conveniente, da forma desejada!

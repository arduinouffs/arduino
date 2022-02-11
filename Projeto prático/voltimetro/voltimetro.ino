#include "EmonLib.h" //INCLUSÃO DE BIBLIOTECA

#define VOLT_CAL 440.7 //VALOR DE CALIBRAÇÃO (DEVE SER AJUSTADO EM PARALELO COM UM MULTÍMETRO)

EnergyMonitor emon1; //CRIA UMA INSTÂNCIA

void setup(){  
  Serial.begin(9600); //INICIALIZA A SERIAL
  emon1.voltage(A3, VOLT_CAL, 1.7); //PASSA PARA A FUNÇÃO OS PARÂMETROS (PINO ANALÓGIO / VALOR DE CALIBRAÇÃO / MUDANÇA DE FASE)
}

void loop(){
  emon1.calcVI(17,2000); //FUNÇÃO DE CÁLCULO (17 SEMICICLOS, TEMPO LIMITE PARA FAZER A MEDIÇÃO)    
  
  float supplyVoltage   = emon1.Vrms; //VARIÁVEL RECEBE O VALOR DE TENSÃO RMS OBTIDO

  Serial.print("Tensão medida na rede AC: "); //IMPRIME O TEXTO NA SERIAL
  Serial.print(supplyVoltage, 0); //IMPRIME NA SERIAL O VALOR DE TENSÃO MEDIDO E REMOVE A PARTE DECIMAL
  Serial.println("V"); //IMPRIME O TEXTO NA SERIAL
  delay(1000);
}

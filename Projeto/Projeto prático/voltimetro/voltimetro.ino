#include "EmonLib.h" //INCLUSÃO DE BIBLIOTECA

#define VOLT_CAL 458 //VALOR DE CALIBRAÇÃO (DEVE SER AJUSTADO EM PARALELO COM UM MULTÍMETRO)
#define ZMPT101BPIN A3
EnergyMonitor emon1; //CRIA UMA INSTÂNCIA

void setup(){  
  Serial.begin(9600); //INICIALIZA A SERIAL
  emon1.voltage(ZMPT101BPIN, VOLT_CAL, 1.7); //PASSA PARA A FUNÇÃO OS PARÂMETROS (PINO ANALÓGIO / VALOR DE CALIBRAÇÃO / MUDANÇA DE FASE)
  emon1.current(1, 111.1);       // Current: input pin, calibration.
}

void loop(){
  emon1.calcVI(17,2000); //FUNÇÃO DE CÁLCULO (17 SEMICICLOS, TEMPO LIMITE PARA FAZER A MEDIÇÃO)    
//  emon1.serialprint();           // Print out all variables (realpower, apparent power, Vrms, Irms, power factor)
  
//  float realPower       = emon1.realPower;        //extract Real Power into variable
//  float apparentPower   = emon1.apparentPower;    //extract Apparent Power into variable
//  float powerFActor     = emon1.powerFactor;      //extract Power Factor into Variable
//  float supplyVoltage   = emon1.Vrms;             //extract Vrms into Variable (Voltagem)
//  float Irms            = emon1.Irms;             //extract Irms into VariableSERIAL (Corente)

  Serial.println(emon1.Vrms);
  
  delay(1000);
}

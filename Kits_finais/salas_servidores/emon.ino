#include <EmonLib.h>

// caso precise, defina cabeçalho de função aqui

EnergyMonitor tensao;

float read_voltage(int PIN, float VOLT_CAL) {
  tensao.voltage(PIN, VOLT_CAL, 1.7);
  tensao.calcVI(17,2000);
  return tensao.Vrms;
}

#include "DACControl.h"

void DACControl::begin() {
  pinMode(HW::DAC1_CS, OUTPUT);
  pinMode(HW::DAC2_CS, OUTPUT);
  digitalWriteFast(7, HIGH);
  digitalWriteFast(8, HIGH);
  SPI.begin();
  // Kept open intentionally: only the timer ISR uses SPI thereafter.
  SPI.beginTransaction(SPISettings(DAC_SPI_HZ, MSBFIRST, SPI_MODE0));
  midpoint();
}

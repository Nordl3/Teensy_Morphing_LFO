#pragma once
#include <cstdint>
#define MSBFIRST 1
#define SPI_MODE0 0
struct SPISettings {SPISettings(uint32_t,int,int){}};
struct SPIClass {
  uint16_t last[4]={};int call=0;
  void begin(){}
  void beginTransaction(SPISettings){}
  uint16_t transfer16(uint16_t w){last[call++%4]=w;return 0;}
};
extern SPIClass SPI;

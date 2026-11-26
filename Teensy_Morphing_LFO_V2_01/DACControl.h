#pragma once
#include <Arduino.h>
#include <SPI.h>
#include "Settings.h"

// SPI is deliberately owned exclusively by the audio ISR once started.
// No SD, SPI display, or other SPI peripheral may use this bus as written.
class DACControl {
public:
  void begin();
  inline void writeFour(uint16_t a1, uint16_t a2, uint16_t b1, uint16_t b2) {
    // MCP4822 command: bit15 channel B, bit13 1x gain if set (unset -> 2x),
    // bit12 active, bits11:0 DAC code. Retain legacy 2x gain behaviour.
    digitalWriteFast(7, LOW);
    SPI.transfer16(0x1000u | (a1 & 0x0FFFu));
    digitalWriteFast(7, HIGH);
    digitalWriteFast(7, LOW);
    SPI.transfer16(0x9000u | (a2 & 0x0FFFu));
    digitalWriteFast(7, HIGH);
    digitalWriteFast(8, LOW);
    SPI.transfer16(0x1000u | (b1 & 0x0FFFu));
    digitalWriteFast(8, HIGH);
    digitalWriteFast(8, LOW);
    SPI.transfer16(0x9000u | (b2 & 0x0FFFu));
    digitalWriteFast(8, HIGH);
  }
  inline void midpoint() {
    writeFour(DAC_OUTPUT_MAX/2, DAC_OUTPUT_MAX/2,
              DAC_OUTPUT_MAX/2, DAC_OUTPUT_MAX/2);
  }
};

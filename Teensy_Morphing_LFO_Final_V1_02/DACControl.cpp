#include "DACControl.h"

// Constructor to set up the chip select pins for DAC1 and DAC2
DACControl::DACControl(int csPin1, int csPin2) {
    _csPin1 = csPin1;
    _csPin2 = csPin2;
}

// Initialize SPI and configure chip select pins
void DACControl::begin() {
    SPI.begin();  // Initialize the main SPI bus
    pinMode(_csPin1, OUTPUT);
    pinMode(_csPin2, OUTPUT);
    digitalWriteFast(_csPin1, HIGH);  // Keep DAC1 disabled initially
    digitalWriteFast(_csPin2, HIGH);  // Keep DAC2 disabled initially
}

// Helper function to select the correct DAC
void DACControl::selectDAC(int dacNumber) {
    int csPin = (dacNumber == 1) ? _csPin1 : _csPin2;
    digitalWriteFast(csPin, LOW);
}

// Write a value to the specified DAC and channel
void DACControl::writeDAC(int dacNumber, int channel, uint16_t value) {
    selectDAC(dacNumber);
    // Set channel, gain (2x), and 12-bit value
    uint16_t command = ((channel << 15) | 0x1000) | (value & 0x0FFF);  
    SPI.transfer16(command);
    digitalWriteFast((dacNumber == 1) ? _csPin1 : _csPin2, HIGH);
}

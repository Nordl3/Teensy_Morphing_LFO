#ifndef DACCONTROL_H
#define DACCONTROL_H

#include <SPI.h>  // Include SPI library for DAC communication

class DACControl {
public:
    DACControl(int csPin1, int csPin2);  // Constructor: takes in the chip select pins for DAC1 and DAC2
    void begin();                        // Initialize SPI and set up the DACs
    void writeDAC(int dacNumber, int channel, uint16_t value);  // Write value to the DAC

private:
    int _csPin1;  // Chip Select pin for DAC1
    int _csPin2;  // Chip Select pin for DAC2

    void selectDAC(int dacNumber);  // Helper function to select the correct DAC
};

#endif  // DACCONTROL_H

#ifndef DACCONTROL_H
#define DACCONTROL_H

// include SPI library for DAC communication
#include <SPI.h>

class DACControl
{
    public:
        // CONSTRUCTOR: take in cchip select pins for DAC1 and DAC2
        DACControl(int csPin1, int csPin2);

        // Initialise SPI and set up DACs
        void begin();

        // write value to DAC
        void writeDAC(int dacNumber, int channel, uint16_t value);

    private:
        // chip select pins for DACS
        int _csPin1; // DAC1
        int _csPin2; // DAC2

        // help function to select the correct DAC
        void selectDAC(int dacNumber);
};

#endif  // DACCONTROL_H

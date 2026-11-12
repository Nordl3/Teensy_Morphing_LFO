#include "DACControl.h"

// Constructor
DACControl::DACControl(int csPin1, int csPin2)
{
    // Pass in both chip selector pins
    _csPin1 = csPin1;
    _csPin2 = csPin2;
}

void DACControl::begin()
{
    // Initialise main SPI bus
    SPI.begin();

    //set pinmodes
    pinMode(_csPin1, OUTPUT);
    pinMode(_csPin2, OUTPUT);

    // disable dacs initially
    digitalWrite(_csPin1, HIGH);
    digitalWrite(_csPin2, HIGH);
}

// Helper function to select the correct DAC
void DACControl::selectDAC(int dacNumber)
{
    // tertiary operator
    // if(dacnumber = 1) {select pin1} else{select pin2}
    // but if dacnumber1 == cspin1 and dacnumber2 == cspin2
    // consider joining them earlier somehow
    int csPin = (dacNumber == 1) ? _csPin1 : _csPin2;

    // set that pin low
    digitalWrite(csPin, LOW);
}

// Write a value to the specified DAC and channel
void DACControl::writeDAC(int dacNumber, int channel, uint16_t value)
{
    // set a dacNumber LOW
    selectDAC(dacNumber);

     // set channel, gain (in octal) and value 
    // channel bitshifted by 15 OR with a HEX number (research)
    // value is AND to a value (research)   
    uint16_t command = ((channel << 15) | 0x1000) | (value & 0x0FFF);

    // research what this does
    SPI.transfer16(command);

    // digital write tertiary operator
    // if(dacnumber = 1) {select pin1} else{select pin2}
    // same thing as before could maybe do away with these checks
    // what dacnumber = what pin
    digitalWrite((dacNumber == 1) ? _csPin1 : _csPin2, HIGH);
}

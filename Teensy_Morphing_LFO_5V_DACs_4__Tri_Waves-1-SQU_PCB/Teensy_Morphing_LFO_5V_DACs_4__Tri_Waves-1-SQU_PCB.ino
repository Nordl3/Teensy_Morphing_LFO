#include <SPI.h>

const int CS1 = 7;  // Chip Select for DAC1
const int CS2 = 8;  // Chip Select for DAC2

uint16_t value = 0;        // 12-bit value to output to DAC (0-4095)
int increment = 1;         // Used to control the triangle wave direction
const int maxValue = 4095; // Maximum 12-bit value for the DAC

int squareState = LOW; // State of the square wave on pin 9

void setup() {
  // Initialize SPI
  SPI.begin();
  
  // Set chip select pins as outputs
  pinMode(CS1, OUTPUT);
  pinMode(CS2, OUTPUT);
  
  // Set CS pins high to deselect the DACs
  digitalWrite(CS1, HIGH);
  digitalWrite(CS2, HIGH);

  // Set up pin 9 as output for the square wave
  pinMode(9, OUTPUT);
  digitalWrite(9, squareState);
}

void loop() {
  // Update the value for the triangle wave
  value += increment;
  
  // Reverse direction at the top and bottom of the wave
  if (value >= maxValue || value <= 0) {
    increment = -increment;

    // Toggle the square wave state
    squareState = !squareState;
    digitalWrite(9, squareState);
  }
  
  // Send triangle wave to DAC1 (channels A and B)
  sendToDAC(CS1, 0, value);  // Channel A
  sendToDAC(CS1, 1, value);  // Channel B
  
  // Send triangle wave to DAC2 (channels A and B)
  sendToDAC(CS2, 0, value);  // Channel A
  sendToDAC(CS2, 1, value);  // Channel B
  
  delayMicroseconds(100);  // Adjust this for desired frequency
}

// Function to send data to the DAC
void sendToDAC(int csPin, int channel, uint16_t value) {
  uint16_t command = (0x1000 | (channel << 15)) | (value & 0x0FFF);  // Build 16-bit command
  
  // Select the DAC
  digitalWrite(csPin, LOW);
  
  // Send the command over SPI
  SPI.transfer16(command);
  
  // Deselect the DAC
  digitalWrite(csPin, HIGH);
}

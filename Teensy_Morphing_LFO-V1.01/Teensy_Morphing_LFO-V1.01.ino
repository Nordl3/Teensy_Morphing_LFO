#include <SPI.h>
#include "DACControl.h"  // Include the DACControl library
#include "waveforms.h"   // Include the external LUTs

// Pin for square wave output
const int SQUARE_PIN = 33;
const int SYNC_PIN = 2;  // Pin for external sync/reset input

// Define DAC chip select pins
const int CS_PIN_DAC1 = 28;
const int CS_PIN_DAC2 = 29;

// Instantiate the DACControl object
DACControl dacControl(CS_PIN_DAC1, CS_PIN_DAC2);

// Potentiometer pins for rate, morph, and phase control
const int POT_RATE_PIN = A17;

// Phase control potentiometer pins for each DAC output
const int POT_PHASE_PIN_DACA1 = A15; // Phase control for DACA1
const int POT_PHASE_PIN_DACA2 = A0;  // Phase control for DACA2
const int POT_PHASE_PIN_DACB1 = A1;  // Phase control for DACB1
const int POT_PHASE_PIN_DACB2 = A2;  // Phase control for DACB2

// Morph control pots for each DAC output
const int POT_MORPH_PIN_DACA1 = A9;  // Morph control for DACA1
const int POT_MORPH_PIN_DACA2 = A8;  // Morph control for DACA2
const int POT_MORPH_PIN_DACB1 = A7;  // Morph control for DACB1
const int POT_MORPH_PIN_DACB2 = A6;  // Morph control for DACB2

// Timing variables for square wave
unsigned long lastUpdate = 0;
bool squareWaveState = LOW;

// Global variables for rate range
float minRateHz = 0.1;  // 10 seconds period (0.1 Hz)
float maxRateHz = 10.0;  // 10 Hz
float currentRateHz = 1.0; // Default rate

// Global variables for waveform processing
volatile bool resetTriggered = false;  // Flag to indicate reset
volatile unsigned int currentLUTIndex = 0;  // Marked as volatile for ISR access
const int maxDACValue = 3090;  // Correct max value for 12-bit DACs to avoid clipping

// Interrupt service routine (ISR) for external sync/reset
void syncISR() {
    resetTriggered = true;  // Trigger reset when sync is detected
}

void setup() {
    Serial.begin(9600);

    // Initialize pin modes
    pinMode(SQUARE_PIN, OUTPUT);
    pinMode(SYNC_PIN, INPUT);  // External sync/reset pin

    dacControl.begin();  // Initialize the DACs using DACControl

    // Attach the interrupt for the external sync/reset (trigger on rising edge)
    attachInterrupt(digitalPinToInterrupt(SYNC_PIN), syncISR, RISING);
}

void loop() {
    unsigned long currentMicros = micros();

    // Read the potentiometer value for rate control
    int potRateValue = analogRead(POT_RATE_PIN);
    float normalizedPotValue = potRateValue / 1023.0;

    // Apply exponential scaling for smoother control
    float expScaledValue = pow(normalizedPotValue, 3.0);

    // Map the scaled value to the desired frequency range
    currentRateHz = minRateHz + expScaledValue * (maxRateHz - minRateHz);

    // Calculate the period in microseconds based on the current rate in Hertz
    unsigned long squarePeriod = (1.0 / currentRateHz) * 1000000;

    // Read phase potentiometer values
    int potPhaseValue_DACA1 = analogRead(POT_PHASE_PIN_DACA1);
    int potPhaseValue_DACA2 = analogRead(POT_PHASE_PIN_DACA2);
    int potPhaseValue_DACB1 = analogRead(POT_PHASE_PIN_DACB1);
    int potPhaseValue_DACB2 = analogRead(POT_PHASE_PIN_DACB2);

    // Read morph potentiometer values for each DAC channel
    int potMorphValue_DACA1 = analogRead(POT_MORPH_PIN_DACA1);
    int potMorphValue_DACA2 = analogRead(POT_MORPH_PIN_DACA2);
    int potMorphValue_DACB1 = analogRead(POT_MORPH_PIN_DACB1);
    int potMorphValue_DACB2 = analogRead(POT_MORPH_PIN_DACB2);

    // Call the function to update the square wave and other waveforms
    updateWaveforms(currentMicros, squarePeriod, potPhaseValue_DACA1, potPhaseValue_DACA2, potPhaseValue_DACB1, potPhaseValue_DACB2, potMorphValue_DACA1, potMorphValue_DACA2, potMorphValue_DACB1, potMorphValue_DACB2);
}

// Function to update waveforms based on the squarePeriod
void updateWaveforms(unsigned long currentMicros, unsigned long squarePeriod, int potPhaseValue_DACA1, int potPhaseValue_DACA2, int potPhaseValue_DACB1, int potPhaseValue_DACB2, int potMorphValue_DACA1, int potMorphValue_DACA2, int potMorphValue_DACB1, int potMorphValue_DACB2) {
    // Apply a scaling factor to step size to increase the update rate
    unsigned long stepSize = squarePeriod / LUT_SIZE;
    stepSize = stepSize > 10 ? stepSize : 10;  // Ensure a minimum step size of 10 microseconds

    // Check for the sync reset trigger
    if (resetTriggered) {
        // Reset all waveforms and the square wave
        currentLUTIndex = 0;
        resetTriggered = false;  // Clear the reset flag
    }

    if (currentMicros - lastUpdate >= stepSize) {
        lastUpdate = currentMicros;

        // Calculate phase offsets using integer math
        unsigned int phaseOffset_DACA1 = (potPhaseValue_DACA1 * LUT_SIZE) / 1023;
        unsigned int phaseOffset_DACA2 = (potPhaseValue_DACA2 * LUT_SIZE) / 1023;
        unsigned int phaseOffset_DACB1 = (potPhaseValue_DACB1 * LUT_SIZE) / 1023;
        unsigned int phaseOffset_DACB2 = (potPhaseValue_DACB2 * LUT_SIZE) / 1023;

        // Calculate lutIndexWithPhase for each DAC output
        unsigned int lutIndexWithPhase_DACA1 = (currentLUTIndex + phaseOffset_DACA1) % LUT_SIZE;
        unsigned int lutIndexWithPhase_DACA2 = (currentLUTIndex + phaseOffset_DACA2) % LUT_SIZE;
        unsigned int lutIndexWithPhase_DACB1 = (currentLUTIndex + phaseOffset_DACB1) % LUT_SIZE;
        unsigned int lutIndexWithPhase_DACB2 = (currentLUTIndex + phaseOffset_DACB2) % LUT_SIZE;

        // Apply morph factors per channel from the respective morph pots
        float morphFactor_DACA1 = constrain(potMorphValue_DACA1 / 1023.0f, 0.0f, 1.0f);
        float morphFactor_DACA2 = constrain(potMorphValue_DACA2 / 1023.0f, 0.0f, 1.0f);
        float morphFactor_DACB1 = constrain(potMorphValue_DACB1 / 1023.0f, 0.0f, 1.0f);
        float morphFactor_DACB2 = constrain(potMorphValue_DACB2 / 1023.0f, 0.0f, 1.0f);

        // --------- Ramp Morphing Waveform on DACB2 ---------
        unsigned int rampUpLength_DACB2 = (unsigned int)(LUT_SIZE * morphFactor_DACB2);
        unsigned int rampDownStart_DACB2 = rampUpLength_DACB2;

        int rampLUTValue_DACB2;
        unsigned int lutIndex_DACB2_final = lutIndexWithPhase_DACB2;

        if (rampUpLength_DACB2 > 0 && lutIndex_DACB2_final < rampUpLength_DACB2) {
            rampLUTValue_DACB2 = (lutIndex_DACB2_final * maxDACValue) / rampUpLength_DACB2;
        } else if ((LUT_SIZE - rampDownStart_DACB2) > 0) {
            rampLUTValue_DACB2 = maxDACValue - ((lutIndex_DACB2_final - rampDownStart_DACB2) * maxDACValue) / (LUT_SIZE - rampDownStart_DACB2);
        } else {
            rampLUTValue_DACB2 = 0;
        }

        dacControl.writeDAC(2, 1, rampLUTValue_DACB2);

        // --------- Sine-Triangle Morphing Waveform on DACB1 ---------
        int triangleValue_DACB1 = triangleLUT[lutIndexWithPhase_DACB1];
        int sineValue_DACB1 = sineLUT[lutIndexWithPhase_DACB1];

        int sineTriangleLUTValue_DACB1 = (int)((1.0f - morphFactor_DACB1) * triangleValue_DACB1 + morphFactor_DACB1 * sineValue_DACB1);
        dacControl.writeDAC(2, 0, sineTriangleLUTValue_DACB1);

        // --------- Exponential Morphing Waveform on DACA2 ---------
        int expoRiseValue_DACA2 = expoRiseLUT[lutIndexWithPhase_DACA2];
        int invExpoRiseValue_DACA2 = invExpoRiseLUT[lutIndexWithPhase_DACA2];

        int expoMorphLUTValue_DACA2 = (int)((1.0f - morphFactor_DACA2) * expoRiseValue_DACA2 + morphFactor_DACA2 * invExpoRiseValue_DACA2);
        dacControl.writeDAC(1, 1, expoMorphLUTValue_DACA2);

        // --------- Modified Multi-Wave Morphing Waveform on DACA1 ---------
        const uint16_t* waveforms[] = {
            rampDownLUT, 
            expoFallLUT, 
            wobRampDownLUT,
            invExpoFallLUT,
            stepDown4LUT,
            triangleLUT,
            stepUp4LUT,
            invExpoRiseLUT,
            wobRampUpLUT,
            expoRiseLUT, 
            rampUpLUT
        };

        const int numWaveforms = 11;
        const int numTransitions = numWaveforms - 1;

        // Calculate the morph section and local morph factor
        float scaledMorph_DACA1 = morphFactor_DACA1 * numTransitions;
        int section_DACA1 = (int)scaledMorph_DACA1;
        float localMorphFactor_DACA1;
        if (section_DACA1 >= numTransitions) {
            section_DACA1 = numTransitions - 1;
            localMorphFactor_DACA1 = 1.0f;
        } else {
            localMorphFactor_DACA1 = scaledMorph_DACA1 - section_DACA1;
        }

        const uint16_t* waveformA_DACA1 = waveforms[section_DACA1];
        const uint16_t* waveformB_DACA1 = waveforms[section_DACA1 + 1];

        int morphedValue_DACA1 = (int)((1.0f - localMorphFactor_DACA1) * waveformA_DACA1[lutIndexWithPhase_DACA1] + localMorphFactor_DACA1 * waveformB_DACA1[lutIndexWithPhase_DACA1]);
        dacControl.writeDAC(1, 0, morphedValue_DACA1);

        // Update the square wave output on the SQUARE_PIN
        if (currentLUTIndex == 0 || currentLUTIndex == LUT_SIZE / 2) {
            squareWaveState = !squareWaveState;
            digitalWrite(SQUARE_PIN, squareWaveState);
        }

        currentLUTIndex = (currentLUTIndex + 1) % LUT_SIZE;
    }
}

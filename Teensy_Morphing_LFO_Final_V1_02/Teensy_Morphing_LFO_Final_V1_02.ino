#include <SPI.h>
#include "DACControl.h"  // Include the DACControl library
#include "waveforms.h"   // Include the external LUTs

// Pin for square wave output
const int SQUARE_PIN = 9;

// Pin for external sync/reset input
const int SYNC_PIN = 6;  

// Define DAC chip select pins
const int CS_PIN_DAC1 = 7;
const int CS_PIN_DAC2 = 8;

// Instantiate the DACControl object
DACControl dacControl(CS_PIN_DAC1, CS_PIN_DAC2);

// Potentiometer pin for rate control
const int POT_RATE_PIN = A9;

// Phase control potentiometer pins for each DAC output
const int POT_PHASE_PIN_DACA1 = A2; // Phase control for DACA1
const int POT_PHASE_PIN_DACA2 = A4; // Phase control for DACA2
const int POT_PHASE_PIN_DACB1 = A8; // Phase control for DACB1
const int POT_PHASE_PIN_DACB2 = A6; // Phase control for DACB2

// Morph control pots for each DAC output
const int POT_MORPH_PIN_DACA1 = A1; // Morph control for DACA1
const int POT_MORPH_PIN_DACA2 = A3; // Morph control for DACA2
const int POT_MORPH_PIN_DACB1 = A7; // Morph control for DACB1
const int POT_MORPH_PIN_DACB2 = A5; // Morph control for DACB2

// Timing variables for square wave
unsigned long lastUpdate = 0;
bool squareWaveState = LOW;

// Global variables for rate range
float minRateHz = 0.1;      // 0.1 Hz (10 seconds period)
float maxRateHz = 11.5;     // Updated to 11.5 Hz
float currentRateHz = 1.0;  // Default rate

// Global variables for waveform processing
volatile bool resetTriggered = false;       // Flag to indicate reset
volatile unsigned int currentLUTIndex = 0;  // Marked as volatile for ISR access
const int maxDACValue = 4095;               // Use full 12-bit resolution

// Function prototype for updateWaveforms
void updateWaveforms(
    unsigned long currentMicros,
    unsigned long squarePeriod,
    int potPhaseValue_DACA1, int potPhaseValue_DACA2, int potPhaseValue_DACB1, int potPhaseValue_DACB2,
    int potMorphValue_DACA1, int potMorphValue_DACA2, int potMorphValue_DACB1, int potMorphValue_DACB2
);

// Interrupt service routine (ISR) for external sync/reset
void syncISR() {
    resetTriggered = true;  // Trigger reset when sync is detected
}

void setup() {
    Serial.begin(9600);

    // Initialize pin modes
    pinMode(SQUARE_PIN, OUTPUT);
    pinMode(SYNC_PIN, INPUT_PULLUP);  // External sync/reset pin

    dacControl.begin();  // Initialize the DACs using DACControl

    // Attach the interrupt for the external sync/reset (trigger on rising edge)
    attachInterrupt(digitalPinToInterrupt(SYNC_PIN), syncISR, RISING);
}

void loop() {
    unsigned long currentMicros = micros();

    // Read the potentiometer value for rate control
    int potRateValue = analogRead(POT_RATE_PIN);

    // Normalize the pot value to 0.0 to 1.0
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
    updateWaveforms(
        currentMicros,
        squarePeriod,
        potPhaseValue_DACA1, potPhaseValue_DACA2, potPhaseValue_DACB1, potPhaseValue_DACB2,
        potMorphValue_DACA1, potMorphValue_DACA2, potMorphValue_DACB1, potMorphValue_DACB2
    );
}

// Function to update waveforms based on the squarePeriod
void updateWaveforms(
    unsigned long currentMicros,
    unsigned long squarePeriod,
    int potPhaseValue_DACA1, int potPhaseValue_DACA2, int potPhaseValue_DACB1, int potPhaseValue_DACB2,
    int potMorphValue_DACA1, int potMorphValue_DACA2, int potMorphValue_DACB1, int potMorphValue_DACB2
) {
    unsigned long stepSize = squarePeriod / LUT_SIZE;
    if (stepSize == 0) {
        stepSize = 1;  // Ensure a minimum step size
    }

    // Check for the sync reset trigger
    if (resetTriggered) {
        // Reset all waveforms and the square wave
        currentLUTIndex = 0;
        resetTriggered = false;  // Clear the reset flag
    }

    if (currentMicros - lastUpdate >= stepSize) {
        lastUpdate = currentMicros;

        // Invert the phase potentiometer values to fix reverse operation
        int invertedPotPhaseValue_DACA1 = 1023 - potPhaseValue_DACA1;
        int invertedPotPhaseValue_DACA2 = 1023 - potPhaseValue_DACA2;
        int invertedPotPhaseValue_DACB1 = 1023 - potPhaseValue_DACB1;
        int invertedPotPhaseValue_DACB2 = 1023 - potPhaseValue_DACB2;

        // Calculate phase offsets using inverted potentiometer values
        unsigned int phaseOffset_DACA1 = (invertedPotPhaseValue_DACA1 * LUT_SIZE) / 1023;
        unsigned int phaseOffset_DACA2 = (invertedPotPhaseValue_DACA2 * LUT_SIZE) / 1023;
        unsigned int phaseOffset_DACB1 = (invertedPotPhaseValue_DACB1 * LUT_SIZE) / 1023;
        unsigned int phaseOffset_DACB2 = (invertedPotPhaseValue_DACB2 * LUT_SIZE) / 1023;

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

        // Invert the waveform value
        dacControl.writeDAC(2, 1, maxDACValue - rampLUTValue_DACB2);

        // --------- Sine-Triangle Morphing Waveform on DACB1 ---------
        int triangleValue_DACB1 = triangleLUT[lutIndexWithPhase_DACB1];
        int sineValue_DACB1 = sineLUT[lutIndexWithPhase_DACB1];

        int sineTriangleLUTValue_DACB1 = (int)((1.0f - morphFactor_DACB1) * triangleValue_DACB1 + morphFactor_DACB1 * sineValue_DACB1);

        // Invert the waveform value
        dacControl.writeDAC(2, 0, maxDACValue - sineTriangleLUTValue_DACB1);

        // --------- Morphing Through 5 LUTs on DACA2 ---------
        const uint16_t* lutArray_DACA2[] = {
            expoFallLUT,       // 1: Exponential fall
            invExpoFallLUT,    // 2: Inverse exponential fall
            sineLUT,           // 3: Sine wave
            invExpoRiseLUT,    // 4: Inverse exponential rise
            expoRiseLUT        // 5: Exponential rise
        };

        const int numLUTs_DACA2 = 5;  // Total number of LUTs
        const int numTransitions_DACA2 = numLUTs_DACA2 - 1;

        // Calculate which section of the morph we're in and the local morph factor
        float scaledMorph_DACA2 = morphFactor_DACA2 * numTransitions_DACA2;
        int section_DACA2 = (int)scaledMorph_DACA2;
        float localMorphFactor_DACA2;
        if (section_DACA2 >= numTransitions_DACA2) {
            section_DACA2 = numTransitions_DACA2 - 1;
            localMorphFactor_DACA2 = 1.0f;
        } else {
            localMorphFactor_DACA2 = scaledMorph_DACA2 - section_DACA2;
        }

        // Get the two LUTs for interpolation
        const uint16_t* lutA_DACA2 = lutArray_DACA2[section_DACA2];
        const uint16_t* lutB_DACA2 = lutArray_DACA2[section_DACA2 + 1];

        // Interpolate between the two LUTs
        int morphedValue_DACA2 = (int)((1.0f - localMorphFactor_DACA2) * lutA_DACA2[lutIndexWithPhase_DACA2] +
                                       localMorphFactor_DACA2 * lutB_DACA2[lutIndexWithPhase_DACA2]);

        // Invert the waveform value
        dacControl.writeDAC(1, 1, maxDACValue - morphedValue_DACA2);

        // --------- Modified Multi-Wave Morphing Waveform on DACA1 ---------
        const uint16_t* waveforms[] = {
            expoFallLUT, 
            wobRampDownLUT,
            invExpoFallLUT,
            stepDown4LUT,
            triangleLUT,
            stepUp4LUT,
            invExpoRiseLUT,
            wobRampUpLUT,
            expoRiseLUT
        };

        const int numWaveforms = 9;
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

        // Invert the waveform value
        dacControl.writeDAC(1, 0, maxDACValue - morphedValue_DACA1);

        // Update the square wave output on the SQUARE_PIN
        if (currentLUTIndex == 0 || currentLUTIndex == LUT_SIZE / 2) {
            squareWaveState = !squareWaveState;
            digitalWrite(SQUARE_PIN, squareWaveState);
        }

        currentLUTIndex = (currentLUTIndex + 1) % LUT_SIZE;
    }
}

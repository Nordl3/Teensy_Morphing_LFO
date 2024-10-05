#include <SPI.h>
#include "DACControl.h"  // Include the DACControl library
#include "waveforms.h"   // Include the external LUTs

// Pin for square wave output and mode switch
const int SQUARE_PIN = 33;
const int MODE_SWITCH_PIN = 22;  // Pin for mode switch (button)
const int SYNC_PIN = 2;          // Pin for external sync/reset input
const int SYNC_ENABLE_PIN = 23;  // Pin for sync enable/disable button

// Pins for LEDs
const int AUDIO_MODE_LED_PIN = 20;  // LED to indicate audio mode (on for audio mode, off for LFO mode)
const int SYNC_MODE_LED_PIN = 21;   // LED to indicate sync mode (on for sync enabled, off for sync disabled)

// Define DAC chip select pins
const int CS_PIN_DAC1 = 28;
const int CS_PIN_DAC2 = 29;

// Instantiate the DACControl object
DACControl dacControl(CS_PIN_DAC1, CS_PIN_DAC2);

// Potentiometer pins for rate, morph, and phase control
const int POT_RATE_PIN = A17;
const int POT_MORPH_PIN = A16;

// Phase control potentiometer pins for each DAC output
const int POT_PHASE_PIN_DACA1 = A15; // Phase control for DACA1
const int POT_PHASE_PIN_DACA2 = A0;  // Phase control for DACA2
const int POT_PHASE_PIN_DACB1 = A1;  // Phase control for DACB1
const int POT_PHASE_PIN_DACB2 = A2;  // Phase control for DACB2

// Timing variables for square wave
unsigned long lastUpdate = 0;
bool squareWaveState = LOW;

// Global variables for rate range
float minRateHz = 0.01;  // Default to 0.01 Hz for LFO mode
float maxRateHz = 10.0;  // Default max 10 Hz for LFO mode
float currentRateHz = 1.0; // Default rate
bool audioMode = false;  // Start in LFO mode
bool syncEnabled = true;  // Sync enabled by default

// Global variables for waveform processing
volatile bool resetTriggered = false;  // Flag to indicate reset
volatile unsigned int currentLUTIndex = 0;  // Marked as volatile for ISR access
const int maxDACValue = 4095;  // Max value for 12-bit DACs (MCP4822)

// Interrupt service routine (ISR) for external sync/reset
void syncISR() {
    if (syncEnabled) {
        resetTriggered = true;  // Only trigger reset if sync is enabled
    }
}

void setup() {
    Serial.begin(9600);

    // Initialize pin modes
    pinMode(SQUARE_PIN, OUTPUT);
    pinMode(MODE_SWITCH_PIN, INPUT_PULLUP);  // Button to switch between LFO and audio mode
    pinMode(SYNC_PIN, INPUT);                // External sync/reset pin
    pinMode(SYNC_ENABLE_PIN, INPUT_PULLUP);  // Button to enable/disable sync
    pinMode(AUDIO_MODE_LED_PIN, OUTPUT);     // Audio mode LED (pin 20)
    pinMode(SYNC_MODE_LED_PIN, OUTPUT);      // Sync mode LED (pin 21)

    // Set initial LED states
    digitalWrite(AUDIO_MODE_LED_PIN, LOW);   // Start with LFO mode (LED off)
    digitalWrite(SYNC_MODE_LED_PIN, HIGH);   // Start with sync enabled (LED on)

    dacControl.begin();  // Initialize the DACs using DACControl

    // Attach the interrupt for the external sync/reset (trigger on rising edge)
    attachInterrupt(digitalPinToInterrupt(SYNC_PIN), syncISR, RISING);
}

void loop() {
    unsigned long currentMicros = micros();

    // Check if the sync enable/disable button is pressed
    if (digitalRead(SYNC_ENABLE_PIN) == LOW) {
        syncEnabled = !syncEnabled;  // Toggle sync enable/disable
        delay(300);  // Simple debounce delay for the button press

        // Update the sync mode LED (pin 21)
        digitalWrite(SYNC_MODE_LED_PIN, syncEnabled ? HIGH : LOW);
    }

    // Check if mode switch is pressed
    if (digitalRead(MODE_SWITCH_PIN) == LOW) {
        // Toggle between LFO and Audio mode when the button is pressed
        audioMode = !audioMode;
        delay(300);  // Simple debounce delay for the button press

        // Update the audio mode LED (pin 20)
        digitalWrite(AUDIO_MODE_LED_PIN, audioMode ? HIGH : LOW);
    }

    // Set frequency range based on mode
    if (audioMode) {
        minRateHz = 10.0;  // Audio mode: 10 Hz
        maxRateHz = 8600.0; // Audio mode: ~8.6 kHz
    } else {
        minRateHz = 0.1;  // LFO mode: 0.01 Hz
        maxRateHz = 10.0;  // LFO mode: Up to 10 Hz
    }

    // Read the potentiometer value for rate control
    int potRateValue = analogRead(POT_RATE_PIN);
    float normalizedPotValue = potRateValue / 1023.0;

    // Apply exponential scaling using integer math
    float expScaledValue = pow(normalizedPotValue, 3.0);  // Exponent 3.0 for scaling

    // Map the scaled value to the desired frequency range
    currentRateHz = minRateHz + expScaledValue * (maxRateHz - minRateHz);

    // Calculate the period in microseconds based on the current rate in Hertz
    unsigned long squarePeriod = (1.0 / currentRateHz) * 1000000;

    // Read phase potentiometer values
    int potPhaseValue_DACA1 = analogRead(POT_PHASE_PIN_DACA1);
    int potPhaseValue_DACA2 = analogRead(POT_PHASE_PIN_DACA2);
    int potPhaseValue_DACB1 = analogRead(POT_PHASE_PIN_DACB1);
    int potPhaseValue_DACB2 = analogRead(POT_PHASE_PIN_DACB2);

    // Call the function to update the square wave and other waveforms
    updateWaveforms(currentMicros, squarePeriod, potPhaseValue_DACA1, potPhaseValue_DACA2, potPhaseValue_DACB1, potPhaseValue_DACB2);
}

// Function to update waveforms based on the squarePeriod
void updateWaveforms(unsigned long currentMicros, unsigned long squarePeriod, int potPhaseValue_DACA1, int potPhaseValue_DACA2, int potPhaseValue_DACB1, int potPhaseValue_DACB2) {
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

        // Read morph control
        int morphPotValue = analogRead(POT_MORPH_PIN);
        float morphFactor = constrain(morphPotValue / 1023.0f, 0.0f, 1.0f);  // Clamp between 0.0 and 1.0

        // --------- Ramp Morphing Waveform on DACB2 ---------
        unsigned int rampUpLength = (unsigned int)(LUT_SIZE * morphFactor);
        unsigned int rampDownStart = rampUpLength;

        int rampLUTValue;
        unsigned int lutIndex_DACB2 = lutIndexWithPhase_DACB2;

        if (rampUpLength > 0 && lutIndex_DACB2 < rampUpLength) {
            rampLUTValue = (lutIndex_DACB2 * maxDACValue) / rampUpLength;
        } else if ((LUT_SIZE - rampDownStart) > 0) {
            rampLUTValue = maxDACValue - ((lutIndex_DACB2 - rampDownStart) * maxDACValue) / (LUT_SIZE - rampDownStart);
        } else {
            rampLUTValue = 0;
        }

        dacControl.writeDAC(2, 1, rampLUTValue);

        // --------- Sine-Triangle Morphing Waveform on DACB1 ---------
        float morphFactorSine = morphFactor;

        int triangleValue = triangleLUT[lutIndexWithPhase_DACB1];
        int sineValue = sineLUT[lutIndexWithPhase_DACB1];

        int sineTriangleLUTValue = (int)((1.0f - morphFactorSine) * triangleValue + morphFactorSine * sineValue);
        dacControl.writeDAC(2, 0, sineTriangleLUTValue);

        // --------- Exponential Morphing Waveform on DACA2 ---------
        int expoRiseValue = expoRiseLUT[lutIndexWithPhase_DACA2];
        int invExpoRiseValue = invExpoRiseLUT[lutIndexWithPhase_DACA2];

        int expoMorphLUTValue = (int)((1.0f - morphFactor) * expoRiseValue + morphFactor * invExpoRiseValue);
        dacControl.writeDAC(1, 1, expoMorphLUTValue);

        // --------- Modified Multi-Wave Morphing Waveform on DACA1 ---------
        const uint16_t* waveforms[] = {
            //rampDownLUT, 
            //expoFallLUT, 
            wobRampDownLUT,
            invExpoFallLUT,
            stepDown4LUT,
            triangleLUT,
            stepUp4LUT,
            invExpoRiseLUT,
            wobRampUpLUT,
            //expoRiseLUT, 
            //rampUpLUT
        };

        const int numWaveforms = 7;
        const int numTransitions = numWaveforms - 1;

        // Calculate the morph section and local morph factor
        float scaledMorph = morphFactor * numTransitions;
        int section = (int)scaledMorph;
        float localMorphFactor;
        if (section >= numTransitions) {
            section = numTransitions - 1;
            localMorphFactor = 1.0f;
        } else {
            localMorphFactor = scaledMorph - section;
        }

        const uint16_t* waveformA = waveforms[section];
        const uint16_t* waveformB = waveforms[section + 1];

        int morphedValue = (int)((1.0f - localMorphFactor) * waveformA[lutIndexWithPhase_DACA1] + localMorphFactor * waveformB[lutIndexWithPhase_DACA1]);
        dacControl.writeDAC(1, 0, morphedValue);

        // Update the square wave output on the SQUARE_PIN
        if (currentLUTIndex == 0 || currentLUTIndex == LUT_SIZE / 2) {
            squareWaveState = !squareWaveState;
            digitalWrite(SQUARE_PIN, squareWaveState);
        }

        currentLUTIndex = (currentLUTIndex + 1) % LUT_SIZE;
    }
}

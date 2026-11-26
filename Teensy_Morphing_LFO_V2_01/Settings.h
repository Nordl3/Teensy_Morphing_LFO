#pragma once
#include <Arduino.h>

// Nordl3 Teensy Morphing LFO V2.01 - configurable hardware pins.
// PIN MAP FROM STABLE Final_V1.01 (not the conflicting V1.04 pin map).
namespace HW {
constexpr uint8_t DAC1_CS = 7;
constexpr uint8_t DAC2_CS = 8;
constexpr uint8_t SYNC_IN = 6;
constexpr uint8_t SQUARE_OUT = 9;
constexpr uint8_t RATE_ADC = A9;
constexpr uint8_t MORPH[4] = {A1,A3,A7,A5};
constexpr uint8_t PHASE[4] = {A2,A4,A8,A6};
}

// All hardware changes optional. Do not wire a raw +/-12V Eurorack CV to an ADC pin.
// Default: VCO derives pitch from the existing A9 RATE/CV summing circuitry.
// To use A10 instead, supply a professionally conditioned/protected 0..3.3V signal,
// then change CV_ADC to A10. A10 is a underside pad on Teensy 4.0.
constexpr uint8_t CV_ADC = HW::RATE_ADC;

// Set 1 only AFTER wiring both momentary switches and three LED resistors.
#define ENABLE_OPTIONAL_PANEL 0
constexpr uint8_t BTN_PREV = 3;        // Normally open switch to GND
constexpr uint8_t BTN_NEXT = 4;        // Normally open switch to GND
constexpr uint8_t LED_PINS[3] = {0,1,5}; // LEDs with series resistors to GND

constexpr uint16_t ADC_MAX_CODE = 4095;   // 12-bit reads; effective accuracy depends on noise
constexpr uint32_t AUDIO_SAMPLE_RATE = 48000;
constexpr uint32_t DAC_SPI_HZ = 12000000;    // conservative starting point
constexpr uint16_t DAC_OUTPUT_MAX = 4095;     // stable V1.01 used 4095; set 3090 if headroom requires
constexpr float LFO_MIN_HZ = 0.1f;
constexpr float LFO_MAX_HZ = 11.5f;
constexpr float VCO_MIN_HZ = 0.05f;
constexpr float VCO_PREVIEW_MIN_HZ = 20.0f;
constexpr float VCO_PREVIEW_MAX_HZ = 20000.0f;
constexpr float VCO_MAX_HZ = 20000.0f;       // VCO target; frequency/timbre quality requires hardware verification
constexpr uint32_t CONTROL_INTERVAL_MS = 10;
constexpr uint16_t AUTO_LEVEL_MAX_GAIN_Q12 = 32768; // cap at 8x to avoid boosting silence/noise

static_assert(DAC_OUTPUT_MAX <= 4095, "MCP4822 is a 12-bit DAC");
static_assert(VCO_MAX_HZ < AUDIO_SAMPLE_RATE / 2, "VCO pitch must remain below Nyquist");

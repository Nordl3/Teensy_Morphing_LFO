# Teensy Morphing LFO for Eurorack

**Description**: Voltage Controlled Morphing LFO for Teensy, designed for Eurorack systems.

## Overview

This project is a versatile LFO module that generates four morphing analog waveforms along with a square wave output. It provides extensive voltage control capabilities and manual adjustments for each waveform, making it ideal for adding expressive modulation to your Eurorack system.

## Features

- **Waveforms**: 4 analog morphing waveforms and 1 square wave.
- **Voltage Control**: The module includes voltage control for the rate with an adjustable CV input.
- **Synchronization**: Sync input for oscillator synchronization, which can be disabled via a switch.

## Panel Interface

- **Inputs**:

  - **Rate CV Input**: Accepts a control voltage to modulate the rate of the LFO.
  - **Oscillator Sync Input**: Syncs the oscillators to an external clock, with an option to disable.

- **Outputs**:

  - **5 Eurorack Jack Outputs**: One output for each waveform (4 analog morphing waves + 1 square wave).

- **Controls**:

  - **Rate Pot**: Controls the frequency of all waveforms.
  - **Rate CV Amount Pot**: Attenuates the effect of the rate CV input.
  - **Phase Pots (x4)**: Independent phase control for each analog waveform.
  - **Morph Pots (x4)**: Independent morph control for each analog waveform.

- **Switches**:

  - **Sync Disable Switch**: Disables the oscillator sync input.

- **LED Indicators**:

  - **5 LEDs**: One LED per waveform to provide visual feedback.

## Code Summary

The provided code for the Teensy Morphing LFO project includes several key modules:

- **Waveform Generation**: Utilizes lookup tables (LUTs) to generate various morphing waveforms.
- **DAC Control**: Manages two (MCP4822) DACs using the SPI interface to provide precise analog output for each waveform.
- **Rate and Phase Control**: Implements rate and phase adjustments for each waveform, including voltage control for modulation purposes.

### Key Files

- **`Teensy_Morphing_LFO-V1.01.ino`**: The main sketch for managing the LFO, setting up the DACs, reading inputs, and controlling the waveform generation.
- **`DACControl.cpp`** and **`DACControl.h`**: Manages SPI communication to control the DACs and handle analog waveform output.
- **`waveforms.h`**: Contains lookup tables for generating different waveforms, including ramp-up, ramp-down, sine, triangle, and more.

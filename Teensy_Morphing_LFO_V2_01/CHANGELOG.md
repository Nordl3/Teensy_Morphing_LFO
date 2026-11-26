# Changelog

## V2.01 — 20 kHz VCO extension

- Expanded the uncalibrated RATE sweep from nominal 20 Hz..2 kHz to 20 Hz..20 kHz.
- Raised the calibrated VCO/DDS pitch limit from 12 kHz to 20 kHz without changing the 48 kHz audio update rate.
- Added a fundamental-only factory wavetable band above 10.24 kHz; expanded LUT Deluxe bank conversion to generate a matching 20 kHz-safe band by Fourier projection.
- Retained VCO as factory mode, LFO range, all original pins and optional panel assignments.
- Expanded host and converter software tests. Actual high-frequency DAC and analogue output quality remain to be measured.

## V2.01 — VCO-default revision

- Factory default mode changed from LFO to VCO, retaining saved-mode EEPROM override.
- Uncalibrated VCO pitch is provisional until 1V/oct calibration and bench testing.

## V2.01 — experimental audio-rate firmware candidate

- Based the wiring and DAC command pattern on the confirmed stable Final_V1.01.
- Replaced variable LUT stepping with an IntervalTimer-driven 48 kHz DDS audio core.
- Added four phase/frequency accumulators, root square clock, sync reset, and 8 complementary wavetable scenes.
- Generated pitch-filtered 256-sample waveform frames; main-loop dynamic full-swing correction during morphing.
- Added serial scenes, modes, optional scene-panel wiring, CV reference calibration and EEPROM persistence.
- Added standalone Windows-friendly Python importer for LUT Deluxe Ultimate four-voice ZIP exports.
- Added host-side C++ and converter tests. Actual Teensyduino compilation and bench testing are pending.

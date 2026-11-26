# Changelog

## V2.01 — experimental audio-rate firmware candidate

- Based the wiring and DAC command pattern on the confirmed stable Final_V1.01.
- Replaced variable LUT stepping with an IntervalTimer-driven 48 kHz DDS audio core.
- Added four phase/frequency accumulators, root square clock, sync reset, and 8 complementary wavetable scenes.
- Generated pitch-filtered 256-sample waveform frames; main-loop dynamic full-swing correction during morphing.
- Added serial scenes, modes, optional scene-panel wiring, CV reference calibration and EEPROM persistence.
- Added standalone Windows-friendly Python importer for LUT Deluxe Ultimate four-voice ZIP exports.
- Added host-side C++ and converter tests. Actual Teensyduino compilation and bench testing are pending.

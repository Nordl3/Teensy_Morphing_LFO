# Teensy Morphing LFO / VCO V2.01

Four analogue waveforms, one Eurorack module, and an entirely new oscillator engine.

**Experimental / bench-test firmware.** Built for the original Teensy 4.0 + dual MCP4822 module. **It has not yet been flashed to the actual module.** The known-working `Teensy_Morphing_LFO_Final_V1.01` stays untouched as a rollback.

Project: https://github.com/Nordl3/Teensy_Morphing_LFO  
Wavetable designer: https://nordl3.github.io/LUT-Deluxe/

## What V2.01 does

- **48 kHz timer-driven audio engine** with four independent 32-bit phase accumulators, rather than stepping through 512 samples with a variable microsecond delay.
- **LFO and VCO** operation; LFO preserves the familiar 0.1 to 11.5 Hz RATE range. VCO has an intentionally conservative pitch ceiling of 12 kHz until hardware testing.
- **1V/oct calibration support**, using the existing conditioned A9 RATE/CV input (or optionally an external *properly conditioned* ADC input). Calibration is not a substitute for a linear analogue CV front end.
- **Four complementary musical voices**: each has a frequency relationship, pitch-band-selective table, movable phase offset, and continuously morphable waveform.
- **Eight built-in four-voice bank scenes**, from the V1.x-inspired palette to drawbars, odd/even partial subdivisions, four spectral bands, quadrature, formants, detuned Reese-style stacks and West Coast timbres.
- **Full-level morph compensation** done at 100 Hz in the control loop (not the audio interrupt), giving approximately full peak-to-peak DAC code range even between very different frames; gain is capped at 8x so near-silent harmonics are not magnified without bound.
- **Pitch-dependent, pre-filtered wavetable bands**. High frequencies automatically switch to variants with fewer harmonics. Band switches are not crossfaded in this build and can be audible.
- **Direct integration with LUT Deluxe Ultimate** via its *Four-Voice Teensy ZIP* export and the supplied pure-Python bank converter. No SD/PSRAM additions needed; banks are compiled into program flash.
- **Optional 2 buttons and 3 LEDs** for scene selection / mode switching, disabled by default in `Settings.h`. Serial commands work without any hardware addition.
- **Sync/reset input and square output** retained on the stable V1.01 pins.
- Preset, reference-frequency and CV calibration persistence in Teensy EEPROM (only upon explicit `save`).

### Current constraints

- The 4 DAC channels update sequentially over a shared SPI bus. With LDAC grounded as on the current board, they do **not** latch simultaneously; very fine phase alignment and high-end frequency performance need oscilloscope verification.
- 48 kHz sample rate does not imply clean 20 kHz complex-wave VCOs. At high pitches the filtered tables contain few harmonics, and the MCP4822 analogue output may roll off. Start at lower VCO pitches, examine the waveform and measure before increasing limits.
- The built-in Teensy ADC is set to 12-bit output codes, but PJRC warns that electrical noise means fewer effective bits in practice; the CV reading is smoothed and calibration readings are averaged. The existing RATE control is an **analogue summing network**, not a clean dedicated 1V/oct ADC input. Keep the rate pot fixed during pitch calibration. Tracking accuracy and octave span are not yet established.
- The original V1.01 waveform morph curves are represented by a related factory scene generated from your older waveform tables. This is not a bit-for-bit preservation of the original output; keep original V1.01 for that.
- No new DAC or ADC drivers, simultaneous DAC latching, DMA, PSRAM, MIDI note input, audio recording, SD loading or live external CV-to-waveform morph have been added. These would require hardware/interface decisions and/or more firmware.

## Before first flash

1. Save a copy of the known-good `Teensy_Morphing_LFO_Final_V1.01` project and ensure you can reflash it.
2. Open **`Teensy_Morphing_LFO_V2_01/Teensy_Morphing_LFO_V2_01.ino`** in Arduino IDE with Teensyduino. Select **Teensy 4.0**. You do **not** need PlatformIO, VSCode or extra Arduino libraries.
3. Inspect `Settings.h`: pin mapping is based on **Final_V1.01**, not the V1.04 wiring. DAC limit defaults to 4095 codes (matching stable V1.01). If the analogue conditioning clips on your unit, use `DAC_OUTPUT_MAX = 3090` until it has been measured.
4. Do not install any buttons or LEDs yet. Leave `ENABLE_OPTIONAL_PANEL 0`.
5. Disconnect sensitive destinations; start on a scope or safe monitor path. Power normally, check rail voltages, confirm four channels, waveform range, 0.1..11.5 Hz LFO sweep and sync/square output before entering VCO mode.
6. The Serial Monitor runs at **115200 baud**. Type `help`.

The original sketch was obtained from `Nordl3/Teensy_Morphing_LFO/Teensy_Morphing_LFO_Final_V1.01`. Stable V1.01 assigns the RATE input to A9; not A5, which appears in the later V1.04 test sketch. The original 2x-gain MCP4822 command pattern is preserved.

## Original hardware pin map (Final_V1.01)

| Function | Pin |
|---|---|
| RATE/CV | A9 |
| Morph, channels 1–4 | A1, A3, A7, A5 |
| Phase, channels 1–4 | A2, A4, A8, A6 |
| MCP4822 select 1 / select 2 | D7 / D8 |
| External sync (rising) | D6 (INPUT_PULLUP) |
| Square wave | D9 |
| SPI | Hardware MOSI/SCK |

The optional control interface uses **D3 and D4** for normally-open switches to ground, with `INPUT_PULLUP`; **D0, D1 and D5** for 3 LEDs with suitable series resistors. Only fit and enable these after checking actual spare connections. One button steps back, the other steps forward. Hold both for approximately 1.1 s to toggle LFO/VCO. Three LEDs show the scene number in binary; user-bank scene blinks all LEDs.

## The banks

| Scene | Musical relationship |
|---|---|
| 0 Legacy Palette | Loosely recreates the V1.x multi-morph paths, triangle/sine and morphable ramp |
| 1 Drawbar Harmonics | 16′, 8′, 5⅓′ and 4′ frequency ratios |
| 2 Interleaved Partials | Each voice gets a disjoint group of 4th-spaced harmonic partials |
| 3 Four-Band Spectrum | Foundation, body, presence and air harmonics; upper parts disappear naturally at high pitch |
| 4 Quadrature Orbit | 0°, 90°, 180°, 270° phase offsets; one nominal oscillator frequency |
| 5 Formant Choir | Four distinct moving vocal-formant peaks |
| 6 Reese Cluster | Half-frequency sub, two near-unison detuned voices, and a higher overtone |
| 7 West Coast Fold | Phase distortion, folding, FM-like and harmonic timbres |
| 8 (optional) LUT Deluxe | Imported four-voice pack, only present once `UserBank.h` is generated |

The presets configure the 4 DACs as *related voices*, not a fixed Bass/Synth/Keys/Strings quartet. All eight existing morph and phase knobs remain active. The scene ratio/phase offsets are added to the knob settings; the external square remains at root pitch.

## Use the existing CV input for 1V/oct

First try VCO as an uncalibrated oscillator with the current rate/CV circuit; `mode vco` uses an approximate 20 Hz–2 kHz pot sweep **until calibration**, which is *not* 1V/oct. Then:

1. Feed a **safe, accurately measured 0.000 V Eurorack CV** to the module's existing RATE CV socket. Leave the physical RATE pot at one fixed setting.
2. On serial monitor type `cal0` to capture the conditioned internal ADC reading.
3. Apply +1.000 V to the same input (not directly to Teensy) and type `cal1`.
4. Type `status` to check counts. You need a meaningful ADC difference, without rail clipping; the sign can be positive or negative.
5. Set the pitch that should correspond to 0V using `ref 110` (110 Hz is the default). Type `mode vco` and tune/check 0V, +1V, +2V etc using a reference frequency meter or scope.
6. If tracking is sensible, type `save`. No EEPROM writes occur unless you explicitly save.

A9's existing pot/CV summing stage may be unsuitable for accurate multi-octave tracking. If it saturates or the RATE pot affects pitch mapping unpredictably, no firmware can correct missing voltage data. Do not connect Eurorack voltages directly to an unprotected Teensy ADC. The optional `CV_ADC=A10` path is for a suitably scaled, biased and protected 0..3.3 V signal only.

### Serial commands

```
help
status
list
mode lfo
mode vco
scene 4
cal0
cal1
calclear
ref 110
save
```

## Use your own LUT Deluxe waveform bank

1. In [LUT Deluxe Ultimate](https://nordl3.github.io/LUT-Deluxe/), select **Export Studio → Four-Voice Teensy ZIP**. Select the original 12-bit DAC target (3090 or 4095).
2. Copy that ZIP onto your Windows computer. Extract this entire firmware ZIP first.
3. Open CMD in the folder that contains `Teensy_Morphing_LFO_V2_01.ino` and run:

   ```cmd
   python tools\convert_ultimate_bank.py "C:\path\to\LUTDeluxe_FourVoice.zip"
   ```

   The script generates/replaces `UserBank.h` in the sketch directory. Standard Python only; no pip required.
4. Restart/compile Arduino IDE with the updated `UserBank.h`, then flash the Teensy. `scene 8` now selects the user bank (via Serial or optional panel).

By default the converter reduces a potentially giant Ultimate bank to 8 morph frames, 256 samples per frame and 9 frequency bands, which needs **147,456 bytes** for 4 voices. It automatically translates 16-bit exported codes to the original 12-bit MCP4822. It also carries across each voice's ratio, octave, semitone, fine detuning and phase metadata. To adjust sizes:

```cmd
python tools\convert_ultimate_bank.py "C:\path\to\Bank.zip" --frames 8 --samples 512 --bands 9
```

The converter checks a fixed flash allocation budget; it does not guarantee the entire compiled firmware fits. Inspect the Arduino IDE flash usage after compilation. At lower table sample sizes resampling isn't a perfect anti-alias filter; use the existing pitch-filtered export bands for the best results. To revert to firmware without a user bank, restore the placeholder `UserBank.h` from the ZIP.

## Performance and verification

This is a **best-effort engineering candidate**, not a hardware-certified release.

- Arduino firmware uses a `IntervalTimer` callback at 48,000 ticks/second. It writes four MCP4822 channels via 12 MHz SPI (16-bit words); calculations in the ISR are fixed point.
- ADC scanning, frequency calculation, band selection, and full-level compensation are performed in the main loop every 10 ms. No `pow`, `float` or `analogRead` runs in the DAC ISR.
- 32-bit phase increment is `round(f × 2^32 / 48000)`; independent voice frequencies are derived from the shared root pitch and preset ratios.
- DSP uses periodic linear sample interpolation, adjacent-frame morph interpolation, and pitch-aware pre-filtered tables. There is no DMA in this build: it may not be needed at 48 kHz, subject to measurement.
- Tests included: `tests/test_host.cpp` (uses tiny hardware stubs), `tests/test_converter.py` (fake four-voice export). These are not substitute for Teensyduino compilation or rack tests.

### Bench checklist

- Compile with the actual installed Teensyduino and confirm flash/RAM fit.
- Use DHO804/DS1052E to confirm all 4 outputs, frequency, 1/2/4x ratios, 90° quadrature, and level while sweeping morph pots.
- Measure sample timing jitter and look for missing DAC frames at 48 kHz.
- Check lowest and highest pot positions for inverse phase and zero-crossing reset behaviour.
- Test 1V/oct at 0, +1, +2, +3 V through the module's CV input and compare actual pitch in cents; watch for analogue saturation.
- Check for glitches on scene switching and pitch-band transitions, and verify 3090-versus-4095 code output limits on your conditioning stage.

## Source/provenance

- Stable hardware and core behaviour: [Final_V1.01](https://github.com/Nordl3/Teensy_Morphing_LFO/tree/main/Teensy_Morphing_LFO_Final_V1.01)
- `OriginalWaveformsV104.h` is an archived copy of your uploaded V1.04 lookup header; the factory tables are **newly generated**, inspired in part by those earlier waves. Changing V2.01 does not modify the original repository revision.
- Factory wavetable data is generated by `tools/generate_factory.py`; this optional developer script requires NumPy, but using/flashing firmware does not.
- `tools/convert_ultimate_bank.py` is standard-library Python.

**No physical module was available for validation when V2.01 was prepared.**

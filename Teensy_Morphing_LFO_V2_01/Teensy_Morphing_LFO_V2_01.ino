/*
  Nordl3 - Teensy Morphing LFO / VCO 2.01
  Replaces time-stepped 512-entry playback with fixed-rate 32-bit DDS.
  Pin map is based on Teensy_Morphing_LFO_Final_V1.01 from GitHub.
  First use: LFO mode. VCO 1V/oct requires calibration of actual analogue front-end.
  No PSRAM, SD, new ADC, DMA, or panel wiring required.
*/
#include <Arduino.h>
#include <SPI.h>
#include <EEPROM.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include "Settings.h"
#include "DACControl.h"
#include "FactoryBanks.h"
#include "UserBank.h"

#if !defined(__IMXRT1062__)
#warning V2.01 is designed for Teensy 4.0/4.1. The timer and flash usage need verification elsewhere.
#endif

static DACControl dac;
static IntervalTimer audioTimer;

enum OperatingMode : uint8_t { MODE_LFO = 0, MODE_VCO = 1 };
struct BankSource {
  const uint16_t* samples;
  const uint32_t* bandCeilings;
  const float* ratios;
  const float* phases;
  uint16_t frames, sampleCount, bands, codeMax;
  const char* name;
};
struct AudioControls {
  const uint16_t* bank;
  uint16_t frames, samples, bands;
  uint8_t indexBits;
  uint32_t rootStep;
  uint32_t steps[4];
  uint32_t phaseOffsets[4];
  uint8_t selectedBand[4];
  uint8_t frameA[4];
  uint16_t blend[4];          // 0..4096
  uint16_t gainQ12[4];       // full-output-range normalization; 4096=1x
  int32_t offsetCode[4];
};

static AudioControls sharedControl[2] = {};
static volatile uint8_t activeControl = 0;
static uint32_t phaseAcc[4] = {0,0,0,0};
static uint32_t rootPhase = 0;
static volatile uint8_t resetRequested = 0;
static volatile uint32_t audioTicks = 0;

static OperatingMode mode = MODE_LFO;
static uint8_t scene = 0;
static float referenceHz = 110.0f;   // A2 for 0V after calibration
static float cvZero = 0, cvOne = 0;  // raw ADC values measured at 0V and 1V
static bool haveZero = false, haveOne = false;
static float rootHz = 1.0f;
static float adcFiltered[9];
static float cvFiltered = 0;
static float lastAdcRaw = 0;
static uint32_t lastControlMs = 0;
static char serialLine[80] = {0};
static uint8_t serialCount = 0;

struct StoredSettings {
  uint32_t magic;
  float referenceHz;
  float cvZero, cvOne;
  uint8_t haveZero, haveOne, mode, scene;
  uint32_t checksum;
};
constexpr uint32_t SETTINGS_MAGIC = 0x4C463231; // LF21
static uint32_t checksumOf(const StoredSettings &s) {
  uint32_t h = 2166136261u;
  const uint8_t *p = reinterpret_cast<const uint8_t*>(&s);
  for (size_t i=0; i<offsetof(StoredSettings,checksum); ++i) {
    h ^= p[i]; h *= 16777619u;
  }
  return h;
}
static bool cvCalibrated() {
  return haveZero && haveOne && fabsf(cvOne-cvZero) > 8.0f;
}
static uint8_t sceneCount() {
  return FACTORY_SCENES + (USER_BANK_AVAILABLE ? 1 : 0);
}
static BankSource selectedBank() {
#if USER_BANK_AVAILABLE
  if (scene == FACTORY_SCENES) {
    return {USER_BANK_DATA,USER_BANK_LIMITS,USER_BANK_RATIOS,USER_BANK_PHASES,
      USER_BANK_FRAMES,USER_BANK_SAMPLES,USER_BANK_BANDS,USER_BANK_MAXCODE,"LUT Deluxe User Bank"};
  }
#endif
  return {reinterpret_cast<const uint16_t*>(&FACTORY_TABLES[scene]),
    FACTORY_LIMITS,FACTORY_RATIOS[scene],FACTORY_PHASES[scene],
    FACTORY_FRAMES,FACTORY_SAMPLES,FACTORY_BANDS,FACTORY_MAXCODE,FACTORY_NAMES[scene]};
}

static inline uint16_t boundedCode(int32_t n) {
  if (n<=0) return 0;
  if (n>=DAC_OUTPUT_MAX) return DAC_OUTPUT_MAX;
  return (uint16_t)n;
}
static inline uint16_t readTable(const AudioControls &a, uint8_t ch,
         uint8_t band, uint8_t frame, uint16_t idx) {
  uint32_t off = (((uint32_t)ch*a.bands + band)*a.frames + frame)*a.samples + idx;
  return a.bank[off];
}
static inline int32_t interpolate(int32_t x, int32_t y, uint16_t alpha) {
  return x + ((y-x)*(int32_t)alpha >> 12);
}
static inline uint16_t renderVoice(const AudioControls &a, uint8_t ch, uint32_t phase) {
  uint16_t i0 = phase >> (32 - a.indexBits);
  uint16_t i1 = (i0+1) & (a.samples-1);
  uint16_t frac = (phase >> (20 - a.indexBits)) & 4095u;
  uint8_t band = a.selectedBand[ch], frame = a.frameA[ch];
  int32_t a0 = readTable(a,ch,band,frame,i0);
  int32_t a1 = readTable(a,ch,band,frame,i1);
  int32_t sampleA = interpolate(a0,a1,frac);
  uint8_t nextFrame = frame + (frame+1 < a.frames ? 1 : 0);
  int32_t b0 = readTable(a,ch,band,nextFrame,i0);
  int32_t b1 = readTable(a,ch,band,nextFrame,i1);
  int32_t sampleB = interpolate(b0,b1,frac);
  int32_t mixed = interpolate(sampleA,sampleB,a.blend[ch]);
  int32_t output = ((mixed * (int32_t)a.gainQ12[ch]) >> 12) + a.offsetCode[ch];
  return boundedCode(output);
}

// ONLY the ISR writes to SPI; there are no floating-point or ADC operations here.
static void audioISR() {
  const AudioControls &a = sharedControl[activeControl];
  if (resetRequested) {
    resetRequested=0;
    rootPhase=0;
    for (uint8_t c=0; c<4; ++c) phaseAcc[c]=0;
  }
  uint16_t code[4];
  for (uint8_t c=0; c<4; ++c) {
    phaseAcc[c] += a.steps[c];
    code[c] = renderVoice(a,c,phaseAcc[c]+a.phaseOffsets[c]);
  }
  rootPhase += a.rootStep;
  digitalWriteFast(9, (rootPhase & 0x80000000u) ? HIGH:LOW);
  dac.writeFour(code[0],code[1],code[2],code[3]);
  ++audioTicks;
}
static void syncISR() { resetRequested=1; }

static uint16_t adcOf(uint8_t channelIndex) {
  if(channelIndex==0) return analogRead(HW::RATE_ADC);
  if(channelIndex<5) return analogRead(HW::MORPH[channelIndex-1]);
  return analogRead(HW::PHASE[channelIndex-5]);
}
static void initialADC() {
  analogReadResolution(12); // Teensy 4.0 ADC 0..4095; noise reduces effective bits.
  for(uint8_t i=0; i<9; ++i) adcFiltered[i]=(float)adcOf(i);
  cvFiltered = (float)analogRead(CV_ADC);
}
static float readPitchHz() {
  if(mode==MODE_LFO) {
    float u=constrain(adcFiltered[0]/(float)ADC_MAX_CODE,0.0f,1.0f);
    return LFO_MIN_HZ + u*u*u*(LFO_MAX_HZ-LFO_MIN_HZ);
  }
  float cv=constrain(cvFiltered,0.0f,(float)ADC_MAX_CODE);
  if(cvCalibrated()) {
    float volts=constrain((cv-cvZero)/(cvOne-cvZero),-8.0f,10.0f);
    return constrain(referenceHz*exp2f(volts),VCO_MIN_HZ,VCO_MAX_HZ);
  }
  // Safe uncalibrated pitch-preview mode; NOT a 1V/oct relationship.
  return 20.0f*powf(100.0f,cv/(float)ADC_MAX_CODE);
}
static uint32_t stepFromHz(float hz) {
  hz = constrain(hz,0.0f,VCO_MAX_HZ);
  return (uint32_t)(hz*(4294967296.0 / (double)AUDIO_SAMPLE_RATE)+0.5);
}
static uint8_t pitchBand(const BankSource &src, float hz) {
  if(mode==MODE_LFO || src.bands<2) return 0;
  for(uint8_t band=1;band<src.bands; ++band)
    if(hz <= src.bandCeilings[band]) return band;
  return src.bands-1;
}
static uint8_t countBits(uint16_t n) {
  uint8_t result=0;
  while(n>1){ ++result; n>>=1; }
  return result;
}

static void autoLevel(AudioControls &p, const BankSource &src, uint8_t c) {
  // One cycle scanned at control rate, not in the audio ISR.
  // This preserves a near-full DAC swing through morphs including complex banks.
  int32_t mn=INT32_MAX,mx=INT32_MIN;
  for(uint16_t i=0;i<p.samples;i++) {
    int32_t a=readTable(p,c,p.selectedBand[c],p.frameA[c],i);
    uint8_t next=p.frameA[c]+(p.frameA[c]+1<p.frames ? 1:0);
    int32_t b=readTable(p,c,p.selectedBand[c],next,i);
    int32_t v=interpolate(a,b,p.blend[c]);
    if(v<mn) mn=v;
    if(v>mx) mx=v;
  }
  if(mx-mn < 4) { // near-silent partial (e.g., high-pitch spectral partition)
    p.gainQ12[c]=0;
    p.offsetCode[c]=DAC_OUTPUT_MAX/2;
    return;
  }
  uint32_t q = (uint32_t)(((uint64_t)DAC_OUTPUT_MAX*4096u)/(uint32_t)(mx-mn));
  if(q>AUTO_LEVEL_MAX_GAIN_Q12) q=AUTO_LEVEL_MAX_GAIN_Q12;
  p.gainQ12[c]=(uint16_t)q;
  p.offsetCode[c]=((int32_t)DAC_OUTPUT_MAX - (((int32_t)(mn+mx)*(int32_t)q)>>12))/2;
}

static void updateControls() {
  // Filtering is deliberately outside ISR. A9 is filtered twice only if separate CV pin is chosen.
  for(uint8_t i=0; i<9; ++i)
    adcFiltered[i] += (float)((int)adcOf(i)-adcFiltered[i])*0.22f;
  lastAdcRaw=analogRead(CV_ADC);
  cvFiltered += (lastAdcRaw-cvFiltered)*0.22f;
  rootHz=readPitchHz();
  BankSource source=selectedBank();
  uint8_t next=1-activeControl;
  AudioControls &p=sharedControl[next];
  p.bank=source.samples;
  p.frames=source.frames;p.samples=source.sampleCount;p.bands=source.bands;
  p.indexBits=countBits(p.samples);
  p.rootStep=stepFromHz(rootHz);
  for(uint8_t c=0;c<4;c++){
    float ratio=source.ratios[c];
    float pitch=constrain(rootHz*ratio,VCO_MIN_HZ,VCO_MAX_HZ);
    p.steps[c]=stepFromHz(pitch);
    float phasePot=1.0f-constrain(adcFiltered[c+5]/(float)ADC_MAX_CODE,0.0f,1.0f);
    float phaseCycles=phasePot+source.phases[c];
    phaseCycles-=floorf(phaseCycles);
    p.phaseOffsets[c]=(uint32_t)(phaseCycles*4294967296.0);
    float morph=constrain(adcFiltered[c+1]/(float)ADC_MAX_CODE,0.0f,1.0f);
    float morphPos=morph*(p.frames-1);
    uint8_t frame=(uint8_t)morphPos;
    if(frame>=p.frames-1) {frame=p.frames-2; morphPos=(float)(p.frames-1);}
    p.frameA[c]=frame;
    p.blend[c]=(uint16_t)constrain((morphPos-frame)*4096.0f+0.5f,0.0f,4096.0f);
    p.selectedBand[c]=pitchBand(source,pitch);
    autoLevel(p,source,c);
  }
  noInterrupts();
  activeControl=next;
  interrupts();
}
static void updateScene(int newScene) {
  int n=sceneCount();
  scene=(uint8_t)((newScene%n+n)%n);
  updateControls();
  Serial.print("Scene ");Serial.print(scene);Serial.print(": ");Serial.println(selectedBank().name);
}
static void updateMode(OperatingMode newMode) {
  mode=newMode;updateControls();
  Serial.print("Mode: ");Serial.println(mode==MODE_LFO?"LFO":"VCO");
}

static void saveSettings() {
  StoredSettings c={};c.magic=SETTINGS_MAGIC;c.referenceHz=referenceHz;
  c.cvZero=cvZero;c.cvOne=cvOne;c.haveZero=haveZero?1:0;
  c.haveOne=haveOne?1:0;c.mode=mode;c.scene=scene;
  c.checksum=checksumOf(c);
  EEPROM.put(0,c);
  Serial.println("Saved settings in EEPROM.");
}
static void loadSettings() {
  StoredSettings c={};EEPROM.get(0,c);
  if(c.magic!=SETTINGS_MAGIC || c.checksum!=checksumOf(c)) return;
  if(isfinite(c.referenceHz) && c.referenceHz>=0.1f && c.referenceHz<=1000) referenceHz=c.referenceHz;
  if(isfinite(c.cvZero) && isfinite(c.cvOne) && fabsf(c.cvOne-c.cvZero)>8.0f) {
    cvZero=c.cvZero;cvOne=c.cvOne;haveZero=c.haveZero;haveOne=c.haveOne;
  }
  mode=(c.mode==MODE_VCO?MODE_VCO:MODE_LFO);
  if(c.scene<sceneCount())scene=c.scene;
}
static float captureADCMean() {
  // Average actual ADC readings for calibration, without blocking the audio ISR.
  uint32_t total=0;
  for(uint8_t k=0;k<32;k++) total+=(uint16_t)analogRead(CV_ADC);
  return (float)total/32.0f;
}
static void printStatus() {
  Serial.println("---- Nordl3 V2.01 ----");
  Serial.print("Mode: ");Serial.println(mode==MODE_LFO?"LFO":"VCO");
  Serial.print("Scene: ");Serial.print(scene);Serial.print(" ");Serial.println(selectedBank().name);
  Serial.print("Root frequency Hz: ");Serial.println(rootHz,3);
  Serial.print("CV raw A/D: ");Serial.print((int)lastAdcRaw);
  Serial.print(" (smoothed ");Serial.print(cvFiltered,1);Serial.println(")");
  Serial.print("Pitch calibration: ");Serial.println(cvCalibrated()?"0V/1V stored":"UNCONFIGURED - VCO preview scale only");
  Serial.print("CV0 / CV1 counts: ");Serial.print(cvZero,1);Serial.print(" / ");Serial.println(cvOne,1);
  Serial.print("Hz at CV0: ");Serial.println(referenceHz,2);
  noInterrupts();uint32_t ticks=audioTicks;interrupts();
  Serial.print("Audio ticks since boot: ");Serial.println(ticks);
  Serial.print("AUDIO_SAMPLE_RATE: ");Serial.println(AUDIO_SAMPLE_RATE);
}
static void printHelp() {
  Serial.println("help | status | list | mode lfo | mode vco | scene N");
  Serial.println("cal0 : capture conditioned CV ADC at applied 0V");
  Serial.println("cal1 : capture conditioned CV ADC at applied 1V");
  Serial.println("calclear : discard V/oct calibration");
  Serial.println("ref 110 : set root Hz at 0V; save : persist mode/scene/calibration");
  Serial.println("WARNING: calibrate without changing RATE POT; A9 may be a summed input.");
}
static void handleSerial(char *s) {
  if(!strcmp(s,"help")) printHelp();
  else if(!strcmp(s,"status")) printStatus();
  else if(!strcmp(s,"list")) {
    for(uint8_t i=0;i<sceneCount();i++){
      Serial.print(i);Serial.print(" ");
      Serial.println(i<FACTORY_SCENES?FACTORY_NAMES[i]:"LUT Deluxe User Bank");
    }
  }
  else if(!strcmp(s,"mode lfo")) updateMode(MODE_LFO);
  else if(!strcmp(s,"mode vco")) updateMode(MODE_VCO);
  else if(!strncmp(s,"scene ",6)) {
    char *end;long n=strtol(s+6,&end,10);
    if(end!=s+6 && *end=='\0' && n>=0 && n<sceneCount()) updateScene((int)n);
    else Serial.println("Scene number out of range. Use 'list'.");
  }
  else if(!strcmp(s,"cal0")) {
    cvZero=captureADCMean();haveZero=true;updateControls();
    Serial.print("0V ADC captured = ");Serial.println(cvZero,1);
  }
  else if(!strcmp(s,"cal1")) {
    cvOne=captureADCMean();haveOne=true;updateControls();
    Serial.print("1V ADC captured = ");Serial.println(cvOne,1);
    if(!cvCalibrated())Serial.println("ERROR: insufficient ADC span; calibration invalid.");
  }
  else if(!strcmp(s,"calclear")) {
    haveZero=haveOne=false;updateControls();Serial.println("Calibration cleared (not saved).");
  }
  else if(!strncmp(s,"ref ",4)) {
    float hz=atof(s+4);
    if(hz>=0.1f && hz<=1000.f) {referenceHz=hz;updateControls();Serial.println("Reference updated.");}
    else Serial.println("Reference must be 0.1..1000 Hz.");
  }
  else if(!strcmp(s,"save")) saveSettings();
  else Serial.println("Unknown command. Type help.");
}
static void checkSerial() {
  while(Serial.available()) {
    char c=(char)Serial.read();
    if(c=='\r'||c=='\n') {
      if(serialCount) {serialLine[serialCount]='\0';handleSerial(serialLine);serialCount=0;}
    } else if(c>=32 && c<127 && serialCount<sizeof(serialLine)-1) serialLine[serialCount++]=c;
  }
}

#if ENABLE_OPTIONAL_PANEL
struct DebouncedButton {
  uint8_t pin; bool stableHigh,rawHigh;uint32_t changedAt;
  void init(uint8_t p){pin=p;pinMode(pin,INPUT_PULLUP);stableHigh=rawHigh=digitalRead(pin);changedAt=millis();}
  void poll(){bool r=digitalRead(pin);if(r!=rawHigh){rawHigh=r;changedAt=millis();}
    if(millis()-changedAt>25)stableHigh=rawHigh;}
};
static DebouncedButton prevButton,nextButton;
static bool prevHeld=false,nextHeld=false,longFired=false;
static uint32_t bothStarted=0;
static void panelPoll(){
  prevButton.poll();nextButton.poll();
  bool a=!prevButton.stableHigh,b=!nextButton.stableHigh;
  if(a&&b){
    if(!bothStarted)bothStarted=millis();
    if(!longFired && millis()-bothStarted>=1100){updateMode(mode==MODE_LFO?MODE_VCO:MODE_LFO);longFired=true;}
  } else if(!a&&!b){
    if(!longFired){if(prevHeld && !nextHeld)updateScene(scene-1);
                   if(nextHeld && !prevHeld)updateScene(scene+1);}
    bothStarted=0;longFired=false;prevHeld=false;nextHeld=false;
  } else {prevHeld|=a;nextHeld|=b;bothStarted=0;}
  uint8_t pattern=scene&7;
  if(scene>=FACTORY_SCENES)pattern=((millis()/300)&1)?7:0;
  for(uint8_t i=0;i<3;i++) digitalWrite(LED_PINS[i],(pattern>>i)&1);
}
#endif
void setup() {
  Serial.begin(115200);
  pinMode(HW::SYNC_IN, INPUT_PULLUP);
  pinMode(HW::SQUARE_OUT,OUTPUT);
  digitalWriteFast(9,LOW);
  initialADC();
  loadSettings();
#if ENABLE_OPTIONAL_PANEL
  prevButton.init(BTN_PREV);nextButton.init(BTN_NEXT);
  for(uint8_t i=0;i<3;i++){pinMode(LED_PINS[i],OUTPUT);digitalWrite(LED_PINS[i],LOW);}
#endif
  dac.begin();
  updateControls();
  attachInterrupt(digitalPinToInterrupt(HW::SYNC_IN),syncISR,RISING);
  if(!audioTimer.begin(audioISR,1000000.0f/AUDIO_SAMPLE_RATE)) {
    Serial.println("FATAL: IntervalTimer could not start. DAC held at midpoint.");
    while(true){delay(1000);}
  }
  audioTimer.priority(112);
  lastControlMs=millis();
  Serial.println("Nordl3 Teensy Morphing LFO V2.01 - type help");
  printStatus();
}
void loop() {
  uint32_t now=millis();
  if((uint32_t)(now-lastControlMs)>=CONTROL_INTERVAL_MS) {
    lastControlMs=now;
    updateControls();
  }
  checkSerial();
#if ENABLE_OPTIONAL_PANEL
  panelPoll();
#endif
}

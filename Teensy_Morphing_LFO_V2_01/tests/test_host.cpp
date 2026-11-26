#include <cassert>
#include <cmath>
#include <iostream>
#include "stubs/Arduino.h"
#include "stubs/SPI.h"
#include "stubs/EEPROM.h"
SerialClass Serial;
SPIClass SPI;
EEPROMClass EEPROM;
#include "../Teensy_Morphing_LFO_V2_01.ino"
#include "../DACControl.cpp"
int main(){
 for(int i=0;i<32;i++)gAnalog[i]=512;
 setup();
 assert(mode==MODE_VCO);
 assert(sceneCount()==8+(USER_BANK_AVAILABLE?1:0));
 assert(sharedControl[activeControl].steps[0]>0);
 for(int i=0;i<10000;i++)audioISR();
 assert(audioTicks==10000);
 for(uint16_t word:SPI.last)assert((word&0xFFF)<=4095);
 updateMode(MODE_VCO);
 assert(!cvCalibrated());
 cvFiltered=0;
 assert(std::fabs(readPitchHz()-20.0f)<0.001f);
 cvFiltered=(float)ADC_MAX_CODE;
 assert(std::fabs(readPitchHz()-20000.0f)<1.0f);
 assert(VCO_MAX_HZ==20000.0f);
 assert(std::fabs((double)stepFromHz(20000)*AUDIO_SAMPLE_RATE/4294967296.0-20000.0)<0.01);
 gAnalog[HW::RATE_ADC]=440;
 updateControls();
 handleSerial((char*)"cal0");
 gAnalog[HW::RATE_ADC]=650;
 updateControls();
 handleSerial((char*)"cal1");
 assert(cvCalibrated());
 cvFiltered=cvZero;
 assert(std::fabs(readPitchHz()-referenceHz)<0.1f);
 cvFiltered=cvOne;
 assert(std::fabs(readPitchHz()-referenceHz*2)<0.1f);
 cvFiltered=cvZero+8.0f*(cvOne-cvZero);
 assert(std::fabs(readPitchHz()-20000.0f)<1.0f);
 updateScene(4);
 assert(scene==4);
 cvFiltered=(float)ADC_MAX_CODE;
 updateControls();
 {
   const AudioControls &hi=sharedControl[activeControl];
   assert(hi.bands==10);
   assert(hi.selectedBand[0]==9); // 20 kHz -> fundamental-only pitch band
   assert(hi.selectedBand[1]==9);
   assert(hi.steps[0]==stepFromHz(20000.0f));
   uint16_t lo=4095, up=0;
   uint32_t phase=0;
   for(int i=0;i<480;i++) {
     phase+=hi.steps[0];
     uint16_t value=renderVoice(hi,0,phase);
     if(value<lo)lo=value;
     if(value>up)up=value;
   }
   assert(up-lo>200); // high-pitch waveform is not DC/silent
 }
 assert(sharedControl[activeControl].phaseOffsets[0] != sharedControl[activeControl].phaseOffsets[1]);
 saveSettings();
 mode=MODE_LFO;scene=0;referenceHz=220;
 loadSettings();
 assert(scene==4 && mode==MODE_VCO && std::fabs(referenceHz-110)<0.1f);
 for(int s=0;s<sceneCount();s++) {
   updateScene(s);
   for(int v=0;v<4;v++) {
     auto &a=sharedControl[activeControl];
     assert(a.gainQ12[v]<=32768);
     assert(a.steps[v]>0);
     for(int i=0;i<1024;i++) {
       auto n=renderVoice(a,v,(uint32_t)i*4194304u);
       assert(n<=DAC_OUTPUT_MAX);
     }
   }
 }
 std::cout << "PASS: all available scenes, DDS ISR, DAC codes, phase relationships, calibration and EEPROM\n";
}

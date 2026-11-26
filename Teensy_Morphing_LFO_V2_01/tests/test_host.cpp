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
 assert(mode==MODE_LFO);
 assert(sceneCount()==8+(USER_BANK_AVAILABLE?1:0));
 assert(sharedControl[activeControl].steps[0]>0);
 for(int i=0;i<10000;i++)audioISR();
 assert(audioTicks==10000);
 for(uint16_t word:SPI.last)assert((word&0xFFF)<=4095);
 updateMode(MODE_VCO);
 assert(!cvCalibrated());
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
 updateScene(4);
 assert(scene==4);
 updateControls();
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

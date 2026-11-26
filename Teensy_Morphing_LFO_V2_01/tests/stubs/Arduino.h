#pragma once
#include <stdint.h>
#include <stddef.h>
#include <cmath>
#include <string>
#include <algorithm>
#define __IMXRT1062__ 1
#define PROGMEM
#define OUTPUT 1
#define INPUT_PULLUP 2
#define LOW 0
#define HIGH 1
#define RISING 3
#define A1 15
#define A2 16
#define A3 17
#define A4 18
#define A5 19
#define A6 20
#define A7 21
#define A8 22
#define A9 23
#define A10 24
#define MSBFIRST 1
#define SPI_MODE0 0
inline uint32_t gMillis = 0;
inline unsigned long millis(){return gMillis;}
inline uint16_t gAnalog[32] = {};
inline int analogRead(uint8_t pin){return gAnalog[pin];}
inline void analogReadResolution(int){}
inline void pinMode(uint8_t,int){}
inline bool digitalRead(uint8_t){return true;}
inline void digitalWrite(uint8_t,int){}
inline void digitalWriteFast(uint8_t,int){}
inline void attachInterrupt(uint8_t,void (*)(),int){}
inline uint8_t digitalPinToInterrupt(uint8_t p){return p;}
inline void noInterrupts(){}
inline void interrupts(){}
inline void delay(int){}
template<typename T> T constrain(T x,T l,T h){return std::min(h,std::max(l,x));}
class SerialClass {
public:
 void begin(int){}
 void print(const char*){} void print(int){} void print(unsigned int){} void print(float,int=2){} void print(long){}
 void println(const char*){} void println(int){} void println(unsigned int){} void println(float,int=2){}
 int available(){return 0;} int read(){return -1;}
};
extern SerialClass Serial;
class IntervalTimer {
public:
 bool begin(void (*f)(),float){callback=f;return true;}
 void priority(int){}
 void (*callback)() = nullptr;
};

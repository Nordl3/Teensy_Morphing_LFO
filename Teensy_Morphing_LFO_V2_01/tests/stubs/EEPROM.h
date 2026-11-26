#pragma once
#include <cstring>
struct EEPROMClass {
 char b[256]={};
 template<typename T> void put(int,T const&v){std::memcpy(b,&v,sizeof(T));}
 template<typename T> void get(int,T &v){std::memcpy(&v,b,sizeof(T));}
};
extern EEPROMClass EEPROM;

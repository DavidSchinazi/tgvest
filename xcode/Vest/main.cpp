//
//  main.cpp
//  Vest
//
//  Created by David Schinazi on 4/23/17.
//  Copyright © 2017 DiscoFish. All rights reserved.
//

// otherwise FastLED headers do not compile
//#define USE_GET_MILLISECOND_TIMER
//#define __COMPILING_AVR_LIBC__
//#define __STDC_LIMIT_MACROS



#pragma clang diagnostic ignored "-W#warnings"


// problems in delay_basic.h
//#define _UTIL_DELAY_BASIC_H_
//#include <inttypes.h>
//void _delay_loop_1(uint8_t __count);
//void _delay_loop_2(uint16_t __count);

// problems in csdtlib.h
//#include <bits/c++config.h>
//#undef _GLIBCXX_HAVE_MBSTATE_T
//#undef _GLIBCXX_USE_WCHAR_T
//#define _GLIBCXX_USE_C99_LONG_LONG_DYNAMIC 1
//#define _GLIBCXX_USE_C99_DYNAMIC 1

//#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Wdocumentation"
//#pragma clang diagnostic ignored "-Wunknown-attributes"
//#pragma clang diagnostic ignored "-Wnullability-completeness"


#include <Arduino.h>

//#pragma clang diagnostic pop

#include <osapi.h>
#define F_CPU 80000000UL

//#include <wchar.h>

// for clockless_esp8266.h
#define SKIP_ARDUINO_ASM 1

//typedef uint8_t uint8;
//typedef uint32_t uint32;

#include "../../examples/Vest/Vest.ino"

int main(int argc, const char * argv[]) {

  setup();
  while (true) {
    loop();
  }
  return 0;
}

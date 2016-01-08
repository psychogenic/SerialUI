/*
 * SUIStreamDigi.cpp
 *
 *  Created on: Jan 7, 2016
 *      Author: Pat Deegan
 *      Part of the SerialUI Project
 *      Copyright (C) 2015 Pat Deegan, http://psychogenic.com
 */

#include "includes/SUIStream.h"

#ifdef SUI_PLATFORM_DIGISPARKUSB

namespace SUI {



/*
 * class SUIStream -- a streaming class based on Arduino Stream.
 *
 * In this case, we need to augment the default stream with some
 * additional methods.
 *
 * For the moment, these are left as non-functional stubs.
 */

// size_t readBytesUntil( char terminator, char *buffer, size_t length) { return 0;}
long SUIStream::parseInt(char skipChar=1) { return -1; }


unsigned long SUIStream::timeout() { return timeout_ms; }
void SUIStream::setTimeout(unsigned long timeout) { timeout_ms = timeout ; }

size_t SUIStream::write(uint8_t i) { return DigiUSB.write(i);  }



} /* namespace SUI */

#endif /* SUI_PLATFORM_DIGISPARKUSB */


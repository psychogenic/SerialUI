/*
 * SovaStreamDigi.cpp
 * Copyright (C) 2016-2017 Pat Deegan, psychogenic.com.
 *
 * Part of SovA, released as a supporting component of SerialUI.
 * http://www.flyingcarsandstuff.com/projects/SerialUI
 *
 *
 * This program library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See file LICENSE.txt for further informations on licensing
 * terms.
 *
 */


#include "includes/sova/SovAPlatform.h"
#ifdef SOVA_PLATFORM_DIGISPARKUSB

#include "includes/sova/stream/SovAStream.h"
namespace SovA {



/*
 * class SovA::Stream -- a streaming class based on Arduino Stream.
 *
 * In this case, we need to augment the default stream with some
 * additional methods.
 *
 * For the moment, these are left as non-functional stubs.
 */

// size_t readBytesUntil( char terminator, char *buffer, size_t length) { return 0;}
long SovA::Stream::parseInt(char skipChar=1) { return -1; }


unsigned long SovA::Stream::timeout() { return timeout_ms; }
void SovA::Stream::setTimeout(unsigned long timeout) { timeout_ms = timeout ; }

size_t SovA::Stream::write(uint8_t i) { return DigiUSB.write(i);  }



} /* namespace SovA */

#endif /* SOVA_PLATFORM_DIGISPARKUSB */


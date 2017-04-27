/*
 *
 * SOVAPlat_DigiUSB.h -- SovA system platform-specifics.
 * Copyright (C) 2013-2017 Pat Deegan, psychogenic.com. All rights reserved.
 *
 *
 * http://www.flyingcarsandstuff.com/projects/SovA
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
 *
 * ************************* OVERVIEW *************************
 *
 * Here we'll store all the platform-specific includes and structures.
 * This is a version to be used with the Digistump Digispark and its
 * DigiUSB serial line.
 *
 * NOTE: It is currently non-functional.  A trivial examples compiles
 * to just a few bytes too large, even with all the
 * compile-time flags set using SOVA_BUILD_FOR_DIGISPARK (in SovAConfig.h)
 * and we don't have enough room for
 *
 */


#ifndef SOVAPlat_DigiUSB_h

#define SOVAPlat_DigiUSB_h

#include "../SovAConfig.h"

#ifdef SOVA_PLATFORM_DIGISPARKUSB


// system/avr includes
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <inttypes.h>
#include "Arduino.h"
#include <DigiUSB.h>




// SOVA is the namespace in which we keep all our goodies.
namespace SovA {


// need a custom base implementation of the SovA::Stream
#define SOVA_BASEIMPLEMENTATION_CUSTOM_DIGI
//typedef Print SovAStreamBaseType;

inline size_t _ard_float2str_and_len(double fl, char * intoptr) {
		return strlen(dtostrf(fl, 5, 2, intoptr));
	}

/*
 * StreamDelegate -- wraps all the uses of Serial in a single place,
 * to ease implementation of different types of comm.
 * All you need are the following 7 methods to behave like Serial (HardwareSerial.h)
 * methods and the rest should just work.
 */

class StreamDelegate {

public:

	static int peeked;

	static inline int available() { return DigiUSB.available();}
    static int read() ;
    static int peek();
    static void flush() ;
    static inline size_t write(uint8_t i) { return DigiUSB.write(i); }
    static inline  void begin(unsigned long speed) { DigiUSB.begin(); }


};



} /* end namespace SovA */


#endif /* SOVA_PLATFORM_DIGISPARKUSB */
#endif /* SOVAPlat_DigiUSB_h */

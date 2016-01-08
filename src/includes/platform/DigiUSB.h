/*
 *
 * SUIPlat_DigiUSB.h -- SerialUI system platform-specifics.
 * Copyright (C) 2013 Pat Deegan.  All rights reserved.
 *
 *
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
 *
 * ************************* OVERVIEW *************************
 *
 * Here we'll store all the platform-specific includes and structures.
 * This is a version to be used with the Digistump Digispark and its
 * DigiUSB serial line.
 *
 * NOTE: It is currently non-functional.  A trivial examples compiles
 * to just a few bytes too large, even with all the
 * compile-time flags set using SUI_BUILD_FOR_DIGISPARK (in SUIConfig.h)
 * and we don't have enough room for
 *
 */


#ifndef SUIPlat_DigiUSB_h
#define SUIPlat_DigiUSB_h

#include "SUIConfig.h"

#ifdef SUI_PLATFORM_DIGISPARKUSB


// system/avr includes
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <inttypes.h>
#include "Arduino.h"
#include <DigiUSB.h>




// SUI is the namespace in which we keep all our goodies.
namespace SUI {


// need a custom base implementation of the SUIStream
#define SUI_BASEIMPLEMENTATION_CUSTOM_DIGI
typedef Print SerialUIStreamBaseType;

inline size_t _ard_float2str_and_len(double fl, char * intoptr) {
		return strlen(dtostrf(fl, 5, 2, intoptr));
	}

/*
 * StreamImplementation -- wraps all the uses of Serial in a single place,
 * to ease implementation of different types of comm.
 * All you need are the following 7 methods to behave like Serial (HardwareSerial.h)
 * methods and the rest should just work.
 */

class StreamImplementation {

public:

	static int peeked;

	static inline int available() { return DigiUSB.available();}
    static int read() ;
    static int peek();
    static void flush() ;
    static inline size_t write(uint8_t i) { return DigiUSB.write(i); }
    static inline  void begin(unsigned long speed) { DigiUSB.begin(); }


};



} /* end namespace SUI */


#endif /* SUI_PLATFORM_DIGISPARKUSB */
#endif /* SUIPlat_DigiUSB_h */

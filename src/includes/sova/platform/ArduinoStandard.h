/*
 * ArduinoStandard.h
 *
 *  Created on: Jan 10, 2016
 *      Author: Pat Deegan
 *      Part of SovA, supporting the SerialUI library
 *      Copyright (C) 2016-2017 Pat Deegan, http://psychogenic.com
 *
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

#ifndef SOVA_SRC_INCLUDES_PLATFORM_ARDUINOSTANDARD_H_

#define SOVA_SRC_INCLUDES_PLATFORM_ARDUINOSTANDARD_H_



/*
 * ArduinoStandard... this is boiler plate that is used
 * by a number of relatively standard Arduino/arduino-like
 * systems.
 */
#include "../SovAConfig.h"

#ifdef SOVA_BUILDFOR_ARDUINO_STANDARD
#include "../stream/delegate/DelegateDirect.h"
#include "../stream/SovAStream.h"

#ifndef PLATFORM_NOW_MILLIS
#define PLATFORM_NOW_MILLIS()		millis()
#endif


#ifdef PLATFORM_DESKTOP

#define dtostrf(fl, wdth, prec, ptr)		sprintf(ptr, "%.2f", fl)

#define SOVA_PLATFORM_SUPPORTS_PROGMEM

#else

#ifdef SOVA_ARCH_SAM


// For Due, it seems all the *_P() functions
// are simply defined as their regular equivalents, but
// there's no strncpy_P and strncmp_P defined, so we
// create aliases in that case:
#ifndef strncpy_P
#define strncpy_P(to, from, s)		strncpy(to, from, s)
#endif

#ifndef strncmp_P
#define strncmp_P(a, b, s)			strncmp(a,b,s)
#endif

#define SOVA_CONVERT_FLOAT_TO_STRING_AND_RETLEN(fl, intoptr)	sprintf(intoptr, "%.2f", fl);

#else
#define SOVA_CONVERT_FLOAT_TO_STRING_AND_RETLEN(fl, intoptr) SovA::_ard_float2str_and_len(fl, intoptr)
#endif


#endif



// SOVA is the namespace in which we keep all our goodies.
namespace SovA {

#ifndef SOVA_ARCH_SAM
inline size_t _ard_float2str_and_len(double fl, char * intoptr) {
		return strlen(dtostrf(fl, 5, 2, intoptr));
	}
#endif


// used the standard base implementation of the SovA::Stream
#define SOVA_BASEIMPLEMENTATION_STANDARD


typedef ::Stream SovAStandardSysStreamType;
typedef SovA::Delegate::Direct<SovAStandardSysStreamType> StreamDelegate;





} /* end namespace SovA */

#endif /* SOVA_BUILDFOR_ARDUINO_STANDARD */




#endif /* SOVA_SRC_INCLUDES_PLATFORM_ARDUINOSTANDARD_H_ */

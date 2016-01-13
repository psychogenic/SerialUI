/*
 * ArduinoStandard.h
 *
 *  Created on: Jan 10, 2016
 *      Author: Pat Deegan
 *      Part of the SerialUI Project
 *      Copyright (C) 2015 Pat Deegan, http://psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_PLATFORM_ARDUINOSTANDARD_H_
#define SERIALUI_SRC_INCLUDES_PLATFORM_ARDUINOSTANDARD_H_



/*
 * ArduinoStandard... this is boiler plate that is used
 * by a number of relatively standard Arduino/arduino-like
 * systems.
 */
#include "../SUIConfig.h"

#ifdef SUI_BUILDFOR_ARDUINO_STANDARD
#include "../stream/delegate/DelegateDirect.h"

#ifndef PLATFORM_NOW_MILLIS
#define PLATFORM_NOW_MILLIS()		millis()
#endif


#ifdef PLATFORM_DESKTOP

#define dtostrf(fl, wdth, prec, ptr)		sprintf(ptr, "%.2f", fl)

#define SUI_PLATFORM_SUPPORTS_PROGMEM

#else

#ifdef SUI_ARCH_SAM


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

#define SUI_CONVERT_FLOAT_TO_STRING_AND_RETLEN(fl, intoptr)	sprintf(intoptr, "%.2f", fl);

#else
#define SUI_CONVERT_FLOAT_TO_STRING_AND_RETLEN(fl, intoptr) SUI::_ard_float2str_and_len(fl, intoptr)
#endif


#endif


// SUI is the namespace in which we keep all our goodies.
namespace SUI {

#ifndef SUI_ARCH_SAM
inline size_t _ard_float2str_and_len(double fl, char * intoptr) {
		return strlen(dtostrf(fl, 5, 2, intoptr));
	}
#endif

typedef Stream SerialUIUnderlyingStreamType;


// used the standard base implementation of the SUIStream
#define SUI_BASEIMPLEMENTATION_STANDARD


typedef SUI::Delegate::Direct<SerialUIUnderlyingStreamType> StreamDelegate;





} /* end namespace SUI */

#endif /* SUI_BUILDFOR_ARDUINO_STANDARD */




#endif /* SERIALUI_SRC_INCLUDES_PLATFORM_ARDUINOSTANDARD_H_ */

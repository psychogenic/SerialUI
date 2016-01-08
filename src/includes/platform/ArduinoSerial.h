/*
 *
 * SUIPlat_ArduinoSerial.h -- SerialUI system platform-specifics.
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
 *
 */

#ifndef SERIALUI_SRC_INCLUDES_SUIPLAT_ARDUINOSERIAL_H_
#define SERIALUI_SRC_INCLUDES_SUIPLAT_ARDUINOSERIAL_H_

#include "../SUIConfig.h"


#ifdef SUI_PLATFORM_ARDUINOSTANDARD


// system/avr includes
#include <inttypes.h>
#ifdef PLATFORM_DESKTOP

#define dtostrf(fl, wdth, prec, ptr)		sprintf(ptr, "%.2f", fl)

#else

#ifdef SUI_BUILD_FOR_DUE


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

#include <avr/pgmspace.h>
#include "Arduino.h"


#endif


// SUI is the namespace in which we keep all our goodies.
namespace SUI {

#ifndef SUI_PLATFORM_ARDUINO_DUE
inline size_t _ard_float2str_and_len(double fl, char * intoptr) {
		return strlen(dtostrf(fl, 5, 2, intoptr));
	}
#endif

typedef Stream SerialUIStreamBaseType;

// used the standard base implementation of the SUIStream
#define SUI_BASEIMPLEMENTATION_STANDARD
#define SUI_SUISTREAM_NEEDSVIRTUAL

typedef Stream StreamInstanceType;


/*
 * StreamImplementation -- wraps all the uses of Serial in a single place,
 * to ease implementation of different types of comm.
 * All you need are the following 7 methods to behave like Serial (HardwareSerial.h)
 * methods and the rest should just work.
 */
class StreamImplementation {

public:
	static void setStream(StreamInstanceType * strm);
	static inline int available() {  return stream_to_use->available();}
    static inline int read() { return stream_to_use->read(); }
    static inline int peek() { return stream_to_use->peek(); }
    static inline void flush() { stream_to_use->flush(); }
    static inline size_t write(uint8_t i) { return stream_to_use->write(i); }
    static void begin(unsigned long speed) ;

private:
    static StreamInstanceType * stream_to_use;
    static bool stream_to_use_override;


};



} /* end namespace SUI */

#endif /* SUI_PLATFORM_ARDUINOSTANDARD */

#endif /* SERIALUI_SRC_INCLUDES_SUIPLAT_ARDUINOSERIAL_H_ */

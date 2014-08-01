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

#ifndef SUIPlat_ArduinoSerial_h
#define SUIPlat_ArduinoSerial_h


// system/avr includes
#include <inttypes.h>
#ifndef PLATFORM_DESKTOP

#include <avr/pgmspace.h>
#include "Arduino.h"

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

#endif


// SUI is the namespace in which we keep all our goodies.
namespace SUI {

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

/*
 * class SUIStream -- a streaming class based on Arduino Stream.
 *
 * In this case, there's nothing to do--we can use Stream as-is so
 * we just derive a class from Stream and leave it empty.
 */


#define SUI_SUISTREAM_NEEDSVIRTUAL
class SUIStream : public Stream
{
public:
	// nothing to do
	virtual ~SUIStream() {}

	unsigned long timeout();
	void setTimeout(unsigned long timeout);

	// parseULong -- gives us access to large valued ints (unsigned)
	unsigned long parseULong(char skipChar);
	unsigned long parseULong();

	// parseInHex -- allows us to parse hex ints like 3e and F3D9
	unsigned long parseIntHex();
	unsigned long parseIntHex(char skipChar);

private:
	int timedPeek();

	int peekNextDigit(bool includeHex=false);
	unsigned long timeout_ms;
};

} /* end namespace SUI */


#endif /* SUIPlat_ArduinoSerial_h */

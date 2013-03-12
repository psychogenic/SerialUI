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
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <inttypes.h>

#include "Arduino.h"


// SUI is the namespace in which we keep all our goodies.
namespace SUI {

/*
 * StreamImplementation -- wraps all the uses of Serial in a single place,
 * to ease implementation of different types of comm.
 * All you need are the following 7 methods to behave like Serial (HardwareSerial.h)
 * methods and the rest should just work.
 */
class StreamImplementation {

public:

	static inline int available() { return Serial.available();}
    static inline int read() { return Serial.read(); }
    static inline int peek() { return Serial.peek(); }
    static inline void flush() { Serial.flush(); }
    static inline size_t write(uint8_t i) { return Serial.write(i); }
    static inline  void begin(unsigned long speed) {
    	Serial.begin(speed);
    	// we also set a sane timeout for reads, as this was causing issues
    	// assuming code will call begin() prior to setTimeout() (if used)
    	Serial.setTimeout(SUI_SERIALUI_READBYTES_TIMEOUT_DEFAULT_MS);
    }


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

	unsigned long timeout() { return timeout_ms; }
	void setTimeout(unsigned long timeout) { timeout_ms = timeout ; Stream::setTimeout(timeout); }


private:
	unsigned long timeout_ms;
};

} /* end namespace SUI */


#endif /* SUIPlat_ArduinoSerial_h */

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


// system/avr includes
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <inttypes.h>
#include "Arduino.h"
#include <DigiUSB.h>




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

	static int peeked;

	static inline int available() { return DigiUSB.available();}
    static int read() ;
    static int peek();
    static void flush() ;
    static inline size_t write(uint8_t i) { return DigiUSB.write(i); }
    static inline  void begin(unsigned long speed) { DigiUSB.begin(); }


};


/*
 * class SUIStream -- a streaming class based on Arduino Stream.
 *
 * In this case, we need to augment the default stream with some
 * additional methods.
 *
 * For the moment, these are left as non-functional stubs.
 */

#undef SUI_SUISTREAM_NEEDSVIRTUAL
class SUIStream : public Print
{
public:

	// virtual ~SUIStream() {}

	// Non-working, required methods...  (stubs)
	// size_t readBytesUntil( char terminator, char *buffer, size_t length) { return 0;}
	long parseInt(char skipChar=1) { return -1; }


	unsigned long timeout() { return _timeout; }
	void setTimeout(unsigned long timeout) { _timeout = timeout ; }

	/*
	int timedRead() {
		// straight from Arduino Stream, LGPLed
		// Copyright (c) 2010 David A. Mellis.
		int c;
		_startMillis = millis();
		do {
			c = StreamImplementation::read();
			if (c >= 0)
				return c;
		} while (millis() - _startMillis < _timeout);
		return -1; // -1 indicates timeout
	}
	*/




	virtual size_t write(uint8_t i) { return DigiUSB.write(i);  }

private:
	unsigned long _startMillis;
	unsigned long _timeout;

	/* DEADBEEF (tried to save space by avoiding deriving from Print
	 * and implementing these manually... even avoiding the String versions,
	 * WString and a bunch of other stuff gets included by DigiUSB so we don't
	 * save any space even by doing this:

		void printNumber(unsigned long a, uint8_t n){ DigiUSB.println(a, n); }
	    void printFloat(double a, uint8_t n){ DigiUSB.println(a, n); }
	    void print(const char str[]){ DigiUSB.print(str); }
	    void print(char a, int i = BYTE){ DigiUSB.print(a, i); }
	    void print(unsigned char a, int i= BYTE){ DigiUSB.print(a, i); }
	    void print(int a, int i= DEC){ DigiUSB.print(a, i); }
	    void print(unsigned int a, int i= DEC){ DigiUSB.print(a, i); }
	    void print(long a, int i= DEC){ DigiUSB.print(a, i); }
	    void print(unsigned long a, int i= DEC){ DigiUSB.print(a, i); }
	    void print(double a, int i = 2){ DigiUSB.print(a, i); }
	    void println(const char str[]){ DigiUSB.println(str); }
	    void println(char a, int i= BYTE){ DigiUSB.println(a, i); }
	    void println(unsigned char a, int i= BYTE){ DigiUSB.println(a, i); }
	    void println(int a, int i= DEC){ DigiUSB.println(a, i); }
	    void println(unsigned int a, int i= DEC){ DigiUSB.println(a, i); }
	    void println(long a, int i= DEC){ DigiUSB.println(a, i); }
	    void println(unsigned long a, int i= DEC){ DigiUSB.println(a, i); }
	    void println(double a, int i= 2){ DigiUSB.println(a, i); }

	*/
};

} /* end namespace SUI */


#endif /* SUIPlat_DigiUSB_h */

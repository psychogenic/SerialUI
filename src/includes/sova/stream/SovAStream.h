/*
 * SovAStream.h
 *
 *  Created on: Jan 7, 2016
 *      Author: Pat Deegan
 *      Part of SovA, supporting the SerialUI library
 *      Copyright (C) 2016-2017 Pat Deegan, http://psychogenic.com
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

#ifndef SOVA_SRC_INCLUDES_SOVASTREAM_H_

#define SOVA_SRC_INCLUDES_SOVASTREAM_H_

#include "../SovAPlatform.h"
#include "delegate/Delegate.h"


namespace SovA {

typedef struct Uint8BufferStruct {
	uint8_t * buf;
	size_t len;
	Uint8BufferStruct(uint8_t * b, size_t l) : buf(b), len(l) {};
	Uint8BufferStruct() : buf(NULL), len(0){}



} Uint8Buffer;

class Sink; // forward decl

class Source {
public:
	virtual ~Source() {}
    virtual int available() = 0;
    virtual int read() = 0;
    virtual int peek() = 0;
	virtual size_t readBytes( uint8_t *buffer, size_t length) = 0;


	virtual long parseInt() = 0;
	virtual float parseFloat() = 0;
	// parseULong -- gives us access to large valued ints (unsigned)
	// inline unsigned long parseULong(char skipChar);
	virtual unsigned long parseULong() = 0;


	Source & operator>>(Sink &);
	Source & operator>>(unsigned long & v) {
		v = this->parseULong();
		return *this;
	}



	Source & operator>>(long & v) {
		v = this->parseInt();
		return *this;
	}

	Source & operator>>(float & v) {
		v = this->parseFloat();
		return *this;
	}




};

#define SINKOP_IMPLEMENT(type)		\
	inline Sink & operator<<(type v) { \
	this->print(v); return *this; \
}

class Sink : public Print {
public:
	static const char * endl; // = "\r\n";
	virtual ~Sink() {}

	virtual size_t write(uint8_t i)  = 0;
    virtual size_t write(const uint8_t *buffer, size_t size) = 0;


    Sink & operator<<(Source & src);
    Sink & operator<<(const Uint8Buffer &);

    SINKOP_IMPLEMENT(const String &);
    SINKOP_IMPLEMENT(char);
    SINKOP_IMPLEMENT(const char *);
    SINKOP_IMPLEMENT(int);
    SINKOP_IMPLEMENT(long int);
    SINKOP_IMPLEMENT(unsigned char);
    SINKOP_IMPLEMENT(unsigned int);
    SINKOP_IMPLEMENT(unsigned long int);
    SINKOP_IMPLEMENT(float);
    // SINKOP_IMPLEMENT(SOVA_FLASHSTRING_PTR);



};

/*
 * Just a stream wrapper, passing everything to the delegate, plus
 * a Print interface.
 */
class Stream : public Source, public Sink
{
public:
	Stream();

	Stream(Delegate::Interface * useDelegate) ;
	virtual ~Stream();

	virtual operator bool() { return true;} // always "ready"
    virtual int available() { return delegate()->available();}
    virtual int read() { return delegate()->read(); }
    virtual int peek() { return delegate()->peek(); }
    inline void flush() { delegate()->flush(); }

    inline void begin(unsigned long speed) { delegate()->begin(speed); }
	inline unsigned long timeout() { return delegate()->timeout();}
	inline void setTimeout(unsigned long tmout) { delegate()->setTimeout(tmout);}

	virtual long parseInt() { return delegate()->parseInt();}
	virtual float parseFloat() { return delegate()->parseFloat();}

	// parseULong -- gives us access to large valued ints (unsigned)
	// inline unsigned long parseULong(char skipChar);
	virtual unsigned long parseULong() { return delegate()->parseULong();}

	// parseInHex -- allows us to parse hex ints like 3e and F3D9
	inline unsigned long parseIntHex() { return delegate()->parseIntHex(); }
	// unsigned long parseIntHex(char skipChar);

	inline size_t readBytes( char *buffer, size_t length) { return delegate()->readBytes(buffer, length); }
	virtual size_t readBytes( uint8_t *buffer, size_t length) { return readBytes((char *)buffer, length); }

	inline size_t readBytesUntil( char terminator, char *buffer, size_t length) { return delegate()->readBytesUntil(terminator, buffer, length);}
	inline size_t readBytesUntil( char terminator, uint8_t *buffer, size_t length) { return readBytesUntil(terminator, (char *)buffer, length); }





    bool find(char *target) { return delegate()->find(target) ;}
    inline bool find(uint8_t *target) { return find ((char *)target); }
    // returns true if target string is found, false if timed out (see setTimeout)

    bool find(char *target, size_t length) { return delegate()->find(target, length) ;}
    inline bool find(uint8_t *target, size_t length) { return find ((char *)target, length); }

    inline bool find(char target) { return find (&target, 1); }

    bool findUntil(char *target, char *terminator) { return delegate()->findUntil(target, terminator) ;}
    inline  bool findUntil(uint8_t *target, char *terminator) { return findUntil((char *)target, terminator); }

    bool findUntil(char *target, size_t targetLen, char *terminate, size_t termLen) { return delegate()->findUntil(target, targetLen, terminate, termLen) ;}
    inline bool findUntil(uint8_t *target, size_t targetLen, char *terminate, size_t termLen) {return findUntil((char *)target, targetLen, terminate, termLen); }


    // Arduino String functions to be added here
    inline String readString() { return delegate()->readString();}
    String readStringUntil(char terminator) { return delegate()->readStringUntil(terminator);}


#ifdef DEADBEEF
#ifdef SOVA_PROGMEM_PTR

    /* Stream interface extensions for progmem strings */
	/*
	 * print_P(PGM_STRING)
	 * Same as Serial.print() but for progmem strings.
	 */
	DEPRECATED size_t print_P(SOVA_PROGMEM_PTR msg);
	DEPRECATED size_t print_P(SOVA_FLASHSTRING_PTR msg) { return this->print(msg);}

	/*
	 * println_P(PGM_STRING)
	 * Same as Serial.println() but for progmem strings.
	 *
	 */
	DEPRECATED size_t println_P(SOVA_PROGMEM_PTR msg);
	DEPRECATED size_t println_P(SOVA_FLASHSTRING_PTR msg) { return this->println(msg);}

#endif
#endif





	virtual size_t write(uint8_t i) { return delegate()->write(i);}
    virtual size_t write(const uint8_t *buffer, size_t size) { return delegate()->write(buffer, size); }
	// virtual long parseInt(char skipChar=1);


    inline size_t write(char * buf, size_t len) { return this->write((const uint8_t *)buf, len); }
    inline size_t write(const char * buf, size_t len) { return this->write((const uint8_t *)buf, len); }

	inline void useDelegate(Delegate::Interface * d) { streamDelegate = d;}
	inline Delegate::Interface * delegate() { return streamDelegate;}

private:

	Delegate::Interface * streamDelegate;
	Delegate::Interface * dynDefaultDelegate;


};


}



#endif /* SOVA_SRC_INCLUDES_SOVASTREAM_H_ */

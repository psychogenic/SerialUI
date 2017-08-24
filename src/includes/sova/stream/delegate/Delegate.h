/*
 * Delegate.h
 *
 *  Created on: Jan 9, 2016
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

#ifndef SOVA_SRC_INCLUDES_STREAM_DELEGATE_H_

#define SOVA_SRC_INCLUDES_STREAM_DELEGATE_H_

#include "../../SovAExtIncludes.h"
#include "../../SovAConfig.h"

namespace SovA {
namespace Delegate {


/*
 * Interface -- interface common to al delegates
 */
typedef void(*DelegatePollCallback)(void);

class Interface {
public:
	Interface() : timeout_ms(SOVA_DEFAULT_TIMEOUT_VALUE_MS), poll_cb(NULL) {}

	virtual ~Interface() {}
    virtual void begin(unsigned long speed) = 0;


	virtual int available() = 0;
    virtual int read() = 0;
    virtual int peek() = 0;
    virtual void flush() = 0;
    virtual size_t write(const uint8_t *buffer, size_t size) = 0;
    virtual size_t write(uint8_t i) = 0;

    inline void setPollCallback(DelegatePollCallback setTo) { poll_cb = setTo;}
    inline void poll() { if (poll_cb) { poll_cb(); }};

    virtual float parseFloat() = 0;
    virtual long parseInt() = 0;

    virtual void tick(bool doFlush=false) = 0;


    size_t readBytes(char *buffer, size_t length);
    size_t readBytesUntil( char terminator, char *buffer, size_t length); // as readBytes with terminator character


    bool find(char *target);   // reads data from the stream until the target string is found
    inline bool find(uint8_t *target) { return find ((char *)target); }
    // returns true if target string is found, false if timed out (see setTimeout)

    bool find(char *target, size_t length);   // reads data from the stream until the target string of given length is found
    inline bool find(uint8_t *target, size_t length) { return find ((char *)target, length); }
    // returns true if target string is found, false if timed out

    inline bool find(char target) { return find (&target, 1); }

    bool findUntil(char *target, char *terminator);   // as find but search ends if the terminator string is found
    inline  bool findUntil(uint8_t *target, char *terminator) { return findUntil((char *)target, terminator); }

    bool findUntil(char *target, size_t targetLen, char *terminate, size_t termLen);   // as above but search ends if the terminate string is found
    inline bool findUntil(uint8_t *target, size_t targetLen, char *terminate, size_t termLen) {return findUntil((char *)target, targetLen, terminate, termLen); }


    // parseULong -- gives us access to large valued ints (unsigned)

	unsigned long parseULong();
	// parseInHex -- allows us to parse hex ints like 3e and F3D9
	unsigned long parseIntHex();

	String readString();
	String readStringUntil(char terminator);









	inline unsigned long timeout() { return timeout_ms;}
	virtual void setTimeout(unsigned long tms) { timeout_ms = tms;}

protected:
    unsigned long parseULong(char skipChar);
	unsigned long parseIntHex(char skipChar);

	int timedPeek();
	int timedRead();
	int peekNextDigit(bool includeHex=false);


	struct MultiTarget {
	    const char *str;  // string you're searching for
	    size_t len;       // length of string you're searching for
	    size_t index;     // index used by the search routine.
	  };

	  // This allows you to search for an arbitrary number of strings.
	  // Returns index of the target that is found first or -1 if timeout occurs.
	 int findMulti(struct MultiTarget *targets, int tCount);

private:
	unsigned long timeout_ms;
	DelegatePollCallback poll_cb;

};

class InterfaceParsingImpl : public Interface {

public:
	virtual ~InterfaceParsingImpl() {}

    virtual float parseFloat();
    virtual long parseInt();

private:
    long parseInt(char skipChar);
    float parseFloat(char skipChar);


};


template<typename STREAMTYPE>
class StreamContainer {

public:
	StreamContainer() : stream_to_use(NULL), stream_to_use_override(false){}
	StreamContainer(STREAMTYPE * defaultStream) : stream_to_use(defaultStream), stream_to_use_override(false){}
	virtual ~StreamContainer() {}

	virtual void setStream(STREAMTYPE * strm) {
		stream_to_use = strm;
		stream_to_use_override = true;
	}

protected:
	inline STREAMTYPE * streamToUse() { return stream_to_use;}
	inline bool streamToUseOverridden() { return stream_to_use_override;}

private:

    STREAMTYPE * stream_to_use;
    bool stream_to_use_override;

};


/*
 * SovA::Delegate::Delegate -- base class for standard delegates
 */
template<typename STREAMTYPE>
class Delegate : public StreamContainer<STREAMTYPE>, public Interface {
public:

	Delegate() {}
	Delegate(STREAMTYPE * defStream) : StreamContainer<STREAMTYPE>(defStream) {}

	virtual ~Delegate() {}

};



} /* namespace Delegate */
}/* namespace SovA */



#endif /* SOVA_SRC_INCLUDES_STREAM_DELEGATE_H_ */

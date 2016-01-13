/*
 * SUIStream.h
 *
 *  Created on: Jan 7, 2016
 *      Author: Pat Deegan
 *      Part of the SerialUI Project
 *      Copyright (C) 2015 Pat Deegan, http://psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_SUISTREAM_H_
#define SERIALUI_SRC_INCLUDES_SUISTREAM_H_

#include "../SUIPlatform.h"
#include "delegate/Delegate.h"


namespace SUI {

/*
 * Just a stream wrapper, passing everything to the delegate, plus
 * a Print interface.
 */
class SUIStream : public Print
{
public:
	SUIStream(Delegate::Interface * useDelegate) : streamDelegate(useDelegate) {}
	virtual ~SUIStream() {}


    inline int available() { return delegate()->available();}
    inline int read() { return delegate()->read(); }
    inline int peek() { return delegate()->peek(); }
    inline void flush() { delegate()->flush(); }

    inline void begin(unsigned long speed) { delegate()->begin(speed); }
	inline unsigned long timeout() { return delegate()->timeout();}
	inline void setTimeout(unsigned long tmout) { delegate()->setTimeout(tmout);}

	inline long parseInt() { return delegate()->parseInt();}
	inline float parseFloat() { return delegate()->parseFloat();}

	// parseULong -- gives us access to large valued ints (unsigned)
	// inline unsigned long parseULong(char skipChar);
	inline unsigned long parseULong() { return delegate()->parseULong();}

	// parseInHex -- allows us to parse hex ints like 3e and F3D9
	inline unsigned long parseIntHex() { return delegate()->parseIntHex(); }
	// unsigned long parseIntHex(char skipChar);

	inline size_t readBytes( char *buffer, size_t length) { return delegate()->readBytes(buffer, length); }
	inline size_t readBytes( uint8_t *buffer, size_t length) { return readBytes((char *)buffer, length); }

	inline size_t readBytesUntil( char terminator, char *buffer, size_t length) { return delegate()->readBytesUntil(terminator, buffer, length);}
	inline size_t readBytesUntil( char terminator, uint8_t *buffer, size_t length) { return readBytesUntil(terminator, (char *)buffer, length); }


#ifdef SUI_PROGMEM_PTR

    /* Stream interface extensions for progmem strings */
	/*
	 * print_P(PGM_STRING)
	 * Same as Serial.print() but for progmem strings.
	 */
	DEPRECATED size_t print_P(SUI_PROGMEM_PTR msg);
	DEPRECATED size_t print_P(SUI_FLASHSTRING_PTR msg) { return this->print(msg);}

	/*
	 * println_P(PGM_STRING)
	 * Same as Serial.println() but for progmem strings.
	 *
	 */
	DEPRECATED size_t println_P(SUI_PROGMEM_PTR msg);
	DEPRECATED size_t println_P(SUI_FLASHSTRING_PTR msg) { return this->println(msg);}

#endif





	virtual size_t write(uint8_t i) { return delegate()->write(i);}
    virtual size_t write(const uint8_t *buffer, size_t size) { return delegate()->write(buffer, size); }
	// virtual long parseInt(char skipChar=1);

	inline void useDelegate(Delegate::Interface * d) { streamDelegate = d;}
	inline Delegate::Interface * delegate() { return streamDelegate;}



protected:


private:
	/*
	int timedPeek();

	int peekNextDigit(bool includeHex=false);
	unsigned long timeout_ms;

	*/
	Delegate::Interface * streamDelegate;


};


}



#endif /* SERIALUI_SRC_INCLUDES_SUISTREAM_H_ */

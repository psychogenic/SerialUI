/*
 * Delegate.h
 *
 *  Created on: Jan 9, 2016
 *      Author: Pat Deegan
 *      Part of the SerialUI Project
 *      Copyright (C) 2015 Pat Deegan, http://psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_STREAM_DELEGATE_H_
#define SERIALUI_SRC_INCLUDES_STREAM_DELEGATE_H_

#include "../../SUIExtIncludes.h"


namespace SUI {
namespace Delegate {


/*
 * Interface -- interface common to al delegates
 */
class Interface {
public:
	Interface() : timeout_ms(0) {}

	virtual ~Interface() {}
    virtual void begin(unsigned long speed) = 0;


	virtual int available() = 0;
    virtual int read() = 0;
    virtual int peek() = 0;
    virtual void flush() = 0;
    virtual size_t write(const uint8_t *buffer, size_t size) = 0;
    virtual size_t write(uint8_t i) = 0;



    virtual float parseFloat() = 0;
    virtual long parseInt() = 0;

    virtual void tick() = 0;


    size_t readBytes(char *buffer, size_t length);
    size_t readBytesUntil( char terminator, char *buffer, size_t length); // as readBytes with terminator character
	// parseULong -- gives us access to large valued ints (unsigned)

	unsigned long parseULong();
	// parseInHex -- allows us to parse hex ints like 3e and F3D9
	unsigned long parseIntHex();

	inline unsigned long timeout() { return timeout_ms;}
	virtual void setTimeout(unsigned long tms) { timeout_ms = tms;}

protected:
    unsigned long parseULong(char skipChar);
	unsigned long parseIntHex(char skipChar);

	int timedPeek();
	int timedRead();
	int peekNextDigit(bool includeHex=false);

private:
	unsigned long timeout_ms;
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


template<class STREAMTYPE>
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
 * SUI::Delegate::Delegate -- base class for standard delegates
 */
template<class STREAMTYPE>
class Delegate : public StreamContainer<STREAMTYPE>, public Interface {
public:

	Delegate() {}
	Delegate(STREAMTYPE * defStream) : StreamContainer<STREAMTYPE>(defStream) {}

	virtual ~Delegate() {}

};



} /* namespace Delegate */
}/* namespace SUI */



#endif /* SERIALUI_SRC_INCLUDES_STREAM_DELEGATE_H_ */

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
#include "Delegate.h"


namespace SUI {

class SUIStream : public SerialUIStreamBaseType
{
public:
	SUIStream(StreamDelegate * useDelegate) : timeout_ms(0), streamDelegate(useDelegate) {}
	virtual ~SUIStream() {}


	unsigned long timeout();
	void setTimeout(unsigned long timeout);

	// parseULong -- gives us access to large valued ints (unsigned)
	unsigned long parseULong(char skipChar);
	unsigned long parseULong();

	// parseInHex -- allows us to parse hex ints like 3e and F3D9
	unsigned long parseIntHex();
	unsigned long parseIntHex(char skipChar);

	virtual size_t write(uint8_t i);
	virtual long parseInt(char skipChar=1);

protected:

	inline void useDelegate(StreamDelegate * d) { streamDelegate = d;}
	inline StreamDelegate * delegate() { return streamDelegate;}

private:
	int timedPeek();

	int peekNextDigit(bool includeHex=false);
	unsigned long timeout_ms;
	StreamDelegate * streamDelegate;

};


}



#endif /* SERIALUI_SRC_INCLUDES_SUISTREAM_H_ */

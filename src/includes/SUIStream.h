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

#include "SUIPlatform.h"


namespace SUI {

class SUIStream : public SerialUIStreamBaseType
{
public:
#ifdef SUI_SUISTREAM_NEEDSVIRTUAL
	virtual ~SUIStream() {}
#endif

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
private:
	int timedPeek();

	int peekNextDigit(bool includeHex=false);
	unsigned long timeout_ms;
};


}



#endif /* SERIALUI_SRC_INCLUDES_SUISTREAM_H_ */

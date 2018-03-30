/*
 * SinkBuffer.h
 *
 *  Created on: 2017-08-06
 *      Author: Pat Deegan
 *      Part of SovA, supporting the SerialUI library
 *      Copyright (C) 2016-2017 Pat Deegan, http://psychogenic.com
 *
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




#ifndef SRC_INCLUDES_UTILS_SINKBUFFER_H_
#define SRC_INCLUDES_UTILS_SINKBUFFER_H_


#include "../SovAConfig.h"
#include "../SovATypes.h"
#include "../SovAPlatform.h"
#include "../stream/SovAStream.h"

namespace SovA {
namespace Utils {


/* SinkBuffer -- utility class to provide print functionality to
 * a ram buffer array.
 */
template<SovA::Utils::BufferSize BUFSIZE>
class SinkBuffer : public SovA::Sink {

public:
	SinkBuffer() : pos(0) {
		reset();
	}

	virtual size_t write(uint8_t i) {
		if (pos <BUFSIZE) {
			myBuf[pos++] = i;
			return 1;
		}
		return 0;
	}
	virtual size_t write(const uint8_t *buffer, size_t size) {
		size_t maxLen = (BUFSIZE - pos);
		if (size > maxLen) {
			size = maxLen;
		}
		memcpy(&(myBuf[pos]), buffer, size);
		pos += size;
		return size;
	}

	void reset() {
		memset(myBuf, 0, BUFSIZE + 1);
		pos = 0;
	}

	inline char * charBuffer(uint8_t idx=0) { return (char*)&(myBuf[idx]);}
	inline uint8_t * buffer() {return myBuf;}


	inline SovA::Utils::BufferSize size() { return BUFSIZE ;}
	inline SovA::NumBytes	length() { return pos; } // space 'used'
	inline SovA::NumBytes 	spaceAvailable() { return BUFSIZE - pos;}


	inline void incrementPosition(uint8_t by) { pos += by;} // use with care

private:
	uint8_t myBuf[BUFSIZE+1];
	SovA::Utils::BufferSize pos;
};


}
}
#endif /* SRC_INCLUDES_UTILS_SINKBUFFER_H_ */

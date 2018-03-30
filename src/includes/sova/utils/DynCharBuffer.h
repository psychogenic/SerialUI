/*
 * DynCharBuffer.h -- basic version of std::string to manage dyn alloc'ed char buffers
 *
 *  Created on: Mar 30, 2018
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

#ifndef SERIALUI_SRC_INCLUDES_SOVA_UTILS_DYNCHARBUFFER_H_
#define SERIALUI_SRC_INCLUDES_SOVA_UTILS_DYNCHARBUFFER_H_

#include "../SovAExtIncludes.h"

namespace SovA {
namespace Utils {

/*
 * Usage:
 *
 * char * ho = new char[22]; // get dyn alloc'ed char[] somehow
 *
 *  // pass ownership/management to DynCharBuffer
 * DynCharBuffer foo(ho);
 *
 * if (! foo ) {
 *  return alloc_has_failed();
 * }
 *
 *  etc ... when foo goes out of scope, the buffer will be freed using delete[]
 */

class DynCharBuffer {
public:
	DynCharBuffer(char * newBuffer);
	~DynCharBuffer();

	inline bool operator!() {
		return (bufptr == NULL);
	}
	inline uint8_t size() {
		return strlen(bufptr);
	}

	inline const char * c_str() { return bufptr;}

	inline char & operator[](uint8_t idx) {
		return bufptr[idx];
	}
	inline const char & operator[](uint8_t idx) const {
		return bufptr[idx];
	}

	void clear() ;
private:
	char * bufptr;

};

} /* namespace Utils */
} /* namespace SovA */

#endif /* SERIALUI_SRC_INCLUDES_SOVA_UTILS_DYNCHARBUFFER_H_ */

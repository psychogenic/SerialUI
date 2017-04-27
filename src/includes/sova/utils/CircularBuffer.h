/*
 * CirculalBuffer.h
 *
 *  Created on: Jan 10, 2016
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
 * This is a basic, templated, circular buffer.  It makes a few
 * assumptions that are appropriate in our context, namely
 * that we are dealing with simple stuff (namely bytes, or at least
 * stuff that has default constructors and deals well with assignment
 * operators and such), and that we don't expect to be using buffers
 * greater that 255 items long.
 *
 *
 */

#ifndef SOVA_SRC_INCLUDES_UTILS_CIRCULARBUFFER_H_
#define SOVA_SRC_INCLUDES_UTILS_CIRCULALBUFFER_H_

#include "../SovAConfig.h"
#include "../SovATypes.h"
#include "../SovAPlatform.h"

#define SOVA_CIRCBUF_DEBUG
#if defined(SOVA_CIRCBUF_DEBUG) and defined(SOVA_INCLUDE_DEBUG)
#define SOVA_CIRCBUF_OUTPUT_DEBUG(...)		SOVA_OUTPUT_DEBUG_DEFAULTSTREAM(__VA_ARGS__)
#define SOVA_CIRCBUF_OUTPUTLN_DEBUG(...)		SOVA_OUTPUTLN_DEBUG_DEFAULTSTREAM(__VA_ARGS__)
#else

#define SOVA_CIRCBUF_OUTPUT_DEBUG(...)
#define SOVA_CIRCBUF_OUTPUTLN_DEBUG(...)
#endif

#define SOVA_CIRCBUF_LOCKING
#ifdef SOVA_CIRCBUF_LOCKING
//define SOVA_CIRCBUF_LOCK()		while(locked){;} locked=true
#define SOVA_CIRCBUF_LOCK()		locked=true
#define SOVA_CIRCBUF_UNLOCK()	locked=false
#else
#define SOVA_CIRCBUF_LOCK()
#define SOVA_CIRCBUF_UNLOCK()
#endif

namespace SovA {
namespace Utils {

template<class ITEMTYPE, SovA::Utils::BufferSize BUFSIZE>
class CircularBuffer {

public:
	CircularBuffer() :

#ifdef SOVA_CIRCBUF_LOCKING
	locked(false),
#endif
			head(1), tail(0), num_items(0)
	{
	}

#ifdef SOVA_CIRCBUF_LOCKING
	inline bool isLocked() { return locked;}
#endif

	inline BufferSize size() const {
		return num_items;
	}
	inline BufferSize capacity() const {
		return BUFSIZE;
	}
	inline bool empty() const {
		return !num_items;
	}

	inline BufferSize availableCapacity() const {
		return (BUFSIZE - num_items);
	}

	inline ITEMTYPE & operator[](BufferSize n) {
		return buffer[index_to_bufarray_idx(n)];
	}
	inline const ITEMTYPE & operator[](BufferSize n) const {
		return buffer[index_to_bufarray_idx(n)];
	}

	void push_back(ITEMTYPE item) {
		SOVA_CIRCBUF_OUTPUT_DEBUG("cpb,");


		SOVA_CIRCBUF_LOCK();
		tail = tail_next();
		buffer[tail] = item;

		// while ?
		if (num_items >= BUFSIZE) {
			head_increment(); // this reduces num_items by 1
		}
		++num_items;
		SOVA_CIRCBUF_UNLOCK();
	}
	inline void pop_front() {
		SOVA_CIRCBUF_OUTPUT_DEBUG("cpop");
		SOVA_CIRCBUF_LOCK();
		head_increment();
		SOVA_CIRCBUF_UNLOCK();
		// Note: would need to destroy head pos if
		// these were objects
	}
	void clear() {
		// note: would need to descroy items, if they
		// were objects and such
		SOVA_CIRCBUF_LOCK();
		num_items = 0;
		head = 1;
		tail = 0;
		SOVA_CIRCBUF_UNLOCK();
	}

	BufferSize fillFrom(const ITEMTYPE fromBuf[], BufferSize numElements)
	{
		SOVA_CIRCBUF_OUTPUT_DEBUG("cfill");
		for (BufferSize i=0; i<numElements; i++)
		{
			push_back(fromBuf[i]);
		}
		return numElements;
	}

	BufferSize transferTo(ITEMTYPE into[], BufferSize maxElements)
	{
		SOVA_CIRCBUF_OUTPUT_DEBUG("cxfer");

		BufferSize numToCopy = num_items < maxElements ? num_items : maxElements;

		if (! numToCopy) {
			return 0;
		}

		for (BufferSize i=0; i<numToCopy; i++)
		{
			into[i] = buffer[index_to_bufarray_idx(0)];
			pop_front();
		}
		return numToCopy;
	}


private:

	inline BufferSize index_to_bufarray_idx(BufferSize index) const {
		return ((index + head) % BUFSIZE);
	}

	inline void head_increment() {
		++head;
		--num_items;
		if (head >= BUFSIZE)
			head = 0;
	}

	inline BufferSize tail_next() {
		BufferSize next = tail + 1;
		return (next >= BUFSIZE) ? 0 : next;
	}

#ifdef SOVA_CIRCBUF_LOCKING
	volatile bool locked;
#endif
	BufferSize head;
	BufferSize tail;
	BufferSize num_items;
	ITEMTYPE buffer[BUFSIZE];

};

}

}

#endif /* SOVA_SRC_INCLUDES_UTILS_CIRCULARBUFFER_H_ */

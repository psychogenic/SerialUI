/*
 * CirculalBuffer.h
 *
 *  Created on: Jan 10, 2016
 *      Author: Pat Deegan
 *      Part of the SerialUI Project
 *      Copyright (C) 2015 Pat Deegan, http://psychogenic.com
 *
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

#ifndef SERIALUI_SRC_INCLUDES_UTILS_CIRCULARBUFFER_H_
#define SERIALUI_SRC_INCLUDES_UTILS_CIRCULALBUFFER_H_

#include "../SUIExtIncludes.h"
#include "../SUIPlatform.h"
#include "../SUIConfig.h"

//define SUI_CIRCBUF_DEBUG
#if defined(SUI_CIRCBUF_DEBUG) and defined(SUI_INCLUDE_DEBUG)
#define SUI_CIRCBUF_OUTPUT_DEBUG(...)		SUI_OUTPUT_DEBUG_DEFAULTSTREAM(__VA_ARGS__)
#define SUI_CIRCBUF_OUTPUTLN_DEBUG(...)		SUI_OUTPUTLN_DEBUG_DEFAULTSTREAM(__VA_ARGS__)
#else

#define SUI_CIRCBUF_OUTPUT_DEBUG(...)
#define SUI_CIRCBUF_OUTPUTLN_DEBUG(...)
#endif

//define SUI_CIRCBUF_LOCKING
#ifdef SUI_CIRCBUF_LOCKING
#define SUI_CIRCBUF_LOCK()		while(locked){;} locked=true
#define SUI_CIRCBUF_UNLOCK()	locked=false
#else
#define SUI_CIRCBUF_LOCK()
#define SUI_CIRCBUF_UNLOCK()
#endif

namespace SUI {
namespace Utils {



typedef uint8_t BufferSize;

template<class ITEMTYPE, BufferSize BUFSIZE>
class CircularBuffer {

public:
	CircularBuffer() :

#ifdef SUI_CIRCBUF_LOCKING
	locked(false),
#endif
			head(1), tail(0), num_items(0)
	{
	}

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
		SUI_CIRCBUF_OUTPUT_DEBUG("cpb,");


		SUI_CIRCBUF_LOCK();
		buffer[tail_next()] = item;
		if (num_items == BUFSIZE) {
			head_increment();
		}

		tail_increment();
		SUI_CIRCBUF_UNLOCK();
	}
	inline void pop_front() {
		SUI_CIRCBUF_OUTPUT_DEBUG("cpop");
		SUI_CIRCBUF_LOCK();
		head_increment();
		SUI_CIRCBUF_UNLOCK();
		// Note: would need to destroy head pos if
		// these were objects
	}
	void clear() {
		// note: would need to descroy items, if they
		// were objects and such
		SUI_CIRCBUF_LOCK();
		num_items = 0;
		head = 1;
		tail = 0;
		SUI_CIRCBUF_UNLOCK();
	}

	BufferSize fillFrom(const ITEMTYPE fromBuf[], BufferSize numElements)
	{
		SUI_CIRCBUF_OUTPUT_DEBUG("cfill");
		SUI_CIRCBUF_LOCK();
		for (BufferSize i=0; i<numElements; i++)
		{
			push_back(fromBuf[i]);
		}
		SUI_CIRCBUF_UNLOCK();
		return numElements;
	}

	BufferSize transferTo(ITEMTYPE into[], BufferSize maxElements)
	{
		SUI_CIRCBUF_OUTPUT_DEBUG("cxfer");
		SUI_CIRCBUF_LOCK();
		BufferSize numToCopy = num_items < maxElements ? num_items : maxElements;

		if (! numToCopy) {
			SUI_CIRCBUF_UNLOCK();
			return 0;
		}

		for (BufferSize i=0; i<numToCopy; i++)
		{
			into[i] = buffer[index_to_bufarray_idx(0)];
			pop_front();
		}

		SUI_CIRCBUF_UNLOCK();
		return numToCopy;
	}


private:

	inline BufferSize index_to_bufarray_idx(BufferSize index) const {
		return ((index + head) % BUFSIZE);
	}

	void head_increment() {
		++head;
		--num_items;
		if (head == BUFSIZE)
			head = 0;
	}
	void tail_increment() {
		++num_items;
		tail = tail_next();
	}

	BufferSize tail_next() {
		BufferSize next = tail + 1;
		return (next >= BUFSIZE) ? 0 : next;
	}

#ifdef SUI_CIRCBUF_LOCKING
	volatile bool locked;
#endif
	BufferSize head;
	BufferSize tail;
	BufferSize num_items;
	ITEMTYPE buffer[BUFSIZE];

};

}

}

#endif /* SERIALUI_SRC_INCLUDES_UTILS_CIRCULARBUFFER_H_ */

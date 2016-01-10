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

namespace SUI {
namespace Utils {

typedef uint8_t BufferSize;

template<class ITEMTYPE, BufferSize BUFSIZE>
class CircularBuffer {

public:
	CircularBuffer() :
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

	inline ITEMTYPE & operator[](BufferSize n) {
		return buffer[index_to_bufarray_idx(n)];
	}
	inline const ITEMTYPE & operator[](BufferSize n) const {
		return buffer[index_to_bufarray_idx(n)];
	}

	void push_back(ITEMTYPE item) {
		buffer[tail_next()] = item;
		if (num_items == BUFSIZE) {
			head_increment();
		}

		tail_increment();
	}
	inline void pop_front() {
		head_increment();
		// Note: would need to destroy head pos if
		// these were objects
	}
	void clear() {
		// note: would need to descroy items, if they
		// were objects and such
		num_items = 0;
		head = 1;
		tail = 0;
	}

	void fillFrom(ITEMTYPE fromBuf[], BufferSize numElements)
	{
		for (BufferSize i=0; i<numElements; i++)
		{
			push_back(fromBuf[i]);
		}
		return;
	}

	BufferSize transferTo(ITEMTYPE into[], BufferSize maxElements)
	{
		BufferSize numToCopy = num_items < maxElements ? num_items : maxElements;

		if (! numToCopy)
			return 0;

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

	BufferSize head;
	BufferSize tail;
	BufferSize num_items;
	ITEMTYPE buffer[BUFSIZE];

};

}

}

#endif /* SERIALUI_SRC_INCLUDES_UTILS_CIRCULARBUFFER_H_ */

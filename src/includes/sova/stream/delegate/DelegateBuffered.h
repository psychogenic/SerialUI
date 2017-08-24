/*
 * DelegateBuffered.h
 *
 *  Created on: Jan 10, 2016
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

#ifndef SOVA_SRC_INCLUDES_STREAM_DELEGATEBUFFERED_H_

#define SOVA_SRC_INCLUDES_STREAM_DELEGATEBUFFERED_H_
#include "Delegate.h"
#include "../../SovAExtIncludes.h"
#include "../../SovAConfig.h"
#include "../../platform/DefaultStream.h"
#include "../../utils/CircularBuffer.h"

namespace SovA {
namespace Delegate {


template<class BYTETYPE, SovA::Utils::BufferSize BUFSIZE>
class Buffered : public SovA::Delegate::InterfaceParsingImpl {
public:
	Buffered() : SovA::Delegate::InterfaceParsingImpl() {}
	virtual ~Buffered() {}

	virtual int available() {  if(inBuffer.size()) {return inBuffer.size();}  if (hasDataAfterYield(&inBuffer)) {return inBuffer.size();}  return 0;}
    virtual int read() { if (inBuffer.empty() && !(hasDataAfterYield(&inBuffer))) return -1; int val = inBuffer[0]; inBuffer.pop_front(); return val;}
    virtual int peek() { if (inBuffer.empty()) return -1;  return inBuffer[0]; }

    virtual size_t write(const uint8_t *buffer, size_t size) { return outBuffer.fillFrom(buffer,size);}
    virtual size_t write(uint8_t i) { outBuffer.push_back(i); return 1; }

protected:

    virtual void yieldToOS() {}

    inline bool hasDataAfterYield(Utils::CircularBuffer<BYTETYPE, BUFSIZE> * buf) {

    	this->poll();
    	return (inBuffer.size()) ? true : false;
    	/*
    	this->yieldToOS();
    	if (buf->empty())
    		return false;

    	return true;
    	*/
    }
    inline SovA::Utils::BufferSize writeQueueSize() { return outBuffer.size();}
    inline SovA::Utils::BufferSize readQueueSize() { return inBuffer.size();}
    inline Utils::CircularBuffer<BYTETYPE, BUFSIZE> & writeQueue() { return outBuffer;}
    inline Utils::CircularBuffer<BYTETYPE, BUFSIZE> & readQueue() { return inBuffer;}


    Utils::CircularBuffer<BYTETYPE, BUFSIZE> inBuffer; // bytes coming in from outside world, available as read()s
    Utils::CircularBuffer<BYTETYPE, BUFSIZE> outBuffer; // bytes going out to outside world, stored with write()s

};




} /* namespace Delegate */
}/* namespace SovA */



#endif /* SOVA_SRC_INCLUDES_STREAM_DELEGATEBUFFERED_H_ */

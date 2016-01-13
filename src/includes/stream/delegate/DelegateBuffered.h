/*
 * DelegateBuffered.h
 *
 *  Created on: Jan 10, 2016
 *      Author: Pat Deegan
 *      Part of the SerialUI Project
 *      Copyright (C) 2015 Pat Deegan, http://psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_STREAM_DELEGATEBUFFERED_H_
#define SERIALUI_SRC_INCLUDES_STREAM_DELEGATEBUFFERED_H_
#include "Delegate.h"
#include "../../SUIExtIncludes.h"
#include "../../SUIConfig.h"
#include "../../platform/DefaultStream.h"
#include "../../utils/CircularBuffer.h"

namespace SUI {
namespace Delegate {


template<class BYTETYPE, SUI::Utils::BufferSize BUFSIZE>
class Buffered : public SUI::Delegate::InterfaceParsingImpl {
public:
	Buffered() : SUI::Delegate::InterfaceParsingImpl() {}
	virtual ~Buffered() {}

	virtual int available() {  return inBuffer.size();}
    virtual int read() { if (inBuffer.empty()) return -1; int val = inBuffer[0]; inBuffer.pop_front(); return val;}
    virtual int peek() { if (inBuffer.empty()) return -1;  return inBuffer[0]; }

    virtual size_t write(const uint8_t *buffer, size_t size) { return outBuffer.fillFrom(buffer,size);}
    virtual size_t write(uint8_t i) { outBuffer.push_back(i); return 1; }

protected:
    inline SUI::Utils::BufferSize writeQueueSize() { return outBuffer.size();}
    inline SUI::Utils::BufferSize readQueueSize() { return inBuffer.size();}
    inline Utils::CircularBuffer<BYTETYPE, BUFSIZE> & writeQueue() { return outBuffer;}
    inline Utils::CircularBuffer<BYTETYPE, BUFSIZE> & readQueue() { return inBuffer;}


    Utils::CircularBuffer<BYTETYPE, BUFSIZE> inBuffer; // bytes coming in from outside world, available as read()s
    Utils::CircularBuffer<BYTETYPE, BUFSIZE> outBuffer; // bytes going out to outside world, stored with write()s

};




} /* namespace Delegate */
}/* namespace SUI */



#endif /* SERIALUI_SRC_INCLUDES_STREAM_DELEGATEBUFFERED_H_ */

/*
 * DelegateDummy.h
 *
 *  Created on: Jan 11, 2016
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

#ifndef SOVA_SRC_INCLUDES_STREAM_DELEGATE_DELEGATEDUMMY_H_

#define SOVA_SRC_INCLUDES_STREAM_DELEGATE_DELEGATEDUMMY_H_

#include "../../SovAConfig.h"
#include "DelegateBuffered.h"

#ifdef SOVA_STREAMDELEGATE_DUMMY_ENABLE

#define SOVA_DUMMYBUFFER_SIZE	(SOVA_PROGMEM_STRING_ABS_MAXLEN + 1)
#define SOVA_DUMMYBUFFER_LOWWATERMARK		((SOVA_DUMMYBUFFER_SIZE/3))
#define SOVA_DUMMYBUFFER_HIGHWATERMARK		(2*(SOVA_DUMMYBUFFER_SIZE/3))

namespace SovA {
namespace Delegate {

typedef Buffered<uint8_t, SOVA_DUMMYBUFFER_SIZE> DummyBufferedBase;

class Dummy : public DummyBufferedBase {
public:
	Dummy() : DummyBufferedBase() {}
	virtual ~Dummy() {}
	virtual long parseInt() { return 0;}
	virtual void flush() { this->tick();};
	virtual void tick(bool doFlush=true);
    virtual void begin(unsigned long speed);
    virtual void setTimeout(unsigned long timeout);

    virtual int read();
    virtual int peek() ;
    virtual size_t write(const uint8_t *buffer, size_t size);
    virtual size_t write(uint8_t i);

private:
    void bufferPendingReads();
    void flushPendingWrites();

};


} /* namespace Delegate */
}/* namespace SovA */

#endif /* SOVA_STREAMDELEGATE_DUMMY_ENABLE */

#endif /* SOVA_SRC_INCLUDES_STREAM_DELEGATE_DELEGATEDUMMY_H_ */

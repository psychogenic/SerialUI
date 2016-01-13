/*
 * DelegateDummy.h
 *
 *  Created on: Jan 11, 2016
 *      Author: Pat Deegan
 *      Part of the SerialUI Project
 *      Copyright (C) 2015 Pat Deegan, http://psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_STREAM_DELEGATE_DELEGATEDUMMY_H_
#define SERIALUI_SRC_INCLUDES_STREAM_DELEGATE_DELEGATEDUMMY_H_

#include "../../SUIConfig.h"
#include "DelegateBuffered.h"

#ifdef SUI_STREAMDELEGATE_DUMMY_ENABLE

#define SUI_DUMMYBUFFER_SIZE	(SUI_SERIALUI_PROGMEM_STRING_ABS_MAXLEN + 1)
#define SUI_DUMMYBUFFER_LOWWATERMARK		((SUI_DUMMYBUFFER_SIZE/3))
#define SUI_DUMMYBUFFER_HIGHWATERMARK		(2*(SUI_DUMMYBUFFER_SIZE/3))

namespace SUI {
namespace Delegate {

typedef Buffered<uint8_t, SUI_DUMMYBUFFER_SIZE> DummyBufferedBase;

class Dummy : public DummyBufferedBase {
public:
	Dummy() : DummyBufferedBase() {}
	virtual ~Dummy() {}
	virtual long parseInt() { return 0;}
	virtual void flush() { this->tick();};
	virtual void tick();
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
}/* namespace SUI */

#endif /* SUI_STREAMDELEGATE_DUMMY_ENABLE */

#endif /* SERIALUI_SRC_INCLUDES_STREAM_DELEGATE_DELEGATEDUMMY_H_ */

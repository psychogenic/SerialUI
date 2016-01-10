/*
 * DelegateDirect.h
 *
 *  Created on: Jan 8, 2016
 *      Author: Pat Deegan
 *      Part of the SerialUI Project
 *      Copyright (C) 2015 Pat Deegan, http://psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_STREAM_DELEGATEDIRECT_H_
#define SERIALUI_SRC_INCLUDES_STREAM_DELEGATEDIRECT_H_

#include "Delegate.h"
#include "../SUIConfig.h"
#include "../platform/DefaultStream.h"

namespace SUI {
namespace Delegate {

/*
 * The Direct delegate assumes it's template class
 * has the functionality to handle all of it's interface
 * directly--i.e. a call to Direct<X>::foo() just calls X x->foo()
 */
template<class STREAMTYPE>
class Direct : public Delegate<STREAMTYPE>{
public:
	Direct(STREAMTYPE * defStream) : stream_to_use(defStream), stream_to_use_override(false) {}
	virtual ~Direct() {}

	virtual void setStream(STREAMTYPE * strm) {
			stream_to_use = strm;
			stream_to_use_override = true;
	}
	virtual int available() {  return stream_to_use->available();}
    virtual int read() { return stream_to_use->read(); }
    virtual int peek() { return stream_to_use->peek(); }
    virtual void flush() { stream_to_use->flush(); }

    virtual size_t write(const uint8_t *buffer, size_t size) { return stream_to_use->write(buffer,size);}
    virtual size_t write(uint8_t i) { return stream_to_use->write(i); }
    virtual void setTimeout(unsigned long timeout)  { stream_to_use->setTimeout(timeout); }
    virtual void begin(unsigned long speed)  {


    	   	if (!stream_to_use_override)
    		{
    	   		// when overridden, it's your responsibility to
    	   		// begin()... here, there's no override used,
    	   		// so we make the call ourselves.
    	   		SUI_PLATFORM_HARDWARESERIAL_DEFAULT.begin(speed);
    	   	}
    	   	// we also set a sane timeout for reads, as this was causing issues
    	   	// assuming code will call begin() prior to setTimeout() (if used)
    	   	stream_to_use->setTimeout(SUI_SERIALUI_READBYTES_TIMEOUT_DEFAULT_MS);

    }

private:
    STREAMTYPE * stream_to_use;
    bool stream_to_use_override;


};


} /* namespace Delegate */
}/* namespace SUI */




#endif /* SERIALUI_SRC_INCLUDES_STREAM_DELEGATEDIRECT_H_ */

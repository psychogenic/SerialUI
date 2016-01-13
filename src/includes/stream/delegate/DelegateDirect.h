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
#include "../../SUIConfig.h"
#include "../../platform/DefaultStream.h"

namespace SUI {
namespace Delegate {

/*
 * The Direct delegate assumes it's template class
 * has the functionality to handle all of it's interface
 * directly--i.e. a call to Direct<X>::foo() just calls X x->foo()
 */

template<class STREAMTYPE>
class Direct : public Delegate<STREAMTYPE>
{
public:
	Direct(STREAMTYPE * defStream) : Delegate<STREAMTYPE>(defStream) {}
	virtual ~Direct() {}

	virtual int available() {  return this->streamToUse()->available();}
    virtual int read() { return this->streamToUse()->read(); }
    virtual int peek() { return this->streamToUse()->peek(); }
    virtual void flush() { this->streamToUse()->flush(); }



    virtual float parseFloat() { return this->streamToUse()->parseFloat();}
	virtual long parseInt() { return this->streamToUse()->parseInt(); }

    virtual size_t write(const uint8_t *buffer, size_t size) { return this->streamToUse()->write(buffer,size);}
    virtual size_t write(uint8_t i) { return this->streamToUse()->write(i); }
    virtual void setTimeout(unsigned long timeout)  {
    	this->Delegate<STREAMTYPE>::setTimeout(timeout);
    	this->streamToUse()->setTimeout(timeout); }
    virtual void tick() {}
    virtual void begin(unsigned long speed)  {


    	   	if (!this->streamToUseOverridden())
    		{
    	   		// when overridden, it's your responsibility to
    	   		// begin()... here, there's no override used,
    	   		// so we make the call ourselves.
    	   		SUI_PLATFORM_HARDWARESERIAL_DEFAULT.begin(speed);
    	   	}
    	   	// we also set a sane timeout for reads, as this was causing issues
    	   	// assuming code will call begin() prior to setTimeout() (if used)
    	   	this->streamToUse()->setTimeout(SUI_SERIALUI_READBYTES_TIMEOUT_DEFAULT_MS);

    }



};


} /* namespace Delegate */
}/* namespace SUI */




#endif /* SERIALUI_SRC_INCLUDES_STREAM_DELEGATEDIRECT_H_ */

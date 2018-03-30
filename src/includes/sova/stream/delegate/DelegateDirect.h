/*
 * DelegateDirect.h
 *
 *  Created on: Jan 8, 2016
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

#ifndef SOVA_SRC_INCLUDES_STREAM_DELEGATEDIRECT_H_

#define SOVA_SRC_INCLUDES_STREAM_DELEGATEDIRECT_H_

#include "Delegate.h"
#include "../../SovAConfig.h"
#include "../../platform/DefaultStream.h"

namespace SovA {
namespace Delegate {

/*
 * The Direct delegate assumes it's template class
 * has the functionality to handle all of it's interface
 * directly--i.e. a call to Direct<X>::foo() just calls X x->foo()
 */

template<typename SOVASTREAMTYPE>
class Direct : public SovA::Delegate::Delegate<SOVASTREAMTYPE>
{
public:
	Direct(SOVASTREAMTYPE * defStream) : SovA::Delegate::Delegate<SOVASTREAMTYPE>(defStream) {}
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
    	this->Delegate<SOVASTREAMTYPE>::setTimeout(timeout);
    	this->streamToUse()->setTimeout(timeout); }
    virtual void tick(bool doFlush=true) {(void)doFlush;}
    virtual void begin(unsigned long speed)  {


    	   	if (!this->streamToUseOverridden())
    		{
    	   		// when overridden, it's your responsibility to
    	   		// begin()... here, there's no override used,
    	   		// so we make the call ourselves.
    	   		SOVA_PLATFORM_HARDWARESERIAL_DEFAULT.begin(speed);
    	   	}
    	   	// we also set a sane timeout for reads, as this was causing issues
    	   	// assuming code will call begin() prior to setTimeout() (if used)
    	   	this->streamToUse()->setTimeout(this->timeout());

    }



};


} /* namespace Delegate */
}/* namespace SovA */




#endif /* SOVA_SRC_INCLUDES_STREAM_DELEGATEDIRECT_H_ */

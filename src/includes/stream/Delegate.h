/*
 * Delegate.h
 *
 *  Created on: Jan 9, 2016
 *      Author: Pat Deegan
 *      Part of the SerialUI Project
 *      Copyright (C) 2015 Pat Deegan, http://psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_STREAM_DELEGATE_H_
#define SERIALUI_SRC_INCLUDES_STREAM_DELEGATE_H_

#include "../SUIExtIncludes.h"


namespace SUI {
namespace Delegate {


/*
 * Interface -- interface common to al delegates
 */
class Interface {
public:
	virtual ~Interface() {}
    virtual void begin(unsigned long speed) = 0;

	virtual int available() = 0;
    virtual int read() = 0;
    virtual int peek() = 0;
    virtual void flush() = 0;
    virtual size_t write(const uint8_t *buffer, size_t size) = 0;
    virtual size_t write(uint8_t i) = 0;
    virtual void setTimeout(unsigned long timeout) = 0;

};


/*
 * SUI::Delegate::Delegate -- base class for standard delegates
 */
template<class STREAMTYPE>
class Delegate : public Interface {
public:
	virtual ~Delegate() {}
	virtual void setStream(STREAMTYPE * strm)  = 0;

};



} /* namespace Delegate */
}/* namespace SUI */



#endif /* SERIALUI_SRC_INCLUDES_STREAM_DELEGATE_H_ */

/*
 * TrackedString.h
 *
 *  Created on: May 26, 2018
 *      Author: Pat Deegan
 *
 *  TrackedString is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_TRACKED_TRACKEDSTRING_H_
#define SERIALUI_SRC_INCLUDES_TRACKED_TRACKEDSTRING_H_

#include "TrackedVariable.h"

namespace SerialUI {
namespace Tracked {


class String: public StateImplComparable<Type::String, TopLevelString, String>
{
public:
	String(StaticString name) : StateImplComparable(name, "") {}
	String(StaticString name, TopLevelString s) : StateImplComparable(name, s) {}
	String(StaticString name, ID sid, TopLevelString v) : StateImplComparable(name, sid, v) {}

	String& operator+= (TopLevelString v);
	String operator+(const TopLevelString& b) const;


	String& operator= (const char *cstr) ;
    String & operator += (const char *cstr);
    String & operator += (char c) ;
    String & operator += (unsigned char num);
    String & operator += (int num) ;
    String & operator += (unsigned int num);
    String & operator += (long num) ;
    String & operator += (unsigned long num);
    String & operator += (float num) ;
    String & operator += (double num);
    char operator [] (unsigned int index) const ;
    char& operator [] (unsigned int index) ;


	virtual void printValueTo(Comm::Sink * toSink, bool asBinary=false) const {
		if (asBinary) {
			toSink->write(this->value.length());
		}
		toSink->print(this->value);

	}

#ifndef DESKTOP_COMPILE
    // very arduino-specific
    char charAt(unsigned int index) const ;
    void setCharAt(unsigned int index, char c) ;

    // parsing/conversion
    long toInt(void) const ;
    float toFloat(void) const ;
    double toDouble(void) const ;
#endif


};


} /* namespace Tracked */
} /* namespace SerialUI */

#endif /* SERIALUI_SRC_INCLUDES_TRACKED_TRACKEDSTRING_H_ */

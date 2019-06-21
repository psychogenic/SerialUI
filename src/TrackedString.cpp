/*
 * TrackedString.cpp
 *
 *  Created on: May 26, 2018
 *      Author: Pat Deegan
 *
 *  TrackedString is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#include "includes/tracked/TrackedString.h"

namespace SerialUI {
namespace Tracked {


String& String::operator+= (TopLevelString v) {
		TopLevelString o =  this->value;
		o += v;
		this->assign(o);
		return *this;
}
String String::operator+(const TopLevelString& b) const {
	TopLevelString nstr = this->value + b;
	return String(this->name(), nstr);
}


String& String::operator= (const char *cstr) {
	TopLevelString s(cstr);

	this->assign(s);
	return *this;
}
String & String::operator += (const char *cstr) {
	TopLevelString s( this->value );
	s += cstr;
	this->assign(s);
	return *this;
}
String & String::operator += (char c) {
	TopLevelString s( this->value );
	s += c;
	this->assign(s);
	return *this;
}
String & String::operator += (unsigned char num) {
	TopLevelString s( this->value );
	s += num;
	this->assign(s);
	return *this;
}
String & String::operator += (int num)  {
	TopLevelString s( this->value );
	s += num;
	this->assign(s);
	return *this;
}
String & String::operator += (unsigned int num) {
	TopLevelString s( this->value );
	s += num;
	this->assign(s);
	return *this;
}
String & String::operator += (long num)  {
	TopLevelString s( this->value );
	s += num;
	this->assign(s);
	return *this;
}
String & String::operator += (unsigned long num) {
	TopLevelString s( this->value );
	s += num;
	this->assign(s);
	return *this;
}
String & String::operator += (float num) {
	TopLevelString s( this->value );
	s += num;
	this->assign(s);
	return *this;
}
String & String::operator += (double num) {
	TopLevelString s( this->value );
	s += num;
	this->assign(s);
	return *this;
}
char String::operator [] (unsigned int index) const {
	return this->value[index];
}
char& String::operator [] (unsigned int index) {
	char & c = this->value[index];
	this->markAsChanged();
	return c;
}



#ifndef DESKTOP_COMPILE
// very arduino-specific
char String::charAt(unsigned int index) const {
	return this->value.charAt(index);
}
void String::setCharAt(unsigned int index, char c) {
	this->value.setCharAt(index, c);
	this->markAsChanged();
}

// parsing/conversion
long String::toInt(void) const {
	return this->value.toInt();
}
float String::toFloat(void) const {
	return this->value.toFloat();
}
double String::toDouble(void) const {
#ifdef SERIALUI_PLATFORM_NRF52
#warning "no WString toDouble() on platform -- returning float instead"
	return this->value.toFloat();
#else
	return this->value.toDouble();
#endif
}

#endif

} /* namespace Tracked */
} /* namespace SerialUI */

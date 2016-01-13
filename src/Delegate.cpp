/*
 * Delegate.cpp
 *
 *  Created on: Jan 12, 2016
 *      Author: Pat Deegan
 *      Part of the SerialUI Project
 *      Copyright (C) 2015 Pat Deegan, http://psychogenic.com
 */

#include "includes/stream/delegate/Delegate.h"
#include "includes/SUIPlatform.h"

#define NO_SKIP_CHAR  1

namespace SUI {
namespace Delegate {


unsigned long Interface::parseULong()
{
	return parseULong(NO_SKIP_CHAR);
}
unsigned long Interface::parseULong(char skipChar)
{
	uint32_t value = 0;
	int c;

	this->tick();
	c = peekNextDigit();
	// ignore non numeric leading characters
	if (c < 0)
		return 0; // zero returned if timeout

	do {

		this->tick();
		if (c == skipChar)
			; // ignore this character
		else if (c >= '0' && c <= '9') // is c a digit?
			value = value * 10 + c - '0';
		this->read(); // consume the character we got with peek
		c = timedPeek();
	} while ((c >= '0' && c <= '9') || c == skipChar);


	return value;

}
// read characters from stream into buffer
// terminates if length characters have been read, or timeout (see setTimeout)
// returns the number of characters placed in the buffer
// the buffer is NOT null terminated.
//
size_t Interface::readBytes(char *buffer, size_t length)
{
  size_t count = 0;
  while (count < length) {
    int c = timedRead();
    if (c < 0) break;
    *buffer++ = (char)c;
    count++;
  }
  return count;
}
size_t Interface::readBytesUntil(char terminator, char *buffer, size_t length)
{
  if (length < 1) return 0;
  size_t index = 0;
  while (index < length) {
    int c = this->timedRead();
    if (c < 0 || c == terminator) break;
    *buffer++ = (char)c;
    index++;
  }
  return index; // return number of characters, not including null terminator
}



unsigned long Interface::parseIntHex()
{
	return parseIntHex(NO_SKIP_CHAR);
}

unsigned long Interface::parseIntHex(char skipChar)
{

	uint32_t value = 0;
	int c;

	this->tick();
	c = peekNextDigit(true);
	// ignore non numeric leading characters
	if (c < 0)
		return 0; // zero returned if timeout

	do {
		if (c == skipChar)
			; // ignore this character
		else if (c >= '0' && c <= '9') // is c a digit?
			value = (value * 16) + c - '0';
		else if (c >= 'A' && c <= 'F')
			value = (value * 16) + ((c - 'A') + 10);
		else if (c >= 'a' && c <= 'f')
			value = (value * 16) + ((c - 'a') + 10);

		this->read(); // consume the character we got with peek
		c = timedPeek();
	} while ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f') || c == skipChar);


	return value;

}
// private method to read stream with timeout
int Interface::timedRead()
{
  int c;
  unsigned long startMillis = PLATFORM_NOW_MILLIS();
  do {
    c = read();
    if (c >= 0) return c;
  } while(PLATFORM_NOW_MILLIS() - startMillis < this->timeout());
  return -1;     // -1 indicates timeout
}

int Interface::timedPeek()
{
  int c;
  unsigned long startMillis = PLATFORM_NOW_MILLIS();
  do {
	this->tick();
    c = this->peek();
    if (c >= 0) return c;
  } while( (PLATFORM_NOW_MILLIS() - startMillis) <= this->timeout());
  return -1;     // -1 indicates timeout
}
int Interface::peekNextDigit(bool includeHex)
{
  int c;
  while (1) {
    c = timedPeek();
    if (c < 0) return c;  // timeout
    if (c == '-') return c;
    if (c >= '0' && c <= '9') return c;
    if (includeHex)
    {
    	if ( (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
    		return c;
    }
    this->read();  // discard non-numeric
  }
  return -1;
}



float InterfaceParsingImpl::parseFloat()
{
	return parseFloat(NO_SKIP_CHAR);

}
long InterfaceParsingImpl::parseInt() {

	return parseInt(NO_SKIP_CHAR);
}


float InterfaceParsingImpl::parseFloat(char skipChar)
{
	  bool isNegative = false;
	  bool isFraction = false;
	  long value = 0;
	  char c;
	  float fraction = 1.0;

	  this->tick();
	  c = this->peekNextDigit();
	    // ignore non numeric leading characters
	  if(c < 0)
	    return 0; // zero returned if timeout

	  do{
	    if(c == skipChar)
	      ; // ignore
	    else if(c == '-')
	      isNegative = true;
	    else if (c == '.')
	      isFraction = true;
	    else if(c >= '0' && c <= '9')  {      // is c a digit?
	      value = value * 10 + c - '0';
	      if(isFraction)
	         fraction *= 0.1;
	    }
	    this->read();  // consume the character we got with peek
	    c = this->timedPeek();
	  }
	  while( (c >= '0' && c <= '9')  || c == '.' || c == skipChar );

	  if(isNegative)
	    value = -value;
	  if(isFraction)
	    return value * fraction;
	  else
	    return value;

}
long InterfaceParsingImpl::parseInt(char skipChar) {

	  bool isNegative = false;
	  long value = 0;
	  int c;
	  this->tick();
	  c = this->peekNextDigit();
	  // ignore non numeric leading characters
	  if(c < 0)
	    return 0; // zero returned if timeout

	  do{
	    if(c == skipChar)
	      ; // ignore this charactor
	    else if(c == '-')
	      isNegative = true;
	    else if(c >= '0' && c <= '9')        // is c a digit?
	      value = value * 10 + c - '0';

	    this->read();  // consume the character we got with peek
	    c = this->timedPeek();
	  }
	  while( (c >= '0' && c <= '9') || c == skipChar );

	  if(isNegative)
	    value = -value;
	  return value;
}

}
}


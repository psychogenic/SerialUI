/*
 * Delegate.cpp
 * Copyright (C) 2016-2017 Pat Deegan, psychogenic.com.
 *
 * Released as a supporting component of SerialUI.
 * http://www.flyingcarsandstuff.com/projects/SerialUI
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


#include "includes/sova/stream/delegate/Delegate.h"
#include "includes/sova/SovAPlatform.h"

#define NO_SKIP_CHAR  1

namespace SovA {
namespace Delegate {


unsigned long Interface::parseULong()
{
	return parseULong(NO_SKIP_CHAR);
}
unsigned long Interface::parseULong(char skipChar)
{
	uint32_t value = 0;
	int c;

	c = peekNextDigit();
	// ignore non numeric leading characters
	if (c < 0)
		return 0; // zero returned if timeout

	do {

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

	c = peekNextDigit(true);
	// ignore non numeric leading characters
	if (c < 0)
		return 0; // zero returned if timeout

	do {
		if (c == skipChar) {
			; // ignore this character
		} else if (c >= '0' && c <= '9') // is c a digit?
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
  uint8_t tickCount = 0;
  unsigned long startMillis = PLATFORM_NOW_MILLIS();
  do {
	if (tickCount++ % 20 == 0) {
		this->poll();
	}
	this->tick(false);
    c = read();
    if (c >= 0) return c;
  } while(PLATFORM_NOW_MILLIS() - startMillis < this->timeout());
  return -1;     // -1 indicates timeout
}

int Interface::timedPeek()
{
  int c;
  uint8_t tickCount = 0;
  unsigned long startMillis = PLATFORM_NOW_MILLIS();
  do {
	if (tickCount++ % 20 == 0) {
	  		this->poll();
	}
	this->tick(false);
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


// find returns true if the target string is found
bool  Interface::find(char *target)
{
 return findUntil(target, strlen(target), NULL, 0);
}

// reads data from the stream until the target string of given length is found
// returns true if target string is found, false if timed out
bool Interface::find(char *target, size_t length)
{
 return findUntil(target, length, NULL, 0);
}

// as find but search ends if the terminator string is found
bool  Interface::findUntil(char *target, char *terminator)
{
 return findUntil(target, strlen(target), terminator, strlen(terminator));
}

// reads data from the stream until the target string of the given length is found
// search terminated if the terminator string is found
// returns true if target string is found, false if terminated or timed out
bool Interface::findUntil(char *target, size_t targetLen, char *terminator, size_t termLen)
{
 if (terminator == NULL) {
   MultiTarget t[1] = {{target, targetLen, 0}};
   return findMulti(t, 1) == 0 ? true : false;
 } else {
   MultiTarget t[2] = {{target, targetLen, 0}, {terminator, termLen, 0}};
   return findMulti(t, 2) == 0 ? true : false;
 }
}



int Interface::findMulti( struct Interface::MultiTarget *targets, int tCount) {
  // any zero length target string automatically matches and would make
  // a mess of the rest of the algorithm.
  for (struct MultiTarget *t = targets; t < targets+tCount; ++t) {
    if (t->len <= 0)
      return t - targets;
  }

  while (1) {
    int c = timedRead();
    if (c < 0)
      return -1;

    for (struct MultiTarget *t = targets; t < targets+tCount; ++t) {
      // the simple case is if we match, deal with that first.
      if (c == t->str[t->index]) {
        if (++t->index == t->len)
          return t - targets;
        else
          continue;
      }

      // if not we need to walk back and see if we could have matched further
      // down the stream (ie '1112' doesn't match the first position in '11112'
      // but it will match the second position so we can't just reset the current
      // index to 0 when we find a mismatch.
      if (t->index == 0)
        continue;

      int origIndex = t->index;
      do {
        --t->index;
        // first check if current char works against the new current index
        if (c != t->str[t->index])
          continue;

        // if it's the only char then we're good, nothing more to check
        if (t->index == 0) {
          t->index++;
          break;
        }

        // otherwise we need to check the rest of the found string
        int diff = origIndex - t->index;
        size_t i;
        for (i = 0; i < t->index; ++i) {
          if (t->str[i] != t->str[i + diff])
            break;
        }

        // if we successfully got through the previous loop then our current
        // index is good.
        if (i == t->index) {
          t->index++;
          break;
        }

        // otherwise we just try the next index
      } while (t->index);
    }
  }
  // unreachable
  return -1;
}


String Interface::readString()
{
  String ret;
  int c = timedRead();
  while (c >= 0)
  {
    ret += (char)c;
    c = timedRead();
  }
  return ret;
}

String Interface::readStringUntil(char terminator)
{
  String ret;
  int c = timedRead();
  while (c >= 0 && c != terminator)
  {
    ret += (char)c;
    c = timedRead();
  }
  return ret;
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


/*
 * SUIStream.cpp
 *
 *  Created on: Jan 7, 2016
 *      Author: Pat Deegan
 *      Part of the SerialUI Project
 *      Copyright (C) 2015 Pat Deegan, http://psychogenic.com
 */


#include "includes/SUIPlatform.h"

#ifdef SUI_BASEIMPLEMENTATION_STANDARD

#include "includes/stream/SUIStream.h"
namespace SUI {


#define NO_SKIP_CHAR  1
size_t SUIStream::write(uint8_t i)
{
	return streamDelegate->write(i);
}

long SUIStream::parseInt(char skipChar)
{
	return SerialUIStreamBaseType::parseInt(skipChar);
}
unsigned long SUIStream::timeout() { return timeout_ms; }
void SUIStream::setTimeout(unsigned long timeout) { timeout_ms = timeout ; streamDelegate->setTimeout(timeout); }

unsigned long SUIStream::parseULong()
{
	return parseULong(NO_SKIP_CHAR);
}
unsigned long SUIStream::parseULong(char skipChar)
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
		read(); // consume the character we got with peek
		c = timedPeek();
	} while ((c >= '0' && c <= '9') || c == skipChar);


	return value;

}


unsigned long SUIStream::parseIntHex()
{
	return parseIntHex(NO_SKIP_CHAR);
}

unsigned long SUIStream::parseIntHex(char skipChar)
{
	uint32_t value = 0;
	int c;

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

		read(); // consume the character we got with peek
		c = timedPeek();
	} while ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f') || c == skipChar);


	return value;

}

int SUIStream::timedPeek()
{
  int c;
  unsigned long startMillis = millis();
  do {
    c = peek();
    if (c >= 0) return c;
  } while(millis() - startMillis < timeout());
  return -1;     // -1 indicates timeout
}
int SUIStream::peekNextDigit(bool includeHex)
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
    read();  // discard non-numeric
  }
  return -1;
}

}

#endif /* SUI_BASEIMPLEMENTATION_STANDARD */

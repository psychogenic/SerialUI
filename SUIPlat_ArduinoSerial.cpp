/*
 * SUIPlat_Arduino.cpp
 *
 *  Created on: 2013-12-13
 *      Author: malcalypse
 */


#include "includes/SUIConfig.h"

#ifdef SUI_PLATFORM_ARDUINOSERIAL
#include "includes/SUIPlat_ArduinoSerial.h"



// SUI is the namespace in which we keep all our goodies.
namespace SUI {


unsigned long SUIStream::timeout() { return timeout_ms; }
void SUIStream::setTimeout(unsigned long timeout) { timeout_ms = timeout ; Stream::setTimeout(timeout); }
#define NO_SKIP_CHAR  1


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
int SUIStream::peekNextDigit()
{
  int c;
  while (1) {
    c = timedPeek();
    if (c < 0) return c;  // timeout
    if (c == '-') return c;
    if (c >= '0' && c <= '9') return c;
    read();  // discard non-numeric
  }
  return -1;
}


} /* namespace SUI */
#endif

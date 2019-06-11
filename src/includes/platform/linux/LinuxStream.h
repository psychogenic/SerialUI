/*
 * LinuxStream.h
 *
 *  Created on: Apr 27, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */

#ifndef SERIALUI_SRC_INCLUDES_PLATFORM_LINUX_LINUXSTREAM_H_
#define SERIALUI_SRC_INCLUDES_PLATFORM_LINUX_LINUXSTREAM_H_

/*
  Stream.h - base class for character-based streams.
  Copyright (c) 2010 David A. Mellis.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  parsing functions based on TextFinder library by Michael Margolis
*/

#include "LinuxExtIncludes.h"

#include "LinuxPrint.h"


// compatability macros for testing
/*
#define   getInt()            parseInt()
#define   getInt(skipChar)    parseInt(skipchar)
#define   getFloat()          parseFloat()
#define   getFloat(skipChar)  parseFloat(skipChar)
#define   getString( pre_string, post_string, buffer, length)
readBytesBetween( pre_string, terminator, buffer, length)
*/

class Stream : public Print
{
  private:
    unsigned long _timeout;      // number of milliseconds to wait for the next char before aborting timed read
    unsigned long _startMillis;  // used for timeout measurement
    int timedRead();    // private method to read stream with timeout
    int timedPeek();    // private method to peek stream with timeout
    int peekNextDigit(); // returns the next numeric digit in the stream or -1 if timeout

  public:
    virtual int available() = 0;
    virtual int read() = 0;
    virtual int peek() = 0;
    virtual void flush() = 0;

    Stream() : _timeout(1000), _startMillis(0) {}
    virtual ~Stream() {}

// parsing methods

    virtual void setTimeout(unsigned long timeout);  // sets maximum milliseconds to wait for stream data, default is 1 second

    virtual  bool find(char *target);   // reads data from the stream until the target string is found
  // returns true if target string is found, false if timed out (see setTimeout)

    virtual  bool find(char *target, size_t length);   // reads data from the stream until the target string of given length is found
  // returns true if target string is found, false if timed out

    virtual bool findUntil(char *target, char *terminator);   // as find but search ends if the terminator string is found

    virtual bool findUntil(char *target, size_t targetLen, char *terminate, size_t termLen);   // as above but search ends if the terminate string is found


    virtual long parseInt(); // returns the first valid (long) integer value from the current position.
  // initial characters that are not digits (or the minus sign) are skipped
  // integer is terminated by the first character that is not a digit.

    virtual float parseFloat();               // float version of parseInt

    virtual size_t readBytes( char *buffer, size_t length); // read chars from stream into buffer
  // terminates if length characters have been read or timeout (see setTimeout)
  // returns the number of characters placed in the buffer (0 means no valid data found)

    virtual size_t readBytesUntil( char terminator, char *buffer, size_t length); // as readBytes with terminator character
  // terminates if length characters have been read, timeout, or if the terminator character  detected
  // returns the number of characters placed in the buffer (0 means no valid data found)

  // Arduino String functions to be added here
    virtual String readString();
    virtual String readStringUntil(char terminator);

  protected:
  long parseInt(char skipChar); // as above but the given skipChar is ignored
  // as above but the given skipChar is ignored
  // this allows format characters (typically commas) in values to be ignored

  float parseFloat(char skipChar);  // as above but the given skipChar is ignored
};




#endif /* SERIALUI_SRC_INCLUDES_PLATFORM_LINUX_LINUXSTREAM_H_ */

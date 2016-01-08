/*
 * Desktop.h
 *
 *  Created on: Jan 7, 2016
 *      Author: Pat Deegan
 *      Part of the SerialUI Project
 *      Copyright (C) 2015 Pat Deegan, http://psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_PLATFORM_DESKTOP_H_
#define SERIALUI_SRC_INCLUDES_PLATFORM_DESKTOP_H_

#include "../SUIConfig.h"

#ifdef PLATFORM_DESKTOP

#define PGM_P	const char *

#define SUI_CONVERT_FLOAT_TO_STRING_AND_RETLEN(fl, intoptr) 0





// SUI is the namespace in which we keep all our goodies.
namespace SUI {


class DummyStream {
public:
	DummyStream() {}
	virtual ~DummyStream() {}
		virtual int available() {  return 0;}
	    virtual int read() { return 0; }
	    virtual int peek() { return 0; }
	    virtual void flush() { }
	    virtual size_t write(uint8_t i) { return 1;}
	    virtual void begin(unsigned long speed) {}
};
typedef DummyStream SerialUIStreamBaseType;

// used the standard base implementation of the SUIStream
#define SUI_BASEIMPLEMENTATION_STANDARD
#define SUI_SUISTREAM_NEEDSVIRTUAL

typedef DummyStream StreamInstanceType;


/*
 * StreamImplementation -- wraps all the uses of Serial in a single place,
 * to ease implementation of different types of comm.
 * All you need are the following 7 methods to behave like Serial (HardwareSerial.h)
 * methods and the rest should just work.
 */
class StreamImplementation {

public:
	static void setStream(StreamInstanceType * strm);
	static inline int available() {  return stream_to_use->available();}
    static inline int read() { return stream_to_use->read(); }
    static inline int peek() { return stream_to_use->peek(); }
    static inline void flush() { stream_to_use->flush(); }
    static inline size_t write(uint8_t i) { return stream_to_use->write(i); }
    static void begin(unsigned long speed) ;

private:
    static StreamInstanceType * stream_to_use;
    static bool stream_to_use_override;


};



} /* end namespace SUI */

#endif


#endif /* SERIALUI_SRC_INCLUDES_PLATFORM_DESKTOP_H_ */

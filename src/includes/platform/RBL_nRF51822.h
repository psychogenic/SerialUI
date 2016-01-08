/*
 * SUIPlat_nRF51822.h
 *
 *  Created on: Jan 7, 2016
 *      Author: Pat Deegan
 *      Part of the SerialUI Project
 *      Copyright (C) 2015 Pat Deegan, http://psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_SUIPLAT_NRF51822_H_
#define SERIALUI_SRC_INCLUDES_SUIPLAT_NRF51822_H_

#include "../SUIConfig.h"

#ifdef SUI_PLATFORM_RBLNRF51822

// system/avr includes
#include <inttypes.h>


#define SUI_CONVERT_FLOAT_TO_STRING_AND_RETLEN(fl, intoptr)	sprintf(intoptr, "%.2f", fl)


#ifndef PLATFORM_DESKTOP
#include <avr/pgmspace.h>
#include "Arduino.h"



// Seems all the *_P() functions
// are simply defined as their regular equivalents, but
// there's no strncpy_P and strncmp_P defined, so we
// create aliases in that case:
#ifndef strncpy_P
#define strncpy_P(to, from, s)		strncpy(to, from, s)
#endif

#ifndef strncmp_P
#define strncmp_P(a, b, s)			strncmp(a,b,s)
#endif


#endif


// SUI is the namespace in which we keep all our goodies.
namespace SUI {

// used the standard base implementation of the SUIStream
#define SUI_BASEIMPLEMENTATION_STANDARD
#define SUI_SUISTREAM_NEEDSVIRTUAL
typedef WStream SerialUIStreamBaseType;
typedef WStream StreamInstanceType;


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


#endif /* SUI_PLATFORM_RBLNRF51822 */

#endif /* SERIALUI_SRC_INCLUDES_SUIPLAT_NRF51822_H_ */

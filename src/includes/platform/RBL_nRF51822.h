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

#include "../stream/delegate/DelegateDirect.h"


#define PLATFORM_NOW_MILLIS()		millis()

#define SUI_FLASHSTRING_PTR		const __FlashStringHelper*



#define SUI_CONVERT_FLOAT_TO_STRING_AND_RETLEN(fl, intoptr)	sprintf(intoptr, "%.2f", fl)


#ifndef PLATFORM_DESKTOP

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



// typedef WStream SerialUIStreamBaseType;
typedef WStream SerialUIUnderlyingStreamType;


// used the standard base implementation of the SUIStream
#define SUI_BASEIMPLEMENTATION_STANDARD


typedef SUI::Delegate::Direct<SerialUIUnderlyingStreamType> StreamDelegate;



} /* end namespace SUI */


#endif /* SUI_PLATFORM_RBLNRF51822 */

#endif /* SERIALUI_SRC_INCLUDES_SUIPLAT_NRF51822_H_ */

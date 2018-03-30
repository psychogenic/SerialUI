/*
 *
 * SovAConfig.h -- SovA compile time configuration.
 * Copyright (C) 2013-2016 Pat Deegan.  All rights reserved.
 *
 * http://www.flyingcarsandstuff.com/projects/SerialUI/
 *
 * Please let me know if you use SerialUI/SovA in your projects, and
 * provide a URL if you'd like me to link to it from the SovA
 * home.
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
 *  ************************* OVERVIEW *************************
 *  This file is used to determine compile-time settings, for
 *  optimizations and determining which resource files to use
 *  (e.g. for language strings).
 *
 *  See below for available flags.
 */
#ifndef SOVA_CONFIGURATION_H_
#define SOVA_CONFIGURATION_H_


#define SOVA_DEFAULT_TIMEOUT_VALUE_MS		2000


/* *********************** Platform and auto-detection thereof **************** */

/*
 * SOVA_PLATFORM_X
 *
 * As of version 2, SovA does what it can to auto-detect the
 * platform you're building for.  If you're not using the Arduino IDE,
 * or not using a Makefile that includes the standard defines for all
 * the platform, you may need to _hardcode_ a platform, otherwise
 * detection should work.
 *
 * To hardcode a platform, define ONE OF the available
 * SOVA_PLATFORM_X values to select the serial implementation platform.
 *
 * SOVA_PLATFORM_ARDUINO_AVR: regular Arduino/Arduino-compatible.
 *  Is the default and will usually just work(tm)
 *
 * SOVA_PLATFORM_XMEGA: Xmegaduino, XMEGA-based devices... for some reason,
 * this needs to be #defined with the current release of xmegaduino...?
 *
 * SOVA_PLATFORM_ARDUINO_SAM:	Arduino Due and such
 *
 * SOVA_PLATFORM_RBLNRF51822: nRF51822-centered Arduino SDK, by
 * RedBearLab, for things like the BLE Nano http://redbearlab.com/blenano/
 *
 * SOVA_PLATFORM_ESP8266: ESP8266 devices, like the Olimex MOD-WIFI and the
 * Sparkfun ESP Thing, etc.
 *
 *
 * SOVA_PLATFORM_DIGISPARKUSB:  DigiSpark, needs testing/work.
 *
 * NOTE: for SOVA_PLATFORM_DIGISPARKUSB, I've had to add a
 * 	#include <DigiUSB.h>
 * at the top of my sketch to get it to compile at all...
 */
// NOTE: define only 1, and ONLY if auto-detect doesn't work...
// #define SOVA_PLATFORM_ARDUINO_AVR
// #define SOVA_PLATFORM_ARDUINO_SAM
// #define SOVA_PLATFORM_RBLNRF51822
// #define SOVA_PLATFORM_DIGISPARKUSB
// #define SOVA_PLATFORM_XMEGA
// #define SOVA_PLATFORM_ESP8266
// #define SOVA_PLATFORM_WIRINGPI

//define SOVA_PLATFORM_XMEGA
// Auto-detection stuff...

#define SOVA_USESOVASTREAM_IMPLEMENTATION

//#define SOVA_DEBUGSERIALPORT	Serial4

// most standard arduinos
#if not defined(ARDUINO_ARCH_AVR)
#include "platform/AVRDetection.h"
#endif


#if defined(ARDUINO_ARCH_AVR) \
	or defined (SOVA_PLATFORM_ARDUINO_AVR) \
	or defined (SOVA_ATMEGA_AUTODECTED)

	#define SOVA_ARCH_AVR

	#ifndef SOVA_PLATFORM_ARDUINO_AVR
		#define SOVA_PLATFORM_AUTODETECTED
		#define SOVA_PLATFORM_ARDUINO_AVR
	#endif
	#ifndef SOVA_BUILDFOR_ARDUINO_STANDARD
		#define SOVA_BUILDFOR_ARDUINO_STANDARD
	#endif
#endif

#ifndef SOVA_ARCH_AVR
#include "platform/XMegaDetection.h"
	// XMEGADUINO
	#if defined(ARDUINO_ARCH_XMEGA) or defined(SOVA_PLATFORM_XMEGA) \
		or defined(SOVA_XMEGA_AUTODECTED)

		#define SOVA_ARCH_AVR

		#ifndef SOVA_PLATFORM_XMEGA
			#define SOVA_PLATFORM_AUTODETECTED
			#define SOVA_PLATFORM_XMEGA
		#endif
		#define SOVA_BUILDFOR_ARDUINO_STANDARD
	#endif
#endif



#if defined(ARDUINO_ARCH_ESP8266) or defined(ARDUINO_MOD_WIFI_ESP8266) \
	or defined(ESP8266) or defined(SOVA_PLATFORM_ESP8266)
	#ifndef SOVA_PLATFORM_ESP8266
		#define SOVA_PLATFORM_AUTODETECTED
		#define SOVA_PLATFORM_ESP8266
	#endif
	#define SOVA_BUILDFOR_ARDUINO_STANDARD
	#define SOVA_ARCH_ESP8266
#endif


// BLE Nano
#if defined(ARDUINO_RBL_nRF51822) or defined(ARDUINO_ARCH_NRF51822)
	#ifndef SOVA_PLATFORM_RBLNRF51822
		#define SOVA_PLATFORM_AUTODETECTED
		#define SOVA_PLATFORM_RBLNRF51822
	#endif /* SOVA_PLATFORM_RBLNRF51822 */
	#define SOVA_ARCH_NRF51822
#endif /* RBL - NRF* platform */


// DUE
#if defined(ARDUINO_SAM_DUE) or defined(ARDUINO_ARCH_SAM) or defined(SOVA_PLATFORM_ARDUINO_SAM)
	#ifndef SOVA_PLATFORM_ARDUINO_SAM
		#define SOVA_PLATFORM_ARDUINO_SAM
		#define SOVA_PLATFORM_AUTODETECTED
		// is actually just standard Arduino +
		// the SOVA_ARCH_SAM flag
		#define SOVA_BUILDFOR_ARDUINO_STANDARD
		/* SOVA_ARCH_SAM
		*/
		#define SOVA_ARCH_SAM
	#endif
#endif

// SOVA_PLATFORM_WIRINGPI
// TODO: FIXME...

// Default (to AVR standard Arduino)
#if ! ( \
		   defined(SOVA_PLATFORM_WIRINGPI) \
		or defined(SOVA_PLATFORM_RBLNRF51822) \
		or defined(SOVA_PLATFORM_ARDUINO_SAM) \
		or defined(SOVA_PLATFORM_XMEGA) \
		or defined(SOVA_PLATFORM_DIGISPARKUSB) \
		or defined(SOVA_PLATFORM_ESP8266) \
		or defined(SOVA_PLATFORM_ARDUINO_AVR) \
		or defined(PLATFORM_DESKTOP))
	#error "Could not auto-detect platform... define an appropriate SOVA_PLATFORM_ARDUINO_* in SovAConfig.h"
#endif /* One SOVA_PLATFORM_* defined */



#ifdef PLATFORM_DESKTOP
#define SOVA_STREAMDELEGATE_DUMMY_ENABLE
#endif

// Shorthand for Digispark implementation flags
// attempting to reduce code size as
// much as possible (development/currently
// non-functional)
// #define SOVA_BUILD_FOR_DIGISPARK

#ifdef SOVA_BUILD_FOR_DIGISPARK
#warning "Building for digispark--undef'ing most features"
#undef SOVA_INCLUDE_EXTRA_SAFETYCHECKS
#undef SOVA_INCLUDE_DEBUG
#undef SOVA_SOVA_ECHO_WARNINGS
#undef SOVA_MENU_INCLUDE_DESTRUCTION_CLEANUP
#undef SOVA_MENU_ENABLE_SUBMENUS
#undef SOVA_ENABLE_MODES

#undef SOVA_BUILDFOR_ARDUINO_STANDARD
#define SOVA_PLATFORM_DIGISPARKUSB
#endif



#ifdef SOVA_INCLUDE_DEBUG
#warning "SOVA_INCLUDE_DEBUG is enabled"
#endif



// SOVA_PROGMEM_STRING_ABS_MAXLEN
// only used when converting PGM_P to printables...
// only applies to standard AVR arduino and,
// normally, you should be using the F() macro instead
// of this stuff anyway
#define SOVA_PROGMEM_STRING_ABS_MAXLEN		127

#define SOVA_PIPE_BUFFER_SIZE				50




#define SOVA_VERSION			1
#define SOVA_SUBVERSION			2
#define SOVA_PATCHLEVEL			2



#define SOVAXSTR(s) SOVASTR(s)
#define SOVASTR(s) 	#s


#define SOVA_VERSION_TOSTRING(V, SV)		V "." SV
#define SOVA_VERSION_STRING	SOVA_VERSION_TOSTRING(SOVAXSTR(SOVA_VERSION), SOVAXSTR(SOVA_SUBVERSION))

#define SOVA_VERSION_AT_LEAST(v, sub)  ( ((SOVA_SUBVERSION >= sub) and (SOVA_VERSION >= v)) or \
											(SOVA_VERSION > v))


#endif /* SOVA_CONFIGURATION_H_ */

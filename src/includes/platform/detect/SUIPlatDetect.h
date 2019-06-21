/*
 *
 *  ************************* OVERVIEW *************************
 *  This file is used to determine compile-time settings.
 *
 *  See below for available flags.
 */
#ifndef SUI_PLAT_DETECT_H_
#define SUI_PLAT_DETECT_H_


/* *********************** Platform and auto-detection thereof **************** */

/*
 * SERIALUI_PLATFORM_X
 *
 * As of version 2, SovA does what it can to auto-detect the
 * platform you're building for.  If you're not using the Arduino IDE,
 * or not using a Makefile that includes the standard defines for all
 * the platform, you may need to _hardcode_ a platform, otherwise
 * detection should work.
 *
 * To hardcode a platform, define ONE OF the available
 * SERIALUI_PLATFORM_X values to select the serial implementation platform.
 *
 * SERIALUI_PLATFORM_ARDUINO_AVR: regular Arduino/Arduino-compatible.
 *  Is the default and will usually just work(tm)
 *
 * SERIALUI_PLATFORM_XMEGA: Xmegaduino, XMEGA-based devices... for some reason,
 * this needs to be #defined with the current release of xmegaduino...?
 *
 * SERIALUI_PLATFORM_ARDUINO_SAM:	Arduino Due and such
 *
 * SERIALUI_PLATFORM_RBLNRF51822: nRF51822-centered Arduino SDK, by
 * RedBearLab, for things like the BLE Nano http://redbearlab.com/blenano/
 *
 * SERIALUI_PLATFORM_ESP8266: ESP8266 devices, like the Olimex MOD-WIFI and the
 * Sparkfun ESP Thing, etc.
 *
 *
 * SERIALUI_PLATFORM_DIGISPARKUSB:  DigiSpark, needs testing/work.
 *
 * NOTE: for SERIALUI_PLATFORM_DIGISPARKUSB, I've had to add a
 * 	#include <DigiUSB.h>
 * at the top of my sketch to get it to compile at all...
 */
// NOTE: define only 1, and ONLY if auto-detect doesn't work...
// #define SERIALUI_PLATFORM_ARDUINO_AVR
// #define SERIALUI_PLATFORM_ARDUINO_SAM
// #define SERIALUI_PLATFORM_RBLNRF51822
// #define SERIALUI_PLATFORM_DIGISPARKUSB
// #define SERIALUI_PLATFORM_XMEGA
// #define SERIALUI_PLATFORM_ESP8266
// #define SERIALUI_PLATFORM_WIRINGPI

//define SERIALUI_PLATFORM_XMEGA
// Auto-detection stuff...

// most standard arduinos



#include "AVRDetection.h"
#if defined(ARDUINO_ARCH_AVR) or defined(SERIALUI_ATMEGA_AUTODECTED)
	#define SERIALUI_PLATFORM_ARDUINOSTANDARD
	#define SERIALUI_ARCH_AVR
	#define SERIALUI_PLATFORM_ARDUINO_AVR
#endif

#ifndef SERIALUI_ARCH_AVR
#include "XMegaDetection.h"
	// XMEGADUINO
	#if defined(ARDUINO_ARCH_XMEGA) or defined(SERIALUI_PLATFORM_XMEGA) \
		or defined(SERIALUI_XMEGA_AUTODECTED)
		#define SERIALUI_PLATFORM_ARDUINOSTANDARD
		#define SERIALUI_ARCH_AVR

		#ifndef SERIALUI_PLATFORM_XMEGA
			#define SERIALUI_PLATFORM_AUTODETECTED
			#define SERIALUI_PLATFORM_XMEGA
		#endif
	#endif
#endif



#if defined(ARDUINO_ARCH_ESP8266) or defined(ARDUINO_MOD_WIFI_ESP8266) \
	or defined(ESP8266) or defined(SERIALUI_PLATFORM_ESP8266)
	#ifndef SERIALUI_PLATFORM_ESP8266
		#define SERIALUI_PLATFORM_AUTODETECTED
		#define SERIALUI_PLATFORM_ESP8266
	#endif
	#define SERIALUI_PLATFORM_ARDUINOSTANDARD
	#define SERIALUI_ARCH_ESP8266
#endif

#if defined(ARDUINO_ARCH_ESP32) or defined(ESP32)
	#ifndef SERIALUI_PLATFORM_ESP32
		#define SERIALUI_PLATFORM_AUTODETECTED
		#define SERIALUI_PLATFORM_ESP32
	#endif
	#define SERIALUI_PLATFORM_ARDUINOSTANDARD
	#define SERIALUI_ARCH_ESP32
#endif


#if 0
XXXXX DEADBEEF XXXX -- support Arduino primo and sandeepmistry
https://github.com/sandeepmistry/arduino-nRF5, below
// BLE Nano
#if defined(ARDUINO_RBL_nRF51822) or defined(ARDUINO_ARCH_NRF51822) or \
	defined(ARDUINO_ARCH_NRF5) or defined(NRF52)
	#ifndef SERIALUI_PLATFORM_RBLNRF51822
		#define SERIALUI_PLATFORM_AUTODETECTED
		#define SERIALUI_PLATFORM_RBLNRF51822
	#endif /* SERIALUI_PLATFORM_RBLNRF51822 */
	#define SERIALUI_ARCH_NRF51822
#endif /* RBL - NRF* platform */



#if defined(ARDUINO_RBL_nRF52832)
	#ifndef SERIALUI_PLATFORM_RBLNRF52
		#define SERIALUI_PLATFORM_AUTODETECTED
		#define SERIALUI_PLATFORM_RBLNRF52
	#endif /* SERIALUI_PLATFORM_RBLNRF51822 */
	#define SERIALUI_PLATFORM_RBLNRF52
#endif /* RBL - NRF* platform */

#endif

#ifndef ARDUINO_RBL_nRF52832
	#if defined(ARDUINO_ARCH_NRF5) or defined(NRF52) or defined(NRF51)
		#ifndef SERIALUI_PLATFORM_NRF52
			#define SERIALUI_PLATFORM_AUTODETECTED
			#define SERIALUI_PLATFORM_NRF52
		#endif /* SERIALUI_PLATFORM_RBLNRF51822 */
		#define SERIALUI_PLATFORM_NRF52
	#endif
#endif




// DUE
#if defined(ARDUINO_SAM_DUE) or defined(ARDUINO_ARCH_SAM) or defined(SERIALUI_PLATFORM_ARDUINO_SAM)
	#ifndef SERIALUI_PLATFORM_ARDUINO_SAM
		#define SERIALUI_PLATFORM_ARDUINO_SAM
		#define SERIALUI_PLATFORM_AUTODETECTED
		// is actually just standard Arduino +
		// the SERIALUI_ARCH_SAM flag
		#define SERIALUI_PLATFORM_ARDUINOSTANDARD
		/* SERIALUI_ARCH_SAM
		*/
		#define SERIALUI_ARCH_SAM
	#endif
#endif

// SERIALUI_PLATFORM_WIRINGPI
// TODO: FIXME...

// Default (to AVR standard Arduino)
#if ! ( \
		   defined(SERIALUI_PLATFORM_WIRINGPI) \
		or defined(SERIALUI_PLATFORM_RBLNRF51822) \
		or defined(SERIALUI_PLATFORM_RBLNRF52) \
		or defined(SERIALUI_PLATFORM_NRF52) \
		or defined(SERIALUI_PLATFORM_ARDUINO_SAM) \
		or defined(SERIALUI_PLATFORM_XMEGA) \
		or defined(SERIALUI_PLATFORM_DIGISPARKUSB) \
		or defined(SERIALUI_PLATFORM_ESP8266) \
		or defined(SERIALUI_PLATFORM_ESP32) \
		or defined(SERIALUI_PLATFORM_ARDUINO_AVR) \
		or defined(PLATFORM_DESKTOP))
#if defined(__linux__)
	#define SERIALUI_PLATFORM_LINUX
	#define SERIALUI_ARCH_LINUX
#else
	#error "Could not auto-detect platform... define an appropriate SERIALUI_PLATFORM_ARDUINO_* in SovAConfig.h"
#endif

#endif /* One SERIALUI_PLATFORM_* defined */




#ifdef SERIALUI_INCLUDE_DEBUG
#warning "SERIALUI_INCLUDE_DEBUG is enabled"
#endif



#endif /* SERIALUI_CONFIGURATION_H_ */

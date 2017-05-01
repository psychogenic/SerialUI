/*
 *
 * SOVAPlatform.h -- SovA system platform-specifics.
 * Copyright (C) 2013-2017 Pat Deegan, psychogenic.com. All rights reserved.
 *
 *
 * http://www.flyingcarsandstuff.com/projects/SerialUI/
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
 *
 * ************************* OVERVIEW *************************
 *
 * Here we'll store all the platform-specific includes and structures.
 *
 */

#ifndef SOVA_PLATFORM_MAIN_H_
#define SOVA_PLATFORM_MAIN_H_

#include "SovAExtIncludes.h"
#include "platform/PlatformExtIncludes.h"
#include "platform/DefaultStream.h"
#include "SovAConfig.h"

#ifdef SOVA_PLATFORM_ESP8266
#include "platform/ESP8266.h"
#endif


#if defined(SOVA_PLATFORM_ARDUINO_AVR ) or defined(SOVA_PLATFORM_XMEGA)
#include "platform/ArduinoAVR.h"
#endif

#ifdef  SOVA_PLATFORM_ARDUINO_SAM
#include "platform/ArduinoSAM.h"
#endif

#ifdef SOVA_PLATFORM_DIGISPARKUSB
#include "platform/DigiUSB.h"
#endif




#ifdef SOVA_BUILDFOR_ARDUINO_STANDARD
// may get included by in addition to one
// of the above, in a few cases.
#include "platform/ArduinoStandard.h"
#endif







#ifdef SOVA_INCLUDE_DEBUG
#define SOVA_OUTPUT_DEBUG_DEFAULTSTREAM(...)		SOVA_PLATFORM_HARDWARESERIAL_DEFAULT.print(__VA_ARGS__)
#define SOVA_OUTPUTLN_DEBUG_DEFAULTSTREAM(...)		SOVA_PLATFORM_HARDWARESERIAL_DEFAULT.println(__VA_ARGS__)
#else
#define SOVA_OUTPUT_DEBUG_DEFAULTSTREAM(...)
#define SOVA_OUTPUTLN_DEBUG_DEFAULTSTREAM(...)

#endif




//if ( (SOVA_FLASHSTRING_PTR) != SOVA_PROGMEM_PTR)
#ifdef SOVA_PROGMEM_PTR
/*
#define PRINT_FLASHSTR(...)		print_P(__VA_ARGS__)
#define PRINTLN_FLASHSTR(...)	println_P(__VA_ARGS__)
#define STRLEN_FLASHSTR(...)	strlen_P(reinterpret_cast<const char *>(__VA_ARGS__))
#define STRCAT_FLASHSTR(...)	strcat_P(__VA_ARGS__)
#define STRCMP_FLASHSTR(...)	strcmp_P(__VA_ARGS__)
#define STRNCMP_FLASHSTR(...)	strncmp_P(__VA_ARGS__)
*/


#define PRINT_FLASHSTR(...)		print(__VA_ARGS__)
#define PRINTLN_FLASHSTR(...)	println(__VA_ARGS__)
#define STRLEN_FLASHSTR(...)	strlen_P(reinterpret_cast<const char *>(__VA_ARGS__))
#define STRCAT_FLASHSTR(a, b)	strcat_P(reinterpret_cast<char *>(a), reinterpret_cast<const char *>(b))
#define STRCMP_FLASHSTR(a, b)	strcmp_P(reinterpret_cast<const char *>(a), reinterpret_cast<const char *>(b))
#define STRNCMP_FLASHSTR(a, b, len)	strncmp_P(reinterpret_cast<const char *>(a), reinterpret_cast<const char *>(b), (len))
#define STRNCMP_FLASHSTR(a, b, len)	strncmp_P(reinterpret_cast<const char *>(a), reinterpret_cast<const char *>(b), (len))







#else
#define PRINT_FLASHSTR(...)		print(__VA_ARGS__)
#define PRINTLN_FLASHSTR(...)	println(__VA_ARGS__)
#define STRLEN_FLASHSTR(...)	strlen(reinterpret_cast<const char *>(__VA_ARGS__))
#define STRCAT_FLASHSTR(a, b)	strcat(reinterpret_cast<char *>(a), reinterpret_cast<const char *>(b))
#define STRCMP_FLASHSTR(a, b)	strcmp(reinterpret_cast<const char *>(a), reinterpret_cast<const char *>(b))
#define STRNCMP_FLASHSTR(a, b, len)	strncmp(reinterpret_cast<const char *>(a), reinterpret_cast<const char *>(b), (len))
#define STRNCMP_FLASHSTR(a, b, len)	strncmp(reinterpret_cast<const char *>(a), reinterpret_cast<const char *>(b), (len))

#endif




#ifdef __GNUC__
#define DEPRECATED __attribute__((deprecated))
#define DEPRECATED_MACRO	_Pragma ("GCC warning \"This is deprecated SovA usage...\"")
#elif defined(_MSC_VER)
#define DEPRECATED __declspec(deprecated)
#define DEPRECATED_MACRO
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED
#define DEPRECATED_MACRO
#endif



#endif /* SOVA_PLATFORM_MAIN_H_ */

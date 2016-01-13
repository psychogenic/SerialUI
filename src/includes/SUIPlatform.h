/*
 *
 * SUIPlatform.h -- SerialUI system platform-specifics.
 * Copyright (C) 2013 Pat Deegan.  All rights reserved.
 *
 *
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
 *
 * ************************* OVERVIEW *************************
 *
 * Here we'll store all the platform-specific includes and structures.
 *
 */

#ifndef SERIALUI_PLATFORM_MAIN_H_
#define SERIALUI_PLATFORM_MAIN_H_

#include "SUIConfig.h"
#include "SUIExtIncludes.h"
#include "platform/PlatformExtIncludes.h"
#include "platform/DefaultStream.h"

#ifdef SUI_PLATFORM_ESP8266
#include "platform/ESP8266.h"
#endif


#ifdef SUI_PLATFORM_ARDUINO_AVR
#include "platform/ArduinoAVR.h"
#endif

#ifdef  SUI_PLATFORM_ARDUINO_SAM
#include "platform/ArduinoSAM.h"
#endif

#ifdef SUI_PLATFORM_DIGISPARKUSB
#include "platform/DigiUSB.h"
#endif


#ifdef SUI_PLATFORM_RBLNRF51822
#include "platform/RBL_nRF51822.h"
#endif

#ifdef PLATFORM_DESKTOP

#include "platform/Desktop.h"

#endif



#ifdef SUI_BUILDFOR_ARDUINO_STANDARD
// may get included by in addition to one
// of the above, in a few cases.
#include "platform/ArduinoStandard.h"
#endif







#ifdef SUI_INCLUDE_DEBUG
#define SUI_OUTPUT_DEBUG_DEFAULTSTREAM(...)		SUI_PLATFORM_HARDWARESERIAL_DEFAULT.print(__VA_ARGS__)
#define SUI_OUTPUTLN_DEBUG_DEFAULTSTREAM(...)		SUI_PLATFORM_HARDWARESERIAL_DEFAULT.println(__VA_ARGS__)
#else
#define SUI_OUTPUT_DEBUG_DEFAULTSTREAM(...)
#define SUI_OUTPUTLN_DEBUG_DEFAULTSTREAM(...)

#endif




//if ( (SUI_FLASHSTRING_PTR) != SUI_PROGMEM_PTR)
#ifdef SUI_PROGMEM_PTR
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
#define DEPRECATED_MACRO	_Pragma ("GCC warning \"This is deprecated SerialUI usage...\"")
#elif defined(_MSC_VER)
#define DEPRECATED __declspec(deprecated)
#define DEPRECATED_MACRO
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED
#define DEPRECATED_MACRO
#endif



#endif /* SERIALUI_PLATFORM_MAIN_H_ */

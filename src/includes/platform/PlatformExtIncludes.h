/*
 * PlatformExtIncludes.h
 *
 *  Created on: Jan 8, 2016
 *      Author: Pat Deegan
 *      Part of the SerialUI Project
 *      Copyright (C) 2015 Pat Deegan, http://psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_PLATFORM_PLATFORMEXTINCLUDES_H_
#define SERIALUI_SRC_INCLUDES_PLATFORM_PLATFORMEXTINCLUDES_H_

#include "../SUIConfig.h"

#ifdef SUI_ARCH_AVR
#include <avr/pgmspace.h>
#endif

#ifdef SUI_ARCH_ESP8266
#include <pgmspace.h>
#endif

#include <Arduino.h>


#endif /* SERIALUI_SRC_INCLUDES_PLATFORM_PLATFORMEXTINCLUDES_H_ */

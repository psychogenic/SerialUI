/*
 * DefaultStream.h
 *
 *  Created on: Jan 8, 2016
 *      Author: Pat Deegan
 *      Part of SovA, supporting the SerialUI library
 *      Copyright (C) 2016-2017 Pat Deegan, http://psychogenic.com
 *
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
 */

#ifndef SOVA_SRC_INCLUDES_PLATFORM_DEFAULTSTREAM_H_
#define SOVA_SRC_INCLUDES_PLATFORM_DEFAULTSTREAM_H_

#include "../SovAConfig.h"

#if defined(SOVA_BUILDFOR_ARDUINO_STANDARD) \
	or defined(SOVA_PLATFORM_RBLNRF51822) \
	or defined(SOVA_PLATFORM_ESP8266) \
	or defined(SOVA_PLATFORM_DIGISPARKUSB) \
	or defined(PLATFORM_DESKTOP)

// default "Serial" implementation to use
#define SOVA_PLATFORM_HARDWARESERIAL_DEFAULT		Serial

#endif



#endif /* SOVA_SRC_INCLUDES_PLATFORM_DEFAULTSTREAM_H_ */

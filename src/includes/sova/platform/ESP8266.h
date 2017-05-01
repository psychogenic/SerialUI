/*
 * ESP8266.h
 *
 *  Created on: Jan 9, 2016
 *      Author: Pat Deegan
 *      Part of SovA, supporting the SerialUI library
 *      Copyright (C) 2016-2017 Pat Deegan, http://psychogenic.com
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

#ifndef SOVA_SRC_INCLUDES_PLATFORM_ESP8266_H_
#define SOVA_SRC_INCLUDES_PLATFORM_ESP8266_H_


// overrides for ESP8266
// #define SOVA_FLASHSTRING_PTR		const __FlashStringHelper*
#define SOVA_FLASHSTRING_PTR		const char *
#define SOVA_FLASHSTRING_DISABLE

#ifdef F
	#undef F
	#define F(arg)	(arg)
#endif


#define PLATFORM_NOW_MILLIS()		millis()





#endif /* SOVA_SRC_INCLUDES_PLATFORM_ESP8266_H_ */

/*
 * DefaultStream.h
 *
 *  Created on: Jan 8, 2016
 *      Author: Pat Deegan
 *      Part of the SerialUI Project
 *      Copyright (C) 2015 Pat Deegan, http://psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_PLATFORM_DEFAULTSTREAM_H_
#define SERIALUI_SRC_INCLUDES_PLATFORM_DEFAULTSTREAM_H_

#include "../SUIConfig.h"

#if defined(SUI_BUILDFOR_ARDUINO_STANDARD) \
	or defined(SUI_PLATFORM_RBLNRF51822) \
	or defined(SUI_PLATFORM_ESP8266) \
	or defined(SUI_PLATFORM_DIGISPARKUSB)

// default "Serial" implementation to use
#define SUI_PLATFORM_HARDWARESERIAL_DEFAULT		Serial

#endif



#endif /* SERIALUI_SRC_INCLUDES_PLATFORM_DEFAULTSTREAM_H_ */

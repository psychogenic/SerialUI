/*
 * SUIPlatnRF52Arduino.h
 *
 *  Created on: Oct 13, 2018
 *      Author: Pat Deegan
 *
 *  SUIPlatnRF52Arduino is part of the XYZPROJECT project.
 *  Copyright (C) 2018 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_PLATFORM_SUIPLATNRF52ARDUINO_H_
#define SERIALUI_SRC_INCLUDES_PLATFORM_SUIPLATNRF52ARDUINO_H_

#include <Arduino.h>

#include <BLEPeripheral.h>
#include <BLESerial.h>


extern BLESerial SUIBLESerialDev;

#define SERIALUI_CHANNELS_USE_STANDARDSTREAMS
#define SUI_PLATFORM_SOURCE_DEFAULT		SUIBLESerialDev

#define SUI_STR(...)	F(__VA_ARGS__)

#define SUI_PLATFORM_TIMENOW_MS()				millis()
#define SUI_PLATFORM_DELAY_MS(d)				delay(d)

namespace SerialUI {

#define SUI_FLASHSTRING		const __FlashStringHelper*
typedef const __FlashStringHelper* StaticString;
typedef const char * DynamicString;

#define SUI_STATICSTRING_IS_DISTINCT_TYPE
#define SUI_PLATFORM_STATICSTRING_LENGTH(s)		strlen(reinterpret_cast<const char *>(s))



namespace Comm {

typedef Stream SourceBase;
typedef Print SinkBase;
#ifdef DESKTOP_COMPILE
typedef GlobalSerialWrapper SourceType;
#else
typedef BLESerial SourceType;
#endif

}

}


#endif /* SERIALUI_SRC_INCLUDES_PLATFORM_SUIPLATNRF52ARDUINO_H_ */

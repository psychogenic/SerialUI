/*
 * SUIPlatnRF52Arduino.h
 *
 *  Created on: Oct 13, 2018
 *      Author: Pat Deegan
 *
 *  SUIPlatnRF52Arduino is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_PLATFORM_SUIPLATNRF52ARDUINO_H_
#define SERIALUI_SRC_INCLUDES_PLATFORM_SUIPLATNRF52ARDUINO_H_

#include <Arduino.h>


#define SERIALUI_AUTHENTICATOR_ENABLE



#define SERIALUI_MENUSTRUCTURE_NUMITEMS_ATSTARTUP	20
#define SERIALUI_MENUSTRUCTURE_NUMITEMS_GROWLIST	10

#define SERIALUI_TRACKEDVIEW_MAXNUM_STATES		8
#define SERIALUI_TRACKING_NUMITEMS_ATSTARTUP	10

#define SERIALUI_CHANNELS_USE_STANDARDSTREAMS
#define SUI_PLATFORM_SOURCE_DEFAULT		SUIBLESerialDev

#define SUI_STR(...)	F(__VA_ARGS__)

#define SUI_PLATFORM_TIMENOW_MS()				millis()
#define SUI_PLATFORM_DELAY_MS(d)				delay(d)

#include "nrf52/NRF52BLESerial.h"

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
typedef NRF52BLESerial SourceType;
#endif

}

}


#ifdef SERIALUI_ENABLE_DEBUG_OUTPUT
#define SERIALUI_BEGIN_DEBUGOUTCHANNEL(...)	Serial.begin(57600)
#define SERIALUI_DEBUG_OUT(...)		Serial.print( __VA_ARGS__ )
#define SERIALUI_DEBUG_OUTLN(...)	Serial.println( __VA_ARGS__ )
#endif





#endif /* SERIALUI_SRC_INCLUDES_PLATFORM_SUIPLATNRF52ARDUINO_H_ */

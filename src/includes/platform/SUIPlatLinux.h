/*
 * SUIPlatLinux.h
 *
 *  Created on: Apr 27, 2019
 *      Author: Pat Deegan
 *
 *  SUIPlatArduino is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_PLATFORM_SUIPLATLINUX_H_
#define SERIALUI_SRC_INCLUDES_PLATFORM_SUIPLATLINUX_H_

#include <Gobbledegook.h>


#include "linux/SUILinuxSupport.h"
#include "linux/LinuxBLESerial.h"

#define SERIALUI_CHANNELS_USE_STANDARDSTREAMS

#define SERIALUI_SERIALIZER_JSON_ENABLE
#define SERIALUI_PYTHONMODULES_SUPPORT_ENABLE

#define SERIALUI_MENUSTRUCTURE_NUMITEMS_ATSTARTUP	50
#define SERIALUI_MENUSTRUCTURE_NUMITEMS_GROWLIST	25

#define SERIALUI_TRACKEDVIEW_MAXNUM_STATES		10
#define SERIALUI_TRACKING_NUMITEMS_ATSTARTUP	10


#define SERIALUI_AUTHENTICATOR_ENABLE


#define SUI_PLATFORM_SOURCE_DEFAULT		_linBLESerial

#define SUI_STR(...)	(__VA_ARGS__)

#define SUI_PLATFORM_TIMENOW_MS()				millis()
#define SUI_PLATFORM_DELAY_MS(d)				delay(d)
#ifndef F
#define F(...)		(__VA_ARGS__)
#endif

namespace SerialUI {

#define SUI_FLASHSTRING		const char *
typedef const char * StaticString;
typedef const char * DynamicString;

// define SUI_STATICSTRING_IS_DISTINCT_TYPE
#define SUI_PLATFORM_STATICSTRING_LENGTH(s)		strlen(reinterpret_cast<const char *>(s))






namespace Comm {

typedef Stream SourceBase;
typedef Print SinkBase;
typedef LinuxBLESerial SourceType;


}

}

#ifdef SERIALUI_ENABLE_DEBUG_OUTPUT
#define SERIALUI_DEBUG_OUT(...)		std::cerr << __VA_ARGS__
#define SERIALUI_DEBUG_OUTLN(...)	std::cerr << __VA_ARGS__ << std::endl
#endif

#endif /* SERIALUI_SRC_INCLUDES_PLATFORM_SUIPLATLINUX_H_ */

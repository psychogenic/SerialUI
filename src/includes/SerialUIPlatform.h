/*
 * SerialUIPlatform.h
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  SerialUIPlatform is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUIV3_SRC_INCLUDES_SERIALUIPLATFORM_H_
#define SERIALUIV3_SRC_INCLUDES_SERIALUIPLATFORM_H_

#include "SerialUIConfig.h"

#include "platform/detect/SUIPlatDetect.h"


#ifdef SERIALUI_PLATFORM_ARDUINOSTANDARD
#include "platform/SUIPlatArduino.h"
#endif


#ifdef SERIALUI_PLATFORM_NRF52
#include "platform/SUIPlatNRF52Arduino.h"
#endif

#ifdef SERIALUI_PLATFORM_LINUX
#include "platform/SUIPlatLinux.h"
#endif

namespace SerialUI {


size_t staticStringLength(StaticString aKey);

bool staticStringMatch(StaticString aKey,
		DynamicString theTest,
		bool allowPartials=true);
}

#ifndef SERIALUI_DEBUG_OUT
#define SERIALUI_DEBUG_OUT(...)
#endif

#ifndef SERIALUI_DEBUG_OUTLN
#define SERIALUI_DEBUG_OUTLN(...)
#endif

#endif /* SERIALUIV3_SRC_INCLUDES_SERIALUIPLATFORM_H_ */

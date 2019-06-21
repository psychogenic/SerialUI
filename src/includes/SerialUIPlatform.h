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
#ifdef SERIALUI_WARN_PLATFORM_DETECTION_DEBUG
#warning "PLATFORM: Arduino standard"
#endif /* SERIALUI_WARN_PLATFORM_DETECTION_DEBUG */
#include "platform/SUIPlatArduino.h"
#define SUI_PLATDETECT_SUCCESS
#endif


#ifdef SERIALUI_PLATFORM_NRF52
#ifdef SERIALUI_WARN_PLATFORM_DETECTION_DEBUG
#warning "PLATFORM: NRF52"
#endif /* SERIALUI_WARN_PLATFORM_DETECTION_DEBUG */
#include "platform/SUIPlatNRF52Arduino.h"
#define SUI_PLATDETECT_SUCCESS
#endif


#ifdef SERIALUI_PLATFORM_LINUX
#ifdef SERIALUI_WARN_PLATFORM_DETECTION_DEBUG
#warning "PLATFORM: Linux"
#endif /* SERIALUI_WARN_PLATFORM_DETECTION_DEBUG */
#include "platform/SUIPlatLinux.h"
#define SUI_PLATDETECT_SUCCESS
#endif

#ifndef SUI_PLATDETECT_SUCCESS
#warning "SERIALUI FAIL: Could not detect platform?"
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

#ifndef SERIALUI_BEGIN_DEBUGOUTCHANNEL
#define SERIALUI_BEGIN_DEBUGOUTCHANNEL(...)
#endif

#ifndef F
#define F(...)		__VA_ARGS__
#endif

#endif /* SERIALUIV3_SRC_INCLUDES_SERIALUIPLATFORM_H_ */

/*
 *
 * SUIPlattform.h -- SerialUI system platform-specifics.
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

#ifndef SUIPlatform_h
#define SUIPlatform_h

#include "SUIConfig.h"

#ifdef SUI_PLATFORM_ARDUINOSERIAL
#include "SUIPlat_ArduinoSerial.h"
#endif

#ifdef SUI_PLATFORM_DIGISPARKUSB
#include "SUIPlat_DigiUSB.h"
#endif

#endif /* SUIPlatform_H */

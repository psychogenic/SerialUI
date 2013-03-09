/*
 *
 * SUIPlat_ArduinoSerial.h -- SerialUI system platform-specifics.
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
 * Implementation for serial using DigiUSB -- NOT WORKING/compiling
 * too large for Digispark.
 *
 */
#include "includes/SUIConfig.h"

#ifdef SUI_PLATFORM_DIGISPARKUSB
// only include this stuff if we're actually using this platform...

#include "includes/SUIPlat_DigiUSB.h"

namespace SUI {
int StreamImplementation::peeked = -1;

int StreamImplementation::read() {
	int retval = -1;
	if (peeked != -1) {

		retval = peeked;
		peeked = -1;
	} else {
		retval = DigiUSB.read();
	}

	return retval;
}

int StreamImplementation::peek() {
	int retval = -1;

	if (peeked == -1)
	{

		peeked  = read();
	}

	return peeked;
}

void StreamImplementation::flush()
{
	peeked = -1;
	return;
}


}

#endif


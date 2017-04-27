
/*
 * deadbeef...
 * Copyright (C) 2016-2017 Pat Deegan, psychogenic.com.
 *
 * Part of SovA, released as a supporting component of SerialUI.
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
 */


#include "includes/sova/SovAConfig.h"

#ifdef SOVA_PLATFORM_DIGISPARKUSB
// only include this stuff if we're actually using this platform...

#include "includes/sova/includes/platform/DigiUSB.h"

namespace SovA {
int StreamDelegate::peeked = -1;

int StreamDelegate::read() {
	int retval = -1;
	if (peeked != -1) {

		retval = peeked;
		peeked = -1;
	} else {
		retval = DigiUSB.read();
	}

	return retval;
}

int StreamDelegate::peek() {
	int retval = -1;

	if (peeked == -1)
	{

		peeked  = read();
	}

	return peeked;
}

void StreamDelegate::flush()
{
	peeked = -1;
	return;
}


}

#endif


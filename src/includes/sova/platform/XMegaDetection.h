/*
 *
 * XMegaDetection.h -- SovA compile time configuration.
 * Copyright (C) 2017 Pat Deegan.  All rights reserved.
 *
 * http://www.flyingcarsandstuff.com/projects/SerialUI
 *
 * Please let me know if you use SerialUI/SovA in your projects, and
 * provide a URL if you'd like me to link to it from the SovA
 * home.
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
 */
#if defined(__AVR_ATxmega16A4__) \
	or defined(__AVR_ATxmega16A4U__) \
	or defined(__AVR_ATxmega16C4__) \
	or defined(__AVR_ATxmega16D4__) \
	or defined(__AVR_ATxmega32A4__) \
	or defined(__AVR_ATxmega32A4U__) \
	or defined(__AVR_ATxmega32C3__) \
	or defined(__AVR_ATxmega32C4__) \
	or defined(__AVR_ATxmega32D3__) \
	or defined(__AVR_ATxmega32D4__) \
	or defined(__AVR_ATxmega8E5__) \
	or defined(__AVR_ATxmega16E5__) \
	or defined(__AVR_ATxmega32E5__) \
	or defined(__AVR_ATxmega64A3__) \
	or defined(__AVR_ATxmega64A3U__) \
	or defined(__AVR_ATxmega64A4U__) \
	or defined(__AVR_ATxmega64B1__) \
	or defined(__AVR_ATxmega64B3__) \
	or defined(__AVR_ATxmega64C3__) \
	or defined(__AVR_ATxmega64D3__) \
	or defined(__AVR_ATxmega64D4__) \
	or defined(__AVR_ATxmega64A1__) \
	or defined(__AVR_ATxmega64A1U__) \
	or defined(__AVR_ATxmega128A3__) \
	or defined(__AVR_ATxmega128A3U__) \
	or defined(__AVR_ATxmega128B1__) \
	or defined(__AVR_ATxmega128B3__) \
	or defined(__AVR_ATxmega128C3__) \
	or defined(__AVR_ATxmega128D3__) \
	or defined(__AVR_ATxmega128D4__) \
	or defined(__AVR_ATxmega192A3__) \
	or defined(__AVR_ATxmega192A3U__) \
	or defined(__AVR_ATxmega192C3__) \
	or defined(__AVR_ATxmega192D3__) \
	or defined(__AVR_ATxmega256A3__) \
	or defined(__AVR_ATxmega256A3U__) \
	or defined(__AVR_ATxmega256A3B__) \
	or defined(__AVR_ATxmega256A3BU__) \
	or defined(__AVR_ATxmega256C3__) \
	or defined(__AVR_ATxmega256D3__) \
	or defined(__AVR_ATxmega384C3__) \
	or defined(__AVR_ATxmega384D3__) \
	or defined(__AVR_ATxmega128A1__) \
	or defined(__AVR_ATxmega128A1U__) \
	or defined(__AVR_ATxmega128A4U__)
	// gots ourselves an xmega!
	#define SOVA_XMEGA_AUTODECTED
#endif

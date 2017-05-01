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
#if defined(__AVR_ATmega603__) \
   or defined(__AVR_ATmega103__) \
   or defined(__AVR_ATmega8U2__) \
   or defined(__AVR_ATmega16U2__) \
   or defined(__AVR_ATmega32U2__) \
   or defined(__AVR_ATmega48__) \
   or defined(__AVR_ATmega48A__) \
   or defined(__AVR_ATmega48PA__) \
   or defined(__AVR_ATmega48P__) \
   or defined(__AVR_ATmega8__) \
   or defined(__AVR_ATmega8A__) \
   or defined(__AVR_ATmega8515__) \
   or defined(__AVR_ATmega8535__) \
   or defined(__AVR_ATmega88__) \
   or defined(__AVR_ATmega88A__) \
   or defined(__AVR_ATmega88P__) \
   or defined(__AVR_ATmega88PA__) \
   or defined(__AVR_ATmega8HVA__) \
   or defined(__AVR_ATmega16__) \
   or defined(__AVR_ATmega161__) \
   or defined(__AVR_ATmega162__) \
   or defined(__AVR_ATmega163__) \
   or defined(__AVR_ATmega164A__) \
   or defined(__AVR_ATmega164P__) \
   or defined(__AVR_ATmega164PA__) \
   or defined(__AVR_ATmega165__) \
   or defined(__AVR_ATmega165A__) \
   or defined(__AVR_ATmega165P__) \
   or defined(__AVR_ATmega165PA__) \
   or defined(__AVR_ATmega168__) \
   or defined(__AVR_ATmega168A__) \
   or defined(__AVR_ATmega168P__) \
   or defined(__AVR_ATmega168PA__) \
   or defined(__AVR_ATmega169__) \
   or defined(__AVR_ATmega169A__) \
   or defined(__AVR_ATmega169P__) \
   or defined(__AVR_ATmega169PA__) \
   or defined(__AVR_ATmega16A__) \
   or defined(__AVR_ATmega16HVA__) \
   or defined(__AVR_ATmega16HVA2__) \
   or defined(__AVR_ATmega16HVB__) \
   or defined(__AVR_ATmega16HVBREVB__) \
   or defined(__AVR_ATmega16M1__) \
   or defined(__AVR_ATmega16U4__) \
   or defined(__AVR_ATmega32__) \
   or defined(__AVR_ATmega32A__) \
   or defined(__AVR_ATmega323__) \
   or defined(__AVR_ATmega324A__) \
   or defined(__AVR_ATmega324P__) \
   or defined(__AVR_ATmega324PA__) \
   or defined(__AVR_ATmega325__) \
   or defined(__AVR_ATmega325A__) \
   or defined(__AVR_ATmega325P__) \
   or defined(__AVR_ATmega325PA__) \
   or defined(__AVR_ATmega3250__) \
   or defined(__AVR_ATmega3250A__) \
   or defined(__AVR_ATmega3250P__) \
   or defined(__AVR_ATmega3250PA__) \
   or defined(__AVR_ATmega328__) \
   or defined(__AVR_ATmega328P__) \
   or defined(__AVR_ATmega329__) \
   or defined(__AVR_ATmega329A__) \
   or defined(__AVR_ATmega329P__) \
   or defined(__AVR_ATmega329PA__) \
   or defined(__AVR_ATmega3290__) \
   or defined(__AVR_ATmega3290A__) \
   or defined(__AVR_ATmega3290P__) \
   or defined(__AVR_ATmega3290PA__) \
   or defined(__AVR_ATmega32C1__) \
   or defined(__AVR_ATmega32HVB__) \
   or defined(__AVR_ATmega32HVBREVB__) \
   or defined(__AVR_ATmega32M1__) \
   or defined(__AVR_ATmega32U4__) \
   or defined(__AVR_ATmega32U6__) \
   or defined(__AVR_ATmega406__) \
   or defined(__AVR_ATmega64RFR2__) \
   or defined(__AVR_ATmega644RFR2__) \
   or defined(__AVR_ATmega64__) \
   or defined(__AVR_ATmega64A__) \
   or defined(__AVR_ATmega640__) \
   or defined(__AVR_ATmega644__) \
   or defined(__AVR_ATmega644A__) \
   or defined(__AVR_ATmega644P__) \
   or defined(__AVR_ATmega644PA__) \
   or defined(__AVR_ATmega645__) \
   or defined(__AVR_ATmega645A__) \
   or defined(__AVR_ATmega645P__) \
   or defined(__AVR_ATmega6450__) \
   or defined(__AVR_ATmega6450A__) \
   or defined(__AVR_ATmega6450P__) \
   or defined(__AVR_ATmega649__) \
   or defined(__AVR_ATmega649A__) \
   or defined(__AVR_ATmega6490__) \
   or defined(__AVR_ATmega6490A__) \
   or defined(__AVR_ATmega6490P__) \
   or defined(__AVR_ATmega649P__) \
   or defined(__AVR_ATmega64C1__) \
   or defined(__AVR_ATmega64HVE__) \
   or defined(__AVR_ATmega64HVE2__) \
   or defined(__AVR_ATmega64M1__) \
   or defined(__AVR_ATmega128__) \
   or defined(__AVR_ATmega128A__) \
   or defined(__AVR_ATmega1280__) \
   or defined(__AVR_ATmega1281__) \
   or defined(__AVR_ATmega1284__) \
   or defined(__AVR_ATmega1284P__) \
   or defined(__AVR_ATmega128RFR2__) \
   or defined(__AVR_ATmega1284RFR2__) \
   or defined(__AVR_ATmega2560__) \
   or defined(__AVR_ATmega2561__) \
   or defined(__AVR_ATmega256RFR2__) \
   or defined(__AVR_ATmega2564RFR2__)
	// gots ourselves an ATmega!
	#define SOVA_ATMEGA_AUTODECTED
#endif

/*
 *
 * SOVAPlat_ArduinoAVR.h -- SovA system platform-specifics.
 * Copyright (C) 2013-2017 Pat Deegan, psychogenic.com. All rights reserved.
 *
 *
 * http://www.flyingcarsandstuff.com/projects/SerialUI/
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

#ifndef SOVA_SRC_INCLUDES_SOVAPLAT_ARDUINOSERIAL_H_
#define SOVA_SRC_INCLUDES_SOVAPLAT_ARDUINOSERIAL_H_
// specifically Arduino *AVR* (mega/xmega)


#ifdef SOVA_FLASHSTRING_PTR
#error "SOVA_FLASHSTRING_PTR already defined??"
#endif
#define SOVA_FLASHSTRING_PTR		const __FlashStringHelper*
#define SOVA_PROGMEM_PTR			PGM_P


#endif /* SOVA_SRC_INCLUDES_SOVAPLAT_ARDUINOSERIAL_H_ */

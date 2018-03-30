/*
 * SovAStreamStandard.cpp
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

#include "includes/sova/SovAPlatform.h"

#ifdef SOVA_BASEIMPLEMENTATION_STANDARD

#include "includes/sova/stream/SovAStream.h"
namespace SovA {


SovA::Stream::Stream() : Source(), Sink()  {
	dynDefaultDelegate = new SovA::StreamDelegate(&SOVA_PLATFORM_HARDWARESERIAL_DEFAULT);
	streamDelegate = dynDefaultDelegate;

}

SovA::Stream::Stream(Delegate::Interface * useDelegate) : streamDelegate(useDelegate), dynDefaultDelegate(NULL)
{
	if (! streamDelegate)
	{
		dynDefaultDelegate = new SovA::StreamDelegate(&SOVA_PLATFORM_HARDWARESERIAL_DEFAULT);
		streamDelegate = dynDefaultDelegate;

	}
}
SovA::Stream::~Stream() {
	if (dynDefaultDelegate)
	{
		delete dynDefaultDelegate;
	}
}

#ifdef DEADBEEF
#ifdef SOVA_PROGMEM_PTR
size_t SovA::Stream::print_P(SOVA_PROGMEM_PTR message) {
	char p_buffer[SOVA_PROGMEM_STRING_ABS_MAXLEN + 1];
	strncpy_P(p_buffer, message, SOVA_PROGMEM_STRING_ABS_MAXLEN);

	return this->print(p_buffer);

}


size_t SovA::Stream::println_P(SOVA_PROGMEM_PTR message) {
	char p_buffer[SOVA_PROGMEM_STRING_ABS_MAXLEN + 1];
	strncpy_P(p_buffer, message, SOVA_PROGMEM_STRING_ABS_MAXLEN);

	return this->println(p_buffer);


}
#endif
#endif


}

#endif /* SOVA_BASEIMPLEMENTATION_STANDARD */

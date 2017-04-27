/*
 * SovAStreamBase.cpp
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


#include "includes/sova/stream/SovAStream.h"


namespace SovA {

const char * Sink::endl  = "\r\n";
Sink & Sink::operator<<(Source & src) {

	uint8_t buf[SOVA_PIPE_BUFFER_SIZE];
	while (src.available())
	{
		this->write(buf, src.readBytes(buf, SOVA_PIPE_BUFFER_SIZE));
	}
	return *this;
}
Sink & Sink::operator<<(const Uint8Buffer & ubuf)
{

	this->write(ubuf.buf, ubuf.len);

	return *this;

}

Source & Source::operator>>(Sink & snk)
{

	snk << *this;

	return *this;
}

}




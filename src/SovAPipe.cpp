
/*
 * SovAPipe.cpp
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

#include "includes/sova/stream/SovAPipe.h"

namespace SovA {
namespace Pipe {


Flush::Flush(Source* src, Sink* sink, NumBytes maxBytesPerTick) :
		SovA::Pipe::Pipe(src, sink),
		max_bytes_per_tick(maxBytesPerTick)
{
}

void Flush::tick() {
	uint8_t buf[SOVA_PIPE_BUFFER_SIZE];
	size_t numRead = 0;
	size_t numToRequest;

	NumBytes numToTransfer = max_bytes_per_tick ? max_bytes_per_tick : 1;

	if (! data_source->available() )
		return;

	do {

		numToRequest = (numToTransfer < SOVA_PIPE_BUFFER_SIZE) ? numToTransfer : SOVA_PIPE_BUFFER_SIZE;
		numRead = data_source->readBytes(buf, numToRequest);

		if (numRead)
		{
			data_sink->write(buf, numRead);

			if (max_bytes_per_tick) {
				numToTransfer -= numRead;
			}
		}

	} while ( (numToTransfer > 0) && numRead && data_source->available());



}

}
}

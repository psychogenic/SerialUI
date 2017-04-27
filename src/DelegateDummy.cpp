
/*
 * DelegateDummy.cpp
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

#ifdef SOVA_STREAMDELEGATE_DUMMY_ENABLE
#include "includes/sova/platform/DefaultStream.h"
#include "includes/sova/stream/delegate/DelegateDummy.h"

//define SOVA_DUMMYDEL_DEBUG
#if defined(SOVA_DUMMYDEL_DEBUG) and defined(SOVA_INCLUDE_DEBUG)
#define SOVA_DUMMYDEL_OUTPUT_DEBUG(...)		SOVA_OUTPUT_DEBUG_DEFAULTSTREAM(__VA_ARGS__)
#define SOVA_DUMMYDEL_OUTPUTLN_DEBUG(...)		SOVA_OUTPUTLN_DEBUG_DEFAULTSTREAM(__VA_ARGS__)
#else

#define SOVA_DUMMYDEL_OUTPUT_DEBUG(...)
#define SOVA_DUMMYDEL_OUTPUTLN_DEBUG(...)
#endif

namespace SovA {
namespace Delegate {

void Dummy::begin(unsigned long speed) {
	SOVA_PLATFORM_HARDWARESERIAL_DEFAULT.begin(speed);
}

void Dummy::setTimeout(unsigned long timeout)
{

	SOVA_PLATFORM_HARDWARESERIAL_DEFAULT.setTimeout(timeout);

}

int Dummy::read() {
	bufferPendingReads();
	return this->DummyBufferedBase::read();
}
int Dummy::peek() {
	bufferPendingReads();
	return this->DummyBufferedBase::peek();
}

size_t Dummy::write(const uint8_t *buffer, size_t size) {
	if (writeQueue().size() > SOVA_DUMMYBUFFER_LOWWATERMARK) {
		flushPendingWrites();
	}
	return this->DummyBufferedBase::write(buffer, size);
}
size_t Dummy::write(uint8_t i) {

	if (writeQueueSize() > SOVA_DUMMYBUFFER_LOWWATERMARK) {
		flushPendingWrites();
	}
	SOVA_DUMMYDEL_OUTPUT_DEBUG('w');
	SOVA_DUMMYDEL_OUTPUT_DEBUG((int)writeQueueSize());
	return this->DummyBufferedBase::write(i);

}

void Dummy::tick(bool doFlush) {
	// SOVA_DUMMYDEL_OUTPUT_DEBUG('T');
	flushPendingWrites();
	bufferPendingReads();
}

void Dummy::bufferPendingReads() {
	while (SOVA_PLATFORM_HARDWARESERIAL_DEFAULT.available()
			&& readQueue().availableCapacity()) {
		readQueue().push_back(SOVA_PLATFORM_HARDWARESERIAL_DEFAULT.read());
	}
}
void Dummy::flushPendingWrites() {
	uint8_t writeBuf[(SOVA_DUMMYBUFFER_SIZE / 2) + 1];
	//memset(writeBuf, 0, (SOVA_DUMMYBUFFER_SIZE / 2) + 1);

	while (writeQueue().size()) {
		// SOVA_DUMMYDEL_OUTPUT_DEBUG("Dummy has writes: ");
		SOVA_DUMMYDEL_OUTPUTLN_DEBUG((int )writeQueue().size());
		SovA::Utils::BufferSize numXfered = writeQueue().transferTo(writeBuf,
				(SOVA_DUMMYBUFFER_SIZE / 2));
		if (numXfered) {
			SOVA_DUMMYDEL_OUTPUT_DEBUG("writing: ");
			SOVA_DUMMYDEL_OUTPUTLN_DEBUG((int)numXfered);

			// SOVA_PLATFORM_HARDWARESERIAL_DEFAULT.print((const char *)writeBuf);
			SOVA_PLATFORM_HARDWARESERIAL_DEFAULT.write(writeBuf, numXfered);
		} else {
			SOVA_DUMMYDEL_OUTPUTLN_DEBUG("Nothing xfered?");
		}

	}
}

}

}

#endif /* SOVA_STREAMDELEGATE_DUMMY_ENABLE */

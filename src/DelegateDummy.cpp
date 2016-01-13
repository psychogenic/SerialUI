/*
 * DelegateDummy.cpp
 *
 *  Created on: Jan 11, 2016
 *      Author: Pat Deegan
 *      Part of the SerialUI Project
 *      Copyright (C) 2015 Pat Deegan, http://psychogenic.com
 */

#include "includes/stream/delegate/DelegateDummy.h"

#ifdef SUI_STREAMDELEGATE_DUMMY_ENABLE
#include "includes/platform/DefaultStream.h"

//define SUI_DUMMYDEL_DEBUG
#if defined(SUI_DUMMYDEL_DEBUG) and defined(SUI_INCLUDE_DEBUG)
#define SUI_DUMMYDEL_OUTPUT_DEBUG(...)		SUI_OUTPUT_DEBUG_DEFAULTSTREAM(__VA_ARGS__)
#define SUI_DUMMYDEL_OUTPUTLN_DEBUG(...)		SUI_OUTPUTLN_DEBUG_DEFAULTSTREAM(__VA_ARGS__)
#else

#define SUI_DUMMYDEL_OUTPUT_DEBUG(...)
#define SUI_DUMMYDEL_OUTPUTLN_DEBUG(...)
#endif

namespace SUI {
namespace Delegate {

void Dummy::begin(unsigned long speed) {
	SUI_PLATFORM_HARDWARESERIAL_DEFAULT.begin(speed);
}

void Dummy::setTimeout(unsigned long timeout)
{

	SUI_PLATFORM_HARDWARESERIAL_DEFAULT.setTimeout(timeout);

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
	if (writeQueue().size() > SUI_DUMMYBUFFER_LOWWATERMARK) {
		flushPendingWrites();
	}
	return this->DummyBufferedBase::write(buffer, size);
}
size_t Dummy::write(uint8_t i) {

	if (writeQueueSize() > SUI_DUMMYBUFFER_LOWWATERMARK) {
		flushPendingWrites();
	}
	SUI_DUMMYDEL_OUTPUT_DEBUG('w');
	SUI_DUMMYDEL_OUTPUT_DEBUG((int)writeQueueSize());
	return this->DummyBufferedBase::write(i);

}

void Dummy::tick() {
	// SUI_DUMMYDEL_OUTPUT_DEBUG('T');
	flushPendingWrites();
	bufferPendingReads();
}

void Dummy::bufferPendingReads() {
	while (SUI_PLATFORM_HARDWARESERIAL_DEFAULT.available()
			&& readQueue().availableCapacity()) {
		readQueue().push_back(SUI_PLATFORM_HARDWARESERIAL_DEFAULT.read());
	}
}
void Dummy::flushPendingWrites() {
	uint8_t writeBuf[(SUI_DUMMYBUFFER_SIZE / 2) + 1];
	//memset(writeBuf, 0, (SUI_DUMMYBUFFER_SIZE / 2) + 1);

	while (writeQueue().size()) {
		// SUI_DUMMYDEL_OUTPUT_DEBUG("Dummy has writes: ");
		SUI_DUMMYDEL_OUTPUTLN_DEBUG((int )writeQueue().size());
		SUI::Utils::BufferSize numXfered = writeQueue().transferTo(writeBuf,
				(SUI_DUMMYBUFFER_SIZE / 2));
		if (numXfered) {
			SUI_DUMMYDEL_OUTPUT_DEBUG("writing: ");
			SUI_DUMMYDEL_OUTPUTLN_DEBUG((int)numXfered);

			// SUI_PLATFORM_HARDWARESERIAL_DEFAULT.print((const char *)writeBuf);
			SUI_PLATFORM_HARDWARESERIAL_DEFAULT.write(writeBuf, numXfered);
		} else {
			SUI_DUMMYDEL_OUTPUTLN_DEBUG("Nothing xfered?");
		}

	}
}

}

}

#endif /* SUI_STREAMDELEGATE_DUMMY_ENABLE */

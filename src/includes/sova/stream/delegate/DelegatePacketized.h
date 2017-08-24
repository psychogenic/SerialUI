/*
 * DelegatePacketized.h
 *
 *  Created on: Jan 12, 2016
 *      Author: Pat Deegan
 *      Part of SovA, supporting the SerialUI library
 *      Copyright (C) 2016-2017 Pat Deegan, http://psychogenic.com
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

#ifndef SOVA_SRC_INCLUDES_STREAM_DELEGATE_DELEGATEPACKETIZED_H_

#define SOVA_SRC_INCLUDES_STREAM_DELEGATE_DELEGATEPACKETIZED_H_


#include "../../SovAExtIncludes.h"
#include "DelegateBuffered.h"

namespace SovA {
namespace Delegate {

typedef uint8_t PacketByteType;
typedef uint8_t PacketBufferSize;

template<PacketBufferSize PACKETBUFFERSIZE, PacketBufferSize MTU>
class Packetized : public Buffered<PacketByteType, PACKETBUFFERSIZE> {
public:
	Packetized() : numNewlines(0) {}
	virtual ~Packetized() {}

	virtual void flush() { flushPendingWrites(true); };
	virtual void tick(bool doFlush=false) {flushPendingWrites(doFlush);}

    virtual size_t write(const uint8_t *buffer, size_t size);
    virtual size_t write(uint8_t i);

protected:

    // override this for when data needs to go out (note size will always
    // be <= MTU)
    virtual bool sendOutgoing(PacketByteType * buf, PacketBufferSize size) = 0;


    // use this when data comes in:
    virtual bool storeIncoming(PacketByteType * buf, PacketBufferSize size);


private:
    uint8_t numNewlines;
    void flushPendingWrites(bool force=false, uint8_t maxPackets=10);


};


#define PACKETDELEGATE_NONFORCE_MIN_PACKETSIZE(singlepacket)	(singlepacket/2)
#define PACKETDELEGATE_AUTO_TX_SIZE(bufsize)					((6*bufsize/7))

#define PACKETDELEGATE_DEBUG_ENABLE
#if defined(PACKETDELEGATE_DEBUG_ENABLE) and defined(SOVA_DEBUGSERIALPORT)
#define PACKETDELEGATE_DEBUG(...)		SOVA_DEBUGSERIALPORT.print(__VA_ARGS__)
#define PACKETDELEGATE_DEBUGLN(...)		SOVA_DEBUGSERIALPORT.println(__VA_ARGS__)
#define PACKETDELEGATE_DEBUG_BEGIN(baud) SOVA_DEBUGSERIALPORT.begin(baud)
#else
#define PACKETDELEGATE_DEBUG(...)
#define PACKETDELEGATE_DEBUGLN(...)
#define PACKETDELEGATE_DEBUG_BEGIN(baud)
#endif


// when we have pending writes in the queue, send
// in blocks of up to MTU, by calling
// sendOutgoing() for each
template<PacketBufferSize PACKETBUFFERSIZE, PacketBufferSize MTU>
void Packetized<PACKETBUFFERSIZE, MTU>::flushPendingWrites(bool force, uint8_t maxPackets) {

	uint8_t packetsSent = 0;
	PacketByteType tmpBuf[MTU + 1];

	if (! this->writeQueue().size())
		return;


	if ( (force && numNewlines) || (this->writeQueue().size() > (PACKETDELEGATE_AUTO_TX_SIZE(PACKETBUFFERSIZE)))) {

		PacketBufferSize minSize = 1; // force ?  1 : PACKETDELEGATE_NONFORCE_MIN_PACKETSIZE(MTU);
		if (this->writeQueue().size() < minSize)
			return;

		if (force)
		{
			PACKETDELEGATE_DEBUG("Forced flush...");
		}

		// we empty the thing... up to max packets
		while (this->writeQueue().size() >= minSize && packetsSent++ < maxPackets)
		{
			PACKETDELEGATE_DEBUG("Would like to send Q...");

#ifdef SOVA_CIRCBUF_LOCKING
		if (this->writeQueue().isLocked())
		{
			// wait 'til later
			PACKETDELEGATE_DEBUG("But locked, trying later...");
			return;

		}
#endif

			numNewlines = 0;

			SovA::Utils::BufferSize numXfered = this->writeQueue().transferTo(tmpBuf, MTU);
			if (numXfered)
			{
				PACKETDELEGATE_DEBUG("Sending ");
				PACKETDELEGATE_DEBUGLN((int)numXfered);

				tmpBuf[numXfered] = 0;
				PACKETDELEGATE_DEBUG(" \t");
				PACKETDELEGATE_DEBUGLN((const char*)tmpBuf);

				this->sendOutgoing(tmpBuf, numXfered);

			} else {

				PACKETDELEGATE_DEBUGLN("nutin'");
			}
		}

	}
}


// everytime data comes in, we push it onto our local read queue

template<PacketBufferSize PACKETBUFFERSIZE, PacketBufferSize MTU>
bool Packetized<PACKETBUFFERSIZE, MTU>::storeIncoming(PacketByteType * buf, PacketBufferSize size)
{
	if (size)
	{
		this->readQueue().fillFrom(buf, size);
		return true;
	}

	return false;
}


template<PacketBufferSize PACKETBUFFERSIZE, PacketBufferSize MTU>
size_t Packetized<PACKETBUFFERSIZE, MTU>::write(const uint8_t *buffer, size_t size) {

	PACKETDELEGATE_DEBUG("write...");

	if ((size + this->writeQueue().size()) > (PACKETBUFFERSIZE - 1))
	{
		size_t spaceLeft = (PACKETBUFFERSIZE - 1) - this->writeQueue().size();
		this->Buffered<PacketByteType, PACKETBUFFERSIZE>::write(buffer, spaceLeft);
		numNewlines++;
		flushPendingWrites(true, 10);

		for (size_t i=spaceLeft; i<size; i++)
		{
			this->write(buffer[i]);
		}

		return size;

	}

	return this->Buffered<PacketByteType, PACKETBUFFERSIZE>::write(buffer, size);
}
template<PacketBufferSize PACKETBUFFERSIZE, PacketBufferSize MTU>
size_t Packetized<PACKETBUFFERSIZE, MTU>::write(uint8_t i) {

	//PACKETDELEGATE_DEBUG("w");

	if (i == '\n' || i == '\r') {
		numNewlines++;
	}
	bool force = (this->writeQueue().size() > (PACKETBUFFERSIZE - 2));

	size_t rVal = this->Buffered<PacketByteType, PACKETBUFFERSIZE>::write(i);

	if (force)
			flushPendingWrites(force, 10);

	return rVal;

}



} /* namespace Delegate */
}/* namespace SovA */



#endif /* SOVA_SRC_INCLUDES_STREAM_DELEGATE_DELEGATEPACKETIZED_H_ */

/*
 * SovAPipe.h
 *
 *  Created on: Jan 15, 2016
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

#ifndef SRC_INCLUDES_STREAM_SOVAPIPE_H_

#define SRC_INCLUDES_STREAM_SOVAPIPE_H_

#include "../SovATypes.h"
#include "SovAStream.h"

namespace SovA {

namespace Pipe {

class Pipe {

public:
	Pipe(Source * src, Sink * sink) : data_source(src), data_sink(sink) {}
	virtual ~Pipe() {}

	virtual void tick() = 0;

protected:
	Source * data_source;
	Sink * data_sink;

};


class Flush : public SovA::Pipe::Pipe {
public:
	Flush(Source * src, Sink * sink, NumBytes maxBytesPerTick=0) ;
	virtual void tick();

private:
	NumBytes max_bytes_per_tick;
};


}

}


#endif /* SRC_INCLUDES_STREAM_SOVAPIPE_H_ */

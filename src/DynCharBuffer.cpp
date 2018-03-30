/*
 * DynCharBuffer.cpp
 *
 *  Created on: Mar 30, 2018
 *      Author: Pat Deegan
 *
 *  DynCharBuffer is part of the XYZPROJECT project.
 *  Copyright (C) 2018 Pat Deegan, psychogenic.com
 */

#include "includes/sova/utils/DynCharBuffer.h"

namespace SovA {
namespace Utils {

DynCharBuffer::DynCharBuffer(char * newBuffer) : bufptr(newBuffer)
{

}

DynCharBuffer::~DynCharBuffer() {

	clear();
}

void DynCharBuffer::clear() {
if (bufptr) {
			delete [] bufptr;
			bufptr = NULL;
		}
}


} /* namespace Utils */
} /* namespace SovA */

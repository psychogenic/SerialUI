/*
 * LinuxBLESerial.h
 *
 *  Created on: Apr 27, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */

#ifndef SERIALUI_SRC_INCLUDES_PLATFORM_LINUX_LINUXBLESERIAL_H_
#define SERIALUI_SRC_INCLUDES_PLATFORM_LINUX_LINUXBLESERIAL_H_
#include "LinuxStream.h"

namespace SerialUI {

class LinuxBLESerial : public Stream {
public:
	LinuxBLESerial();
	bool begin(uint32_t v) { return true;}
	virtual int available();
	virtual int read();
	virtual int peek();
	virtual void flush();
	virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *buffer, size_t size);
};


extern LinuxBLESerial _linBLESerial;

} /* namespace SerialUI */

#endif /* SERIALUI_SRC_INCLUDES_PLATFORM_LINUX_LINUXBLESERIAL_H_ */

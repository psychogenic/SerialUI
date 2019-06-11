/*
 * LinuxBLESerial.cpp
 *
 *  Created on: Apr 27, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */

#include "includes/SerialUIPlatform.h"

#ifdef SERIALUI_PLATFORM_LINUX

#include "includes/platform/linux/LinuxBLESerial.h"

namespace SerialUI {
LinuxBLESerial _linBLESerial;

LinuxBLESerial::LinuxBLESerial() {

}


int LinuxBLESerial::available() {
	return rx_buffer.getOccupied();
}
int LinuxBLESerial::read() {
	uint8_t nextChar;
	if (! rx_buffer.getOccupied()) {
		return -1;
	}
	rx_buffer.readNext(&nextChar);
	return nextChar;
}
int LinuxBLESerial::peek() {
	uint8_t nextChar;
	if (! rx_buffer.getOccupied()) {
		return -1;
	}
	rx_buffer.peekNext(&nextChar);
	return nextChar;

}
void LinuxBLESerial::flush() {

}
size_t LinuxBLESerial::write(uint8_t c) {
	if (! tx_buffer.getFree()) {
		return 0;
	}
	tx_buffer.write(&c, 1);
	return 1;
}
size_t LinuxBLESerial::write(const uint8_t *buffer, size_t size) {
	size_t spaceLeft = tx_buffer.getFree();
	if (! spaceLeft) {
		return 0;
	}
	if (size > spaceLeft) {
		size = spaceLeft;
	}
	return tx_buffer.write(buffer, size);

}

} /* namespace SerialUI */

#endif /* SERIALUI_PLATFORM_LINUX */

/*
 * NRF52BLESerial.cpp
 *
 *  Created on: May 12, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */



#include "includes/SerialUIPlatform.h"
#ifdef SERIALUI_PLATFORM_NRF52


#include "includes/platform/nrf52/NRF52BLESerial.h"

/*
#include <app_timer.h>
#include <RTCInt.h>

APP_TIMER_DEF(m_bleser_poll_timer_id);

*/


SerialUI::NRF52BLESerial	SUIBLESerialDev;



namespace SerialUI {

/*
void bleser_polltimer_handler(void* ignoreme) {
	SUIBLESerialDev.poll();
	SUIBLESerialDev.flush();
}
*/

#define NRF52BLESerialRXPACKETSIZE		20
#define NRF52BLESerialTXBUFSIZE			512
#define NRF52BLESerialTXBUFFLUSHSIZE	(NRF52BLESerialTXBUFSIZE - NRF52BLESerialRXPACKETSIZE)

NRF52BLESerial::NRF52BLESerial() : _bleser(), rx_buf(256), tx_buf(NRF52BLESerialTXBUFSIZE) ,
		last_fill_ms(0),
		last_flush_ms(0)

{
	/*
	app_timer_create(&m_bleser_poll_timer_id,
		                        APP_TIMER_MODE_REPEATED,
								bleser_polltimer_handler);
	*/
}

void NRF52BLESerial::setLocalName(const char * nm) {
	_bleser.setLocalName(nm);
}


void NRF52BLESerial::begin(...) {
	_bleser.begin();

	// app_timer_start(m_bleser_poll_timer_id, 100, NULL);


}

void NRF52BLESerial::poll() {
	fillRXFromBLESerial();
	flush();

}
void NRF52BLESerial::fillRXFromBLESerial() {
	uint8_t mybuf[32];
	uint32_t tNow = SUI_PLATFORM_TIMENOW_MS();
	if ((tNow - last_fill_ms) < 10) {
		return;
	}
	last_fill_ms = tNow;

		_bleser.poll();
		while(_bleser.available()) {

			uint8_t readIn = 0;
			while(readIn < 32 && _bleser.available()) {
				mybuf[readIn] = _bleser.read();
				// SERIALUI_DEBUG_OUT(F("RD: "));
				// SERIALUI_DEBUG_OUTLN((char)mybuf[readIn]);
				readIn++;
			}

			if (readIn) {
				rx_buf.write(mybuf, readIn);
			} else {
				return;
			}
		}

}

void NRF52BLESerial::end() {
	_bleser.end();
	// app_timer_stop(m_bleser_poll_timer_id);
}

int NRF52BLESerial::available(void) {
	poll();
	// fillRXFromBLESerial();
	return rx_buf.getOccupied();
}

int NRF52BLESerial::peek(void) {
	uint8_t next;
	fillRXFromBLESerial();
	if (! rx_buf.getOccupied()) {
		return -1;
	}
	rx_buf.peekNext(&next);
	return next;
}

int NRF52BLESerial::read(void) {
	uint8_t next;
	fillRXFromBLESerial();
	if (rx_buf.readNext(&next)) {

		// SERIALUI_DEBUG_OUT(F("RET: "));
		// SERIALUI_DEBUG_OUTLN((char)next);
		return next;
	}
	return -1;
}

void NRF52BLESerial::flush(void) {
	uint8_t inbuf[20];

	if (! tx_buf.getOccupied()) {

		return;
	}

	while (tx_buf.getOccupied()) {

		// memset(inbuf, 0, 1);

		size_t numread = tx_buf.read(inbuf, 20);
		for (size_t i=0; i< numread; i++) {
			_bleser.write(inbuf[i]);
		}

		delay(30);
		_bleser.flush();
		/*
		if (numread > 0) {
			_bleser.print((char*)inbuf);
			delay(30);
		}
		*/
	}

	_bleser.flush();

}

size_t NRF52BLESerial::write(uint8_t b) {
	uint32_t tNow = SUI_PLATFORM_TIMENOW_MS();
	if ( ((tNow - last_flush_ms) > 100) || (tx_buf.getOccupied() >= NRF52BLESerialTXBUFFLUSHSIZE))  {
		last_flush_ms = tNow;
		flush();
	}

	return tx_buf.addItem(b);
}

NRF52BLESerial::operator bool() {
	return (bool)(_bleser);
}

} /* namespace SerialUI */

#endif /* SERIALUI_PLATFORM_NRF52 */


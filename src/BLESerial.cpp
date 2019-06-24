
/*
 * SUIBLESerial.h
 * from https://github.com/sandeepmistry/arduino-BLEPeripheral

The MIT License (MIT)

Copyright (c) 2014-2016 Sandeep Mistry

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */


#include "includes/SerialUIPlatform.h"

#ifdef SERIALUI_PLATFORM_NRF52

#include "includes/platform/nrf52/BLESerial.h"
#include "includes/SUIGlobals.h"

// #define BLE_SERIAL_DEBUG

SUIBLESerial* SUIBLESerial::_instance = NULL;
static void RemoteSideDisconnected(BLECentral& central) {
	SerialUI::Globals::state()->setShouldExit(true);

}
SUIBLESerial::SUIBLESerial(unsigned char req, unsigned char rdy, unsigned char rst) :
  BLEPeripheral(req, rdy, rst)
{
  this->_txCount = 0;
  this->_rxHead = this->_rxTail = 0;
  this->_flushed = 0;
  SUIBLESerial::_instance = this;

  addAttribute(this->_uartService);
  addAttribute(this->_uartNameDescriptor);
  setAdvertisedServiceUuid(this->_uartService.uuid());
  addAttribute(this->_rxCharacteristic);
  addAttribute(this->_rxNameDescriptor);
  this->_rxCharacteristic.setEventHandler(BLEWritten, SUIBLESerial::_received);
  addAttribute(this->_txCharacteristic);
  addAttribute(this->_txNameDescriptor);
}

void SUIBLESerial::begin(...) {
  setEventHandler(BLEDisconnected, RemoteSideDisconnected);
  BLEPeripheral::begin();
  #ifdef BLE_SERIAL_DEBUG
    Serial.println(F("SUIBLESerial::begin()"));
  #endif
    //


}

void SUIBLESerial::poll() {
  if (millis() < this->_flushed + 100) {
    BLEPeripheral::poll();
  } else {
    flush();
  }
}

void SUIBLESerial::end() {
  this->_rxCharacteristic.setEventHandler(BLEWritten, NULL);
  this->_rxHead = this->_rxTail = 0;
  flush();
  BLEPeripheral::disconnect();
}

int SUIBLESerial::available(void) {
  BLEPeripheral::poll();
  int retval = (this->_rxHead - this->_rxTail + sizeof(this->_rxBuffer)) % sizeof(this->_rxBuffer);
  #ifdef BLE_SERIAL_DEBUG
    Serial.print(F("SUIBLESerial::available() = "));
    Serial.println(retval);
  #endif
  return retval;
}

int SUIBLESerial::peek(void) {
  BLEPeripheral::poll();
  if (this->_rxTail == this->_rxHead) return -1;
  uint8_t byte = this->_rxBuffer[ (this->_rxTail + 1) % sizeof(this->_rxBuffer)];
  #ifdef BLE_SERIAL_DEBUG
    Serial.print(F("SUIBLESerial::peek() = "));
    Serial.print((char) byte);
    Serial.print(F(" 0x"));
    Serial.println(byte, HEX);
  #endif
  return byte;
}

int SUIBLESerial::read(void) {
  BLEPeripheral::poll();
  if (this->_rxTail == this->_rxHead) return -1;
  this->_rxTail = (this->_rxTail + 1) % sizeof(this->_rxBuffer);
  uint8_t byte = this->_rxBuffer[this->_rxTail];
  #ifdef BLE_SERIAL_DEBUG
    Serial.print(F("SUIBLESerial::read() = "));
    Serial.print((char) byte);
    Serial.print(F(" 0x"));
    Serial.println(byte, HEX);
  #endif
  return byte;
}

void SUIBLESerial::flush(void) {
  if (this->_txCount == 0) return;
  this->_txCharacteristic.setValue(this->_txBuffer, this->_txCount);
  this->_flushed = millis();
  this->_txCount = 0;
  BLEPeripheral::poll();
  #ifdef BLE_SERIAL_DEBUG
    Serial.println(F("SUIBLESerial::flush()"));
  #endif
}

size_t SUIBLESerial::write(uint8_t byte) {
  BLEPeripheral::poll();
  if (this->_txCharacteristic.subscribed() == false) return 0;
  this->_txBuffer[this->_txCount++] = byte;
  if (this->_txCount == sizeof(this->_txBuffer)) flush();
  #ifdef BLE_SERIAL_DEBUG
    Serial.print(F("SUIBLESerial::write("));
    Serial.print((char) byte);
    Serial.print(F(" 0x"));
    Serial.print(byte, HEX);
    Serial.println(F(") = 1"));
  #endif
  return 1;
}

SUIBLESerial::operator bool() {
  bool retval = BLEPeripheral::connected();
  #ifdef BLE_SERIAL_DEBUG
    Serial.print(F("SUIBLESerial::operator bool() = "));
    Serial.println(retval);
  #endif
  return retval;
}

void SUIBLESerial::_received(const uint8_t* data, size_t size) {
  for (size_t i = 0; i < size; i++) {
    this->_rxHead = (this->_rxHead + 1) % sizeof(this->_rxBuffer);
    this->_rxBuffer[this->_rxHead] = data[i];
  }
  #ifdef BLE_SERIAL_DEBUG
    Serial.print(F("SUIBLESerial::received("));
    for (int i = 0; i < size; i++) Serial.print((char) data[i]);
    Serial.println(F(")"));
  #endif
}

void SUIBLESerial::_received(BLECentral& /*central*/, BLECharacteristic& rxCharacteristic) {
  SUIBLESerial::_instance->_received(rxCharacteristic.value(), rxCharacteristic.valueLength());
}


#endif /* SERIALUI_PLATFORM_NRF52 */

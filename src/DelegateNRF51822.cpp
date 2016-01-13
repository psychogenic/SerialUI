/*
 * DelegateNRF51822.cpp
 *
 *  Created on: Jan 11, 2016
 *      Author: Pat Deegan
 *      Part of the SerialUI Project
 *      Copyright (C) 2015 Pat Deegan, http://psychogenic.com
 */



#include "includes/stream/delegate/DelegateNRF51822.h"

#ifdef SUI_PLATFORM_RBLNRF51822

namespace SUI {
namespace Delegate {

static ::Timeout                                   wakeup_timeout;

// The Nordic UART Service
static const uint8_t rxtx_service_uuid[]                = {0x71, 0x3D, 0, 0, 0x50, 0x3E, 0x4C, 0x75, 0xBA, 0x94, 0x31, 0x48, 0xF1, 0x8D, 0x94, 0x1E};
static const uint8_t fromclient_uuid[]             		= {0x71, 0x3D, 0, 3, 0x50, 0x3E, 0x4C, 0x75, 0xBA, 0x94, 0x31, 0x48, 0xF1, 0x8D, 0x94, 0x1E};
static const uint8_t toclient_uuid[]             		= {0x71, 0x3D, 0, 2, 0x50, 0x3E, 0x4C, 0x75, 0xBA, 0x94, 0x31, 0x48, 0xF1, 0x8D, 0x94, 0x1E};
// static const uint8_t uart_base_uuid_rev[]           	= {0x1E, 0x94, 0x8D, 0xF1, 0x48, 0x31, 0x94, 0xBA, 0x75, 0x4C, 0x3E, 0x50, 0, 0, 0x3D, 0x71};

uint8_t fromclient_value[SUI_NRF_PACKET_MTU] = {0,};
uint8_t toclient_value[SUI_NRF_PACKET_MTU] = {0,};

GattCharacteristic  fromclient_characteristic(fromclient_uuid, fromclient_value, 1, SUI_NRF_PACKET_MTU, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE ); // | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE );

GattCharacteristic  toclient_characteristic(toclient_uuid, toclient_value, 1, SUI_NRF_PACKET_MTU, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY);

GattCharacteristic *uartChars[] = {&fromclient_characteristic, &toclient_characteristic};

GattService         uartService(rxtx_service_uuid, uartChars, sizeof(uartChars) / sizeof(GattCharacteristic *));





GattService & NRF51822::getService()
{
	return uartService;
}

GattCharacteristic ** NRF51822::getCharacteristics()
{
	return uartChars;
}


// everytime data comes in, we push it onto our local read queue
void NRF51822::handleReceivedData(const GattWriteCallbackParams *Handler)
{
    uint8_t buf[SUI_NRF_PACKET_MTU + 1] = {0,};
    uint16_t bytesRead = SUI_NRF_PACKET_MTU ;

	PACKETDELEGATE_DEBUG("rcvddata for ");
	PACKETDELEGATE_DEBUGLN(Handler->handle);
    if (Handler->handle == fromclient_characteristic.getValueAttribute().getHandle()) {
        // _ble->readCharacteristicValue(fromclient_characteristic.getValueAttribute().getHandle(), buf, &bytesRead);
        if (_ble->gattServer().read(fromclient_characteristic.getValueAttribute().getHandle(), buf, &bytesRead) == BLE_ERROR_NONE)
        {

        	if (! this->storeIncoming(buf, bytesRead) )
        	{
        		PACKETDELEGATE_DEBUGLN("Err: could not store incoming bytes?");
        	}
        } else {

        	PACKETDELEGATE_DEBUGLN("Err reading handle??");
        }

    }
}


static void wakeIt() {

}

void NRF51822::yieldToOS()
{
	// DEADBEEF: messes things up more than anything...
	// wakeup_timeout.attach_us(wakeIt, 500);
	// _ble->waitForEvent();

}
// when we have pending writes in the queue, send
// in blocks of up to SUI_NRF_PACKET_MTU, by updating
// the characteristing
bool NRF51822::sendOutgoing(PacketByteType * buf, PacketBufferSize size) {

	_ble->updateCharacteristicValue(toclient_characteristic.getValueAttribute().getHandle(), buf, size);
	_ble->waitForEvent();

	return true;

}



void NRF51822::begin(unsigned long speed) {

	PACKETDELEGATE_DEBUGLN("started NRF delegate");

}


}

}



#endif /* SUI_PLATFORM_RBLNRF51822 */



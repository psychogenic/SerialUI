/*
 * DelegateNRF51822.h
 *
 *  Created on: Jan 11, 2016
 *      Author: Pat Deegan
 *      Part of the SerialUI Project
 *      Copyright (C) 2015 Pat Deegan, http://psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_STREAM_DELEGATE_DELEGATENRF51822_H_
#define SERIALUI_SRC_INCLUDES_STREAM_DELEGATE_DELEGATENRF51822_H_


#include "../../SUIConfig.h"
#include "DelegatePacketized.h"

#ifdef SUI_PLATFORM_RBLNRF51822



#include <BLE_API.h>



// make this bigger if you like...
//  * smaller: higher risk of data overwrites/loss
//	* bigger:  longer delay between auto-sends, and bigger packet storms
#define SUI_NRFBUFFER_SIZE	(SUI_SERIALUI_PROGMEM_STRING_ABS_MAXLEN + 20)

// SUI_NRF_PACKET_MTU -- mess with this at your own peril
#define SUI_NRF_PACKET_MTU		20

namespace SUI {
namespace Delegate {


typedef Packetized<SUI_NRFBUFFER_SIZE, SUI_NRF_PACKET_MTU> NRFBufferBase;


class NRF51822 : public NRFBufferBase {
public:
	NRF51822(BLE * bleInstance) : NRFBufferBase(), _ble(bleInstance) {}
	virtual ~NRF51822() {}

    virtual void begin(unsigned long speed);

    virtual void yieldToOS();


    // BLE specifics
    GattService & getService();
    void handleReceivedData(const GattWriteCallbackParams *Handler);
    GattCharacteristic ** getCharacteristics();

protected:
    virtual bool sendOutgoing(PacketByteType * buf, PacketBufferSize size);
private:
    BLE * _ble;

};


} /* namespace Delegate */
}/* namespace SUI */


#endif /* SUI_PLATFORM_RBLNRF51822 */


#endif /* SERIALUI_SRC_INCLUDES_STREAM_DELEGATE_DELEGATENRF51822_H_ */

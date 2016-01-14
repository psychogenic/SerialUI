/*
   BLESetup.cpp

    Created on: Jan 13, 2016
        Author: Pat Deegan
        Part of the SerialUI Project
        Copyright (C) 2015 Pat Deegan, http://psychogenic.com
*/

#include "BLENanoBlinker.h"



// 143fd397-81ef-48d8-bd59-01e6f6f52f42
static const uint8_t uart_base_uuid_rev[]           = {0x14, 0x3F, 0xD3, 0x97, 0x81, 0xEF, 0x48, 0xD8, 0xBD, 0x59, 0x01, 0xE6, 0xF6, 0xF5, 0x2F, 0x42 };

BLE 		ble;
Ticker  	suiHandleDataTicker;



void delegate_tick_callback() {

  //OUTPUT_SERIAL_DEBUG("T");
  MyBLEDelegate.tick();
}




/* *************************** BLE functions ******************** */
void writtenHandle(const GattWriteCallbackParams *Handler)
{
  // do whatever extras you may need... then,
  // unless you *know* there's nothing of interest for the
  // "remote uart" delegate

  OUTPUTLN_SERIAL_DEBUG("BLEDataIn!");
  MyBLEDelegate.handleReceivedData(Handler);

}

// Standard BLE Disconnection callback
void disconnectionCallBack(Gap::Handle_t handle, Gap::DisconnectionReason_t reason)
{
  ble.gap().startAdvertising();
}

void BLESetup() {



  OUTPUTLN_SERIAL_DEBUG("BLESetup begins...");


  /* ***************** BLE Setup **************** */

  // BLE setup is all pretty standard except:
  // ble.gattServer().addService(MyBLEDelegate.getService());
  // where we add the delegate's service to the list available...



  // put your setup code here, to run once
  ble.init();
  ble.gap().onDisconnection(disconnectionCallBack);
  ble.gattServer().onDataWritten(writtenHandle);

  // setup adv_data and srp_data
  ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED);
  ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::SHORTENED_LOCAL_NAME,
                                         (const uint8_t *)"TXRX", sizeof("TXRX") - 1);



  ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS,
                                         (const uint8_t *)uart_base_uuid_rev, sizeof(uart_base_uuid_rev));

  // set adv_type
  ble.setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
  // add service
  ble.gattServer().addService(MyBLEDelegate.getService());
  // set device name
  ble.setDeviceName((const uint8_t *)"BLE Blinker");
  // set tx power,valid values are -40, -20, -16, -12, -8, -4, 0, 4
  ble.setTxPower(4);
  // set adv_interval, 100ms in multiples of 0.625ms.
  ble.setAdvertisingInterval(160);
  // set adv_timeout, in seconds
  ble.setAdvertisingTimeout(0);
  // start advertising
  ble.startAdvertising();

  suiHandleDataTicker.attach_us(delegate_tick_callback, 110000);


  OUTPUTLN_SERIAL_DEBUG("BLE Setup done.");
}

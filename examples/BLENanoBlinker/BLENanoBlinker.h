/*
 * BLENanoBlinker.h
 *
 *  Created on: Jan 13, 2016
 *      Author: Pat Deegan
 *      Part of the SerialUI Project
 *      Copyright (C) 2015 Pat Deegan, http://psychogenic.com
 */

#ifndef SERIALUI_EXAMPLES_BLENANOBLINKER_BLENANOBLINKER_H_
#define SERIALUI_EXAMPLES_BLENANOBLINKER_BLENANOBLINKER_H_


#include <Arduino.h>
#include <SerialUI.h>
#include <BLE_API.h>



#define serial_input_terminator   '\n'


// We'll also define a struct to hold our "SuperBlinker" settings, just
// to excercise various functions in this example.
// It will hold RGB settings, a device id and an on/off state.

// dev_id_maxlen, max string length for the device id:
#define dev_id_maxlen  30


/* We'll be using a blinker to show that we're alive,
** so set the ledpin to something connected to an LED
*/
#define ledpin  19


void SUISetup(); // in SUISetup.cpp, called from setup()
void BLESetup(); // in BLESetup.cpp, called from setup()


// deviceInfo struct, a place to store our settings
typedef struct deviceInfo {

  unsigned long red;
  unsigned long green;
  unsigned long blue;
  String dev_id;
  bool state;
} DeviceInfo;

// Our SerialUI instance
extern SUI::SerialUI mySUI;
// A remote delegate, to do our biding over the air (magic!)
extern SUI::Delegate::NRF51822	MyBLEDelegate;

// the BLE Nano instance
extern BLE ble;



#define OUTPUT_DEBUG_ON_SERIAL_AT   115200
#ifdef OUTPUT_DEBUG_ON_SERIAL_AT

#define OUTPUT_SERIAL_DEBUG_BEGIN() Serial.begin(OUTPUT_DEBUG_ON_SERIAL_AT)
#define OUTPUT_SERIAL_DEBUG(...)    Serial.print(__VA_ARGS__)
#define OUTPUTLN_SERIAL_DEBUG(...)  Serial.println(__VA_ARGS__)
#else

#define OUTPUT_SERIAL_DEBUG_BEGIN()
#define OUTPUT_SERIAL_DEBUG(...)
#define OUTPUTLN_SERIAL_DEBUG(...)
#endif




#endif /* SERIALUI_EXAMPLES_BLENANOBLINKER_BLENANOBLINKER_H_ */

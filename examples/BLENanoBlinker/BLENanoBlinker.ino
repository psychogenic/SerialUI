/*
   BLENanoBlinker.cpp

    Created on: Jan 13, 2016
        Author: Pat Deegan
        Part of the SerialUI Project
        Copyright (C) 2015 Pat Deegan, http://psychogenic.com
*/
/*
 **  SerialUI demo/test and tutorial.
 **  Copyright (C) 2013, 2014 Pat Deegan.  All rights reserved.
 **
 ** http://www.flyingcarsandstuff.com/projects/SerialUI
 **
 ** Please let me know if you use SerialUI in your projects, and
 ** provide a URL if you'd like me to link to it from the SerialUI
 ** home.
 **
 ** This program (and accompanying library) is free software;
 ** you can redistribute it and/or modify it under the terms of
 ** the GNU Lesser General Public License as published by the
 ** Free Software Foundation; either version 3 of the License,
 ** or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See file LICENSE.txt for further informations on licensing terms.
 **
 **
 ** ************************* OVERVIEW *************************
 **
 ** SerialUI is useful when you want to provide a user interface
 ** through the serial channel, i.e menus, submenus and commands.
 ** It provides built-in support for setting everything up, as
 ** well as navigation and online help.
 **
 ** This example demonstrates usage by creating the UI for a
 ** fictitious "SuperBlinker" device--some sort of RGB
 ** illumination system.
 **
 **
 ** ************************** USAGE **************************
 **
 ** To use, simply adjust your serial connection to the settings
 ** specified in the "Serial Settings" section below (defaults
 ** to 115200 baud and newline terminators) and set the
 ** ledpin define to a pin that tied to an LED (pin 13 by
 ** default).
 **
 ** Next, compile, upload and connect using the serial monitor
 ** (ensuring the settings match those specified below) and
 ** press "enter".
 **
 **
 ** ************************** MENUS **************************
 **
 ** The menu structure we'll create here looks like:
 **
 **  * information
 **  |
 **  |
 **  + enable -----* on
 **  |             |
 **  |             * off
 **  |
 **  |
 **  + settings ----------* red
 **  |                    |
 **  |                    * green
 **  |                    |
 **  |                    * blue
 **  |                    |
 **  |                    * deviceid
 **  |                    |
 **  |                    * show
 **  * exit
 **
 ** So, a three-option top level menu (information, enable, settings) with
 ** two of those options leading to sub-menus.  Every "leaf" (option that
 ** doesn't lead to a submenu) is a command that uses one of the callbacks
 ** defined below to do its work (see the Callbacks section).
 **
 **
 **
 ** ********************* SAMPLE TRANSCRIPT **********************
 **
 ** Here's a sample of the interaction through the serial
 ** connection:

  +++ Welcome to the SuperBlinker +++
  Enter '?' to list available options.
  > ?
 *** Help for SuperBlinker Main Menu

  information         Retrieve data and current settings
  enable              Enable/disable device
  settings            Perform setup and config

  quit                Exit SerialUI
  ?                   List available menu items
  > settings
  SuperBlinker Settings
  > ?
 *** Help for SuperBlinker Settings

  red                 Set color [0-255]
  green
  blue
  deviceid            Set dev ID [string]
  show

  ..                  Move up to parent menu
  ?                   List available menu items

  > red
  ... 10
  OK
  > green
  ... 20
  OK
  > blue
  ... 30
  > deviceid
  ... YayDevice
  OK
  > show
  (Called 'show_info' from menu: SuperBlinker Settings)
  ID: YayDevice
  Color--> R:10 G:20 B:30
  Device is OFF
  > ..
  SuperBlinker Main Menu
  > quit
 **
 **
 ** The code is fully commented, see below.
 ** Enjoy!
 ** Pat Deegan
*/

// to use SerialUI, you need to include the main header:
#include <SerialUI.h>
#ifndef SUI_PLATFORM_RBLNRF51822
/* SerialUI did NOT detect the RedBearLab Ble Nano env! */
#error "This example may only be compiled for RedBearLab BLE NANO family! Check your boards setting."
#endif
#include <BLE_API.h>

#include "BLENanoBlinker.h"

/*
 ** ********************* setup() ***********************
 **
 ** The standard Arduino setup() function.  Here we'll
 ** setup serial comm and the menu structure.
*/

void setup() {

  OUTPUT_SERIAL_DEBUG_BEGIN();

  // set our blinker pin as an output.
  pinMode(ledpin, OUTPUT);


  BLESetup();
  SUISetup();

}

/*
 ** ********************* loop() ***********************
 **
 ** The standard Arduino loop() function.  Here we'll
 ** handle SerialUI user interaction, and blink our
 ** blinker when nothing is going on.
*/

// we'll keep the blinker state in a boolean global
boolean cur_blink_state = true;
void loop() {


  ble.waitForEvent();

  // Give a little time to the
  // delegate at loop start, in case
  // we *just* received some data
  MyBLEDelegate.tick();

  bool alreadyHaveUser = false;
  if (mySUI.userPresent())
    alreadyHaveUser = true;

  if (alreadyHaveUser || mySUI.checkForUser(0)) {
    OUTPUT_SERIAL_DEBUG('.');

    if (!alreadyHaveUser) {
      // we have a user initiating contact, show the
      // greeting message and prompt
      mySUI.enter();
    }

    // actually respond to requests, using
    mySUI.handleRequests();

    // MyBLEDelegate.tick();
  } /* end if we had a user on the "serial line" */

  // we toggle the LED pin just to show we're alive
  // and not currently processing serial interaction
  cur_blink_state = !cur_blink_state;
  digitalWrite(ledpin, cur_blink_state);

}

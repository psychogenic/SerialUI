/*
**  SerialUI input stream example.
 **  Copyright (C) 2013 Pat Deegan.  All rights reserved.
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
 ** For a complete example of SerialUI usage, see the SuperBlinker
 ** example.
 **
 ** Here, we'll just cover the input stream stuff.  This is
 ** useful if you want to be able to send batches of data to
 ** your device.  Normally, you'll be doing this with a program,
 ** e.g. Druid4Arduino, because the input stream format is
 ** pretty rigid.
 **
 ** In short, when it sends a prompt for a stream input,
 ** SerialUI expects to receive:
 ** LENGTH\n
 ** STREAM_OF_BYTES_OF_STATED_LENGTH
 **
 ** If you know beforehand how much data you want to send,
 ** you could always do this through the serial monitor...
 ** Doubt it'll be much fun, though.
 **
 **
 ** ************************** USAGE **************************
 **
 ** To use, simply adjust your serial connection to the settings
 ** specified in the "Serial Settings" section below (defaults
 ** to 115200 baud and newline terminators)
 **
 ** Next, compile, upload and connect using Druid4Arduino
 ** (http://flyingcarsandstuff.com/projects/druid4arduino/)
 **
 ** Click the button, select a file, and it should be echoed
 ** by druid, through the SerialUI callback.
 **
 ** NOTE: druid expects stuff sent by SerialUI to be pretty clean
 ** text strings, so if you upload some random binary file,
 ** results are "undefined" ;-)
 **
 **
 ** ************************** MENUS **************************
 **
 ** The menu structure we'll create here looks like:
 **
 **  + hello
 **  |
 **  |
 **  + upload
 **
 ** So just a little hello command to see everything's working
 ** and an upload command that will prompt you for a stream-o-data.
 **
 **
 **
 **
 ** The code is fully commented, see below.
 ** Enjoy!
 ** Pat Deegan
 */

// to use SerialUI, you need to include the main header:
#include <SerialUI.h>

// sanity check -- you need SerialUI >= 1.7
#if  not (defined(SERIALUI_VERSION_AT_LEAST) and SERIALUI_VERSION_AT_LEAST(1, 7))
#error "You need to install SerialUI version 1.7 (or later) from http://flyingcarsandstuff.com/"
#endif

/*
** ***************** Serial Settings ******************
 **
 ** Make sure you're serial port/serial monitor is setup
 ** with the correct baud rate or redefine the
 **  serial_baud_rate (integer)
 **
 ** You may also set
 **  serial_input_terminator (character)
 ** according to your environment, though this latest
 ** version handles all the regular EOL situations
 ** automatically, with readBytesToEOL().
 **
 ** NOTE: Setting the correct serial_input_terminator is
 ** only important if you are using a "strange" (i.e not
 ** NL, CR, or CR+NL) input terminator.
 */
#define serial_baud_rate           115200
#define serial_input_terminator   '\n'


// this will be the size of the mouthfull of bytes we take from the stream,
// in one go (max)
#define MY_STREAM_BUFFER_SIZE	32


/*
** ********************* Menu Strings ***********************
 **
 ** SerialUI saves RAM at the expense of program space, namely
 ** by using "PROGMEM" strings (which are stored on the flash).
 **
 ** You can manually declare your strings to be stored in the ROM
 ** if you like, but there's a little define that makes these
 ** declarations easy:
 **
 **    SUI_DeclareString(STRING_NAME, "Contents of string");
 **
 ** The call above would make a variable called STRING_NAME available
 ** with the contents as specified.
 **
 ** NOTE: There is a MAXIMUM LENGTH to these strings (127 characters,
 ** by default).
 ** You can change this max length using the
 **  SUI_SERIALUI_PROGMEM_STRING_ABS_MAXLEN
 ** define in SerialUI.h if you need to (strings need to be in RAM
 ** while they are being transmitted, so just don't eat it all
 ** up by making the max too large).
 */


// Program space string declarations for our imaginary device, "SuperBlinker".
// These will be used (mainly in setupMenu(), below).

SUI_DeclareString(device_greeting,
"+++ Welcome to the Streaming Test +++\r\nEnter '?' to list available options.");

SUI_DeclareString(top_menu_title, "Streamer Main Menu");


SUI_DeclareString(hello_key, "hello");
SUI_DeclareString(hello_help, "Say hi");

SUI_DeclareString(num_key, "num");
SUI_DeclareString(num_help, "A simple numerical input");

SUI_DeclareString(upload_key,"upload");
SUI_DeclareString(upload_help, "Will give you the stream prompt and expect some tasty bytes");

SUI_DeclareString(error_cant_add, "Could not addCommand to mainMenu?");

/*
** ********************* SerialUI instance ***********************
 **
 ** You _could_ use multiple instances of SerialUI, but normally a
 ** single global should suffice.
 **
 ** The constructor is called with two (optional) parameters:
 **
 **  SerialUI([PGM_P greeting_message, [uint8_t num_top_level_menuitems_hint]]);
 **
 **   - a message string to show on entry, declared with SUI_DeclareString
 **   - a hint concerning the number of top level menu items (to avoid the cost
 **     of memory re-allocation, when you have many--say, more than 3)
 **
 ** As mentioned both are optional but a greeting is nice as it lets you know
 ** everything is working.
 */

SUI::SerialUI mySUI = SUI::SerialUI(device_greeting);






/*
** ********************* setup() ***********************
 **
 ** The standard Arduino setup() function.  Here we'll
 ** setup serial comm and the menu structure.
 */
void setup()
{

  // SerialUI acts just like (is actually a facade for)
  // Serial.  Use it, rather than Serial, throughout the
  // program.
  // Setup of SerialUI:
  mySUI.begin(serial_baud_rate); // serial line open/setup
  mySUI.setTimeout(25000);      // timeout for reads (in ms), same as for Serial.
  mySUI.setMaxIdleMs(30000);    // timeout for user (in ms)
  // how we are marking the "end-of-line" (\n, by default):
  mySUI.setReadTerminator(serial_input_terminator);


  setupMenus();



}



/*
** ********************* loop() ***********************
 **
 ** The standard Arduino loop() function.  Here we'll
 ** handle SerialUI user interaction, and blink our
 ** blinker when nothing is going on.
 */

void loop()
{

  /* We checkForUser() periodically, to see
   ** if anyone is attempting to send us some
   ** data through the serial port.
   **
   ** This code checks all the time, for 150 ms,
   ** upon entering the loop.  Should you want to
   ** check for user access only once (say have a
   ** 10 second wait on startup, and then forgo
   ** allowing SerialUI access), then increase the
   ** delay parameter and use checkForUserOnce(), e.g.
   **
   **    mySUI.checkForUserOnce(15000);
   **
   ** to allow 15 seconds to connect on startup only.
   */
  if (mySUI.checkForUser(150))
  {
    // we have a user initiating contact, show the
    // greeting message and prompt
    mySUI.enter();


    /* Now we keep handling the serial user's
     ** requests until they exit or timeout.
     */
    while (mySUI.userPresent())
    {
      // actually respond to requests, using
      mySUI.handleRequests();
    }

  } /* end if we had a user on the serial line */


}


/*
** ********************* Callbacks ***********************
 **
 ** SerialUI is good at handling requests from users,
 ** providing navigation and help information but it is
 ** completely agnostic in terms of application.
 **
 ** There are two types of SerialUI menu items:
 **
 **    * Sub menus: lead you to another level of menu items
 **
 **    * Commands: actually perform some type of action.
 **
 ** Exactly _what_ happens when a user issues a command is
 ** determined by the callback implementations.
 **
 ** A callback is just a function with a
 **
 **  "void NAME(void)"
 **
 ** signature, like:
 **
 **  void do_something() { /* do something cool * / }
 **
 ** Within the callback, you can do stuff like
 **   get user input,
 **   provide output,
 **   determine the menu from which the callback was called.
 ** or anything else you want to do.
 **
 ** The examples below go through setting and displaying
 ** values of the deviceInfo defined above.
 */

// callback: hello
// set device (and the ledpin) on
void hello()
{

  // provide some feedback
  mySUI.println("Why... hello!");
  mySUI.returnOK();
}

void enterNum()
{
  mySUI.print("Enter a number");
  mySUI.showEnterNumericDataPrompt();

  int n = mySUI.parseInt();

  mySUI.print("You entered a: ");
  mySUI.println(n, DEC);
  return mySUI.returnOK();
}

void gotMeSomeBytes(char* buf, uint8_t len, size_t prev_pos, size_t total_file_len)
{
  // ok, here's where you get to do something with the bytes... do whatever you need,
  // here we'll just output some data:
  mySUI.print(prev_pos + 1, DEC);
  mySUI.print('-');
  mySUI.print(prev_pos + len, DEC);
  mySUI.print('/');
  mySUI.print(total_file_len, DEC);
  mySUI.print(": \t");
  buf[len] = 0; // null terminate
  mySUI.println(buf);

}

// callback: upload
// request and process the stream data...
void upload()
{
  char buffer[MY_STREAM_BUFFER_SIZE + 1]; // our buffer, with an extra slot so we can null-terminate in the callback
  mySUI.print("Give me a SUI stream-o-bytes to spit out ");
  if (! mySUI.showEnterStreamPromptAndReceive(buffer, MY_STREAM_BUFFER_SIZE, gotMeSomeBytes))
  {
    return mySUI.returnError("Seems we didn't get any bytes...");
  }

  mySUI.returnOK();
}



/*
** ********************* setupMenus() ***********************
 **
 ** setupMenus creates the top level menu and 2 sub-menus, and
 ** demonstrates a few additional functions of SerialUI setup.
 **
 **
 ** Menu item order is set by the order with which they are added, using
 ** addCommand()/subMenu() (see below).
 
 */


void setupMenus()
{
  /*
  ** Now it's time to start creating menus.  SerialUI always has
   ** one "top level" menu available which you can use to create items for
   ** commands and access to sub menus.
   */

  // Get a handle to the top level menu
  // Note that menus are returned as pointers.
  SUI::Menu * mainMenu = mySUI.topLevelMenu();
  if (! mainMenu)
  {
    // what? Could not create :(
    return mySUI.returnError("Something is very wrong, could not get top level menu?");

  }


  // we can set the name (title) of any menu using
  // setName().  This shows up in help output and when
  // moving up and down the hierarchy.  If it isn't set,
  // the menu key will be used, for sub-menus, and the default
  // SUI_SERIALUI_TOP_MENU_NAME ("TOP") for top level menus.
  mainMenu->setName(top_menu_title);


  /*
  ** addCommand(KEY, CALLBACK [, HELP])
   **
   ** Use addCommand() to add a command menu item to a menu.
   ** The parameters are:
   **
   **  KEY: the (SUI_DeclareString-created) string to use as the command
   **
   **  CALLBACK: the name of the void(void) callback function (as described
   **            in "Callbacks", above).
   **
   **  HELP: optional (SUI_DeclareString-created) string to display for
   **        this item when menu help (?) is invoked.
   **
   ** Return:  returns boolean true on success, false if command could not
   **          be added.
   */
  if (! mainMenu->addCommand(hello_key, hello, hello_help) )
  {
    // should check that addCommand succeeded -- we'll skip this below for brevity.
    // You might want to #ifdef these checks to enable during dev and disable when
    // you know everything's working, if space is tight.

    // what? Could not create :(
    return mySUI.returnError_P(error_cant_add);

  }


  if (! mainMenu->addCommand(num_key, enterNum, num_help) ) 
    return mySUI.returnError_P(error_cant_add);

  if (! mainMenu->addCommand(upload_key, upload, upload_help) ) 
    return mySUI.returnError_P(error_cant_add);


  // Done setting up the menus!

}

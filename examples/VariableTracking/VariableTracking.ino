
/*
**  SerialUI Variable Tracking and streaming output example.
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
** This program has a very simple (single-level) menu structure.
** 
** The goal, here, is to demonstrate variable state tracking and 
** continuous output streaming.
** 
** A handful of global variables are instantiated, and configured
** to be tracked by SerialUI in the setup() function.  From then 
** on, whenever you modify any of them (using the various buttons
** in Druid4Arduino), they will be updated in the GUI automatically.
** 
** Also, turning the "streaming_on" boolean to true (using the 
** toggle_streaming() callback by executing the command from the GUI),
** will begin outputting the current time at various points.
** 
** You may notice that the tracked variable labeled "time" in the 
** GUI refreshes periodically, even without any interaction on your
** part.
** 
** How often this occurs is configured, in Druid4Arduino, by selecting
** one of the options in the main window menu, under
** 	Edit -> Keep-Alive/State Refresh
** 	
** A short period will mean more frequent automatic updates, but also
** means that Druid will be occuppying the Arduino more often, and 
** be busy while it's doing so more frequently (causing a bit of lag
** in the GUI).  Leave it at standard or "Long", if you can, or change
** it to "Short" for time you really need a refresh every few seconds.
**
** ************************** USAGE **************************
**
** To use, simply adjust your serial connection to the settings
** specified in the "Serial Settings" section below (defaults
** to 115200 baud and newline terminators)
**
** Next, compile, upload and connect using the
** Druid4Arduino GUI
** (http://flyingcarsandstuff.com/projects/druid4arduino/
**
**
** ************************** MENUS **************************
**
** The menu structure we'll create here looks is simple and looks like:
**
**  * instructions
**  |
**  * toggle the bool
**  |
**  * toggle streaming
**  |
**  * set float
**  |
**  * set long
**  |
**  * exit
**
** So a few boolean toggles (on/off) a couple of numeric variables
** and an exit to terminate the GUI.
**
**
**
** The code is fully commented, see below.
** Enjoy!
** Pat Deegan
*/

// to use SerialUI, you need to include the main header:
#include <SerialUI.h>


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
                  "+++ VariableTracking Example +++\r\nEnter '?' to list available options.");

SUI_DeclareString(top_menu_title, "VariableTracking Main Menu");

SUI_DeclareString(instructions_key, "instructions");

SUI_DeclareString(toggle_bool_key, "toggle the bool");
SUI_DeclareString(toggle_bool_help, "Boolean On/Off");


SUI_DeclareString(toggle_streaming_key, "toggle streaming");
SUI_DeclareString(toggle_streaming_help, "Streaming On/Off");

SUI_DeclareString(set_float_key, "set float");
SUI_DeclareString(set_float_help, "Set the value of float");

SUI_DeclareString(set_long_key, "set long");
SUI_DeclareString(set_long_help, "Set the value of unsigned long");

SUI_DeclareString(exit_key, "exit");
SUI_DeclareString(exit_help, "exit (and terminate Druid)");


SUI_DeclareString(err_cantadd_command, "Could not addCommand to mainMenu?");


// a few labels for our variable auto-tracking
SUI_DeclareString(label_stream, "streaming");
SUI_DeclareString(label_bool, "the bool");
SUI_DeclareString(label_float, "the float");
SUI_DeclareString(label_long, "the int");
SUI_DeclareString(label_time, "time");






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
 * Our global tracked variables
 */

unsigned long CurTime = 0;
unsigned long My_long = 1234;
float My_float = 3.14;
bool  My_bool = false;
bool  Streaming_on = false;


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
  mySUI.setTimeout(20000);      // timeout for reads (in ms), same as for Serial.
  mySUI.setMaxIdleMs(30000);    // timeout for user (in ms)
  // how we are marking the "end-of-line" (\n, by default):
  mySUI.setReadTerminator(serial_input_terminator);


  // Setup variable state tracking -- will report changes to
  // Druid4Arduino (v >= 1.3.0) so it can automatically display
  // these in GUI.
  // To setup tracking, you need:
  //	* a pointer to a persistent (i.e. global or at least
  //	  guaranteed to be valid) variable to refer to, and
  //	* a label (created with SUI_DeclareString()).
  //
  // Variables will be listed in the order they are added here:
  mySUI.trackState(label_stream, &Streaming_on);
  mySUI.trackState(label_bool, &My_bool);
  mySUI.trackState(label_float, &My_float);
  mySUI.trackState(label_long, &My_long);
  mySUI.trackState(label_time, &CurTime);
  // and that's it, you're variables will be displayed in the druid GUI



  // The SerialUI menu setup is a bit involved, and it
  // needs to know about the functions we'll be using as
  // callbacks. Instead of having a bunch of function
  // declarations, all the work is contained in a function
  // of its own at the bottom of the program.
  // Yes: *DO* check it out!
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
      CurTime = millis();

      if (Streaming_on)
      {
        mySUI.print(F("Streaming at "));
        mySUI.println(CurTime);
      }
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

// callback: toggle_streaming
// turn streaming on/off
void toggle_streaming()
{
  Streaming_on = !Streaming_on;
  mySUI.print(F("Streaming is now "));
  mySUI.println(Streaming_on ? F("ON") : F("OFF"));

  return mySUI.returnOK();
}

// callback: toggle_bool
// turn tracked bool on/off
void toggle_bool()
{
  My_bool = !My_bool;
  mySUI.print(F("The bool is now "));
  mySUI.println(My_bool ? F("TRUE") : F("FALSE"));

  return mySUI.returnOK();
}

// callback: set_float
// set the device value for "My_float"
void set_float()
{
  // Here, we want some additional (numerical) input from
  // the user, so we show the "enter data prompt"
  // using... showEnterNumericDataPrompt
  mySUI.showEnterNumericDataPrompt();

  My_float = mySUI.parseFloat();

  // provide some feedback
  mySUI.println(My_float, 3);
  mySUI.returnOK();
}



// callback: set_green
// same as set_red, but set the device value for "green"
void set_long()
{
  // Here, we want some additional (numerical) input from
  // the user, so we show the "enter data prompt"
  // using... showEnterNumericDataPrompt
  mySUI.showEnterNumericDataPrompt();

  // Now, we actually get the input
  // we can use any Serial method on our SerialUI
  // object, so:
  My_long = mySUI.parseInt();

  // provide some feedback
  mySUI.println(My_long, DEC);
  mySUI.returnOK();
}



// callback: show_instructions
// Output some details about the device
void show_instructions()
{
  mySUI.println(F("VariableTracking instructions:")) ;
  mySUI.print(F("Use the buttons to change the state of tracked variables, "));
  mySUI.println(F("and watch them update on the right-hand side... that's pretty much it")) ;

  return mySUI.returnOK();
}

void do_exit() {
  // though you can always just use the "quit" command from
  // the top level menu, this demonstrates using exit(), which
  // will automatically close the Druid4Arduino GUI, if
  // being used.
  mySUI.print(F("Exit requested, terminating GUI if present"));
  mySUI.exit();
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
  if (! mainMenu->addCommand(instructions_key, show_instructions) )
  {
    // should check that addCommand succeeded -- we'll skip this below for brevity.
    // You might want to #ifdef these checks to enable during dev and disable when
    // you know everything's working, if space is tight.

    // what? Could not create :(
    return mySUI.returnError_P(err_cantadd_command);

  }


  // streaming on/off
  if (! mainMenu->addCommand(toggle_streaming_key, toggle_streaming, toggle_streaming_help) )
    return mySUI.returnError_P(err_cantadd_command);

  // bool on/of
  if (! mainMenu->addCommand(toggle_bool_key, toggle_bool, toggle_bool_help) )
    return mySUI.returnError_P(err_cantadd_command);

  // set float
  if (! mainMenu->addCommand(set_float_key, set_float, set_float_help) )
    return mySUI.returnError_P(err_cantadd_command);

  // set long
  if (! mainMenu->addCommand(set_long_key, set_long, set_long_help) )
    return mySUI.returnError_P(err_cantadd_command);

  // exit
  if (! mainMenu->addCommand(exit_key, do_exit, exit_help) )
    return mySUI.returnError_P(err_cantadd_command);


  // Done setting up the menus!

}

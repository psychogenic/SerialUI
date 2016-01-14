/*
   SUISetup.cpp


    Created on: Jan 13, 2016
        Author: Pat Deegan
        Part of the SerialUI Project
        Copyright (C) 2015 Pat Deegan, http://psychogenic.com
*/

#include "BLENanoBlinker.h"



/*
** ********************* SerialUI instance ***********************
**
** You _could_ use multiple instances of SerialUI, but normally a
** single global should suffice.
**
** The constructor is called with two (optional) parameters:
**
**  SerialUI([uint8_t num_top_level_menuitems_hint]]);
**
**   - a message string to show on entry, declared with SUI_DeclareString
**   - a hint concerning the number of top level menu items (to avoid the cost
**     of memory re-allocation, when you have many--say, more than 3)
**
** As mentioned both are optional but a greeting is nice as it lets you know
** everything is working.
*/

SUI::SerialUI mySUI(4);


// A remote delegate, to do our biding over the air (magic!)
SUI::Delegate::NRF51822 MyBLEDelegate(&ble);





// Just declare a global deviceInfo structure for
// use below, initialized to all-zeros:
DeviceInfo myDevice = {0};
// NOTE: In real life, I think the device resets as we
// close the serial connection, meaning that if you want
// your config to be permanent, you probably have to save
// it to eeprom or somesuch.









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
**  void do_something() { / * do something cool * / }
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

// callback: turn_on
// set device (and the ledpin) on
void turn_on()
{

  // do our business, normally you'd probably
  // use the RGB settings to do something more
  // interesting, but for this example we'll
  // just turn on the ledpin
  myDevice.state = true;
  digitalWrite(ledpin, HIGH);

  // provide some feedback
  mySUI.println(F("ON"));
  mySUI.returnOK();
}



// callback: turn_on
// set device (and the ledpin) off
void turn_off()
{
  // do our business
  myDevice.state = false;
  digitalWrite(ledpin, LOW);

  // provide some feedback
  mySUI.println(F("OFF"));
  mySUI.returnOK();
}

#if 0

// callback: set_devid
// Get a string from the user to set as the
// device id
void set_devid()
{
  // Here, we want some additional input from
  // the user, so we show the "enter data prompt"
  // using... showEnterDataPrompt
  mySUI.showEnterDataPrompt();


  // Now, we actually get the input
  // We want a string (of up to dev_id_maxlen
  // characters), so you can use readBytesToEOL().
  // to ensure we get the whole string
  // no matter what the serial line terminator settings are
  // (newline, carriage return or both) we can use readBytesUntil().
  //
  // readBytesToEOL: works in all line termination setups and won't
  //  mess up with a slow serial line (like 9600baud)
  //
  // readBytesUntil: needs the correct line termination char
  byte numRead = mySUI.readBytesToEOL(myDevice.dev_id, dev_id_maxlen);

  // make sure the string is null-terminated
  myDevice.dev_id[numRead] = '\0';

  // provide some feedback
  mySUI.println(myDevice.dev_id);
  mySUI.returnOK();
}
#endif

void color_has_changed()
{
  mySUI.println(F("Color changed!"));

}

// callback: set_red
// set the device value for "red"
bool validate_color(unsigned long & newRed)
{
  bool isValid = (newRed > 0 && newRed < 255);

  if (! isValid)
  {
    mySUI.println(F("Value must be between 1-255"));
  }

  return isValid;

}

void green_has_changed()
{
  mySUI.println(F("Oh my, green has been modified!"));
}


// callback: show_info
// Output some details about the device
void show_info()
{

  // this callback is used as a command in multiple
  // menus (see setupMenus(), below).  As a demonstration,
  // we'll start by telling the user where this function was
  // called from.

  // You can always get the currently active menu from
  // SerialUI's currentMenu():
  SUI::Menu * current_menu = mySUI.currentMenu();

  // output a line to indicate where we were called from:
  mySUI.print(F("(Called 'show_info' from menu: "));
  current_menu->showName();
  mySUI.println(')');


  // use Serial-like print statements to output the info
  mySUI.print(F("ID: "));
  mySUI.println(myDevice.dev_id);
  mySUI.print(F("Color--> R:"));
  mySUI.print(myDevice.red, DEC);
  mySUI.print(F(" G:"));
  mySUI.print(myDevice.green, DEC);
  mySUI.print(F(" B:"));
  mySUI.println(myDevice.blue, DEC);

  mySUI.print(F("Device is "));
  if (myDevice.state)
  {
    mySUI.println(F("ON"));
  }
  else {
    mySUI.println(F("OFF"));
  }


  mySUI.returnOK();

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
    return mySUI.returnError(F("Something is very wrong, could not get top level menu?"));
  }


  // we can set the name (title) of any menu using
  // setName().  This shows up in help output and when
  // moving up and down the hierarchy.  If it isn't set,
  // the menu key will be used, for sub-menus, and the default
  // SUI_SERIALUI_TOP_MENU_NAME ("TOP") for top level menus.
  mainMenu->setName(F("TOP"));


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
  if (! mainMenu->addCommand(F("info"), show_info, F("show information")) )
  {
    // should check that addCommand succeeded -- we'll skip this below for brevity.
    // You might want to #ifdef these checks to enable during dev and disable when
    // you know everything's working, if space is tight.

    // what? Could not create :(
    return mySUI.returnError(F("Could not addCommand to mainMenu?"));

  }



  /*
  ** subMenu(KEY [, HELP])
  **
  ** Use subMenu to create a sub-menu accessible by KEY.  The params are:
  **
  **  KEY: The (SUI_DeclareString-created) string to use to enter the sub-menu
  **       from the current menu.
  **
  **  HELP: optional (SUI_DeclareString-created) string to display for
  **        this item when menu help (?) is invoked.
  **
  **  Return: returns a SUI::Menu pointer, which will be NULL if the submenu
  **          could not be created.
  **
  */
  SUI::Menu * enableMenu = mainMenu->subMenu(F("enable"), F("enable/disable blinker"));
  if (! enableMenu)
  {
    // ah, could not create :(
    return mySUI.returnError(F("Couldn't create enable menu!?"));


  }

  // enable menu created, add our commands
  enableMenu->addCommand(F("on"), turn_on);
  enableMenu->addCommand(F("off"), turn_off);


  SUI::Menu * settingsMenu = mainMenu->subMenu(F("settings"), F("Configure settings"));
  if (! settingsMenu)
  {
    // ah, could not create :(
    return mySUI.returnError(F("Couldn't create settings menu!?"));

  }
  // settings menu created.
  // set its name and add the commands
  settingsMenu->setName(F("Settings"));

  SUI_FLASHSTRING_PTR SetColorHelp = F("Set color [0-255]");
  SUI_FLASHSTRING_PTR CouldntAddRequset = F("Could not add request?");

  // we could use a regular command callback to get the device info
  // but since we only want the value, we use the new "addRequest"
  // addRequest may be called with (or without) a validator and/or a callback
  // to notify us when some value has changed
  if (!settingsMenu->addRequest(&(myDevice.red), F("red"), SetColorHelp, validate_color)) {
      return mySUI.returnError(CouldntAddRequset);

  }

  // we add a change notification callback to this one, just fer fun
  if (!settingsMenu->addRequest(&(myDevice.green), F("green"), SetColorHelp, validate_color, green_has_changed)) {
      return mySUI.returnError(CouldntAddRequset);

  }

  if (!settingsMenu->addRequest(&(myDevice.blue), F("blue"), SetColorHelp, validate_color)) {
      return mySUI.returnError(CouldntAddRequset);
  }

  // the String version of addRequest requires an addition param: the max size of the
  // string entered
  settingsMenu->addRequest(&(myDevice.dev_id), dev_id_maxlen, F("deviceid"), F("Set dev id [string]"));


  settingsMenu->addCommand(F("show"), show_info);


  if (! mainMenu->addCommand(F("exit"), do_exit, F("exit (and terminate Druid)")) )
  {
    return mySUI.returnError(F("Could not addCommand to mainMenu?"));
  }

  // Done setting up the menus!

}













void SUISetup() {


  OUTPUTLN_SERIAL_DEBUG("SUISetup() begins...");

  mySUI.useDelegate(&MyBLEDelegate);
  mySUI.setGreeting(F("+++ Welcome to the BLEBlinker +++\r\nEnter '?' to list available options."));

  mySUI.begin(9600);

  mySUI.setTimeout(35000);      // timeout for reads (in ms), same as for Serial.
  mySUI.setMaxIdleMs(40000);    // timeout for user (in ms)
  // how we are marking the "end-of-line" (\n, by default):
  mySUI.setReadTerminator(serial_input_terminator);

  setupMenus();


  OUTPUTLN_SERIAL_DEBUG("SUI Setup done.");
}



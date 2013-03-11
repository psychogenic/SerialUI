/*
**  SerialUI demo/test and tutorial.
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
**  + information
**  |
**  |
**  + enable -----+ on
**  |             |
**  |             + off
**  |
**  |
**  + settings ---------+ red
**                      |
**                      + green
**                      |
**                      + blue
**                      |
**                      + deviceid
**                      |
**                      + show
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
#define serial_baud_rate           9600
#define serial_input_terminator   '\n'

/* We'll be using a blinker to show that we're alive,
** so set the ledpin to something connected to an LED
*/
#define ledpin  13


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
	"+++ Welcome to the SuperBlinker +++\r\nEnter '?' to list available options.");

SUI_DeclareString(top_menu_title, "SuperBlinker Main Menu");


SUI_DeclareString(enable_key, "enable");
SUI_DeclareString(enable_help, "Enable/disable device");

SUI_DeclareString(settings_title, "SuperBlinker Settings");
SUI_DeclareString(settings_key, "settings");
SUI_DeclareString(settings_help, "Perform setup and config");

SUI_DeclareString(info_key,"information");
SUI_DeclareString(info_help, "Retrieve data and current settings");

SUI_DeclareString(enable_on_key,"on");
SUI_DeclareString(enable_off_key,"off");

SUI_DeclareString(settings_red_key,"red");
SUI_DeclareString(settings_color_help,"Set color [0-255]");
SUI_DeclareString(settings_green_key, "green");
SUI_DeclareString(settings_blue_key, "blue");
SUI_DeclareString(settings_devid_key, "deviceid");
SUI_DeclareString(settings_devid_help, "Set dev ID [string]");
SUI_DeclareString(settings_show_key, "show");




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



// We'll also define a struct to hold our "SuperBlinker" settings, just 
// to excercise various functions in this example.
// It will hold RGB settings, a device id and an on/off state.

// dev_id_maxlen, max string length for the device id:
#define dev_id_maxlen  30

// deviceInfo struct, a place to store our settings
typedef struct deviceInfo {

  int red;
  int green;
  int blue;
  char dev_id[dev_id_maxlen + 1];
  boolean state;
} 
deviceInfo;

// Just declare a global deviceInfo structure for 
// use below, initialized to all-zeros:
deviceInfo myDevice = {0};




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
  

  // The SerialUI menu setup is a bit involved, and it 
  // needs to know about the functions we'll be using as 
  // callbacks. Instead of having a bunch of function
  // declarations, all the work is contained in a function 
  // of its own at the bottom of the program.
  // Yes: *DO* check it out!
  setupMenus();
  
  
  // set our blinker pin as an output.
  pinMode(ledpin, OUTPUT);

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
  
  
  // we toggle the LED pin just to show we're alive
  // and not currently processing serial interaction
  cur_blink_state = !cur_blink_state;
  digitalWrite(ledpin, cur_blink_state);


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
  mySUI.println("ON");
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
  mySUI.println("OFF");
  mySUI.returnOK();
}



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



// callback: set_red
// set the device value for "red"
void set_red()
{
  // Here, we want some additional input from 
  // the user, so we show the "enter data prompt"
  // using... showEnterDataPrompt
  mySUI.showEnterDataPrompt();
  
  // Now, we actually get the input
  // we can use any Serial method on our SerialUI
  // object, so:
  int new_red = mySUI.parseInt();
  
  // do some validation of the input
  if (new_red < 0 || new_red > 255)
  {
    // returnError() is the only SerialUI method that takes
    // a regular char* argument, so we don't have to 
    // pre-program all the error messages as program memory strings:
    return mySUI.returnError("invalid red val");
  } 

  // input looks good, set it
  myDevice.red = new_red;
  
  // provide some feedback
  mySUI.println(new_red, DEC);
  mySUI.returnOK();
}



// callback: set_green
// same as set_red, but set the device value for "green"
void set_green()
{
  // Here, we want some additional input from 
  // the user, so we show the "enter data prompt"
  // using... showEnterDataPrompt
  mySUI.showEnterDataPrompt();
  
  // Now, we actually get the input
  // we can use any Serial method on our SerialUI
  // object, so:
  int new_green = mySUI.parseInt();
  
  // do some validation of the input
  if (new_green < 0 || new_green > 255)
  {
    // returnError() is the only SerialUI method that takes
    // a regular char* argument, so we don't have to 
    // pre-program all the error messages as program memory strings:
    return mySUI.returnError("invalid green val");
  } 

  // input looks good, set it
  myDevice.green = new_green;
  
  // provide some feedback
  mySUI.println(new_green, DEC);
  mySUI.returnOK();
}

// callback: set_blue
// same as set_red, but set the device value for "blue"
void set_blue()
{
  // Here, we want some additional input from 
  // the user, so we show the "enter data prompt"
  // using... showEnterDataPrompt
  mySUI.showEnterDataPrompt();
  
  // Now, we actually get the input
  // we can use any Serial method on our SerialUI
  // object, so:
  int new_blue = mySUI.parseInt();
  
  // do some validation of the input
  if (new_blue < 0 || new_blue > 255)
  {
    // returnError() is the only SerialUI method that takes
    // a regular char* argument, so we don't have to 
    // pre-program all the error messages as program memory strings:
    return mySUI.returnError("invalid blue val");
  } 

  // input looks good, set it
  myDevice.blue = new_blue;
  
  // provide some feedback
  mySUI.println(new_blue, DEC);
  mySUI.returnOK();
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
  mySUI.print("(Called 'show_info' from menu: ");
  current_menu->showName();
  mySUI.println(")");


  // use Serial-like print statements to output the info
  mySUI.print("ID: ");
  mySUI.println(myDevice.dev_id);  
  mySUI.print("Color--> R:");
  mySUI.print(myDevice.red, DEC);
  mySUI.print(" G:");
  mySUI.print(myDevice.green, DEC);
  mySUI.print(" B:");
  mySUI.println(myDevice.blue, DEC);

  mySUI.print("Device is ");
  if (myDevice.state)
  {
    mySUI.println("ON");
  } 
  else {
    mySUI.println("OFF");
  }

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
  if (! mainMenu->addCommand(info_key, show_info, info_help) )
  {
    // should check that addCommand succeeded -- we'll skip this below for brevity.
    // You might want to #ifdef these checks to enable during dev and disable when 
    // you know everything's working, if space is tight.
    
    // what? Could not create :(
    return mySUI.returnError("Could not addCommand to mainMenu?");
    
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
  SUI::Menu * enableMenu = mainMenu->subMenu(enable_key, enable_help);
  if (! enableMenu)
  {
     // ah, could not create :(
    return mySUI.returnError("Couldn't create enable menu!?");
    
    
  } else {

    // enable menu created, add our commands
    enableMenu->addCommand(enable_on_key, turn_on);
    enableMenu->addCommand(enable_off_key, turn_off);
   
  }

  SUI::Menu * settingsMenu = mainMenu->subMenu(settings_key, settings_help);
  if (! settingsMenu)
  {
    // ah, could not create :(
    return mySUI.returnError("Couldn't create settings menu!?");
    
    
  }  else {
    
    // settings menu created.
    // set its name and add the commands
    settingsMenu->setName(settings_title);
    settingsMenu->addCommand(settings_red_key, set_red, settings_color_help);
    settingsMenu->addCommand(settings_green_key, set_green);
    settingsMenu->addCommand(settings_blue_key, set_blue);
    settingsMenu->addCommand(settings_devid_key, set_devid, settings_devid_help);
    settingsMenu->addCommand(settings_show_key, show_info);
  }
  
  // Done setting up the menus!

}

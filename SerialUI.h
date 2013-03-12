/*
 *
 * SerialUI.h -- Serial User Interface for Arduino-compatible devices.
 * Copyright (C) 2013 Pat Deegan.  All rights reserved.
 *
 * http://www.flyingcarsandstuff.com/projects/SerialUI
 *
 * Please let me know if you use SerialUI in your projects, and
 * provide a URL if you'd like me to link to it from the SerialUI
 * home.
 *
 *
 * This program library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See file LICENSE.txt for further informations on licensing
 * terms.
 *
 *
 * ************************* OVERVIEW *************************
 *
 * SerialUI is useful when you want to provide a user interface
 * through the serial channel (menus, sub-menus and command
 * execution). It provides built-in support for navigation
 * through the menu hierarchy and online help.
 *
 * It is designed to keep RAM usage to a minimum, preferring to
 * use strings stored in the flash ROM (progmem) wherever
 * possible.
 *
 * With SerialUI, you can create a hierarchy of menus and
 * submenus of arbitrary depth (limited only by ROM/RAM space).
 *
 * Each menu contains a list of menu items. There are two
 * types of SerialUI menu items:
 *
 *    * Sub menus: lead you to another level of menu items
 *
 *    * Commands: actually perform some type of action.
 *
 * Exactly _what_ happens when a user issues a command is
 * determined by the callback implementations.
 *
 * A few commands are built-in and don't need to be defined:
 *
 * 	- '?': Help, which displays all the available menu keys and
 * 			help messages, where defined.
 *
 * 	- '..': Up, which moves up to a parent menu from a sub-menu
 *
 *	- 'quit': Exit the SerialUI interface (available in top level
 *			  menu).
 *
 * Using SerialUI::topLevelMenu()/Menu::addCommand()/Menu::subMenu(),
 * we can create any serial user interface.
 * Example menu hierarchy:
 *
 *  + information
 *  |
 *  |
 *  + enable -----+ on
 *  |             |
 *  |             + off
 *  |
 *  |
 *  + settings ---------+ red
 *                      |
 *                      + green
 *                      |
 *                      + blue
 *                      |
 *                      + deviceid
 *                      |
 *                      + show
 *
 *
 * So, a three-option top level menu (information, enable,
 * settings) with two of those options leading to sub-menus.
 * Every "leaf" (option that doesn't lead to a submenu) is a
 * command that uses a callback specified when setting up the
 * menu item (with Menu::addCommand()).
 *
 *
 * ************************** TRANSCIPT **************************
 *
 * Here's a sample of the interaction through the serial
 * connection:

	+++ Welcome to the SuperBlinker +++
	Enter '?' to list available options.
	> ?
	*** Help for: SuperBlinker Main Menu

		* information         Retrieve data and current settings
		+ enable              Enable/disable device
		+ settings            Perform setup and config

		* quit                Exit SerialUI
		* ?                   List available menu items
	> settings
	SuperBlinker Settings
	> ?
	*** Help for: SuperBlinker Settings

		* red                 Set color [0-255]
		* green
		* blue
		* deviceid            Set dev ID [string]
		* show

		* ..                  Move up to parent menu
		* ?                   List available menu items
	> red
	... 10
	OK
	> green
	... 20
	OK
	> blue
	... 42
	OK
	> deviceid
	... Yay Device!
	OK
	> show
	(Called 'show_info' from menu: SuperBlinker Settings)
	ID: Yay Device!
	Color--> R:10 G:20 B:42
	Device is OFF
	> ..
	SuperBlinker Main Menu
	> quit


 *
 *
 * The code for the above menu/interaction is available in
 * examples/SuperBlinker.
 *
 *
 * ************************** CODE SAMPLE **************************
 *
 * Here's a snippet that shows basic usage with an Arduino.
 *
 * PLEASE see the full example, which may be found in
 * examples/SuperBlinker
 *
 *
 * [snip]

	// to use SerialUI, you need to include the main header:
	#include <SerialUI.h>

	// Serial Settings
	#define serial_baud_rate           9600
	#define serial_input_terminator   '\n'



	// Menu Strings
	SUI_DeclareString(device_greeting,
		"+++ Welcome to the SuperBlinker +++\r\nEnter '?' to list available options.");

	SUI_DeclareString(enable_key, "enable");
	SUI_DeclareString(enable_help, "Enable/disable device");

	SUI_DeclareString(info_key,"information");
	SUI_DeclareString(info_help, "Retrieve data and current settings");



	// SerialUI instance
	SUI::SerialUI mySUI = SUI::SerialUI(device_greeting);

	void setup() {

	  // SerialUI acts just like (is actually a facade for)
	  // Serial.  Use it, rather than Serial, throughout the
	  // program.
	  mySUI.begin(serial_baud_rate); // serial line setup
	  mySUI.setTimeout(20000);      // timeout for reads (in ms), same as for Serial.
	  mySUI.setMaxIdleMs(30000);    // timeout for user (in ms)
	  // how we are marking the "end-of-line" (\n, by default):
	  mySUI.setReadTerminator(serial_input_terminator);

	  // Get a handle to the top level menu
	  // Note that menus are returned as pointers.
	  SUI::Menu * mainMenu = mySUI.topLevelMenu();
	  if (! mainMenu)
	  {
		 // what? Could not create :(
		return mySUI.returnError("Something is very wrong, could not get top level menu?");

	  }


	  // Use addCommand() to add a command menu item to a menu.
	  mainMenu->addCommand(info_key, show_info, info_help);
	  if (! mainMenu->addCommand(info_key, show_info, info_help) )
	  {
		 // what? Could not create :(
		return mySUI.returnError("Could not addCommand to mainMenu?");

	  }



	  // Use subMenu to create a sub-menu accessible by KEY.
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
	}


	void loop()
	{
	  if (mySUI.checkForUser(500))
	  {
		// we have a user initiating contact, show the
		// greeting message and prompt
		mySUI.enter();


		// Now we keep handling the serial user's
		// requests until they exit or timeout.
		while (mySUI.userPresent())
		{
		  // actually respond to requests, using
		  mySUI.handleRequests();
		}

	  } // end if we had a user on the serial line

 * [/snip]
 *
 * Enjoy!
 * Pat Deegan
*/

#ifndef SerialUI_h
#define SerialUI_h

// SerialUI includes
#include "includes/SUIConfig.h"
#include "includes/SUIStrings.h"
#include "includes/SUIMenu.h"
#include "includes/SUIPlatform.h"




/*
 * SUI_DeclareString(VARIABLENAME, "value");
 * SUI_DeclareString is a little macro to make setting up the
 * progmem string declarations easier.
 *
 */
#define SUI_DeclareString(name, value) \
	const char name[] PROGMEM = value;






#ifndef NULL
// make sure we always have a NULL symbol.
#define NULL 0x0
#endif



// SUI is the namespace in which we keep all our goodies.
namespace SUI {



/*
 * SerialUI class.
 *
 * The SerialUI class is the main interface to the SerialUI system.
 *
 * It is derived from the Stream class, meaning it can do everything that
 * your standard Serial object can do, along with the extra UI-related stuff.
 *
 * In addition to reading and writing to the serial line, SerialUI mainly handles:
 *
 * 		- Configuring and accessing the Menu hierarchy
 * 		  (topLevelMenu(), currentMenu())
 *
 * 		- Determining SerialUI user presence and handling requests
 * 		  (checkForUser*(), userPresent(), handleRequests())
 *
 * 		- Simplifying transmission of progmem-resident strings to the serial
 * 		  line (print_P(), println_P(), and return*() methods)
 *
 * 	More info on usage above and in the examples/SuperBlinker sample code.
 *
 */
class SerialUI : public SUI::SUIStream

{

public:

	/* 
	 * SerialUI constructor.
	 * The constructor is called with two (optional) parameters:
	 *
	 *  SerialUI([PGM_P greeting_message, [uint8_t num_top_level_menuitems_hint]]);
	 *
	 *   - a message string to show on entry, declared with SUI_DeclareString
	 *   - a hint concerning the number of top level menu items (to avoid the cost
	 *     of memory re-allocation, when you have many--say, more than 3)
	 *
	 * As mentioned both are optional but a greeting is nice as it lets you know
	 * everything is working.
	 */
	SerialUI(PGM_P greeting_message = NULL,
			uint8_t num_top_level_menuitems_hint = 0);

#ifdef SUI_SUISTREAM_NEEDSVIRTUAL
	// d'tor  (virtual class)
	virtual ~SerialUI() {};
#endif

	/* ****************** configuration methods ***************** */
	/*
	 * setMaxIdleMs(MAXIDLETIME)
	 * Set the maximum amount of time (in ms) to wait for user
	 * requests before timing out.
	 */
	void setMaxIdleMs(uint16_t max_ms) {
		user_presence_timeout_ms = max_ms;
	}

	/*
	 * maxIdleMs()
	 * Returns the current setting for maximum idle time (in ms)
	 */
	uint16_t maxIdleMs() {
		return user_presence_timeout_ms;
	}

	/*
	 * setReadTerminator(TERMINATORCHAR)
	 * Set the input terminator character (defaults to '\n')
	 */
	void setReadTerminator(char setTo) { read_terminator_char = setTo; }

	/*
	 * readTerminator()
	 * Returns the current input terminator character.
	 */
	char readTerminator() { return read_terminator_char; }

	/*
	 * readBytesToEOL(BUFFER_PTR, MAX_LENGTH)
	 * A convenience function to read bytes until the
	 * "end-of-line".  This handles the various "regular" cases
	 * (of NL, CR and CR+NL) and also respects the
	 * readTerminator() that may have been set.
	 *
	 * It is designed to wait for the EOL so it won't
	 * terminate early (as can happen on slow serial lines, like
	 * at 9600baud).
	 */
	size_t readBytesToEOL(char* buffer, size_t max_length);



	/*
	 * topLevelMenu([MENUNAME])
	 *
	 * The top level menu always exists, this method gives you access to it
	 * so you can add menu items.
	 *
	 * The MENUNAME optional parameter is used to change the top level menu's
	 * name from the default (though you can use Menu::setName() later, anyway).
	 *
	 * Returns a pointer to the Menu object.
	 */
	Menu * topLevelMenu(PGM_P setNameTo = NULL);

	/*
	 * checkForUser([TIMEOUTMS])
	 *
	 * Used to check if a user is attempting to communicate
	 * (send us data) over the serial line.
	 *
	 * You may call checkForUser periodically. If it returns
	 * true, there is a user sending requests that need to be
	 * handled (see handleRequests())
	 *
	 * TIMOUTMS is a timeout in ms.  This call is blocking, for
	 * a maximum of TIMOUTMS ms.
	 *
	 * Return boolean true if a user is present, false otherwise.
	 *
	 */
	bool checkForUser(uint16_t timeout_ms =
			SUI_SERIALUI_USERCHECK_MAXDELAY_DEFAULT_MS);
	/*
	 * checkForUserOnce([TIMEOUTMS])
	 *
	 * Same as checkForUser() above, but may be used in a loop
	 * to check (and block) ONLY the first time through.
	 */
	bool checkForUserOnce(uint16_t timeout_ms =
			SUI_SERIALUI_USERCHECK_MAXDELAY_DEFAULT_MS);

	/*
	 * userPresent()
	 * Once checkForUser()/checkForUserOnce() have determined a
	 * serial user is present, userPresent() is used to confirm
	 * that they are still around.
	 *
	 * Will return false once the user has exited the SerialUI,
	 * or has timed out.
	 */
	bool userPresent();

	/*
	 * handleRequests();
	 * Handles any pending requests.  Should be called in a loop
	 * that exits once userPresent() goes false (see sample code).
	 */
	void handleRequests();

	/*
	 * currentMenu()
	 * Returns a pointer to the currently active SUI::Menu.
	 */
	Menu * currentMenu() {
		return current_menu;
	}

	/*
	 * enter()
	 * Called after checkForUser*(), when we are ready to process
	 * requests, to show the greeting/prompt.
	 */
	void enter();


	/*
	 * exit()
	 * May be called to force an exit from SerialUI.
	 */
	void exit();


	/* ******************** return messages ******************* */

	/*
	 * returnOK()
	 * Return (print) a standard OK response to user.
	 */
	void returnOK() {
		current_menu->returnOK();
	}

	/*
	 * returnMessage(MESSAGE)
	 * Return (print) a message to user.  MESSAGE must be a
	 * progmem string (most likely created with SUI_DeclareString).
	 */
	void returnMessage(PGM_P message) {
		current_menu->returnMessage(message);
	}

	/*
	 * returnError(ERRORMESSAGE)
	 * Return (print) an error message to user.
	 *
	 * NOTE: This is the only SerialUI method to use "regular"
	 * char* (rather than PROGMEM), in order to avoid forcing
	 * you to hardcode all error messages.
	 *
	 */
	void returnError(const char * errmsg = NULL) {
		current_menu->returnError(errmsg);
	}

	/*
	 * showPrompt()
	 * Output the standard prompt.
	 */
	void showPrompt() ;

	/*
	 * showEnterDataPrompt()
	 * Output the "need more data" prompt, for when you
	 * are expecting user input.
	 */
	void showEnterDataPrompt();

	/*
	 * print_P(PGM_STRING)
	 * Same as Serial.print() but for progmem strings.
	 */
	size_t print_P(PGM_P msg);

	/*
	 * println_P(PGM_STRING)
	 * Same as Serial.println() but for progmem strings.
	 *
	 */
	size_t println_P(PGM_P msg);

	/* Stream interface implementations */
    virtual int available() { return StreamImplementation::available();}
    virtual int read() { return StreamImplementation::read(); }
    virtual int peek() { return StreamImplementation::peek(); }
    virtual void flush() { StreamImplementation::flush(); }

    void begin(unsigned long speed) { StreamImplementation::begin(speed); }


    /* Print (Stream parent) interface implementation */
    virtual size_t write(uint8_t i) { return StreamImplementation::write(i); }
    /* Stream interface extensions for progmem strings */


#ifdef SUI_INCLUDE_DEBUG
    void debug(const char * debugmsg);
    void debug_P(PGM_P debugmesg_p);
    void showFreeRAM();
#endif


private:
	PGM_P greeting_msg;
	Menu top_lev_menu;
	Menu * current_menu;
	bool user_check_performed;
	bool user_present;
	uint16_t user_presence_timeout_ms;
	uint16_t user_presence_last_interaction_ms;
	char read_terminator_char;
	unsigned long millisec_counter_start;


};

} /* end namespace SUI */

#endif

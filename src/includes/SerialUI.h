/*
 *
 * SerialUI.h -- Serial User Interface for Arduino-compatible devices.
 * Copyright (C) 2013-2017 Pat Deegan, psychogenic.com. All rights reserved.
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

#ifndef SERIALUI_MAINAPICLASS_H_
#define SERIALUI_MAINAPICLASS_H_

// SerialUI includes
#include "includes/SUIConfig.h"
#include "includes/SUIExtIncludes.h"
#include "includes/SUIStrings.h"
#include "includes/SUIMenu.h"
#include "includes/SUIPlatform.h"
#include "includes/SUIStateTracking.h"


/*
 * SUI_DeclareString(VARIABLENAME, "value");
 * SUI_DeclareString is a little macro to make setting up the
 * progmem string declarations easier.
 *
 */
#define SUI_DeclareString(name, value) \
	DEPRECATED_MACRO const char name[] PROGMEM = value;




#ifndef NULL
// make sure we always have a NULL symbol.
#define NULL 0x0
#endif



#ifdef SOVA_PROGMEM_PTR
#define SUI_STR(conts)		F(conts)
#else
#define SUI_STR(conts)		F(conts)
#endif




#define SUI_SETFLAG(flags, bitpos)		(flags |= (1 << bitpos))
#define SUI_CLEARFLAG(flags, bitpos)	(flags &= ~(1 << bitpos))
#define SUI_FLAGASBOOL(flags, bitpos)	(flags & (1 << bitpos))

// gen_flags
#define user_present_bit			0
#define user_check_performed_bit	1
#define response_transmitted_bit	2
#define echo_commands_bit			3
#define menu_manual_override_bit	4

#ifdef SUI_SERIALUI_ECHO_ON
#define gen_flags_init		(1 << echo_commands_bit)
#else
#define gen_flags_init		0
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


typedef void(*streamInputCallback)(char* buf, uint8_t len, size_t previous_position, size_t total_len);
typedef bool(*streamInputStartCallback)(size_t total_len);
typedef void(*streamInputEndCallback)(size_t total_len);

#ifdef SUI_ENABLE_USER_PRESENCE_HEARTBEAT
typedef void(*heartbeatFunction)(void);
#endif

typedef SovA::Utils::SinkBuffer<SUI_SERIALUI_PROGMEM_STRING_ABS_MAXLEN> SinkBuf;
typedef SovA::SovAStandardSysStreamType	SerialUIUnderStream;



class SerialUI : public SovA::Stream

{

public:

	/* 
	 * SerialUI constructor.
	 * The constructor is called with two (optional) parameters:
	 *
	 *  SerialUI([uint8_t num_top_level_menuitems_hint, [Stream* underlying_Stream]]);
	 *
	 *   - a message string to show on entry, declared with SUI_DeclareString
	 *   - a hint concerning the number of top level menu items (to avoid the cost
	 *     of memory re-allocation, when you have many--say, more than 3)
	 *   - a pointer to the underlying stream to use (defaults to system (HardwareSerial)Serial).
	 *     This third argument need only be used in special circumstances, e.g. when you want to 
	 *     use SerialUI over SoftwareSerial on some other pins
	 *
	 * As mentioned all are optional but a greeting is nice as it lets you know
	 * everything is working.
	 */
	SerialUI(uint8_t num_top_level_menuitems_hint = 0, SerialUIUnderStream * underlying_stream=NULL);
	SerialUI(uint8_t num_top_level_menuitems_hint, SovA::Delegate::Interface * useDelegate);


#ifdef DEADBEEF
#ifdef SOVA_PROGMEM_PTR
	DEPRECATED SerialUI(SOVA_PROGMEM_PTR greeting_message,
				uint8_t num_top_level_menuitems_hint = 0, SovA::SovAStandardSysStreamType * underlying_stream=NULL);
#endif
#endif

	virtual ~SerialUI();


	inline void setGreeting(SOVA_FLASHSTRING_PTR greets) { greeting_msg = greets;}
	inline void setUID(SOVA_FLASHSTRING_PTR u) { uid = u;}
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
	size_t readBytesToEOL(char* buffer, size_t max_length, bool left_trim=false);



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
	Menu * topLevelMenu(SOVA_FLASHSTRING_PTR setNameTo = NULL);

	inline void goToTopLevelMenu() {
		current_menu = &top_lev_menu;
	}

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
	 * TIMOUTMS is a timeout in ms.  If TIMEOUTMS is 0, the check
	 * will be performed and return immediately with an answer as to
	 * whether a user is present or not.
	 *
	 * With any positive value > 0, the call is blocking, for
	 * a maximum of TIMOUTMS ms (and a minimum of SUI_SERIALUI_USERCHECK_BLOCKFORINPUTDELAY_MS)
	 *
	 * Return boolean true if a user is present, false otherwise.
	 *
	 */
	bool checkForUser(uint16_t timeout_ms = 0);
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
	void handleRequests(uint8_t maxRequests=3);

	/*
	 * currentMenu()
	 * Returns a pointer to the currently active SUI::Menu.
	 */
	Menu * currentMenu() {
		return current_menu;
	}

	/*
	 * currentMenuItem()
	 * Returns a pointer to the menu item active during
	 * the call to callback/validation/changed.
	 * This is a (MenuItem::Base*) so you can access
	 * 	key and
	 * 	help
	 * (flash) strings, but if you know what you're doing, you
	 * may cast this to the appropriate type and thus get access
	 * to more juice.  E.g.
	 *  	// since we *know* this is an options list, in this scenario
	 *  	// cast and get access to the display string:
	 * 		MySUI.println(
	 *	((SUI::MenuItem::Request::OptionsList*)MySUI.currentMenuItem())->optionBySelection(2));
	 *
	 *
	 */
	inline MenuItem::Base * currentMenuItem() { return current_menu->currentItem();}


	/*
	 * Manually force the current SUI::Menu -- useful for things like
	 * "top" commands, to jump back to top level menu.
	 */
	void setCurrentMenu(Menu * setTo);

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
	void exit(bool terminate_gui=true);


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
	void returnMessage(SOVA_FLASHSTRING_PTR message) {
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

#ifndef SOVA_FLASHSTRING_DISABLE
	void returnError(SOVA_FLASHSTRING_PTR errmsg) {
		current_menu->returnError(errmsg);

	}
#endif

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
	 * showEnterNumericDataPrompt()
	 * Output the "need more *numerical* data" prompt, for when
	 * you need the user to enter a numeric value.
	 */
	void showEnterNumericDataPrompt();
#ifdef SUI_ENABLE_STREAMPROMPTING
	size_t showEnterStreamPromptAndReceive(char * bufferToUse, uint8_t bufferSize, streamInputCallback callback,
			streamInputStartCallback startupCallback=NULL, streamInputEndCallback completedCallback=NULL);
#endif




#ifdef SUI_ENABLE_MODES
    void setMode(Mode::Selection setTo) { output_mode = setTo;}
    inline Mode::Selection mode() { return output_mode;}
#endif

    inline bool echoCommands() { return SUI_FLAGASBOOL(gen_flags, echo_commands_bit);}
    void setEchoCommands(bool setTo);


#if defined(SUI_INCLUDE_DEBUG) or defined(SUI_INCLUDE_FREERAMFUNCS)
    void showFreeRAM();
#endif

#ifdef SUI_INCLUDE_DEBUG
    void debug(const char * debugmsg);
    void debug(SOVA_FLASHSTRING_PTR debugmsg);

#endif


#ifdef SUI_ENABLE_USER_PRESENCE_HEARTBEAT
    void setUserPresenceHeartbeat(heartbeatFunction hbf) { heartbeat_function_cb = hbf;}
    void setUserPresenceHeartbeatPeriod(uint16_t ms) { heartbeat_function_period = ms;}
    uint16_t userPresenceHeartbeatPeriod() { return heartbeat_function_period;}
#endif

#ifdef SUI_ENABLE_STATE_TRACKER
    int8_t trackState(SOVA_FLASHSTRING_PTR name, bool * var) ;
    int8_t trackState(SOVA_FLASHSTRING_PTR name, unsigned long * var);
    int8_t trackState(SOVA_FLASHSTRING_PTR name, float * var) ;
    int8_t trackState(SOVA_FLASHSTRING_PTR name, char * var) ;
    int8_t trackState(SOVA_FLASHSTRING_PTR name, ::String * var) ;
    int8_t trackState(Tracked::State * obj) { return addStateTracking(obj);}
    bool showTrackedState(bool force=false);


#endif

	inline bool responseTransmitted() { return SUI_FLAGASBOOL(gen_flags, response_transmitted_bit);}
	inline void markResponseTransmitted() { SUI_SETFLAG(gen_flags, response_transmitted_bit);}
	inline void markResponseRequired() { SUI_CLEARFLAG(gen_flags, response_transmitted_bit);}


private:
    void doInit(uint8_t num_top_level_menuitems_hint);

#ifdef SUI_ENABLE_USER_PRESENCE_HEARTBEAT
    void triggerHeartbeat(uint32_t timeNow);
#endif



    /*
    bool response_transmitted;
	bool user_check_performed;
	bool user_present;
	bool echo_commands;
	bool menu_manual_override;
	*/

    Mode::Selection output_mode;

    uint8_t gen_flags;


	SOVA_FLASHSTRING_PTR uid;
	SOVA_FLASHSTRING_PTR greeting_msg;
	Menu top_lev_menu;
	Menu * current_menu;
	uint16_t user_presence_timeout_ms;
	uint32_t user_presence_last_interaction_ms;
	char read_terminator_char;
	//unsigned long millisec_counter_start;
#ifdef SUI_ENABLE_STREAMPROMPTING
	size_t stream_expected_size;
	size_t stream_cur_count;
#endif
#ifdef SUI_ENABLE_USER_PRESENCE_HEARTBEAT
	heartbeatFunction heartbeat_function_cb;
	uint16_t		  heartbeat_function_period;
	uint32_t		  heartbeat_function_last_called;
#endif

#ifdef SUI_ENABLE_STATE_TRACKER
	uint8_t force_state_tracking_fulldump;
	Tracked::State * stateTrackedVars[SUI_STATE_TRACKER_MAX_VARIABLES];
	void initStateTrackedVars();
	int8_t stateTrackedVarsNextAvailableSlot();
	// int8_t addStateTracking(SOVA_FLASHSTRING_PTR name, TrackedType type, void* var);
	int8_t addStateTracking(Tracked::State* var);

	// int8_t addStateTracking(int8_t slot, TrackedStateVariableDetails* dets);
#endif

	SOVA_FLASHSTRING_PTR end_of_tx_str;



	inline void delegateSynch(bool forceFlush=true) { delegate()->tick(forceFlush);}


};

} /* end namespace SUI */

#endif /* SERIALUI_MAINAPICLASS_H_ */

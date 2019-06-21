/*
 * SerialUI.h
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  SerialUI is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUIV3_SRC_INCLUDES_SERIALUI_H_
#define SERIALUIV3_SRC_INCLUDES_SERIALUI_H_

#include "SerialUIExtIncludes.h"
#include "Menu.h"
#include "comm/ChannelManager.h"
#include "SerialUIPlatform.h"
#include "tracked/tracked.h"
#include "SUIGlobals.h"

#include  <string.h>

#define SUI	SerialUI


namespace SerialUI {



class SerialUI {
public:

	typedef void (*HeartbeatFunction)(void);

	typedef struct HeartBeatDetailsStruct {
		HeartbeatFunction func;
		TimeValue lastTriggerMs;
		uint16_t  periodMs;
		HeartBeatDetailsStruct() : func(NULL), lastTriggerMs(0), periodMs(500) {

		}

		TimeValue nextBeat() {
			return lastTriggerMs + periodMs;
		}
		inline bool isSet() {
			return (func != NULL);
		}
		void trigger(TimeValue tNow) {
			lastTriggerMs = tNow;
			if (! func) {
				return;
			}
			func();
		}

	} HeartBeatDetails;

	SerialUI(uint8_t num_top_level_menuitems_hint = 0,
			Comm::SourceType * commsrc = &(SUI_PLATFORM_SOURCE_DEFAULT)) ;

	virtual ~SerialUI();

	bool begin(uint32_t param);

	void setGreeting(StaticString greets);

	void setName(DynamicString sysbroadcastName);

	void setUID(StaticString u);
	/* ****************** configuration methods ***************** */
	/*
	 * setMaxIdleMs(MAXIDLETIME)
	 * Set the maximum amount of time (in ms) to wait for user
	 * requests before timing out.
	 */
	void setMaxIdleMs(uint16_t max_ms);
	/*
	 * maxIdleMs()
	 * Returns the current setting for maximum idle time (in ms)
	 */
	uint16_t maxIdleMs();

	/*
	 * setReadTerminator(TERMINATORCHAR)
	 * Set the input terminator character (defaults to '\n')
	 */
	void setReadTerminator(char setTo);

	/*
	 * readTerminator()
	 * Returns the current input terminator character.
	 */
	char readTerminator();

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
	size_t readBytesToEOL(char* buffer, size_t max_length, bool left_trim =
			false);

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
	Menu::Menu * topLevelMenu(StaticString setNameTo = NULL);
	Menu::Item::SubMenu * topLevel();

	void goToTopLevelMenu();

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
			SERIALUI_USERCHECK_MAXDELAY_DEFAULT_MS);

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
	void handleRequests(uint8_t maxRequests = 3);
	/*
	 * currentMenu()
	 * Returns a pointer to the currently active SUI::Menu.
	 */
	Menu::Menu * currentMenu();

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
	Menu::Item::Item * currentMenuItem();

	/*
	 * Manually force the current SUI::Menu -- useful for things like
	 * "top" commands, to jump back to top level menu.
	 */
	void setCurrentMenu(Menu::Menu * setTo);

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
	void exit(bool terminate_gui = true);

	/* ******************** return messages ******************* */

	/*
	 * returnOK()
	 * Return (print) a standard OK response to user.
	 */
	void returnOK();

	/*
	 * returnMessage(MESSAGE)
	 * Return (print) a message to user.  MESSAGE must be a
	 * progmem string (most likely created with SUI_DeclareString).
	 */
	void returnMessage(StaticString message);

	/*
	 * returnError(ERRORMESSAGE)
	 * Return (print) an error message to user.
	 *
	 * NOTE: This is the only SerialUI method to use "regular"
	 * char* (rather than PROGMEM), in order to avoid forcing
	 * you to hardcode all error messages.
	 *
	 */
	void returnError(const char * errmsg = NULL);
#ifdef SUI_STATICSTRING_IS_DISTINCT_TYPE
	void returnError(StaticString errmsg = NULL);
#endif


	/*
	 * showPrompt()
	 * Output the standard prompt.
	 */
	void showPrompt();

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

	void setMode(Mode::Selection setTo);
	Mode::Selection mode();

	inline bool echoCommands() {
		return Globals::state()->flags().echo_commands;
	}
	void setEchoCommands(bool setTo);

	void setUserPresenceHeartbeat(HeartbeatFunction hbf);
	void setUserPresenceHeartbeatPeriod(uint16_t ms);
	uint16_t userPresenceHeartbeatPeriod();
	// userHeartbeatIfRequired -- to be used if you want
	// to keep beating the heartbeat function, at the same
	// rythm, when no users are present.  just add to main loop()
	// outside of the serialui stuff.
	void userHeartbeatIfRequired() { triggerHeartbeat(SUI_PLATFORM_TIMENOW_MS());}




	bool trackState(StaticString name, Tracked::State & tracked);
	bool trackState(Tracked::State & tracked);

	/*

	int8_t trackState(StaticString name, bool * var);
	int8_t trackState(StaticString name, unsigned long * var);
	int8_t trackState(StaticString name, float * var);
	int8_t trackState(StaticString name, char * var);
	int8_t trackState(StaticString name, ::String * var);
	*/

	bool showTrackedState(bool force = false);

	inline bool responseTransmitted() {
		return Globals::state()->flags().response_transmitted;
	}
	inline void markResponseTransmitted() {
		Globals::state()->flags().response_transmitted = true;
	}
	inline void markResponseRequired() {

		Globals::state()->flags().response_transmitted = false;
	}

	/* *********** Print interface ************** */

	void updatedLocally(Menu::Item::Request::Request & itm);


#ifdef SUI_STATICSTRING_IS_DISTINCT_TYPE
    size_t print(StaticString s);
#endif
    size_t print(const String &);
    size_t print(DynamicString p);
    size_t print(char);
    size_t print(unsigned char, int = DEC);
    size_t print(int, int = DEC);
    size_t print(unsigned int, int = DEC);
    size_t print(long, int = DEC);
    size_t print(unsigned long, int = DEC);
    size_t print(double, int = 2);

#ifdef SUI_STATICSTRING_IS_DISTINCT_TYPE
    size_t println(StaticString s);
#endif
    size_t println(const String &s);
    size_t println(const char *);
    size_t println(char);
    size_t println(unsigned char, int = DEC);
    size_t println(int, int = DEC);
    size_t println(unsigned int, int = DEC);
    size_t println(long, int = DEC);
    size_t println(unsigned long, int = DEC);
    size_t println(double, int = 2);
    size_t println(void);


private:
	void triggerHeartbeat(TimeValue timeNow);
	Comm::Channel * comm();


	TimeValue _userLastInteractionMs;
	Comm::SourceType * _commsrc;
	// Comm::Channel * _Xcomm;
	HeartBeatDetails _heartbeat;
	uint16_t _userPresenceTimeoutMs;

	char _readTerminatorChar;

};

} /* namespace SerialUI */

#endif /* SERIALUIV3_SRC_INCLUDES_SERIALUI_H_ */

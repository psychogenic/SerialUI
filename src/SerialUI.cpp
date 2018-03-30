/*
 *
 * SerialUI.cpp -- SerialUI implementation
 * Copyright (C) 2013-2017 Pat Deegan, psychogenic.com. All rights reserved.
 *
 * See SerialUI.h for usage details.
 *
 * http://www.flyingcarsandstuff.com/projects/SerialUI
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
 */

#include "SerialUI.h"
#include "includes/SUIPlatform.h"


#define SUI_SERIALUI_HANDLEREQ_DELAY_MS		3

#ifdef SUI_INCLUDE_DEBUG
// debug is ON, call the debug() method:
#define SERIALUI_DEBUG(...)		SOVA_DEBUGSERIALPORT.println(__VA_ARGS__);
#else
// debug is OFF, nullify all the debug output:
#define SERIALUI_DEBUG(msg)		;
#endif


#ifdef SUI_ENABLE_USER_PRESENCE_HEARTBEAT
#define SUI_TRIGGERHEARTBEAT(timeNow)	triggerHeartbeat( (timeNow) )
#else
#define SUI_TRIGGERHEARTBEAT(timeNow)
#endif




namespace SUI {

/*
class SUISinkBuffer : public SovA::Utils::SinkBuffer<SUI_SERIALUI_PROGMEM_STRING_ABS_MAXLEN>  {
public:

	SUISinkBuffer() : SovA::Utils::SinkBuffer<SUI_SERIALUI_PROGMEM_STRING_ABS_MAXLEN>() {

	}
	 Sink & operator<<(Tracked::State & st) {
		 char rBuf[SUI_SERIALUI_PROGMEM_STRING_ABS_MAXLEN / 2] = {0,};
		 uint8_t maxLen = this->spaceAvailable() > (SUI_SERIALUI_PROGMEM_STRING_ABS_MAXLEN / 2) ?
				 	 this->spaceAvailable() : (SUI_SERIALUI_PROGMEM_STRING_ABS_MAXLEN / 2);
		 if (st.toString(rBuf, maxLen))
		 {
			 *this << &(rBuf[0]);
		 }
		 return *this;
	 }
};
*/




SerialUI::SerialUI(uint8_t num_top_level_menuitems_hint,
		SovA::Delegate::Interface * aDelegate) : SovA::Stream(),
		output_mode(Mode::User),
		gen_flags(gen_flags_init),
		uid(NULL),
		greeting_msg(NULL),top_lev_menu(), current_menu(NULL),
				user_presence_timeout_ms(SUI_SERIALUI_USERPRESENCE_MAXTIMEOUT_DEFAULT_MS),
				user_presence_last_interaction_ms(0),
				read_terminator_char(SUI_SERIAL_UI_READ_CHAR_TERMINATOR_DEFAULT),
#ifdef SUI_ENABLE_USER_PRESENCE_HEARTBEAT
				heartbeat_function_cb(NULL),
				heartbeat_function_period(SUI_USER_PRESENCE_HEARTBEAT_PERIOD_DEFAULT_MS),
				heartbeat_function_last_called(0),
#endif
#ifdef SUI_ENABLE_STATE_TRACKER
				force_state_tracking_fulldump(0),
#endif
				end_of_tx_str(SUI_STR(SUI_SERIALUI_PROG_ENDOFTRANSMISSION))
{
	this->useDelegate(aDelegate);

	doInit(num_top_level_menuitems_hint);
}


SerialUI::SerialUI(uint8_t num_top_level_menuitems_hint,
		SovA::SovAStandardSysStreamType * underlying_stream) : SovA::Stream(),
		output_mode(Mode::User),
		gen_flags(gen_flags_init),
		uid(NULL),
		greeting_msg(NULL),top_lev_menu(), current_menu(NULL),
				user_presence_timeout_ms(SUI_SERIALUI_USERPRESENCE_MAXTIMEOUT_DEFAULT_MS),
				user_presence_last_interaction_ms(0),
				read_terminator_char(SUI_SERIAL_UI_READ_CHAR_TERMINATOR_DEFAULT),
#ifdef SUI_ENABLE_USER_PRESENCE_HEARTBEAT
				heartbeat_function_cb(NULL),
				heartbeat_function_period(SUI_USER_PRESENCE_HEARTBEAT_PERIOD_DEFAULT_MS),
				heartbeat_function_last_called(0),
#endif
#ifdef SUI_ENABLE_STATE_TRACKER
				force_state_tracking_fulldump(0),
#endif
				end_of_tx_str(SUI_STR(SUI_SERIALUI_PROG_ENDOFTRANSMISSION))
{


	if (underlying_stream != NULL)
	{
		// would like to use dynamic_cast but ain't gonna happen, here... no-rtti
		// pretty safe in any case, because of how we're setup
		// for every other part of the program, we just use the delegate interface, so
		// when you're doing fancy delegate stuff, the method is
		//  default c'tor + useDelegate() later, no messing with underlying stream
		static_cast<SovA::StreamDelegate*>(delegate())->setStream(underlying_stream);
	}


	doInit(num_top_level_menuitems_hint);
}


SerialUI::~SerialUI() {
#ifdef SUI_ENABLE_STATE_TRACKER
	for (uint8_t i=0; i<SUI_STATE_TRACKER_MAX_VARIABLES; i++)
	{
		if (stateTrackedVars[i] && stateTrackedVars[i]->systemGenerated())
		{
			delete stateTrackedVars[i];
		}
	}
#endif

}

void SerialUI::doInit(uint8_t num_top_level_menuitems_hint) {



	top_lev_menu.init(this, SUI_STR(SUI_SERIALUI_TOP_MENU_NAME), num_top_level_menuitems_hint, NULL);

	current_menu = &top_lev_menu;

#ifdef SUI_INCLUDE_EXTRA_SAFETYCHECKS
	if (greeting_msg
			&& STRLEN_FLASHSTR(greeting_msg) > SUI_SERIALUI_PROGMEM_STRING_ABS_MAXLEN) {
		greeting_msg = SUI_STR(SUI_ERRORMSG_SUIGREETING_TOOLONG);
	}
#endif

#ifdef SUI_ENABLE_STATE_TRACKER
	initStateTrackedVars();
#endif

}


size_t SerialUI::readBytesToEOL(char* buffer, size_t max_length, bool left_trim)
{
	// a modified version of readBytesUntil, from Arduino Stream,  LGPLed
	// Copyright (c) 2010 David A. Mellis.
	size_t count = 0;

	unsigned long millistimeout = PLATFORM_NOW_MILLIS() + timeout();

	bool infiniteWaitForEOL = (timeout() < 1);
	bool isEndline = false;


	memset(buffer, 0, max_length); // forced clean


	// loop around until we either
	//  - get an EOL;
	//  - accumulate max_length chars; or
	//  - timeout
	do {

		if (!this->available()) {
			// nothing in queue, sync & loop
			delegateSynch(true);
			continue;
		}

		// get next char
		int c = read();

		if (c < 0) {
			// sanity check fail
			continue;
		}

		// flag if it's an endline
		if ( c == '\r' || c == '\n' ) {
			isEndline = true;
		} else {
			isEndline = false;
		}


		// if we're left trimming, do so.
		if (left_trim) {

			if (isEndline || c == ' ' || c == '\t') {
				// left trimming and this is whitespace: throw it away
				// and loop
				continue;
			}

			// we were left trimming, but are now done
			left_trim = false;
		}

		if (isEndline || c == read_terminator_char ) {
			// we hit the EOL
			return count;
		}

		// still accumulating, add it to ret buf
		buffer[count++] = (char) c;


	} while ( (count < max_length) && (infiniteWaitForEOL || (PLATFORM_NOW_MILLIS() < millistimeout) ));

	return count;

}
void SerialUI::enter() {
	if (greeting_msg) {
		current_menu->returnMessage(greeting_msg);
	}

#ifdef SUI_ENABLE_STATE_TRACKER
	force_state_tracking_fulldump = 8;

#endif

#ifdef SUI_SERIALUI_SHOW_PROMPTS
	showPrompt();
#endif

}



void SerialUI::exit(bool terminate_gui)
{
#ifdef SUI_ENABLE_MODES

	if (mode() == Mode::Program) {

		if (terminate_gui)
			PRINTLN_FLASHSTR(SUI_STR(SUI_SERIALUI_TERMINATE_GUI_PROG));

		PRINTLN_FLASHSTR(end_of_tx_str);


		// ensure we always restart in "user" mode...
		setMode(Mode::User);

	} else

#endif
	{

		returnMessage(SUI_STR("\r\nThanks for using SerialUI!"));
	}

	SUI_CLEARFLAG(gen_flags, user_present_bit);

	user_presence_last_interaction_ms = 0;
	// go back to top level menu, in case we re-enter SUI later...
	current_menu = &top_lev_menu;


}

Menu * SerialUI::topLevelMenu(SOVA_FLASHSTRING_PTR setNameTo) {
	if (setNameTo) {
		top_lev_menu.setName(setNameTo);
	}

	return &top_lev_menu;
}

bool SerialUI::checkForUserOnce(uint16_t timeout_ms) {

	delegate()->tick();
	if (SUI_FLAGASBOOL(gen_flags, user_check_performed_bit)) {
		// already done this...
#ifdef SUI_NOUSER_HEARTBEAT_ENABLE
		SUI_TRIGGERHEARTBEAT(PLATFORM_NOW_MILLIS());
#endif
		return false;
	}

	SERIALUI_DEBUG(F("Checking for user (once)"));
	SUI_SETFLAG(gen_flags, user_check_performed_bit);


	return checkForUser(timeout_ms);
}

bool SerialUI::checkForUser(uint16_t timeout_ms) {
	uint16_t ms_count = 0;
	while (ms_count <= timeout_ms) {
		delegate()->tick(true);
		if (this->available() > 0) {
			SUI_SETFLAG(gen_flags, user_present_bit);
			this->enter();
			return true;
		}

		// nothing on serial line, as of yet

		// if we're tripping the heartbeat regardless of
		// user presence, then do so here.
		// when a user *is* present, the heartbeat will
		// be handled within handleRequests().
#ifdef SUI_NOUSER_HEARTBEAT_ENABLE
		SUI_TRIGGERHEARTBEAT(PLATFORM_NOW_MILLIS());
#endif


		// if this was a "non-blocking" check, return immediately

		if (! timeout_ms)
			return false;

		// ok, a delay is set... so we'll break it up into smaller
		// chunks and keep checking until we run out of time
		delay(SUI_SERIALUI_USERCHECK_BLOCKFORINPUTDELAY_MS);
		ms_count += SUI_SERIALUI_USERCHECK_BLOCKFORINPUTDELAY_MS;
	}

	return false;
}


bool SerialUI::userPresent() {
	if (! SUI_FLAGASBOOL(gen_flags, user_present_bit)) {
		// already determined they're outta here
		return false;
	}

	// there _was_ a user, but let's see how long it's
	// been since they performed any actions
	if (user_presence_last_interaction_ms &&
			((PLATFORM_NOW_MILLIS() - user_presence_last_interaction_ms)
					> user_presence_timeout_ms)) {

		// yeah, looks like they're gone for coffee...

		SERIALUI_DEBUG(F("User idles excessively, bumping out..."));
		// this will set user_present to false and
		// perform other work
		exit(false);
	}

	return SUI_FLAGASBOOL(gen_flags, user_present_bit);


}
void SerialUI::setCurrentMenu(Menu * setTo)
{
	if (setTo)
	{
		current_menu = setTo;
		SUI_SETFLAG(gen_flags, menu_manual_override_bit);
	}
}


#ifdef SUI_ENABLE_USER_PRESENCE_HEARTBEAT
void SerialUI::triggerHeartbeat(uint32_t timeNow) {

		if (heartbeat_function_cb != NULL)
		{
			if (timeNow >= (heartbeat_function_last_called + heartbeat_function_period))
			{
				// time to call again...
				heartbeat_function_cb();
				heartbeat_function_last_called = timeNow;
			}
		}

}
#endif

void SerialUI::handleRequests(uint8_t maxRequests) {
	Menu * ret_menu;
	uint32_t timeNow;
	for (uint8_t i = 0; i <= maxRequests; i++) {

		timeNow = PLATFORM_NOW_MILLIS();

		delegateSynch(true);

		if (this->available() > 0) {

			SERIALUI_DEBUG(F("Handling pending request"));

			// we have input (and therefore a user), so we reset our presence counter to zero

			ret_menu = current_menu->handleRequest();
			user_presence_last_interaction_ms = PLATFORM_NOW_MILLIS();

			delegateSynch(true);
			if (ret_menu) {
				if (SUI_FLAGASBOOL(gen_flags, menu_manual_override_bit))
				{
					// leave the current menu as is, as it was overridden in
					// the last command
					SUI_CLEARFLAG(gen_flags, menu_manual_override_bit);
				} else {
					// make certain we reflect whatever changes are required
					// (e.g. the item was a submenu)
					current_menu = ret_menu;
				}
			} else {
				exit(false);
				delegateSynch();
				return;
			}

#ifdef SUI_SERIALUI_SHOW_PROMPTS

			showPrompt();

#endif /* end if show prompts */

		} else {

			delay(SUI_SERIALUI_HANDLEREQ_DELAY_MS);
			//println(user_presence_last_interaction_ms, DEC);
		}

		// trigger heartbeat--after handling any pending requests
		// so they don't interfere with any transactions coming in
		SUI_TRIGGERHEARTBEAT(timeNow);

		delegateSynch();
	}

	return;

}


void SerialUI::showPrompt() {



#ifdef SUI_ENABLE_MODES
	if (mode() == Mode::Program)
	{
		// println(' ');
		PRINTLN_FLASHSTR(end_of_tx_str);
		return;
	} else {
#endif

#ifdef SUI_SERIALUI_SHOW_PROMPT_SUBMENU_NAME
	if (current_menu->parent())
	{
		current_menu->showName();
	}
#endif
		PRINT_FLASHSTR(SUI_STR(SUI_SERIALUI_PROMPT));

#ifdef SUI_ENABLE_MODES
	}
#endif

	delegateSynch();
}



void SerialUI::showEnterDataPrompt() {

#ifdef SUI_ENABLE_MODES
	if (mode() == Mode::Program)
	{

		PRINT_FLASHSTR(SUI_STR(SUI_SERIALUI_MOREDATA_STRING_PROMPT_PROG));
		// PRINTLN_FLASHSTR(moredata_prompt_prog_str);
		PRINTLN_FLASHSTR(end_of_tx_str);

		delegateSynch();
		return;
	}
#endif

	PRINT_FLASHSTR(SUI_STR(SUI_SERIALUI_MOREDATA_STRING_PROMPT));
	delegateSynch();
}


void SerialUI::showEnterNumericDataPrompt() {


#ifdef SUI_ENABLE_MODES
	if (mode() == Mode::Program)
	{

		PRINT_FLASHSTR(SUI_STR(SUI_SERIALUI_MOREDATA_NUMERIC_PROMPT_PROG));
		PRINTLN_FLASHSTR(end_of_tx_str);
		delegateSynch();
		return;
	}
#endif

	PRINT_FLASHSTR(SUI_STR(SUI_SERIALUI_MOREDATA_NUMERIC_PROMPT));

	delegateSynch();

}


#ifdef SUI_ENABLE_STREAMPROMPTING
size_t SerialUI::showEnterStreamPromptAndReceive(char * bufferToUse, uint8_t bufferSize, streamInputCallback callback,
		streamInputStartCallback startupCallback, streamInputEndCallback completedCallback)
{

#ifdef SUI_ENABLE_MODES
	if (mode() == Mode::Program)
	{

		PRINT_FLASHSTR(SUI_STR(SUI_SERIALUI_MOREDATA_STREAM_PROMPT_PROG));
		PRINTLN_FLASHSTR(end_of_tx_str);
	} else {
#endif

	PRINT_FLASHSTR(SUI_STR(SUI_SERIALUI_MOREDATA_STREAM_PROMPT));
#ifdef SUI_ENABLE_MODES
	}
#endif

	delegateSynch();
	stream_expected_size = this->parseInt();

	if (stream_expected_size < 1)
		return 0;

	bool doPipedata = true; // if this is true, we'll send the data over to the callback
	// but no matter what, we'll need to consume it.
	if (startupCallback)
	{
		doPipedata = startupCallback(stream_expected_size);
	}



	stream_cur_count = 0;
	// dump the terminator
	delegateSynch(true);
	while (this->available() && (this->peek() == '\r' || this->peek() == '\n'))
	{
		this->read();
		delegateSynch(true);
	}


	while (stream_cur_count < stream_expected_size)
	{
		delegateSynch(true);
		uint8_t lenToRead = (stream_expected_size - stream_cur_count) > bufferSize ? bufferSize : (stream_expected_size - stream_cur_count);
		size_t lenRead = this->readBytes(bufferToUse, lenToRead);
		if (! lenRead)
		{
			// looks like we timed out...
			return stream_cur_count;
		}


		delegateSynch();
		if (doPipedata)
			callback(bufferToUse, lenRead, stream_cur_count, stream_expected_size);

		stream_cur_count += lenRead;
	}

	if (! doPipedata)
		return 0; // we just flushed the data...

	if (completedCallback)
		completedCallback(stream_cur_count);

	return stream_cur_count;
}

#endif

void SerialUI::setEchoCommands(bool setTo)
{
	if (setTo) {
		SUI_SETFLAG(gen_flags, echo_commands_bit);
	} else {

		SUI_CLEARFLAG(gen_flags, echo_commands_bit);
	}

}

#ifdef SUI_PLATFORM_DIGISPARKUSB
// special case here, as the digispark's tiny version
// of Stream/Print doesn't return a size on print--pretty annoying...
#define SUI_PRINTANDRETURN(function, value) \
	function(value); \
	return 1;
#else
// otherwise, we just return the value returned to us:
#define SUI_PRINTANDRETURN(function, value) \
	return function(value);
#endif



#ifdef SUI_ENABLE_STATE_TRACKER
void SerialUI::initStateTrackedVars() {
	for (uint8_t i = 0; i < SUI_STATE_TRACKER_MAX_VARIABLES; i++)
		stateTrackedVars[i] = NULL;
}
int8_t SerialUI::stateTrackedVarsNextAvailableSlot() {
	for (int8_t i = 0; i < SUI_STATE_TRACKER_MAX_VARIABLES; i++) {
		if (stateTrackedVars[i] == NULL)
			return i;
	}

	return -1;

}
int8_t SerialUI::trackState(SOVA_FLASHSTRING_PTR name, bool * var)
{

	Tracked::State * tState = new Tracked::Boolean(name, var);

	if (! tState)
		return -1;

	tState->setSystemGenerated(true);

	return this->addStateTracking(tState) ;


}
int8_t SerialUI::trackState(SOVA_FLASHSTRING_PTR name, unsigned long * var) {

	Tracked::State * tState = new Tracked::UInteger(name, var);

	if (! tState)
		return -1;

	tState->setSystemGenerated(true);

	return this->addStateTracking(tState) ;

}


int8_t SerialUI::trackState(SOVA_FLASHSTRING_PTR name, float * var) {
	Tracked::State * tState = new Tracked::Float(name, var);

	if (! tState)
		return -1;

	tState->setSystemGenerated(true);

	return this->addStateTracking(tState) ;

}

int8_t SerialUI::trackState(SOVA_FLASHSTRING_PTR name, char * var) {
	Tracked::State * tState = new Tracked::CString(name, var);

	if (! tState)
	return -1;

	tState->setSystemGenerated(true);

	return this->addStateTracking(tState);

}

int8_t SerialUI::trackState(SOVA_FLASHSTRING_PTR name, ::String * var) {
	Tracked::State * tState = new Tracked::AString(name, var);

	if (! tState)
		return -1;

	tState->setSystemGenerated(true);

	return this->addStateTracking(tState);


}

int8_t SerialUI::addStateTracking(Tracked::State* var)
{

	if (! var)
		return -1;

	int8_t slot = stateTrackedVarsNextAvailableSlot();
	if (slot < 0) {

		if (var->systemGenerated())
		{
			delete var;
		}
		return slot;
	}


	if (! var)
		return -1;

	stateTrackedVars[slot] = var;

	return slot;
}




bool SerialUI::showTrackedState(bool force)
{

#if defined(SUI_ENABLE_STATE_TRACKER) and defined(SUI_ENABLE_MODES)

	// char outBuf[SUI_SERIALUI_PROGMEM_STRING_ABS_MAXLEN] = {0,};
	// SUISinkBuffer outBuf;
	SinkBuf outBuf;

	// char numBuf[8 * sizeof(long) + 1];
	uint8_t idx = 0;
	uint8_t totlen = 0;

	if ( (mode() != Mode::Program) || stateTrackedVarsNextAvailableSlot() == 0)
	{
		// nothing to show...
		return false;
	}

	if (force_state_tracking_fulldump) {
		// only do this once, the first few times round
		force = true;
		force_state_tracking_fulldump--;
	}

	// outBuf[0] = '\0';
	outBuf << SUI_SERIALUI_PROG_STR_SEP_CHAR; // our initial sep
	while (stateTrackedVars[idx] != NULL && outBuf.spaceAvailable())
	{
		Tracked::State * st = stateTrackedVars[idx++];
		if (! (force || st->hasChanged()) )
		{
			continue;
		}
		outBuf << (char)st->type() << SUI_SERIALUI_PROG_STR_SEP_CHAR
				<< st->name() << SUI_SERIALUI_PROG_STR_SEP_CHAR;

		uint8_t incLen = st->toString(outBuf.charBuffer(outBuf.length()), outBuf.spaceAvailable());
		if (incLen)
		{
			outBuf.incrementPosition(incLen);
		}
		outBuf << SUI_SERIALUI_PROG_STR_SEP_CHAR;
		st->updateCache();

	}

	if (outBuf.length() > 1) {

		PRINT_FLASHSTR(SUI_STR(SUI_SERIALUI_TRACKEDSTATE_PREFIX_PROG));
		print((int) outBuf.length());
		print(outBuf.charBuffer());
	}


	// PRINTLN_FLASHSTR(end_of_tx_str);

	delegateSynch(true);

	return (totlen > 0);

#else
	return false;

#endif
}




#endif

} /* end namespace SUI */

#if defined(SUI_INCLUDE_DEBUG) or defined(SUI_INCLUDE_FREERAMFUNCS)

// debug is ON, we'll need a global-space freeRAM() function:
static int freeRAM()
{
	// neat little function from
	// http://jeelabs.org/2011/05/22/atmega-memory-use/
	extern int __heap_start, *__brkval;
	int v;
	return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}


// add some debug-related method implementations
namespace SUI {


void SerialUI::showFreeRAM()
{
	  extern int __heap_start, *__brkval;
	  int v;
	  PRINT_FLASHSTR(SUI_STR("Free RAM: "));
	  println(freeRAM());

}
}

#endif


#if defined(SUI_INCLUDE_DEBUG)

// add some debug-related method implementations
namespace SUI {


void SerialUI::debug(const char * debugmsg)
{
	SERIALUI_DEBUG(SUI_STR("DEBUG: "));
	SERIALUI_DEBUG(debugmsg);
}

void SerialUI::debug(SOVA_FLASHSTRING_PTR debugmsg)
{

	SERIALUI_DEBUG(SUI_STR("DEBUG: "));
	SERIALUI_DEBUG(debugmsg);
}



} /* end namespace SUI */
#endif


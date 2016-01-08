/*
 *
 * SerialUI.cpp -- SerialUI implementation
 * Copyright (C) 2013 Pat Deegan.  All rights reserved.
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

#define SUI_SERIALUI_HANDLEREQ_DELAY_MS		3

#ifdef SUI_INCLUDE_DEBUG
// debug is ON, call the debug() method:
#define SERIALUI_DEBUG(msg)		debug(msg);
#else
// debug is OFF, nullify all the debug output:
#define SERIALUI_DEBUG(msg)		;
#endif



namespace SUI {

// a few strings we'll need here (only)
SUI_DeclareString(top_menu_name, SUI_SERIALUI_TOP_MENU_NAME);
SUI_DeclareString(prompt_str, SUI_SERIALUI_PROMPT);

SUI_DeclareString(moredata_prompt_str, SUI_SERIALUI_MOREDATA_STRING_PROMPT);
SUI_DeclareString(moredata_prompt_num, SUI_SERIALUI_MOREDATA_NUMERIC_PROMPT);
SUI_DeclareString(moredata_prompt_stream, SUI_SERIALUI_MOREDATA_STREAM_PROMPT);

#ifdef SUI_ENABLE_MODES
SUI_DeclareString(end_of_tx_str, SUI_SERIALUI_PROG_ENDOFTRANSMISSION);
SUI_DeclareString(moredata_prompt_prog_str, SUI_SERIALUI_MOREDATA_STRING_PROMPT_PROG);
SUI_DeclareString(moredata_prompt_prog_num, SUI_SERIALUI_MOREDATA_NUMERIC_PROMPT_PROG);
SUI_DeclareString(moredata_prompt_prog_stream, SUI_SERIALUI_MOREDATA_STREAM_PROMPT_PROG);
SUI_DeclareString(terminate_gui_prog, SUI_SERIALUI_TERMINATE_GUI_PROG);

#ifdef SUI_ENABLE_STATE_TRACKER
SUI_DeclareString(tracked_state_prog_prefix, SUI_SERIALUI_TRACKEDSTATE_PREFIX_PROG);
#endif

#endif


SerialUI::SerialUI(PGM_P greeting_message, uint8_t num_top_level_menuitems_hint, StreamInstanceType * underlying_stream) :
		output_mode(SUIMode_User),
		greeting_msg(greeting_message),top_lev_menu(), current_menu(NULL),
				user_check_performed(false), user_present(false),
				user_presence_timeout_ms(SUI_SERIALUI_USERPRESENCE_MAXTIMEOUT_DEFAULT_MS),
				user_presence_last_interaction_ms(0),
				read_terminator_char(SUI_SERIAL_UI_READ_CHAR_TERMINATOR_DEFAULT),
#ifdef SUI_SERIALUI_ECHO_ON
				echo_commands(true),
#else
				echo_commands(false),
#endif
#ifdef SUI_ENABLE_USER_PRESENCE_HEARTBEAT
				heartbeat_function_cb(NULL),
				heartbeat_function_period(SUI_USER_PRESENCE_HEARTBEAT_PERIOD_DEFAULT_MS),
				heartbeat_function_last_called(0),
#endif
				menu_manual_override(false)
{

	if (underlying_stream != NULL)
	{
		StreamImplementation::setStream(underlying_stream);
	}

	top_lev_menu.init(this, top_menu_name, num_top_level_menuitems_hint, NULL);

	current_menu = &top_lev_menu;

#ifdef SUI_INCLUDE_EXTRA_SAFETYCHECKS
	if (greeting_msg
			&& strlen_P(greeting_msg) > SUI_SERIALUI_PROGMEM_STRING_ABS_MAXLEN) {
		current_menu->returnMessage(PSTR(SUI_ERRORMSG_SUIGREETING_TOOLONG));
	}
#endif

#ifdef SUI_ENABLE_STATE_TRACKER
	initStateTrackedVars();
#endif

}


size_t SerialUI::readBytesToEOL(char* buffer, size_t max_length)
{
	// a modified version of readBytesUntil, from Arduino Stream,  LGPLed
	// Copyright (c) 2010 David A. Mellis.
	size_t count = 0;
	millisec_counter_start = millis();

	bool noTimeout = timeout() < 1;


	if (max_length < 1)
	{
		return 0;
	}

	do {
		int c = read();
		if (c < 0)
			continue;

		if (c == '\r') {
			// hit a CR
			if (peek() == '\n') // get rid of newline if present
				read();
			break;
		}

		if (c == '\n' || c == read_terminator_char)
			break;


		*buffer++ = (char) c;
		count++;

	} while (count < max_length &&  (noTimeout || (millis() - millisec_counter_start) < timeout()));

	return count;

}
void SerialUI::enter() {
	if (greeting_msg) {
		current_menu->returnMessage(greeting_msg);
	}

#ifdef SUI_SERIALUI_SHOW_PROMPTS
	showPrompt();
#endif

}


SUI_DeclareString(goodbye,
		"\r\nThanks for using SerialUI! Goodbye.");


void SerialUI::exit(bool terminate_gui)
{

#ifdef SUI_ENABLE_MODES

	if (mode() == SUIMode_Program) {

		if (terminate_gui)
			println_P(terminate_gui_prog);

		println_P(end_of_tx_str);


		// ensure we always restart in "user" mode...
		setMode(SUIMode_User);

	} else

#endif
	{
		returnMessage(goodbye);
	}
	user_present = false;
	user_presence_last_interaction_ms = 0;
	// go back to top level menu, in case we re-enter SUI later...
	current_menu = &top_lev_menu;


}

Menu * SerialUI::topLevelMenu(PGM_P setNameTo) {
	if (setNameTo) {
		top_lev_menu.setName(setNameTo);
	}

	return &top_lev_menu;
}

bool SerialUI::checkForUserOnce(uint16_t timeout_ms) {
	if (user_check_performed) {
		// already done this...
		return false;
	}

	SERIALUI_DEBUG("Checking for user (once)");

	user_check_performed = true;

	return checkForUser(timeout_ms);
}

bool SerialUI::checkForUser(uint16_t timeout_ms) {
	uint16_t ms_count = 0;
	while (ms_count <= timeout_ms) {
		if (this->available() > 0) {
			user_present = true;
			return true;
		}

		// nothing on serial line, as of yet
		// if this was a "non-blocking" check, return immediately
		if (! timeout_ms)
			return false;

		delay(SUI_SERIALUI_USERCHECK_BLOCKFORINPUTDELAY_MS);
		ms_count += SUI_SERIALUI_USERCHECK_BLOCKFORINPUTDELAY_MS;
	}

	return false;
}


bool SerialUI::userPresent() {
	if (!user_present) {
		return false;
	}

	if (user_presence_last_interaction_ms >= user_presence_timeout_ms) {
		SERIALUI_DEBUG("User idles excessively, bumping out...");
		exit(false);
	}

	return user_present;

}
void SerialUI::setCurrentMenu(Menu * setTo)
{
	if (setTo)
	{
		current_menu = setTo;
		menu_manual_override = true;
	}
}

void SerialUI::handleRequests() {
	Menu * ret_menu;
	for (uint8_t i = 0; i <= 100; i++) {

#ifdef SUI_ENABLE_USER_PRESENCE_HEARTBEAT
		if (heartbeat_function_cb != NULL)
		{
			uint32_t timeNow = millis();
			if (timeNow >= (heartbeat_function_last_called + heartbeat_function_period))
			{
				// time to call again...
				heartbeat_function_cb();
				heartbeat_function_last_called = timeNow;
			}
		}
#endif



		if (this->available() > 0) {

			SERIALUI_DEBUG("Handling pending request");

			// we have input (and therefore a user), so we reset our presence counter to zero
			user_presence_last_interaction_ms = 0;

			ret_menu = current_menu->handleRequest();

			if (ret_menu) {
				if (menu_manual_override)
				{
					// leave the current menu as is, as it was overridden in
					// the last command
					menu_manual_override = false;
				} else {
					// make certain we reflect whatever changes are required
					// (e.g. the item was a submenu)
					current_menu = ret_menu;
				}
			} else {
				exit(false);
				return;
			}

#ifdef SUI_SERIALUI_SHOW_PROMPTS
			showPrompt();
#endif

		} else {

			delay(SUI_SERIALUI_HANDLEREQ_DELAY_MS);
			user_presence_last_interaction_ms += SUI_SERIALUI_HANDLEREQ_DELAY_MS;
			//println(user_presence_last_interaction_ms, DEC);
		}

	}

	return;

}


void SerialUI::showPrompt() {
#ifdef SUI_SERIALUI_SHOW_PROMPT_SUBMENU_NAME
	if (current_menu->parent())
	{
		current_menu->showName();
	}
#endif


	print_P(prompt_str);
#ifdef SUI_ENABLE_MODES
	if (mode() == SUIMode_Program)
	{
		println(' ');
		println_P(end_of_tx_str);
		// return;
	}
#endif

}



void SerialUI::showEnterDataPrompt() {

#ifdef SUI_ENABLE_MODES
	if (mode() == SUIMode_Program)
	{

		println_P(moredata_prompt_prog_str);
		println_P(end_of_tx_str);
		return;
	}
#endif

	print_P(moredata_prompt_str);
}


void SerialUI::showEnterNumericDataPrompt() {


#ifdef SUI_ENABLE_MODES
	if (mode() == SUIMode_Program)
	{

		println_P(moredata_prompt_prog_num);
		println_P(end_of_tx_str);
		return;
	}
#endif

	print_P(moredata_prompt_num);


}


#ifdef SUI_ENABLE_STREAMPROMPTING
size_t SerialUI::showEnterStreamPromptAndReceive(char * bufferToUse, uint8_t bufferSize, streamInputCallback callback,
		streamInputStartCallback startupCallback, streamInputEndCallback completedCallback)
{

#ifdef SUI_ENABLE_MODES
	if (mode() == SUIMode_Program)
	{

		println_P(moredata_prompt_prog_stream);
		println_P(end_of_tx_str);
	} else {
#endif

	print_P(moredata_prompt_stream);
#ifdef SUI_ENABLE_MODES
	}
#endif

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
	while (this->available() && (this->peek() == '\r' || this->peek() == '\n'))
	{
		this->read();
	}


	while (stream_cur_count < stream_expected_size)
	{
		uint8_t lenToRead = (stream_expected_size - stream_cur_count) > bufferSize ? bufferSize : (stream_expected_size - stream_cur_count);
		size_t lenRead = this->readBytes(bufferToUse, lenToRead);
		if (! lenRead)
		{
			// looks like we timed out...
			return stream_cur_count;
		}
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
	echo_commands = setTo;

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

size_t SerialUI::print_P(PGM_P message) {
	char p_buffer[SUI_SERIALUI_PROGMEM_STRING_ABS_MAXLEN + 1];
	strncpy_P(p_buffer, message, SUI_SERIALUI_PROGMEM_STRING_ABS_MAXLEN);

	SUI_PRINTANDRETURN(print, p_buffer);

}


size_t SerialUI::println_P(PGM_P message) {
	char p_buffer[SUI_SERIALUI_PROGMEM_STRING_ABS_MAXLEN + 1];
	strncpy_P(p_buffer, message, SUI_SERIALUI_PROGMEM_STRING_ABS_MAXLEN);

	SUI_PRINTANDRETURN(println, p_buffer);


}


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

int8_t SerialUI::addStateTracking(PGM_P name, TrackedType type, void* var)
{
	int8_t slot = stateTrackedVarsNextAvailableSlot();
	if (slot < 0)
		return slot;

	TrackedStateVariableDetails * trackedDets = new TrackedStateVariableDetails(name, type, var);

	if (! trackedDets)
		return -1;

	stateTrackedVars[slot] = trackedDets;

	return slot;

}

bool SerialUI::showTrackedState()
{

	char outBuf[SUI_SERIALUI_PROGMEM_STRING_ABS_MAXLEN];

	if (stateTrackedVarsNextAvailableSlot() == 0)
	{
		// nothing to show...
		return false;
	}


#ifdef SUI_ENABLE_MODES

	if (mode() == SUIMode_Program) {

		char numBuf[8 * sizeof(long) + 1];

		outBuf[0] = '\0';
		uint8_t totlen = 0;
		uint8_t idx = 0;
		unsigned long m;
		unsigned long tmpInt;
		while (stateTrackedVars[idx] != NULL)
		{
			outBuf[totlen++] = (char)stateTrackedVars[idx]->type;

			outBuf[totlen++] = SUI_SERIALUI_PROG_STR_SEP_CHAR;
			outBuf[totlen] = '\0';

			strcat_P(outBuf, stateTrackedVars[idx]->name);
			totlen += strlen_P(stateTrackedVars[idx]->name);

			outBuf[totlen++] = SUI_SERIALUI_PROG_STR_SEP_CHAR;
			outBuf[totlen] = '\0';

			//double dval = 0; // Assumes 8-bit chars plus zero byte.
			float dval = 0; // Assumes 8-bit chars plus zero byte.
			char *str = &numBuf[sizeof(numBuf) - 1];

			switch (stateTrackedVars[idx]->type)
			{
			case SUITracked_Bool:
				outBuf[totlen++] = (*(stateTrackedVars[idx]->ptr_bool) ? '1' : '0');
				break;
			case SUITracked_UInt:
#ifdef PLATFORM_DESKTOP
				totlen += sprintf(&(outBuf[totlen]), "%i", *(stateTrackedVars[idx]->ptr_int));
#else
				*str = '\0';
				tmpInt = *(stateTrackedVars[idx]->ptr_int);
				do {
					m = tmpInt;
					tmpInt /= 10;
					char c = m - 10 * tmpInt;
					*--str = c + '0';
				} while (tmpInt);

				strcat(&(outBuf[totlen]), str);
				totlen += strlen(str);

#endif
				break;

			case SUITracked_Float:
#ifdef PLATFORM_DESKTOP
				totlen += sprintf(&(outBuf[totlen]), "%.2f", *(stateTrackedVars[idx]->ptr_float));
#else
				dval = *(stateTrackedVars[idx]->ptr_float);
				totlen += SUI_CONVERT_FLOAT_TO_STRING_AND_RETLEN(dval, &(outBuf[totlen]));

						// strlen(dtostrf(dval, 5, 2, &(outBuf[totlen])));
#endif

				break;
			}

			outBuf[totlen++] = SUI_SERIALUI_PROG_STR_SEP_CHAR;
			outBuf[totlen] = '\0';

			idx++;
		}


		print_P(tracked_state_prog_prefix);
		print(strlen(outBuf) + 1);
		print(SUI_SERIALUI_PROG_STR_SEP_CHAR);
		println(outBuf);


		return (totlen > 0);
	}

#endif

	return false;
}

#endif

} /* end namespace SUI */


#ifdef SUI_INCLUDE_DEBUG

// debug is ON, we'll need a global-space freeRAM() function:
int freeRAM()
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
	  print_P(PSTR("Free RAM: "));
	  println(freeRAM());

}

void SerialUI::debug(const char * debugmsg)
{
	print_P(PSTR("DEBUG: "));
	println(debugmsg);
}

void SerialUI::debug_P(PGM_P debugmesg_p)
{
	print_P(PSTR("DEBUG: "));
	println_P(debugmesg_p);

}
} /* end namespace SUI */
#endif


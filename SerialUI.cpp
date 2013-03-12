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

#define SUI_SERIALUI_USERCHECK_DELAY_MS		3
#define SUI_SERIALUI_HANDLEREQ_DELAY_MS		5

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
SUI_DeclareString(moredata_prompt_str, SUI_SERIALUI_MOREDATA_PROMPT);


SerialUI::SerialUI(PGM_P greeting_message, uint8_t num_top_level_menuitems_hint) :
		greeting_msg(greeting_message),top_lev_menu(), current_menu(NULL),
				user_check_performed(false), user_present(false),
				user_presence_timeout_ms(SUI_SERIALUI_USERPRESENCE_MAXTIMEOUT_DEFAULT_MS),
				user_presence_last_interaction_ms(0),
				read_terminator_char(SUI_SERIAL_UI_READ_CHAR_TERMINATOR_DEFAULT)
				{


	top_lev_menu.init(this, top_menu_name, num_top_level_menuitems_hint, NULL);

	current_menu = &top_lev_menu;

#ifdef SUI_INCLUDE_EXTRA_SAFETYCHECKS
	if (greeting_msg
			&& strlen_P(greeting_msg) > SUI_SERIALUI_PROGMEM_STRING_ABS_MAXLEN) {
		current_menu->returnMessage(PSTR(SUI_ERRORMSG_SUIGREETING_TOOLONG));
	}
#endif

}


size_t SerialUI::readBytesToEOL(char* buffer, size_t max_length)
{
	// a modified version of readBytesUntil, from Arduino Stream,  LGPLed
	// Copyright (c) 2010 David A. Mellis.
	size_t count = 0;
	millisec_counter_start = millis();

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

	} while (count < max_length && (millis() - millisec_counter_start) < timeout());

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

void SerialUI::exit()
{
	returnMessage(goodbye);
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
	while (ms_count < timeout_ms) {
		if (available() > 0) {
			user_present = true;
			return true;
		}

		delay(SUI_SERIALUI_USERCHECK_DELAY_MS);
		ms_count += SUI_SERIALUI_USERCHECK_DELAY_MS;
	}

	return false;
}


bool SerialUI::userPresent() {
	if (!user_present) {
		return false;
	}

	if (user_presence_last_interaction_ms >= user_presence_timeout_ms) {
		SERIALUI_DEBUG("User idles excessively, bumping out...");
		exit();
	}

	return user_present;

}

void SerialUI::handleRequests() {
	Menu * ret_menu;
	for (uint8_t i = 0; i <= 100; i++) {
		if (available() > 0) {

			SERIALUI_DEBUG("Handling pending request");

			// we have input (and therefore a user), so we reset our presence counter to zero
			user_presence_last_interaction_ms = 0;

			ret_menu = current_menu->handleRequest();

			if (ret_menu) {
				current_menu = ret_menu;
			} else {
				exit();
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
}

void SerialUI::showEnterDataPrompt() {
	print_P(moredata_prompt_str);
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
void SerialUI::showFreeRAM()
{
	  extern int __heap_start, *__brkval;
	  int v;
	  print_P(PSTR("Free RAM: "));
	  println(freeRAM());

}
} /* end namespace SUI */
#endif


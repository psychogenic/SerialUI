/*
 *
 * SUIMenu.cpp -- SerialUI Menu implementation
 * Copyright (C) 2013 Pat Deegan.  All rights reserved.
 *
 * See SUIMenu.h for usage details.
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



#include "includes/SUIConfig.h"

#include "SerialUI.h"
#include "includes/SUIMenu.h"
#include "includes/SUIStrings.h"
#include "includes/SUIPlatform.h"
#include "includes/MenuItem.h"



#ifdef SUI_DYNAMIC_MEMORY_ALLOCATION_ENABLE
// dynamic memory management enabled
// stdlib included for new
#include <stdlib.h>
#define MENUITEMLIST	item_list
#define MENUFREE(pointer)	delete pointer
#define MENUFREESTRING(pointer)	delete [] pointer
#define MENUFREEKEY(k)	MENUFREESTRING(k)

#define MENUMEMSET(pointer, value, len)		memset(pointer, value, len);
#else
#error "SerialUI: Must use dynamic mem for v2.0+"

#endif


#define SUI_MENU_PROGMEM_STRING_ABS_MAXLEN		SUI_SERIALUI_PROGMEM_STRING_ABS_MAXLEN

#ifdef SUI_INCLUDE_DEBUG

// debug is ON

#if 0
DEADBEEF
#define SUI_MENU_DEBUG_OUTPUT(msg)	sui_driver->debug_P(PSTR(msg));
#else
#define SUI_MENU_DEBUG_OUTPUT(msg)	sui_driver->debug(F(msg));
#endif
#else
// debug is OFF, send debug messages into the void
#define SUI_MENU_DEBUG_OUTPUT(msg) ;
#endif

namespace SUI {


#define help_key SUI_STR(SUI_SERIALUI_HELP_KEY)
#define help_help SUI_STR(SUI_SERIALUI_HELP_HELP)

#ifdef SUI_MENU_ENABLE_SUBMENUS
#define up_key SUI_STR(SUI_SERIALUI_UP_KEY)
#define up_help SUI_STR(SUI_SERIALUI_UP_HELP)
#define submenu_help SUI_STR(SUI_SERIALUI_SUBMENU_HELP)
#endif

#define help_title_prefix SUI_STR(SUI_SERIALUI_HELP_TITLE_PREFIX)


#define help_key_commandprefix SUI_STR(SUI_SERIALUI_KEYHELP_COMMAND_PREFIX)
//#define help_key_submenuprefix SUI_STR(SUI_SERIALUI_KEYHELP_SUBMENU_PREFIX)

#define unknown_sel SUI_STR(SUI_SERIALUI_UNKNOWN_SELECTION)
#define unknown_inmenu SUI_STR(SUI_SERIALUI_UNKNOWN_INMENU)
#define help_hint SUI_STR(SUI_SERIALUI_HELP_HINT)

#define ok_msg SUI_STR(SUI_SERIALUI_MESSAGE_OK)
#define error_generic SUI_STR(SUI_SERIALUI_MESSAGE_ERROR_GENERIC)
#define error_prefix SUI_STR(SUI_SERIALUI_MESSAGE_ERROR_PREFIX)
#define help_sep SUI_STR(SUI_SERIALUI_KEYHELP_SEP)



#define exit_key SUI_STR(SUI_SERIALUI_EXIT_KEY)
#define exit_help SUI_STR(SUI_SERIALUI_EXIT_HELP)

#ifdef SUI_SERIALUI_ECHO_WARNINGS
#define error_cantalloc_submenu SUI_STR(SUI_ERRORMSG_CANTALLOCATE_SUBMENU)
#define error_cantalloc_menuitem SUI_STR(SUI_ERRORMSG_CANTALLOCATE_MENUITEM)
#define error_cantalloc_key SUI_STR(SUI_ERRORMSG_CANTALLOCATE_KEY)

#define error_nomenuitems SUI_STR(SUI_ERRORMSG_NO_MENUITEMS_SET)

#endif




#ifdef SUI_ENABLE_MODES
// Mode related strings
#define key_mode_user SUI_STR(SUI_STRINGS_MODE_USER)
#define key_mode_program SUI_STR(SUI_STRINGS_MODE_PROGRAM)
#define key_ping_command SUI_STR(SUI_STRINGS_PING_COMMAND)

// program-mode strings...
#define help_key_prog_commandprefix SUI_STR(SUI_SERIALUI_KEYHELP_COMMAND_PREFIX_PROG)
#define help_key_prog_submenuprefix SUI_STR(SUI_SERIALUI_KEYHELP_SUBMENU_PREFIX_PROG)
#define help_key_prog_requestbaseprefix SUI_STR(SUI_SERIALUI_KEYHELP_REQUEST_BASE_PREFIX_PROG)

#define help_sep_prog SUI_STR(SUI_SERIALUI_KEYHELP_SEP_PROG)


// prog-mode enter info output
#define prog_mode_info_helpkey SUI_STR(SUI_SERIALUI_HELP_KEY)
#define prog_mode_info_moreprompt_string SUI_STR(SUI_SERIALUI_MOREDATA_STRING_PROMPT_PROG)
#define prog_mode_info_moreprompt_num SUI_STR(SUI_SERIALUI_MOREDATA_NUMERIC_PROMPT_PROG)

#define prog_mode_terminate_gui SUI_STR(SUI_SERIALUI_TERMINATE_GUI_PROG)

#ifdef SUI_ENABLE_STREAMPROMPTING
#define prog_mode_info_moreprompt_stream SUI_STR(SUI_SERIALUI_MOREDATA_STREAM_PROMPT_PROG)
#endif

#define prog_mode_info_EOT SUI_STR(SUI_SERIALUI_PROG_ENDOFTRANSMISSION)

#define prog_mode_info_VERSION SUI_STR(SERIAL_UI_VERSION_STRING)

#endif




//define SUIDRIVER_PRINTLN_FLASH(msg)	sui_driver->println_P(msg)


#if 0
#define SUIDRIVER_PRINTLN_FLASH(msg)	sui_driver->println_P(msg)
#define SUIDRIVER_PRINT_FLASH(msg)	sui_driver->print_P(msg)
#else

#define SUIDRIVER_PRINTLN_FLASH(msg)	sui_driver->println(msg)
#define SUIDRIVER_PRINT_FLASH(msg)	sui_driver->print(msg)
#endif


size_t Menu::max_key_len = 0;

#if 0
DEADBEEF
MenuItemDetailsStruct::MenuItemDetailsStruct(SUI_FLASHSTRING_PTR key, MenuCommand_Callback cb, SUI_FLASHSTRING_PTR help) :
			key_str(key), callback(cb), help_str(help)
{

}

MenuItemStruct::MenuItemStruct()
{

}
MenuItemStruct::MenuItemStruct(SUI_FLASHSTRING_PTR key_pstr, SUI_FLASHSTRING_PTR help_pstr,
		Menu * submenu_ptr, MenuCommand_Callback cmd_ptr)
{
	key = key_pstr;
	help = help_pstr;
	subMenu = submenu_ptr;
	command = cmd_ptr;
	key_size = STRLEN_FLASHSTR(key);
}


#endif



Menu::Menu(SerialUI * suidrv, SUI_FLASHSTRING_PTR name, uint8_t num_items_hint, Menu * parent) :
		sui_driver(suidrv),
		menu_name(name), num_menu_items(0), max_menu_items(0), expand_list_amount(
				SUI_MENU_EXPANDITEMLIST_AMOUNT_DEFAULT), item_list(NULL),
				parent_menu(parent) {
	init(suidrv, name, num_items_hint, parent);
}
Menu::Menu() :
	sui_driver(NULL),
	menu_name(NULL), num_menu_items(0), max_menu_items(0), expand_list_amount(
					SUI_MENU_EXPANDITEMLIST_AMOUNT_DEFAULT), item_list(NULL),
					parent_menu(NULL)
{

}
#ifdef SUI_MENU_INCLUDE_DESTRUCTION_CLEANUP
Menu::~Menu()
{
	clear();
}

void Menu::clear()
{

	if (item_list)
	{
		for (uint8_t i=0; i<num_menu_items; i++)
		{
			if (item_list[i])
				MENUFREE(item_list[i]);
		}
		delete [] item_list;
		item_list = NULL;
	}

	num_menu_items = 0;
	max_menu_items = 0;

}
#endif


void Menu::init(SerialUI * suidrv, SUI_FLASHSTRING_PTR name, uint8_t num_items_hint, Menu * parent) {


	sui_driver = suidrv;
	menu_name = name;
	parent_menu = parent;
	num_menu_items = 0;
	max_menu_items = 0;

	expand_list_amount = SUI_MENU_EXPANDITEMLIST_AMOUNT_DEFAULT;
	item_list = NULL;


	// set our max_key_len to hold *at least*
	// the available built-in commands
	if (max_key_len < 1)
	{
		// max_key_len is now a class-level var, so
		// we only need to do this once.  It will
		// eventually hold the max key length for the
		// largest key in the whole menu system.
#ifdef SUI_ENABLE_MODES
		max_key_len = STRLEN_FLASHSTR(key_mode_program);
#else
		max_key_len = STRLEN_FLASHSTR(help_key);
		if (STRLEN_FLASHSTR(exit_key) > max_key_len)
		{
			max_key_len = STRLEN_FLASHSTR(exit_key);
		}
#endif
	}

#ifdef SUI_INCLUDE_EXTRA_SAFETYCHECKS
	if (STRLEN_FLASHSTR(menu_name) > SUI_MENU_PROGMEM_STRING_ABS_MAXLEN) {
		returnMessage(SUI_STR(SUI_ERRORMSG_MENUNAME_TOOLONG));
	}
#endif


#ifdef SUI_INCLUDE_DEBUG
	if (parent_menu)
	{
		// can't start doing output before SerialUI is initialized
		// so we only do this stuff for sub-menus
		SUI_MENU_DEBUG_OUTPUT("Initializing Menu ");
		showName();
		sui_driver->println(" ");
	}
#endif

	expandItemList(num_items_hint);
}

SUI_FLASHSTRING_PTR Menu::name() {
	return menu_name;
}

void Menu::setName(SUI_FLASHSTRING_PTR namestr) {
	menu_name = namestr;
}

Menu * Menu::parent() {
	return parent_menu;
}


bool Menu::addRequest(long int * val, SUI_FLASHSTRING_PTR key_pstr, SUI_FLASHSTRING_PTR help_pstr,
		bool(*validator)(long int &),
		MenuRequest_Callback valueChangedCb) {
	MenuItem::RequestLong * aReq = new MenuItem::RequestLong(val, key_pstr, help_pstr, validator, valueChangedCb);
	if (! aReq)
		returnMessage(SUI_STR("Can't req!"));

	return addMenuItem(aReq);

}
bool Menu::addRequest(long unsigned int * val, SUI_FLASHSTRING_PTR key_pstr, SUI_FLASHSTRING_PTR help_pstr,
		bool(*validator)(long unsigned int &),
		MenuRequest_Callback valueChangedCb) {
	MenuItem::RequestULong * aReq = new MenuItem::RequestULong(val, key_pstr, help_pstr, validator, valueChangedCb);
	if (! aReq)
		returnMessage(SUI_STR("Can't req!"));

	return addMenuItem(aReq);

}

bool Menu::addRequest(char* val, SUI_FLASHSTRING_PTR key_pstr, SUI_FLASHSTRING_PTR help_pstr,
		bool(*validator)(char &),
		MenuRequest_Callback valueChangedCb) {
	MenuItem::RequestChar * aReq = new MenuItem::RequestChar(val, key_pstr, help_pstr, validator, valueChangedCb);
	if (! aReq)
		returnMessage(SUI_STR("Can't req!"));

	return addMenuItem(aReq);
}

bool Menu::addRequest(bool* val, SUI_FLASHSTRING_PTR key_pstr, SUI_FLASHSTRING_PTR help_pstr,
		bool(*validator)(bool &),
		MenuRequest_Callback valueChangedCb) {
	MenuItem::RequestBool * aReq = new MenuItem::RequestBool(val, key_pstr, help_pstr, validator, valueChangedCb);
	if (! aReq)
		returnMessage(SUI_STR("Can't req!"));

	return addMenuItem(aReq);

}

bool Menu::addRequest(float* val, SUI_FLASHSTRING_PTR key_pstr, SUI_FLASHSTRING_PTR help_pstr,
		bool(*validator)(float &),
		MenuRequest_Callback valueChangedCb) {
	MenuItem::RequestFloat * aReq = new MenuItem::RequestFloat(val, key_pstr, help_pstr, validator, valueChangedCb);
	if (! aReq)
		returnMessage(SUI_STR("Can't req!"));

	return addMenuItem(aReq);
}

bool Menu::addRequest(String* val, uint8_t stringLength, SUI_FLASHSTRING_PTR key_pstr,
		SUI_FLASHSTRING_PTR help_pstr,
		bool(*validator)(String &),
		MenuRequest_Callback valueChangedCb) {
	MenuItem::RequestString * aReq = new MenuItem::RequestString(val, stringLength, key_pstr, help_pstr, validator, valueChangedCb);
	if (! aReq)
		returnMessage(SUI_STR("Can't req!"));

	return addMenuItem(aReq);
}


/*
bool Menu::addCommands(MenuItemDetails detailsList[], uint8_t number)
{

	for (uint8_t i=0; i<number; i++)
	{
		if (! this->addCommand(detailsList[i].key_str, detailsList[i].callback, detailsList[i].help_str))
			return false;

	}

	return true;

}
*/

bool Menu::addCommand(SUI_FLASHSTRING_PTR key_str, MenuCommand_Callback callback,
		SUI_FLASHSTRING_PTR help_str) {


#ifdef SUI_INCLUDE_EXTRA_SAFETYCHECKS

	MenuItem::Command testCom(callback, key_str, help_str);
	if (testCom.key_size > SUI_MENU_PROGMEM_STRING_ABS_MAXLEN) {

		returnMessage(SUI_STR(SUI_ERRORMSG_MENUITEM_KEY_TOOLONG));
	}

	if (help_str && STRLEN_FLASHSTR(help_str) > SUI_MENU_PROGMEM_STRING_ABS_MAXLEN) {
		SUIDRIVER_PRINTLN_FLASH(help_str);
		returnMessage(SUI_STR(SUI_ERRORMSG_MENUITEM_HELP_TOOLONG));
	}

#endif

	MenuItem::Command * aCommand = new MenuItem::Command(callback, key_str, help_str);
	if (! aCommand)
		returnMessage(SUI_STR("Can't create command!"));

	return addMenuItem(aCommand);

}


#ifdef SUI_PROGMEM_PTR
void Menu::returnError_P(SUI_PROGMEM_PTR errmsg) {
	if (errmsg) {
		SUIDRIVER_PRINT_FLASH(error_prefix);
		SUIDRIVER_PRINTLN_FLASH(errmsg);
	} else {
		SUIDRIVER_PRINTLN_FLASH(error_generic);
	}
}
#endif






#ifdef SUI_MENU_ENABLE_SUBMENUS
Menu * Menu::subMenu(SUI_FLASHSTRING_PTR key_str, SUI_FLASHSTRING_PTR help_str) {


	// dynamic memory is enabled, so we new a pointer to fresh
	// memory, and keep a reference to this block within the
	// MenuItem:
	Menu * sub_men = new Menu(sui_driver, key_str, 0, this);

	if (!sub_men) {

		SUI_MENU_DEBUG_OUTPUT("Submenu creation fail");
#ifdef SUI_SERIALUI_ECHO_WARNINGS
		returnMessage(error_cantalloc_submenu);
#endif
		return NULL;
	}

	MenuItem::SubMenu * itm = new MenuItem::SubMenu(sub_men, key_str, help_str);

	if (! itm)
	{

		SUI_MENU_DEBUG_OUTPUT("Submenu item creation fail");
#ifdef SUI_SERIALUI_ECHO_WARNINGS
		returnMessage(error_cantalloc_submenu);
#endif
		MENUFREE(sub_men); // free our old guy
		return NULL;
	}

	if (!addMenuItem(itm)) {
		MENUFREE(sub_men);

		return NULL;
	}

#ifdef SUI_INCLUDE_DEBUG
	SUI_MENU_DEBUG_OUTPUT("Added submenu");
	sui_driver->showFreeRAM();
#endif
	return sub_men;

}
#endif

bool Menu::addMenuItem(MenuItem::Base * itm) {

	if (num_menu_items >= max_menu_items) {
		// we need some more space for this item...


		if (!expandItemList()) {
			SUI_MENU_DEBUG_OUTPUT("Expansion failed!");
			return false;
		}
	}

	// no dynamic mem... we'll be using a static array
	MENUITEMLIST[num_menu_items] = itm;


	if (itm->key_size > max_key_len) {
		max_key_len = itm->key_size;
	}


	num_menu_items++;

	SUI_MENU_DEBUG_OUTPUT("Added menu item for ");
#ifdef SUI_INCLUDE_DEBUG
	sui_driver->print(itm->key);
	sui_driver->print(SUI_STR(" KSize: "));
	sui_driver->println((int)itm->key_size);

#endif

	return true;

}

bool Menu::expandItemList(uint8_t by_amount) {

	MenuItem::Base ** new_list = NULL;

	if (!by_amount) {
		// set optional param to default value
		by_amount = expand_list_amount;
	}

	new_list = new MenuItem::Base* [num_menu_items + by_amount];

	if (! new_list)
	{
#ifdef SUI_INCLUDE_DEBUG

		if (parent_menu)
		{
			// can't start doing output before SerialUI is initialized
			// so we only do this stuff for sub-menus
			SUI_MENU_DEBUG_OUTPUT("Can't alloc new item list for ");
			showName();
			sui_driver->println(" ");

			sui_driver->showFreeRAM();

		}
#endif
		returnMessage(error_cantalloc_menuitem);

		return false;
	}


	if (item_list) {
		// have old list to get move/get rid of
		for (uint8_t i=0; i< num_menu_items; i++)
		{
			new_list[i] = item_list[i];
		}

		delete [] item_list;
		item_list = NULL;
 #ifdef SUI_INCLUDE_DEBUG

		if (parent_menu)
		{
			// can't start doing output before SerialUI is initialized
			// so we only do this stuff for sub-menus
			SUI_MENU_DEBUG_OUTPUT("Dynamically expanding list of menu items for ");
			showName();
			sui_driver->println(" ");

			sui_driver->showFreeRAM();
		}
 #endif


	}

		// we gots a new list, huzzah.

		// make note of our new space...
		max_menu_items += by_amount;
		for (uint8_t i=num_menu_items; i<max_menu_items; i++)
			new_list[i] = NULL;

		// keep that pointer
		item_list = new_list;

		// success!
		return true;

}

void Menu::enter() {

	SUI_MENU_DEBUG_OUTPUT("Entering menu");

	returnMessage(menu_name);
}

Menu * Menu::handleRequest() {

	SUI_MENU_DEBUG_OUTPUT("Menu req rcvd.");

#ifdef SUI_INCLUDE_EXTRA_SAFETYCHECKS

	if (!num_menu_items) {

	#ifdef SUI_SERIALUI_ECHO_WARNINGS
		returnMessage(error_nomenuitems);
	#endif
		// error!
		return this;
	}
#endif

	char * key_entered = newReadKey();

	if (!key_entered) {
		// nothing entered
		sui_driver->println(' ');
		return this;
	}

#ifdef SUI_INCLUDE_DEBUG
	sui_driver->debug("KEY: ");
	sui_driver->debug(key_entered);
	sui_driver->debug("\n");
#endif

#ifdef SUI_SERIALUI_ECHO_ON
	// echo
	if (sui_driver->echoCommands())
	{
#ifdef SUI_ENABLE_MODES
		if (sui_driver->mode() == Mode::User)
#endif
		{
			sui_driver->println(key_entered);
		}
	}

#endif

	MenuItem::Base * itm = itemForKey(key_entered);
	if (itm) {
		// get rid of our new'ed key
		MENUFREEKEY(key_entered);
		Menu * switchTo =  itm->call(this);
		if (switchTo)
			return switchTo;

		return this;
	}

	// not found...

#ifdef SUI_MENU_ENABLE_SUBMENUS
	if (parent_menu && STRNCMP_FLASHSTR(key_entered, up_key, STRLEN_FLASHSTR(up_key)) == 0) {
		// get rid of our new'ed key

		SUI_MENU_DEBUG_OUTPUT("Going up a level");

		MENUFREEKEY(key_entered);
		return upLevel();
	}
#endif

	if (STRNCMP_FLASHSTR(key_entered, help_key, STRLEN_FLASHSTR(help_key)) == 0) {

		SUI_MENU_DEBUG_OUTPUT("Help request");

		// get rid of our new'ed key
		MENUFREEKEY(key_entered);
		showHelp();
		return this;
	}

	if (parent_menu == NULL)
	{
		// in top level, might be a request to exit
		uint8_t exit_keylen = STRLEN_FLASHSTR(exit_key);
		uint8_t key_len = strlen(key_entered);

		uint8_t numToComp = key_len < exit_keylen ? key_len : exit_keylen;

		if (STRNCMP_FLASHSTR(key_entered, exit_key, numToComp) == 0)
		{
			// yeah, consider it a match.

			SUI_MENU_DEBUG_OUTPUT("Exit request");

			MENUFREEKEY(key_entered);
			return NULL; // a null return indicates that this is it
		}

	}


#ifdef SUI_ENABLE_MODES
	// check for program mode command...
	if (STRNCMP_FLASHSTR(key_entered, key_mode_program, STRLEN_FLASHSTR(key_mode_program)) == 0)
	{
			SUI_MENU_DEBUG_OUTPUT("Entering program mode");
			sui_driver->setMode(Mode::Program);
			MENUFREEKEY(key_entered);

			char sepChar[2];
			sepChar[0] = SUI_SERIALUI_PROG_STR_SEP_CHAR;
			sepChar[1] = '\0';

			char outBuf[SUI_SERIALUI_PROGMEM_STRING_ABS_MAXLEN];
			outBuf[0] = '\0';
			strcat(outBuf, sepChar);

			STRCAT_FLASHSTR(outBuf, prog_mode_info_VERSION);
			strcat(outBuf, sepChar);



#ifdef SUI_MENU_ENABLE_SUBMENUS
				STRCAT_FLASHSTR(outBuf, up_key);
#else
				strcat(outBuf, "X");
#endif	/* SUI_MENU_ENABLE_SUBMENUS */
			strcat(outBuf, sepChar);

			STRCAT_FLASHSTR(outBuf, exit_key);
			strcat(outBuf, sepChar);

			STRCAT_FLASHSTR(outBuf, error_generic);
			strcat(outBuf, sepChar);


			STRCAT_FLASHSTR(outBuf, prog_mode_info_helpkey);
			strcat(outBuf, sepChar);

			STRCAT_FLASHSTR(outBuf, help_key_prog_commandprefix);
			strcat(outBuf, sepChar);

			STRCAT_FLASHSTR(outBuf, help_key_prog_submenuprefix);
			strcat(outBuf, sepChar);


			STRCAT_FLASHSTR(outBuf, help_sep_prog);
			strcat(outBuf, sepChar);



			STRCAT_FLASHSTR(outBuf, prog_mode_info_moreprompt_string);
			strcat(outBuf, sepChar);



			STRCAT_FLASHSTR(outBuf, prog_mode_info_moreprompt_num);
			strcat(outBuf, sepChar);

			strcat(outBuf, SUI_SERIALUI_PROMPT);
			strcat(outBuf, sepChar);

			STRCAT_FLASHSTR(outBuf, prog_mode_info_EOT);
			strcat(outBuf, sepChar);

#ifdef SUI_ENABLE_STREAMPROMPTING
			STRCAT_FLASHSTR(outBuf, prog_mode_info_moreprompt_stream);
			strcat(outBuf, sepChar);
#endif


			STRCAT_FLASHSTR(outBuf, prog_mode_terminate_gui);
			strcat(outBuf, sepChar);

			STRCAT_FLASHSTR(outBuf, help_key_prog_requestbaseprefix);
			strcat(outBuf, sepChar);


			sui_driver->print(strlen(outBuf) + 1, DEC);
			sui_driver->println(outBuf);

			return this;
	}

	if (STRNCMP_FLASHSTR(key_entered, key_mode_user, STRLEN_FLASHSTR(key_mode_user)) == 0) {
		SUI_MENU_DEBUG_OUTPUT("Entering 'user' mode");
		sui_driver->setMode(Mode::User);
		MENUFREEKEY(key_entered);
		return this;
	}
#endif	/* SUI_ENABLE_MODES */

	if (STRNCMP_FLASHSTR(key_entered, key_ping_command, STRLEN_FLASHSTR(key_ping_command)) == 0) {

		MENUFREEKEY(key_entered);
			pingRespond();
			return this;
		}


	// get rid of our new'ed key
	unknownCommand(key_entered);
	MENUFREEKEY(key_entered);
	return this;

}

void Menu::printHelpKey(MenuItem::Base * menuitem) {


	/*
	SUI_FLASHSTRING_PTR prefix_cmd;
	SUI_FLASHSTRING_PTR prefix_submenu;
	SUI_FLASHSTRING_PTR help_sep_to_use;
*/
	SUI_FLASHSTRING_PTR help_sep_to_use;

	bool include_pretty_print = true;
	SUI::Mode::Selection curMode = Mode::User;
#ifdef SUI_ENABLE_MODES
	curMode = sui_driver->mode();
	if (sui_driver->mode() == Mode::User)
	{
		help_sep_to_use = help_sep;
		/*
		prefix_cmd = help_key_commandprefix;
		prefix_submenu = help_key_submenuprefix;
		*/
	} else {

		help_sep_to_use = help_sep_prog;
		/*
		prefix_cmd = help_key_prog_commandprefix;
		prefix_submenu = help_key_prog_submenuprefix;
		*/
		include_pretty_print = false;
	}
#else

	help_sep_to_use = help_sep;
	prefix_cmd = help_key_commandprefix;
	prefix_submenu = help_key_submenuprefix;
#endif

	menuitem->printPrefix(this, curMode);
	/*

	if (menuitem->is(MenuItem::Type::SubMenu))
	{
		SUIDRIVER_PRINT_FLASH(prefix_submenu);
	} else {
		SUIDRIVER_PRINT_FLASH(prefix_cmd);
	}
	*/

	SUIDRIVER_PRINT_FLASH(menuitem->key);

	if (menuitem->help || menuitem->is(MenuItem::Type::SubMenu)) {
		if (include_pretty_print)
		{
			// we need to put in some spacing, for the help message
			for (uint8_t i = 0;
				i < (SUI_SERIALUI_KEYHELP_SEP_REPEATS_MAX - menuitem->key_size);
				i++) {

				SUIDRIVER_PRINT_FLASH(help_sep_to_use);
			}
		} else {

			SUIDRIVER_PRINT_FLASH(help_sep_to_use);
		}
	}
}

void Menu::showHelp() {

	bool in_program_mode = false;

#ifdef SUI_ENABLE_MODES
	if (sui_driver->mode() == Mode::Program)
	{
		in_program_mode = true;
	}
#endif

	if (! in_program_mode)
	{
		SUIDRIVER_PRINT_FLASH(help_title_prefix);
		SUIDRIVER_PRINTLN_FLASH(menu_name);
		sui_driver->println(' ');
	}

	for (uint8_t i = 0; i < num_menu_items; i++) {
		MenuItem::Base * itm = MENUITEMLIST[i];

		printHelpKey(itm);
		if (itm->help) {
			SUIDRIVER_PRINTLN_FLASH(itm->help);
		}
#ifdef SUI_MENU_ENABLE_SUBMENUS
		else if (itm->is(MenuItem::Type::SubMenu)) {
			SUIDRIVER_PRINTLN_FLASH(submenu_help);
		}
#endif
		else {
			sui_driver->println(' ');
		}
	}

	if (in_program_mode)
		return; // don't need navigation help in program mode

	sui_driver->println(' ');

	if (parent_menu) {
#ifdef SUI_MENU_ENABLE_SUBMENUS
		SUIDRIVER_PRINT_FLASH(help_key_commandprefix);
		SUIDRIVER_PRINT_FLASH(up_key);
		for (uint8_t i = 0;
					i < SUI_SERIALUI_KEYHELP_SEP_REPEATS_MAX - STRLEN_FLASHSTR(up_key);
					i++) {

			SUIDRIVER_PRINT_FLASH(help_sep);
		}

		SUIDRIVER_PRINTLN_FLASH(up_help);
#endif
	} else {
		SUIDRIVER_PRINT_FLASH(help_key_commandprefix);
		SUIDRIVER_PRINT_FLASH(exit_key);
		for (uint8_t i = 0;
				i < SUI_SERIALUI_KEYHELP_SEP_REPEATS_MAX - STRLEN_FLASHSTR(exit_key);
				i++) {

			SUIDRIVER_PRINT_FLASH(help_sep);
		}
		SUIDRIVER_PRINTLN_FLASH(exit_help);
	}

	SUIDRIVER_PRINT_FLASH(help_key_commandprefix);
	SUIDRIVER_PRINT_FLASH(help_key);
	for (uint8_t i = 0;
			i < SUI_SERIALUI_KEYHELP_SEP_REPEATS_MAX - STRLEN_FLASHSTR(help_key);
			i++) {
		SUIDRIVER_PRINT_FLASH(help_sep);
	}
	SUIDRIVER_PRINTLN_FLASH(help_help);

}

#ifdef SUI_MENU_ENABLE_SUBMENUS
Menu * Menu::upLevel() {
	if (parent_menu) {
		parent_menu->enter();
		return parent_menu;
	}

	return this;
}
#endif

MenuItem::Base * Menu::itemForKey(char * key_found) {

	size_t key_size = strlen(key_found);
	size_t cmp_size = 0;

	for (uint8_t i = 0; i < num_menu_items; i++) {
		MenuItem::Base * itm = MENUITEMLIST[i];

#ifdef SUI_INCLUDE_DEBUG
		sui_driver->debug("Checking key: ");
		sui_driver->debug(itm->key);
#endif
		// compare up to smallest of len(key entered) and len(key of this item)
		cmp_size = (key_size < itm->key_size) ? key_size : itm->key_size;

		if (STRNCMP_FLASHSTR(key_found, itm->key, cmp_size) == 0) {
			// have a match...

#ifdef SUI_INCLUDE_DEBUG

			sui_driver->debug("IS match");
#endif
			return itm;
		}
	}

	return NULL;

}

void Menu::unknownCommand(char * key) {

	SUIDRIVER_PRINT_FLASH(error_generic);
	SUIDRIVER_PRINT_FLASH(unknown_sel);
	sui_driver->print(key);
	SUIDRIVER_PRINT_FLASH(unknown_inmenu);
	SUIDRIVER_PRINTLN_FLASH(menu_name);
	SUIDRIVER_PRINTLN_FLASH(help_hint);
}

char * Menu::newReadKey() {

#ifdef SUI_INCLUDE_EXTRA_SAFETYCHECKS
	if (max_key_len < 1) {
		return NULL;
	}
#endif

	// we'll need--at most--a container that is the size of our largest key for the menu
	char * akey = new char[(max_key_len + 2)];


#ifdef SUI_INCLUDE_DEBUG
	SUI_MENU_DEBUG_OUTPUT("Post read key allocation")
	sui_driver->showFreeRAM();
#endif


	if (!akey) {

#ifdef SUI_SERIALUI_ECHO_WARNINGS
		returnMessage(error_cantalloc_key);
#endif

		return NULL;
	}

	MENUMEMSET(akey, 0, max_key_len + 2);

	uint8_t numRead = sui_driver->readBytesToEOL(akey, max_key_len + 1, true);
	if (!numRead) {
		MENUFREEKEY(akey);
		return NULL;
	}


	// make sure we don't include any return/newlines in our key
	uint8_t num_nonEOL = 0;
	for (uint8_t i = 0; i < numRead; i++) {
		if (akey[i] == '\r' || akey[i] == '\n') {
			akey[i] = 0;
		} else {
			num_nonEOL ++;
		}
	}

	if (num_nonEOL)
	{
		// we had some valid, non-EOL, chars.
		return akey;
	}

	// no non-EOL
	MENUFREEKEY(akey);
	return NULL;
}

void Menu::returnError(const char * errmsg) {
	if (errmsg) {
		SUIDRIVER_PRINT_FLASH(error_prefix);
		sui_driver->println(errmsg);
	} else {
		SUIDRIVER_PRINTLN_FLASH(error_generic);
	}
}

void Menu::returnError(SUI_FLASHSTRING_PTR errmsg) {
	SUIDRIVER_PRINT_FLASH(error_prefix);
	sui_driver->println(errmsg);

}



void Menu::returnOK() {
	sui_driver->println(F(SUI_SERIALUI_MESSAGE_OK));

}


void Menu::returnMessage(SUI_FLASHSTRING_PTR message) { SUIDRIVER_PRINTLN_FLASH(message);}





void Menu::showName() { SUIDRIVER_PRINT_FLASH(menu_name); }

void Menu::pingRespond() {
#ifdef SUI_ENABLE_STATE_TRACKER
	if (sui_driver->showTrackedState())
	{
		return;
	}
#endif

	sui_driver->println(' ');




}

} /* end namespace SUI */


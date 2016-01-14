/*
 * MenuItem.cpp
 *
 *  Created on: Jan 13, 2016
 *      Author: Pat Deegan
 *      Part of the SerialUI Project
 *      Copyright (C) 2015 Pat Deegan, http://psychogenic.com
 */

#include "includes/SUIMenu.h"
#include "includes/MenuItem.h"
#include "includes/SerialUI.h"

namespace SUI {
namespace MenuItem {

Base::Base(SUI_FLASHSTRING_PTR key_pstr, SUI_FLASHSTRING_PTR help_pstr, Type::Type t) : key(key_pstr), help(help_pstr), item_type(t)
{

	key_size = STRLEN_FLASHSTR(key);
}


Menu* SubMenu::call(Menu * callingMenu) {

	subMenu->enter();

	return subMenu;

}

void SubMenu::printPrefix(Menu * callingMenu, SUI::Mode::Selection curMode) {
	if (curMode == SUI::Mode::User)
	{
		callingMenu->driver()->print(SUI_STR(SUI_SERIALUI_KEYHELP_SUBMENU_PREFIX));
		return;
	}

	callingMenu->driver()->print(SUI_STR(SUI_SERIALUI_KEYHELP_SUBMENU_PREFIX_PROG));

}

Menu * Command::call(Menu * callingMenu) {

	command();
	return NULL;
}

void Command::printPrefix(Menu * callingMenu, SUI::Mode::Selection curMode) {
	if (curMode == SUI::Mode::User)
	{
		callingMenu->driver()->print(SUI_STR(SUI_SERIALUI_KEYHELP_COMMAND_PREFIX));
		return;
	}

	callingMenu->driver()->print(SUI_STR(SUI_SERIALUI_KEYHELP_COMMAND_PREFIX_PROG));

}

#if 0

Menu* Request::call(Menu * callingMenu)
{

}

bool RequestBool::showPromptAndGetValue(Menu * callingMenu)
{

	callingMenu->driver()->showEnterNumericDataPrompt();

	bool val = callingMenu->driver()->parseInt();

	if (val != *the_value)
	{
		*the_value = val;
		return true;
	}
	return false;
}

bool RequestLong::showPromptAndGetValue(Menu * callingMenu)
{

	callingMenu->driver()->showEnterNumericDataPrompt();

	long val = callingMenu->driver()->parseInt();

	if (val != *the_value)
	{
		*the_value = val;
		return true;
	}
	return false;

}


bool RequestULong::showPromptAndGetValue(Menu * callingMenu)
{

	callingMenu->driver()->showEnterNumericDataPrompt();

	long unsigned int val = callingMenu->driver()->parseULong();

	if (val != *the_value)
	{
		*the_value = val;
		return true;
	}
	return false;

}




bool RequestFloat::showPromptAndGetValue(Menu * callingMenu)
{

	callingMenu->driver()->showEnterNumericDataPrompt();

	float val = callingMenu->driver()->parseFloat();

	if (val != *the_value)
	{
		*the_value = val;
		return true;
	}
	return false;

}


bool RequestChar::showPromptAndGetValue(Menu * callingMenu)
{
	char aChar;

	callingMenu->driver()->showEnterDataPrompt();
	if (callingMenu->driver()->readBytes(&aChar, 1) < 1)
	{
		return false;
	}

	if (aChar != *the_value)
	{
		*the_value = aChar;
		return true;
	}

	return false;

}

bool RequestString::showPromptAndGetValue(Menu * callingMenu)
{

	char * buf = new char[max_length + 1];

	if (! buf)
	{
		return false;
	}

	memset(buf, 0, max_length + 1);

	callingMenu->driver()->showEnterDataPrompt();

	uint8_t numRead = callingMenu->driver()->readBytesToEOL(buf, max_length);

	if (! numRead)
	{
		delete [] buf;
		return false;
	}

	bool strChanged = false;
	if (strncmp(buf, the_value, max_length) != 0)
	{
		strChanged = true;
		strncpy(the_value, buf, numRead);
	}
	delete [] buf;

	return strChanged;


}

#endif



}
}

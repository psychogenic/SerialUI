/*
 * Command.cpp -- Command menu items.
 * Copyright (C) 2013-2017 Pat Deegan, psychogenic.com.
 *
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



#include "includes/menuitem/Command.h"
#include "includes/SUIMenu.h"
#include "includes/SerialUI.h"

namespace SUI {
namespace MenuItem {




Menu * Command::call(Menu * callingMenu) {

	SerialUI * driver = callingMenu->driver();

	driver->markResponseRequired();
	command();
	if (! driver->responseTransmitted() )
	{
		callingMenu->returnOK();
	}
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

}
}

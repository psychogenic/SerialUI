/*
 * Command.h
 *
 *  Created on: May 14, 2016
 *      Author: Pat Deegan
 *      Part of the SerialUI Project
 *      Copyright (C) 2016 Pat Deegan, http://psychogenic.com
 * 
 */

#ifndef SERIALUI_SRC_INCLUDES_MENUITEM_COMMAND_H_
#define SERIALUI_SRC_INCLUDES_MENUITEM_COMMAND_H_


#include "../MenuItem.h"


namespace SUI {

namespace MenuItem {

class Command: public MenuItem::Base {
public:

	Command(MenuCommand_Callback cb, SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr) :
		MenuItem::Base(key_pstr, help_pstr, MenuItem::Type::Command), command(cb) {}

		MenuCommand_Callback command;

		virtual Menu* call(Menu * callingMenu);

		virtual void printPrefix(Menu * callingMenu, SUI::Mode::Selection curMode) ;

};




}
}


#endif /* SERIALUI_SRC_INCLUDES_MENUITEM_COMMAND_H_ */

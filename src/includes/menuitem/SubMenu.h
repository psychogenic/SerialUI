/*
 * SubMenu.h
 *
 *  Created on: May 14, 2016
 *      Author: Pat Deegan
 *      Part of the SerialUI Project
 *      Copyright (C) 2016 Pat Deegan, http://psychogenic.com
 * 
 */

#ifndef SERIALUI_SRC_INCLUDES_MENUITEM_SUBMENU_H_
#define SERIALUI_SRC_INCLUDES_MENUITEM_SUBMENU_H_

#include "../MenuItem.h"

namespace SUI {
namespace MenuItem {

class SubMenu: public MenuItem::Base {
public:

	SubMenu(Menu * sub, SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr) :
		MenuItem::Base(key_pstr, help_pstr, MenuItem::Type::SubMenu), subMenu(sub) {}

	Menu * subMenu;

	virtual Menu* call(Menu * callingMenu);
	virtual void printPrefix(Menu * callingMenu, SUI::Mode::Selection curMode) ;
};

}
}



#endif /* SERIALUI_SRC_INCLUDES_MENUITEM_SUBMENU_H_ */

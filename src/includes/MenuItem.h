/*
 *
 * MenuItem.h -- Base class for SerialUI menu items.
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


#ifndef SUI_MENUITEM_H_
#define SUI_MENUITEM_H_
#include "SUIPlatform.h"
#include "SUITypes.h"
#include "SerialUI.h"
#include "SUIStrings.h"
namespace SUI {

class Menu;
// forward decl

namespace MenuItem {

namespace Type {
typedef enum {
	Command = 0,
	SubMenu,
	Request
} Type;
}

#define SUI_MENUITEM_NUM_STATIC_STRINGS		5

namespace StaticString {
typedef enum {
	RequestPrefix = 0,
	RequestNumPrefix = 1,
	MetaInfoSeparator = 2,
	RequestCharPrefix = 3,
	RequestStringPrefix = 4,

} Index;
}

class Base {
public:

	SOVA_FLASHSTRING_PTR key;
	SOVA_FLASHSTRING_PTR help;

	MenuItemID	itemid;
	uint8_t key_size;
	Type::Type item_type;

	Base();
	Base(SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr, Type::Type t);
	virtual ~Base() {
	}

	bool is(Type::Type t) {
		return t == item_type;
	}

	inline MenuItemID id() { return itemid;}


	virtual Menu* call(Menu * callingMenu) = 0;

	virtual void printPrefix(Menu * callingMenu,
			SUI::Mode::Selection curMode) = 0;

	virtual void printMetaInfo(Menu * callingMenu) { SUI_UNUSED_PARAM(callingMenu);} // additional info in program mode

protected:

	static SOVA_FLASHSTRING_PTR getString(StaticString::Index idx);

private:
	static MenuItemID item_counter;

	static SOVA_FLASHSTRING_PTR  static_strings[SUI_MENUITEM_NUM_STATIC_STRINGS];


};

}
}
#endif

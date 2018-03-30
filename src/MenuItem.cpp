/*
 * MenuItem.cpp
 *
 *  Created on: Jan 13, 2016
 *      Author: Pat Deegan
 *      Part of the SerialUI Project
 *      Copyright (C) 2015-2017 Pat Deegan, http://psychogenic.com
 */

#include "includes/SUIMenu.h"
#include "includes/MenuItem.h"
#include "includes/SerialUI.h"

namespace SUI {
namespace MenuItem {

MenuItemID Base::item_counter = 0;
SOVA_FLASHSTRING_PTR  Base::static_strings[SUI_MENUITEM_NUM_STATIC_STRINGS];

SOVA_FLASHSTRING_PTR Base::getString(StaticString::Index idx)
{
	static bool is_init = false;

	if (! is_init)
	{
		static_strings[(uint8_t)StaticString::RequestPrefix] = SUI_STR(SUI_SERIALUI_KEYHELP_REQUEST_PREFIX);
		static_strings[(uint8_t)StaticString::RequestNumPrefix] = SUI_STR(SUI_SERIALUI_KEYHELP_REQUEST_NUM_PREFIX_PROG);
		static_strings[(uint8_t)StaticString::MetaInfoSeparator] = SUI_STR(SUI_SERIALUI_KEYHELP_METAINFO_SEP_PROG);
		static_strings[(uint8_t)StaticString::RequestCharPrefix] = SUI_STR(SUI_SERIALUI_KEYHELP_REQUEST_CHAR_PREFIX_PROG);
		static_strings[(uint8_t)StaticString::RequestStringPrefix] = SUI_STR(SUI_SERIALUI_KEYHELP_REQUEST_STR_PREFIX_PROG);

		is_init = true;
	}

	return static_strings[(uint8_t)idx];

}
Base::Base(SOVA_FLASHSTRING_PTR key_pstr, SOVA_FLASHSTRING_PTR help_pstr, Type::Type t) :
		key(key_pstr), help(help_pstr),
		itemid(item_counter++),
		item_type(t)
{

	key_size = STRLEN_FLASHSTR(key);
}



}
}

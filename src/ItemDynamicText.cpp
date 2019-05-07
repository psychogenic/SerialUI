/*
 * ItemDynamicText.cpp
 *
 *  Created on: May 31, 2018
 *      Author: Pat Deegan
 *
 *  ItemDynamicText is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#include "includes/menuitem/ItemDynamicText.h"

namespace SerialUI {
namespace Menu {
namespace Item {


DynamicText::DynamicText(const TopLevelString& dynTitle,
		const TopLevelString& dynContent)
	: Item(Type::DynamicText, SUI_STR("_dt"), NULL),
	  _title(dynTitle),
	  _contents(dynContent)
{
}

DynamicText::DynamicText(ID id,
		const TopLevelString& dynTitle, const TopLevelString& dynContent)
: Item(id, Type::DynamicText, SUI_STR("_dt"), NULL),
  _title(dynTitle),
  _contents(dynContent)
{
}

void DynamicText::setTitle(const TopLevelString& setTo) {
	_title = setTo;
	// TODO: FIXME notify upper layers??
}

void DynamicText::setContents(
		const TopLevelString& setTo) {
	_contents = setTo;
	// TODO: FIXME notify upper layers??
}


} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

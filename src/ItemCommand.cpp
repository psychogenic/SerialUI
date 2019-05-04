/*
 * ItemCommand.cpp
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  ItemCommand is part of the XYZPROJECT project.
 *  Copyright (C) 2018 Pat Deegan, psychogenic.com
 */

#include "includes/menuitem/ItemCommand.h"
#include "includes/SUIGlobals.h"

namespace SerialUI {
namespace Menu {
namespace Item {

Command::Command(Callback cb, StaticString key, StaticString help)
	: Item(Type::Command, key, help), _cb(cb) {

}

Command::Command(ID id, Callback cb, StaticString key, StaticString help)
: Item(id, Type::Command, key, help), _cb(cb)
{

}

void Command::call(Menu * callingMenu) {
	SerialUI::Globals::state()->processingCommand(this);
	if (_cb) {
		_cb();
	}
	SerialUI::Globals::state()->setIdle();
}


} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

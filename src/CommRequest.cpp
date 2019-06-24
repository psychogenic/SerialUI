/*
 * CommRequest.cpp
 *
 *  Created on: May 28, 2018
 *      Author: Pat Deegan
 *
 *  CommRequest is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#include "includes/comm/CommRequest.h"


namespace SerialUI {
namespace Comm {

bool Request::trigger(Menu::Menu * fromMenu) {
	if (isForMenuItem()) {
		menuItem->call(fromMenu);
		return true;
	}

	if (! isForBuiltIn()) {
		return false;
	}

	return false;
}


void Request::setMenuItem(Menu::Item::Item * selectedItem) {
	menuItem = selectedItem;
	if (menuItem) {
		requestType = SerialUI::Request::Type::MenuItem;
	} else {

		requestType = SerialUI::Request::Type::INVALID;
	}
}
void Request::setBuiltIn(SerialUI::Request::BuiltIn::Selection sel) {
	builtIn = sel;
	if (sel == SerialUI::Request::BuiltIn::INVALID) {
		requestType = SerialUI::Request::Type::INVALID;
	} else {
		requestType = SerialUI::Request::Type::BuiltIn;
	}
}

} /* namespace Comm */
} /* namespace SerialUI */

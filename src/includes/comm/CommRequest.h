/*
 * CommRequest.h
 *
 *  Created on: May 28, 2018
 *      Author: Pat Deegan
 *
 *  CommRequest is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_COMM_COMMREQUEST_H_
#define SERIALUI_SRC_INCLUDES_COMM_COMMREQUEST_H_

#include "../SerialUITypes.h"
#include "../menuitem/MenuItem.h"
namespace SerialUI {
namespace Comm {

typedef struct RequestStruct {
public:
	uint8_t requestId;
	SerialUI::Request::Type::Value requestType;
	union {
		Menu::Item::Item * menuItem;
		SerialUI::Request::BuiltIn::Selection builtIn;
	};
	RequestStruct() : requestId(0), requestType(SerialUI::Request::Type::INVALID), menuItem(NULL) {}
	RequestStruct(SerialUI::Request::BuiltIn::Selection selectedAction) :
			requestId(0),
			requestType(SerialUI::Request::Type::BuiltIn), builtIn(selectedAction) {}
	RequestStruct(Menu::Item::Item * selectedItem) : requestId(0),
			requestType(SerialUI::Request::Type::MenuItem), menuItem(selectedItem) {}


	void setMenuItem(Menu::Item::Item * selectedItem);
	void setBuiltIn(SerialUI::Request::BuiltIn::Selection);

	void clear() {
		requestType = SerialUI::Request::Type::INVALID;
		menuItem = NULL;
		requestId = 0;
	}

	bool isValid() const {
		return requestType != SerialUI::Request::Type::INVALID;
	}

	bool isForMenuItem() const {
		return requestType == SerialUI::Request::Type::MenuItem;
	}

	bool isForBuiltIn() const {
		return requestType == SerialUI::Request::Type::BuiltIn;
	}

	bool trigger(Menu::Menu * fromMenu);


} Request;

} /* namespace Comm */
} /* namespace SerialUI */

#endif /* SERIALUI_SRC_INCLUDES_COMM_COMMREQUEST_H_ */

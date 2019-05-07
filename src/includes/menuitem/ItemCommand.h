/*
 * ItemCommand.h
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  ItemCommand is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUIV3_SRC_INCLUDES_MENUITEM_ITEMCOMMAND_H_
#define SERIALUIV3_SRC_INCLUDES_MENUITEM_ITEMCOMMAND_H_

#include "MenuItem.h"
#include "../Menu.h"

namespace SerialUI {
namespace Menu {
namespace Item {

class Command : public Item {
public:
	typedef CommandCallback Callback;
	Command(Callback cb, StaticString key, StaticString help);
	Command(ID id, Callback cb, StaticString key, StaticString help);

	virtual void call(Menu * callingMenu);
private:
	Callback _cb;
};

} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

#endif /* SERIALUIV3_SRC_INCLUDES_MENUITEM_ITEMCOMMAND_H_ */

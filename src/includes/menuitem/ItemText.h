/*
 * ItemText.h
 *
 *  Created on: May 31, 2018
 *      Author: Pat Deegan
 *
 *  ItemText is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_MENUITEM_ITEMTEXT_H_
#define SERIALUI_SRC_INCLUDES_MENUITEM_ITEMTEXT_H_

#include "../SerialUITypes.h"
#include "MenuItem.h"

namespace SerialUI {
namespace Menu {
namespace Item {

class Text : public Item {
public:
	Text(StaticString title, StaticString contents);
	Text(ID id, StaticString title, StaticString contents);

	inline StaticString title() {
		return this->key();
	}

	inline StaticString contents() {
		return this->help();
	}


	virtual void call(Menu * callingMenu) {}

};

} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

#endif /* SERIALUI_SRC_INCLUDES_MENUITEM_ITEMTEXT_H_ */

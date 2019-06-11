/*
 * List.cpp
 *
 *  Created on: Apr 29, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */

#include "includes/menuitem/ItemList.h"

namespace SerialUI {
namespace Menu {
namespace Item {



List::List(ID parentId, StaticString key, StaticString help) :
		SubMenu(parentId, key, help) {
	setType(Type::List);
}

List::List(ID id, ID parentId, StaticString key, StaticString help) :
		SubMenu(id, parentId, key, help) {
	setType(Type::List);
}

} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

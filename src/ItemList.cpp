/*
 * List.cpp
 *
 *  Created on: Apr 29, 2019
 *      Author: malcalypse
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

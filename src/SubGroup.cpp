/*
 * SubGroup.cpp
 *
 *  Created on: Oct 1, 2018
 *      Author: Pat Deegan
 *
 *  SubGroup is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#include "includes/menuitem/SubGroup.h"

namespace SerialUI {
namespace Menu {
namespace Item {


Group::Group(ID parentId, StaticString key, StaticString help) :
		SubMenu(parentId, key, help) {
	setType(Type::Group);
}
Group::Group(ID id, ID parentId, StaticString key, StaticString help) :
		SubMenu(id, parentId, key, help) {
	setType(Type::Group);
}

} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

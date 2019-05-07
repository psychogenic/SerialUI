/*
 * SubGroup.h
 *
 *  Created on: Oct 1, 2018
 *      Author: Pat Deegan
 *
 *  SubGroup is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_MENUITEM_SUBGROUP_H_
#define SERIALUI_SRC_INCLUDES_MENUITEM_SUBGROUP_H_
#include "SubMenu.h"
namespace SerialUI {
namespace Menu {
namespace Item {

class Group : public SubMenu {
public:

	Group(ID parentId, StaticString key, StaticString help);
	Group(ID id, ID parentId, StaticString key, StaticString help);

};

} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

#endif /* SERIALUI_SRC_INCLUDES_MENUITEM_SUBGROUP_H_ */

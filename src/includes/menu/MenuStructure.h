/*
 * MenuStructure.h
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  MenuStructure is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUIV3_SRC_INCLUDES_MENU_MENUSTRUCTURE_H_
#define SERIALUIV3_SRC_INCLUDES_MENU_MENUSTRUCTURE_H_

#include "../Menu.h"
#include "../menuitem/SubMenu.h"
#include "../menuitem/SubGroup.h"
#include "../menuitem/ItemList.h"
#include "../menuitem/ItemCommand.h"
#include "../menuitem/requests/requests.h"
#include "../SerialUIConfig.h"
#include "../GrowableList.h"

#ifndef SERIALUI_MENUSTRUCTURE_NUMITEMS_ATSTARTUP
#define SERIALUI_MENUSTRUCTURE_NUMITEMS_ATSTARTUP	SERIALUI_MENUSTRUCTURE_NUMITEMS_ATSTARTUP_DEFAULT
#endif


namespace SerialUI {
namespace Menu {

class Structure : public GrowableList<Item::Item, Item::ID>{
public:
	Structure(uint8_t numItems=SERIALUI_MENUSTRUCTURE_NUMITEMS_ATSTARTUP);

	uint8_t numItemsWithParent(Item::ID parentId);
	Item::Item * itemById(Item::ID itemId);
	Item::Item * itemByParentAndIndex(Item::ID parentId, uint8_t idx);
	Item::Item * itemByParentAndKey(Item::ID parentId,
			DynamicString aKey, bool allowPartialMatch=true);
	Item::Item * itemByKey(DynamicString aKey, bool allowPartialMatch=false);
	Item::SubMenu * topLevelMenu() { return &(_topLevel);}

	int8_t indexForItemWithParent(Item::ID parentId, Item::ID targetItemId) ;

	template<Item::Type::Value TYPENAME, class TYPE>
	TYPE * getItemByIdCast(Item::ID id) {
		Item::Item * itm = this->getItemById(id);
		if (! itm) {
			return NULL;
		}
		if (itm->type() != TYPENAME) {
			return NULL;
		}
		return (TYPE*)itm;
	}

#if 0
	NOT USEFUL?
	Item::SubMenu * getItemContainerById(Item::ID id) {
		Item::Item * itm = this->getItemById(id);
		if (! itm) {
			return NULL;
		}
		switch(itm->type()) {
		case Item::Type::Menu:
			/* fall-through */
		case Item::Type::Group:
			/* fall-through */
		case Item::Type::List:
			return (Item::SubMenu*)itm;
		default:
			break;
		}
		return NULL;
	}
#endif

	inline Item::SubMenu * getSubMenuById(Item::ID id) {
		return this->getItemByIdCast<Item::Type::Menu, Item::SubMenu>(id);
	}

	inline Item::Group * getGroupById(Item::ID id) {
		return this->getItemByIdCast<Item::Type::Group, Item::Group>(id);
	}

	inline Item::List * getListById(Item::ID id) {
		return this->getItemByIdCast<Item::Type::List, Item::List>(id);
	}
	inline Item::Command * getCommandById(Item::ID id) {
		return this->getItemByIdCast<Item::Type::Command, Item::Command>(id);
	}

	inline Item::SubMenu * getItemContainerById(Item::ID id) {
		Item::SubMenu * cont = getSubMenuById(id);
		if (! cont) {
			cont = getGroupById(id);
			if (! cont) {
				cont = getListById(id);
			}
		}
		return cont;
	}

	inline Item::Request::Request * getRequestById(Item::ID id) {
		return this->getItemByIdCast<Item::Type::Input, Item::Request::Request>(id);
	}

private:

	Item::SubMenu _topLevel;

};

} /* namespace Menu */
} /* namespace SerialUI */

#endif /* SERIALUIV3_SRC_INCLUDES_MENU_MENUSTRUCTURE_H_ */

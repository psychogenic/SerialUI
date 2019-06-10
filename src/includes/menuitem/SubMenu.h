/*
 * SubMenu.h
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  SubMenu is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUIV3_SRC_INCLUDES_MENUITEM_SUBMENU_H_
#define SERIALUIV3_SRC_INCLUDES_MENUITEM_SUBMENU_H_

#include "../SerialUITypes.h"
#include "MenuItem.h"
#include "../Menu.h"


namespace SerialUI {
namespace Menu {
namespace Item {

class SubMenu : public Menu, public Item {
public:
	SubMenu(ID parentId, StaticString key, StaticString help);
	SubMenu(ID id, ID parentId, StaticString key, StaticString help);

	virtual void call(Menu * callingMenu); // implies "entering" the submenu.

	virtual uint8_t numItems();
	virtual Item * itemById(ID itemId);
	virtual Item * itemByIndex(uint8_t idx);
	virtual Item * itemByKey(DynamicString keyToFind);
	// indexForItem NOTE: -1 == NOT FOUND
	virtual int8_t indexForItem(ID itemId);


	virtual ID id() {
		return this->Item::id();
	}

	SubMenu * handleRequest();


	virtual void upLevel() ;
	virtual void outputListing() ;
	virtual void outputHelp() ;
	virtual void switchMode(Mode::Selection switchTo) ;
	virtual uint8_t refreshTracking(bool force=false) ;
	virtual void exitRequested();

	virtual bool addText(StaticString title,
			StaticString contents=NULL);


	virtual bool addView(TrackedView & trackedView);

	virtual  bool addCommand(StaticString key_str, CommandCallback callback,
			StaticString help_str=NULL);

	virtual  bool addRequest(Request::Request & req);


	virtual Menu * subMenu(StaticString key_str, StaticString help_str=NULL);
	virtual Menu * addGroup(StaticString key_str, StaticString help_str=NULL);
	virtual Menu * addList(StaticString key_str, StaticString help_str=NULL);

private:
	void switchModeProgram();
	void switchModeUser();

	bool interruptProcessingForAccessControl();



	SubMenu * handleBuiltinRequest(::SerialUI::Request::BuiltIn::Selection sel);


};

} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

#endif /* SERIALUIV3_SRC_INCLUDES_MENUITEM_SUBMENU_H_ */

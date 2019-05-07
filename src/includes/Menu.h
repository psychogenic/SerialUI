/*
 * Menu.h
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  Menu is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUIV3_SRC_INCLUDES_MENU_H_
#define SERIALUIV3_SRC_INCLUDES_MENU_H_
#include "SerialUITypes.h"



namespace SerialUI {
namespace Menu {

namespace Item {
class Item; // forward decl
class Command;
class TrackedView;
namespace Request {
class Request; // forward decl
}
}

class Menu {
public:
	Menu();
	virtual ~Menu() ;
	virtual Item::ID id() = 0;

	virtual uint8_t numItems() = 0;
	virtual Item::Item * itemById(Item::ID itemId) = 0;
	virtual Item::Item * itemByIndex(uint8_t idx) = 0;
	virtual Item::Item * itemByKey(DynamicString keyToFind) = 0;

	// NOTE: -1 == NOT FOUND
	virtual int8_t indexForItem(Item::ID itemId) = 0;


	/*
	 *
	UpLevel = 0x01,
	ModeUser = 0x02,
	ModeProgram = 0x03,
	ListMenu = 0x04,
	Help = 0x05,
	RefreshTracked = 0x06,
	 */

	virtual void upLevel() = 0;
	virtual void outputListing() = 0;
	virtual void outputHelp() = 0;
	virtual void switchMode(Mode::Selection switchTo) = 0;
	virtual uint8_t refreshTracking(bool force=false) = 0;

	virtual bool addView(Item::TrackedView & trackedView) = 0;

	virtual bool addText(StaticString title,
				StaticString contents=NULL) = 0;

	virtual bool addCommand(StaticString key_str, Item::CommandCallback callback,
			StaticString help_str=NULL) = 0;

	virtual bool addRequest(Item::Request::Request & req) = 0;


	virtual Menu * subMenu(StaticString key_str, StaticString help_str=NULL) = 0;
	virtual Menu * addGroup(StaticString key_str, StaticString help_str=NULL) = 0;
	virtual Menu * addList(StaticString key_str, StaticString help_str=NULL) = 0;

};

} /* namespace Menu */
} /* namespace SerialUI */

#endif /* SERIALUIV3_SRC_INCLUDES_MENU_H_ */

/*
 * ItemRequest.cpp
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  ItemRequest is part of the XYZPROJECT project.
 *  Copyright (C) 2018 Pat Deegan, psychogenic.com
 */

#include "includes/menuitem/ItemRequest.h"
#include "includes/SUIGlobals.h"
namespace SerialUI {
namespace Menu {
namespace Item {
namespace Request {


Request::Request(Type::Value rType, ValueChangedCallback vc):
			Item(SerialUI::Menu::Item::Type::Input), _reqType(rType), _valChangedCb(vc) {

}
Request::Request(Type::Value rType, StaticString key, StaticString help, ValueChangedCallback vc) :
	Item(SerialUI::Menu::Item::Type::Input, key, help), _reqType(rType), _valChangedCb(vc)
{
}


Request::Request(ID id, Type::Value rType, StaticString key, StaticString help, ValueChangedCallback vc) :
	Item(id, SerialUI::Menu::Item::Type::Input, key, help), _reqType(rType), _valChangedCb(vc)
{
}



void Request::valueWasModified() {

	if (_valChangedCb) {
		_valChangedCb();
	}

}


void Request::callBegins() {

	Globals::state()->gettingInput(this);

}
void Request::callEnds(bool successful) {
	Globals::state()->setIdle();

}


} /* namespace Request */
} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

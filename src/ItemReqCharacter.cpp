/*
 * ItemReqCharacter.cpp
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  ItemReqCharacter is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#include "includes/menuitem/requests/ItemReqCharacter.h"
#include "includes/SUIGlobals.h"

namespace SerialUI {
namespace Menu {
namespace Item {
namespace Request {

Character::Character(char initVal, StaticString key,
		StaticString help, ValueChangedCallback vchng, ValidatorCallback validcb):
		TypedRequest<Type::Character, char>(initVal, key, help, vchng, validcb){

}

Character::Character(char initVal, ValueChangedCallback vchng, ValidatorCallback validcb):
		TypedRequest<Type::Character, char>(initVal, vchng, validcb){

}

bool Character::getValue(Menu * callingMenu, char * v) {
	return Globals::commSource()->getCharFor(id(), v);
}

} /* namespace Request */
} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

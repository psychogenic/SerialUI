/*
 * ItemReqBoolean.cpp
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  ItemReqBoolean is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#include "includes/menuitem/requests/ItemReqBoolean.h"
#include "includes/SUIGlobals.h"
namespace SerialUI {
namespace Menu {
namespace Item {
namespace Request {


Boolean::Boolean(bool initVal, ValueChangedCallback vchng, ValidatorCallback validcb) :
	BooleanBase(initVal, vchng, validcb)
{

}
Boolean::Boolean(bool val, StaticString key,
		StaticString help, ValueChangedCallback vchng, ValidatorCallback validcb) :
	BooleanBase(val, key, help, vchng, validcb)
{

}

bool Boolean::getValue(Menu * callingMenu, bool * v) {
	return Globals::commSource()->getBoolFor(id(), v);
}


ITEMPYTHONOVERRIDE_VALIDATION(BooleanBase, Boolean, bool);


Toggle::Toggle(bool initVal, ValueChangedCallback vchng, ValidatorCallback validcb) :
	ToggleBase(initVal, vchng, validcb)
{

}
Toggle::Toggle(bool val, StaticString key,
		StaticString help, ValueChangedCallback vchng, ValidatorCallback validcb) :
	ToggleBase(val, key, help, vchng, validcb)
{

}

bool Toggle::getValue(Menu * callingMenu, bool * v) {
	return Globals::commSource()->getBoolFor(id(), v);
}


ITEMPYTHONOVERRIDE_VALIDATION(ToggleBase, Toggle, bool);


} /* namespace Request */
} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

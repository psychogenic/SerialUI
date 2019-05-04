/*
 * ItemReqBoolean.cpp
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  ItemReqBoolean is part of the XYZPROJECT project.
 *  Copyright (C) 2018 Pat Deegan, psychogenic.com
 */

#include "includes/menuitem/requests/ItemReqBoolean.h"
#include "includes/SUIGlobals.h"
namespace SerialUI {
namespace Menu {
namespace Item {
namespace Request {


Boolean::Boolean(bool initVal, ValueChangedCallback vchng, ValidatorCallback validcb) :
	TypedRequest<Type::Boolean, bool>(initVal, vchng, validcb)
{

}
Boolean::Boolean(bool val, StaticString key,
		StaticString help, ValueChangedCallback vchng, ValidatorCallback validcb) :
	TypedRequest<Type::Boolean, bool>(val, key, help, vchng, validcb)
{

}

bool Boolean::getValue(Menu * callingMenu, bool * v) {
	return Globals::commSource()->getBoolFor(id(), v);
}



Toggle::Toggle(bool initVal, ValueChangedCallback vchng, ValidatorCallback validcb) :
	TypedRequest<Type::Toggle, bool>(initVal, vchng, validcb)
{

}
Toggle::Toggle(bool val, StaticString key,
		StaticString help, ValueChangedCallback vchng, ValidatorCallback validcb) :
	TypedRequest<Type::Toggle, bool>(val, key, help, vchng, validcb)
{

}

bool Toggle::getValue(Menu * callingMenu, bool * v) {
	return Globals::commSource()->getBoolFor(id(), v);
}



} /* namespace Request */
} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

/*
 * ItemReqString.cpp
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  ItemReqString is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */
#include "includes/menuitem/requests/ItemReqString.h"
#include "includes/SUIGlobals.h"

namespace SerialUI {
namespace Menu {
namespace Item {
namespace Request {

String::String(TopLevelString initVal,
		StaticString key,
		StaticString help, uint8_t maxlen,
		ValueChangedCallback vchng,
		ValidatorCallback validcb) :
		ReqStringBase(initVal, key, help,
						vchng, validcb),
						_maxLen(maxlen)
{

}


String::String(uint8_t maxlen,
		ValueChangedCallback vchng,
		ValidatorCallback validcb) :
		ReqStringBase("", vchng, validcb),
						_maxLen(maxlen)
{

}

String::String(
		StaticString key,
		StaticString help, uint8_t maxlen,
		ValueChangedCallback vchng,
		ValidatorCallback validcb) :
		ReqStringBase("", key, help,
						vchng, validcb),
						_maxLen(maxlen)
{

}
bool String::getValue(Menu * callingMenu, TopLevelString * v) {
	if (Globals::commSource()->getStringFor(id(), _maxLen, v))
	{
		v->trim();
		return true;
	}
	return false;
}


ITEMPYTHONOVERRIDE_VALIDATION(ReqStringBase, String, TopLevelString)



} /* namespace Request */
} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

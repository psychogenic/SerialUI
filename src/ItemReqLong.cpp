/*
 * ItemReqLong.cpp
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  ItemReqLong is part of the XYZPROJECT project.
 *  Copyright (C) 2018 Pat Deegan, psychogenic.com
 */

#include "includes/menuitem/requests/ItemReqLong.h"
#include "includes/SUIGlobals.h"

namespace SerialUI {
namespace Menu {
namespace Item {
namespace Request {


Long::Long(long int initVal, StaticString key,
		StaticString help, ValueChangedCallback vchng, ValidatorCallback validcb) :
		CountableTypedRequest<Type::LongInt, long int, Long>(initVal, key, help, vchng, validcb)
{

}
Long::Long(long int initVal, ValueChangedCallback vchng, ValidatorCallback validcb) :
		CountableTypedRequest<Type::LongInt, long int, Long>(initVal, vchng, validcb)
{

}
bool Long::getValue(Menu * callingMenu, long int * v) {
	return Globals::commSource()->getLongIntFor(id(), v);
}

} /* namespace Request */
} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

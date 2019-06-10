/*
 * ItemReqUnsignedLong.cpp
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  ItemReqUnsignedLong is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#include "includes/menuitem/requests/ItemReqUnsignedLong.h"
#include "includes/SUIGlobals.h"

namespace SerialUI {
namespace Menu {
namespace Item {
namespace Request {

UnsignedLong::UnsignedLong(unsigned long int initVal, StaticString key,
		StaticString help, ValueChangedCallback vchng,
		ValidatorCallback validcb) :
		COUNTABLEREQCLASS_PARENT(Type::UnsignedLongInt, unsigned long int, UnsignedLong)(initVal, key, help,
				vchng, validcb) {


}


UnsignedLong::UnsignedLong(unsigned long int initVal, ValueChangedCallback vchng,
		ValidatorCallback validcb) :
		COUNTABLEREQCLASS_PARENT(Type::UnsignedLongInt, unsigned long int, UnsignedLong)(initVal, vchng, validcb) {

}

bool UnsignedLong::getValue(Menu * callingMenu, unsigned long int * v) {
	return Globals::commSource()->getLongUIntFor(id(), v);
}


ITEMPYTHONOVERRIDE_VALIDATION(COUNTABLEREQCLASS_PARENT(Type::UnsignedLongInt, unsigned long int, UnsignedLong),
		UnsignedLong, unsigned long int);

} /* namespace Request */
} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

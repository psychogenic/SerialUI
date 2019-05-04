/*
 * ItemReqFloat.cpp
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  ItemReqFloat is part of the XYZPROJECT project.
 *  Copyright (C) 2018 Pat Deegan, psychogenic.com
 */


#include "includes/menuitem/requests/ItemReqFloat.h"
#include "includes/SUIGlobals.h"

namespace SerialUI {
namespace Menu {
namespace Item {
namespace Request {

Float::Float(float initVal, StaticString key,
		StaticString help, ValueChangedCallback vchng, ValidatorCallback validcb) :
		COUNTABLEREQCLASS_PARENT(Type::Float, float, Float)(initVal, key, help, vchng, validcb)
{

}

Float::Float(float initVal, ValueChangedCallback vchng, ValidatorCallback validcb) :
		COUNTABLEREQCLASS_PARENT(Type::Float, float, Float)(initVal, vchng, validcb)
{

}


bool Float::getValue(Menu * callingMenu, float * v) {
	return Globals::commSource()->getFloatFor(id(), v);
}


} /* namespace Request */
} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

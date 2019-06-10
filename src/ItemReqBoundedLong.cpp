/*
 * ItemReqBoundedLong.cpp
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  ItemReqBoundedLong is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#include "includes/menuitem/requests/ItemReqBoundedLong.h"
#include "includes/SUIGlobals.h"

namespace SerialUI {
namespace Menu {
namespace Item {
namespace Request {


BoundedLong::BoundedLong(long int initVal,
		StaticString key,
		StaticString help, long int min, long int max,
		ValueChangedCallback vchng,
		ValidatorCallback validcb):
		COUNTABLEREQCLASS_PARENT(Type::BoundedLongInt, long int, BoundedLong)
		( initVal, key, help, vchng, validcb),
		_minAllowed(min), _maxAllowed(max),
		_stepVal(1)
{

	ensureSaneBounds();
	if (initVal >= min && initVal <= max) {

		this->setValue(initVal);
	} else {

		this->setValue(_minAllowed);
	}

}

BoundedLong::BoundedLong(
		long int initVal,
		long int min, long int max,
		ValueChangedCallback vchng,
		ValidatorCallback validcb):
		COUNTABLEREQCLASS_PARENT(Type::BoundedLongInt, long int, BoundedLong)(initVal, vchng, validcb),
		_minAllowed(min), _maxAllowed(max),
		_stepVal(1)
{

	ensureSaneBounds();

	if (initVal >= min && initVal <= max) {

		this->setValue(initVal);
	} else {

		this->setValue(_minAllowed);
	}

}
bool BoundedLong::getValue(Menu * callingMenu, long int * v) {

	return Globals::commSource()->getLongIntFor(id(), v);
}

bool BoundedLong::valueIsValid(long int & val)  {

	bool withinBounds = (val >= _minAllowed && val <= _maxAllowed);

	return withinBounds;
}

void BoundedLong::ensureSaneBounds() {

	if (_minAllowed > _maxAllowed)
	{
			long int t = _maxAllowed;
			_maxAllowed = _minAllowed;
			_minAllowed = t;
	}


}



ITEMPYTHONOVERRIDE_VALIDATION(COUNTABLEREQCLASS_PARENT(Type::BoundedLongInt, long int, BoundedLong),
		BoundedLong, long int);





} /* namespace Request */
} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

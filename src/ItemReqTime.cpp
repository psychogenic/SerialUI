/*
 * ItemReqTime.cpp
 *
 *  Created on: Jun 2, 2018
 *      Author: Pat Deegan
 *
 *  ItemReqTime is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#include "includes/menuitem/requests/ItemReqTime.h"

namespace SerialUI {
namespace Menu {
namespace Item {
namespace Request {


Time::Time(uint8_t hour, uint8_t minute, uint8_t second,
		StaticString key,
		StaticString help,
		ValueChangedCallback vchng,
		ValidatorCallback validcb) :
		UnsignedLong(0, key, help, vchng, validcb) {

	setRequestType(Type::Time);
	setValue(toValue(TimeElements(hour, minute, second)));
}


Time::Time(unsigned long int initVal, StaticString key,
		StaticString help, ValueChangedCallback vchng,
		ValidatorCallback validcb) :
		UnsignedLong(initVal, key, help,
				vchng, validcb) {

	setRequestType(Type::Time);
}


Time::Time(unsigned long int initVal, ValueChangedCallback vchng,
		ValidatorCallback validcb) :
		UnsignedLong(initVal, vchng, validcb) {
	setRequestType(Type::Time);
}

uint32_t Time::toValue(const Time::TimeElements & te) {
	return te.timeValue;
}
Time::TimeElements Time::asTimeValue(uint32_t t) {
	return TimeElements(t);

}

Time::TimeElements Time::timeValue() {
	return asTimeValue(currentValue());
}


} /* namespace Request */
} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

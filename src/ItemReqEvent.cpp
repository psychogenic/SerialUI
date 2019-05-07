/*
 * ItemReqEvent.cpp
 *
 *  Created on: Jun 2, 2018
 *      Author: Pat Deegan
 *
 *  ItemReqEvent is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#include "includes/menuitem/requests/ItemReqEvent.h"

namespace SerialUI {
namespace Menu {
namespace Item {
namespace Request {

Event::TimeElements Event::asTimeValue(uint32_t t) {
	Event::TimeElements te(t);
	return te;
}

uint32_t Event::toValue(const TimeElements& te) {
	return te.timeValue;
}

Event::Event(uint8_t startHour, uint8_t startMinute, uint8_t startSeconds,
		uint16_t durationMinutes,
		StaticString key,
		StaticString help,
		ValueChangedCallback vchng,
		ValidatorCallback validcb) :
		UnsignedLong(
				TimeElements(startHour, startMinute, startSeconds, durationMinutes).timeValue,
				key, help,
				vchng, validcb){
	setRequestType(Type::Event);

}

Event::Event(unsigned long int initVal, ValueChangedCallback vchng,
		ValidatorCallback validcb) :
		UnsignedLong(initVal, vchng, validcb){
	setRequestType(Type::Event);
}

Event::Event(unsigned long int initVal, StaticString key, StaticString help,
		ValueChangedCallback vchng, ValidatorCallback validcb) :
		UnsignedLong(initVal, key, help, vchng, validcb) {
	setRequestType(Type::Event);
}

void Event::setAt(const TimeElements& te) {
	this->setValue(te.timeValue);
}

void Event::setDay(Weekday::day day) {
	Event::TimeElements te(this->timeValue());
	te.day = day;
	setAt(te);
}

void Event::setStart(uint8_t hour, uint8_t minute, uint8_t seconds) {

	Event::TimeElements te(this->timeValue());
	te.startHour = hour;
	te.startMinute = minute;
	te.startSecond = seconds;

	setAt(te);
}

Event::TimeElements Event::timeValue() {
	return toValue(this->currentValue());
}

} /* namespace Request */
} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

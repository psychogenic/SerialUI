/*
 * ItemReqTime.h
 *
 *  Created on: Jun 2, 2018
 *      Author: Pat Deegan
 *
 *  ItemReqTime is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQTIME_H_
#define SERIALUI_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQTIME_H_

#include "ItemReqUnsignedLong.h"
namespace SerialUI {
namespace Menu {
namespace Item {
namespace Request {

class Time : public UnsignedLong {
public:
	typedef union TimeElementsUnion {
		struct __attribute__((__packed__)) {
			uint8_t second;
			uint8_t minute;
			uint8_t hour;
			uint8_t reserved;
		};
		uint32_t timeValue;
		TimeElementsUnion(uint32_t v=0) :
			timeValue(v) {

		}

		TimeElementsUnion(uint8_t h, uint8_t m, uint8_t s) :
			second(s < 60 ? s : 0),
			minute(m < 60 ? m : 0 ),
			hour(h < 24 ? h : 0),
			reserved(0)
		{

		}

		uint32_t daySeconds() const {
			return (second + (minute * 60) + (hour * 60 * 60));
		}
		bool operator==(const TimeElementsUnion & other) {
			return (daySeconds() == other.daySeconds());
		}
		inline bool operator!=(const TimeElementsUnion & other) {
			return (! ((*this) == other));
		}
		bool operator<(const TimeElementsUnion & other) {
			return (daySeconds() < other.daySeconds());
		}
		bool operator<=(const TimeElementsUnion & other){
			return (daySeconds() <= other.daySeconds());
		}
		inline bool operator>(const TimeElementsUnion & other) {
			return (! ((*this) <= other));
		}
		inline bool operator>=(const TimeElementsUnion & other) {
			return (! ((*this) < other));
		}

	} TimeElements;

	static TimeElements asTimeValue(uint32_t t);
	static uint32_t toValue(const TimeElements & te);

	Time(uint8_t hour, uint8_t minute, uint8_t second,
			StaticString key,
			StaticString help,
			ValueChangedCallback vchng=NULL,
			ValidatorCallback validcb=NULL);
	Time(unsigned long int initVal=0, ValueChangedCallback vchng=NULL,
			ValidatorCallback validcb=NULL);
	Time(unsigned long int initVal, StaticString key,
			StaticString help, ValueChangedCallback vchng=NULL,
			ValidatorCallback validcb=NULL);

	TimeElements timeValue();

};

} /* namespace Request */
} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

#endif /* SERIALUI_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQTIME_H_ */

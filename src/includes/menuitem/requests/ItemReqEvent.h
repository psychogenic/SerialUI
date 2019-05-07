/*
 * ItemReqEvent.h
 *
 *  Created on: Jun 2, 2018
 *      Author: Pat Deegan
 *
 *  ItemReqEvent is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQEVENT_H_
#define SERIALUI_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQEVENT_H_

#include "ItemReqUnsignedLong.h"
namespace SerialUI {
namespace Menu {
namespace Item {
namespace Request {

class Event : public UnsignedLong {
public:

	typedef union EventTimeRangeUnion {
		struct __attribute__((__packed__)) {
			uint8_t startSecond:6;
			uint8_t startMinute:6;
			uint8_t startHour:5;
			uint8_t endMinute:6;
			uint8_t endHour:5;
			Weekday::day day:3;
			bool 	flag:1;
		};
		uint32_t timeValue;
		EventTimeRangeUnion(uint32_t v=0) :
			timeValue(v) {

		}
		EventTimeRangeUnion(uint8_t start_hour, uint8_t start_minute,
				uint8_t start_seconds, uint16_t durationMinutes=1) {
			startHour = start_hour < 24 ? start_hour : 23;
			startMinute = start_minute < 60 ? start_minute : 59;
			startSecond = start_seconds < 60 ? start_seconds : 59;

			uint16_t endTimeMinutes = (startHour * 60) + startMinute + durationMinutes;
			endHour = (uint8_t) ( endTimeMinutes/ 60 );
			endMinute = endTimeMinutes - (60 * endHour);


		}
		EventTimeRangeUnion(uint8_t start_hour, uint8_t start_minute,
				uint8_t start_seconds, uint8_t end_hour, uint8_t end_minute) {

			startHour = start_hour < 24 ? start_hour : 23;
			startMinute = start_minute < 60 ? start_minute : 59;
			startSecond = start_seconds < 60 ? start_seconds : 59;
			endHour = end_hour < 24 ? end_hour : 23;
			endMinute = end_minute < 60 ? end_minute : 59;


		}
		uint32_t daySecondsStart() const {
			return (startSecond + (startMinute * 60) + (startHour * 60 *60));
		}

		uint32_t daySecondsEnd() const {
			return ((endHour * 60 * 60) + (endMinute * 60));
		}

		bool isValid() const {
			return (daySecondsEnd() > daySecondsStart());
		}

		uint32_t durationSeconds() const {
			return (daySecondsEnd() - daySecondsStart());
		}

	} TimeElements;

	static TimeElements asTimeValue(uint32_t t);
	static uint32_t toValue(const TimeElements & te);

	Event(unsigned long int initVal=0, ValueChangedCallback vchng=NULL,
			ValidatorCallback validcb=NULL);
	Event(unsigned long int initVal, StaticString key,
			StaticString help, ValueChangedCallback vchng=NULL,
			ValidatorCallback validcb=NULL);

	Event(uint8_t startHour, uint8_t startMinute, uint8_t startSeconds,
			uint16_t durationMinutes,
			StaticString key,
			StaticString help,
			ValueChangedCallback vchng=NULL,
			ValidatorCallback validcb=NULL);

	void setAt(const TimeElements & te);
	void setDay(Weekday::day day);
	void setStart(uint8_t hour, uint8_t minute, uint8_t seconds=0);
	TimeElements timeValue();

};

} /* namespace Request */
} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

#endif /* SERIALUI_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQEVENT_H_ */

/*
 * SerialUITypes.h
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  SerialUITypes is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUIV3_SRC_INCLUDES_SERIALUITYPES_H_
#define SERIALUIV3_SRC_INCLUDES_SERIALUITYPES_H_

#include "SerialUIExtIncludes.h"
#include "SerialUIPlatform.h"

#define SERIAL_UI_REQUEST_NOINPUT 0xf0

namespace SerialUI {

typedef ::String TopLevelString;
typedef uint32_t TimeValue;
typedef uint8_t StaticStringLength;


namespace Weekday {

typedef enum {
	ANY = 0,
	Sunday = 1,
	Monday = 2,
	Tuesday = 3,
	Wednesday = 4,
	Thursday = 5,
	Friday = 6,
	Saturday = 7,
} day;

}


namespace Mode {
typedef enum {
	User = 0, Program
} Selection;

}
namespace Tracked {

typedef uint8_t ID;

namespace Type {
typedef enum TrackedTypeEnum {
	INVALID = 0x00,
	Char = 0x01,
	Bool = 0x02,
	Toggle = 0x03,
	Int = 0x04,
	UInt = 0x05,
	BoundedLongInt = 0x06,
	OptionsList = 0x07,
	Float = 0x08,
	String = 0x09,
	DateTime = 0x0A,
	Time = 0x0B,
	Event = 0x0C,

	WeeklySchedule = 0x0D, // TODO:FIXME


	OUTOFBOUNDS = 0x0E
} Value;

}

namespace View {
typedef enum TrackedViewTypeEnum {
    CurrentValue = 0x01,
    HistoryLog = 0x02,
    ChartBar = 0x03,
    ChartLineBasic = 0x04,
    ChartLineBoundaries = 0x05,
    ChartPie = 0x06
} Type;
}
} /* namespace Tracked */

namespace Request {
namespace Type {
typedef enum {
	INVALID = 0x0, BuiltIn = 0x01, MenuItem = 0x02,
} Value;
}

namespace BuiltIn {
typedef enum {
	INVALID = 0x0,
	UpLevel = 0x01,
	ModeUser = 0x02,
	ModeProgram = 0x03,
	ListMenu = 0x04,
	Help = 0x05,
	RefreshTracked = 0x06,
	Exit = 0x07

} Selection;

} /* namespace BuiltIn */
} /* namespace Request */

namespace Menu {
namespace Item {
typedef uint8_t ID;

typedef void (*CommandCallback)(void);

namespace Type {

typedef enum {
	INVALID = 0x00,

	Menu = 0x01,

	Command = 0x02,

	Input = 0x03,

	State = 0x04,

	StaticText = 0x05,

	DynamicText = 0x06,

	Group = 0x07,

	List = 0x08,

	TrackedView = 0x09,

	OUTOFBOUNDS = 0x0A // MUST be largest

} Value;

}/* namespace Type */

namespace Request {
namespace Type {

typedef enum {
	INVALID = 0x00,
	Character = 0x01,
	Boolean = 0x02,
	Toggle = 0x03,
	LongInt = 0x04,
	UnsignedLongInt = 0x05,
	BoundedLongInt = 0x06,
	OptionsList = 0x07,
	Float = 0x08,
	String = 0x09,
	DateTime = 0x0A,
	Time = 0x0B,
	Event = 0x0C,

	WeeklySchedule = 0x0D, // TODO

	Passphrase = 0x0E,

	Color = 0x0F,

	OUTOFBOUNDS = 0x10

} Value;
}
}

} /* namespace Item */
} /* namespace Menu */

namespace Auth {
namespace Transmission {
namespace Type {

typedef enum {
	Plain = 0,
	MD5 = 1,
	SHA256 = 2,

	Custom = 0xff
} Value;
} /* namespace Type */
} /* namespace Transmission */

namespace Request {
namespace Type {

typedef enum {
	Authenticate = 1,
	Setup = 2,
} Value ;

} /* namespace Type */
} /* namespace Request */
} /* namespace Auth */


} /* namespace SerialUI */

#endif /* SERIALUIV3_SRC_INCLUDES_SERIALUITYPES_H_ */

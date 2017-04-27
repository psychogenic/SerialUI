/*
 * SUITypes.h -- Typedefs used internally.
 * Copyright (C) 2013-2017 Pat Deegan, psychogenic.com.
 *
 *
 * http://www.flyingcarsandstuff.com/projects/SerialUI
 *
 *
 * This program library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See file LICENSE.txt for further informations on licensing
 * terms.
 *
 */


#ifndef SERIALUI_SRC_INCLUDES_SUITYPES_H_
#define SERIALUI_SRC_INCLUDES_SUITYPES_H_

#include "SUIExtIncludes.h"

#define SUI_UNUSED_PARAM(x)		(void)(x)

namespace SUI {

namespace Mode {
typedef enum SUIModeEnum {
	User=0,
	Program
} Selection;
}

typedef void (*MenuCommand_Callback)(void);
typedef void (*MenuRequest_Callback)(void);


namespace MenuItem {
typedef uint8_t	MenuItemID;

namespace Request {
namespace Type {

typedef enum ReqTypeEnum {
	Bool='b',
	Char='c',
	Float='f',
	Int='i',
	Options='o',
	String='s',
	Toggle='t',
	UInt='u'

} ID;

} /* namespace Type */
} /* namespace Request */

}

namespace Tracked {
namespace Type {
typedef enum TrackedTypeEnum {
	Bool='b',
	UInt='i',
	Float='f',
	Char='c',
	String='s'
} ID;

} /* namespace Type */
}/* namespace Tracked */


}


#endif /* SERIALUI_SRC_INCLUDES_SUITYPES_H_ */

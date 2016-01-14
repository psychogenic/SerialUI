/*
 * SUITypes.h
 *
 *  Created on: Jan 13, 2016
 *      Author: Pat Deegan
 *      Part of the SerialUI Project
 *      Copyright (C) 2015 Pat Deegan, http://psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_SUITYPES_H_
#define SERIALUI_SRC_INCLUDES_SUITYPES_H_

#include "SUIExtIncludes.h"


namespace SUI {

namespace Mode {
typedef enum SUIModeEnum {
	User=0,
	Program
} Selection;
}

typedef void (*MenuCommand_Callback)(void);
typedef void (*MenuRequest_Callback)(void);


}




#endif /* SERIALUI_SRC_INCLUDES_SUITYPES_H_ */

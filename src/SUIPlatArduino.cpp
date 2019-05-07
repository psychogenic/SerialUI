/*
 * SUIPlatArduino.cpp
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  SUIPlatArduino is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#include "includes/SerialUIPlatform.h"
#include "includes/SerialUITypes.h"


namespace SerialUI {

size_t staticStringLength(StaticString aKey) {
	return SUI_PLATFORM_STATICSTRING_LENGTH(aKey);

}
bool staticStringMatch(StaticString aKey, DynamicString theTest, bool allowPartials) {

	size_t testLen = staticStringLength(aKey);
	size_t bLen = strlen(theTest);
	if (!allowPartials) {

		if (testLen != bLen) {
			return false;
		}
	} else if (bLen < testLen) {
			testLen = bLen;
	}
	TopLevelString k(aKey);


	return (strncmp(k.c_str(), theTest, testLen) == 0);

}

} /* namespace SerialUI */



/*
 * Tracking.cpp
 *
 *  Created on: May 26, 2018
 *      Author: Pat Deegan
 *
 *  Tracking is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */


#include "includes/menu/Tracking.h"
#include "includes/SerialUIPlatform.h"

namespace SerialUI {
namespace Menu {

Tracking::Tracking(uint8_t numItems) :
		GrowableList<Tracked::State, Tracked::ID>(numItems) {
}

Tracked::State * Tracking::itemByName(DynamicString aKey) {
	for (uint8_t i=0; i<numItems(); i++) {
		Tracked::State * st = itemByIndex(i);
		if (staticStringMatch(st->name(), aKey, false)) {
			return st;
		}
	}

	return NULL;
}

} /* namespace Menu */
} /* namespace SerialUI */

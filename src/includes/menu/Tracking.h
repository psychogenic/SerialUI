/*
 * Tracking.h
 *
 *  Created on: May 26, 2018
 *      Author: Pat Deegan
 *
 *  Tracking is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_MENU_TRACKING_H_
#define SERIALUI_SRC_INCLUDES_MENU_TRACKING_H_

#include "../SerialUIConfig.h"
#include "../GrowableList.h"
#include "../tracked/tracked.h"

#ifndef SERIALUI_TRACKING_NUMITEMS_ATSTARTUP
#define SERIALUI_TRACKING_NUMITEMS_ATSTARTUP SERIALUI_TRACKING_NUMITEMS_ATSTARTUP_DEFAULT
#endif

namespace SerialUI {
namespace Menu {

class Tracking : public GrowableList<Tracked::State, Tracked::ID>{
public:
	Tracking(uint8_t numItems=SERIALUI_TRACKING_NUMITEMS_ATSTARTUP);
	Tracked::State * itemByName(DynamicString aKey);
};

} /* namespace Menu */
} /* namespace SerialUI */

#endif /* SERIALUI_SRC_INCLUDES_MENU_TRACKING_H_ */

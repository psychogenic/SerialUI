/*
 * SUIGlobals.h
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  SUIGlobals is part of the XYZPROJECT project.
 *  Copyright (C) 2018 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUIV3_SRC_INCLUDES_SUIGLOBALS_H_
#define SERIALUIV3_SRC_INCLUDES_SUIGLOBALS_H_

#include "SUIState.h"
#include "comm/CommSource.h"
#include "menu/MenuStructure.h"
#include "menu/Tracking.h"
#include "comm/CommChannel.h"

namespace SerialUI {

class Globals {
public:

	static State * state();
	static Comm::Source * commSource();
	static Comm::Channel * commChannel();
	static Menu::Structure * menuStructure();
	static Menu::Tracking * trackedStates();



private:
	Globals();


};

} /* namespace SerialUI */

#endif /* SERIALUIV3_SRC_INCLUDES_SUIGLOBALS_H_ */

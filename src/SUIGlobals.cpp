/*
 * SUIGlobals.cpp
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  SUIGlobals is part of the XYZPROJECT project.
 *  Copyright (C) 2018 Pat Deegan, psychogenic.com
 */

#include "includes/SUIGlobals.h"
#include "includes/comm/ChannelManager.h"

#define SUIGLOBS_SIMPLESING(tp) \
	static tp * _glob_sing = NULL; \
	if (_glob_sing) { \
		return _glob_sing; \
	} \
	_glob_sing = new tp(); \
	return _glob_sing


namespace SerialUI {

Globals::Globals() {
	// TODO Auto-generated constructor stub

}


State * Globals::state() {
	SUIGLOBS_SIMPLESING(State);
}

Menu::Structure * Globals::menuStructure() {
	SUIGLOBS_SIMPLESING(Menu::Structure);
}


Menu::Tracking * Globals::trackedStates() {
	SUIGLOBS_SIMPLESING(Menu::Tracking);

}

Comm::Channel * Globals::commChannel() {
	return Comm::ChannelManager::currentChannel();
}

Comm::Source * Globals::commSource() {
	return Comm::ChannelManager::currentChannel();
}

} /* namespace SerialUI */

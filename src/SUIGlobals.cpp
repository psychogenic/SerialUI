/*
 * SUIGlobals.cpp
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  SUIGlobals is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
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


#ifdef SERIALUI_PYTHONMODULES_SUPPORT_ENABLE
static SerialUI::Python::ExternalModule * curPythonModule = NULL;

Python::ExternalModule * Globals::pythonModule() {
	return curPythonModule;
}
bool Globals::setPythonModule(Python::ExternalModule * mod) {
	if (! mod) {
		curPythonModule = NULL;
		return true;
	}

	curPythonModule = mod;

	if (! mod->load()) {
		return false;
	}
	return true;
}
#endif /* SERIALUI_PYTHONMODULES_SUPPORT_ENABLE */

#ifdef SERIALUI_AUTHENTICATOR_ENABLE
static Auth::Authenticator * curAuthenticatorObj = NULL;

Auth::Authenticator * Globals::authenticator() {
	return curAuthenticatorObj;
}
void Globals::setAuthenticator(Auth::Authenticator* auth) {
	curAuthenticatorObj = auth;
}
#endif /* SERIALUI_AUTHENTICATOR_ENABLE */

} /* namespace SerialUI */

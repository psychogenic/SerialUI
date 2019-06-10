/*
 * SUIGlobals.h
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  SUIGlobals is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUIV3_SRC_INCLUDES_SUIGLOBALS_H_
#define SERIALUIV3_SRC_INCLUDES_SUIGLOBALS_H_

#include "SerialUIPlatform.h"
#include "SUIState.h"
#include "comm/CommSource.h"
#include "menu/MenuStructure.h"
#include "menu/Tracking.h"
#include "comm/CommChannel.h"

#ifdef SERIALUI_AUTHENTICATOR_ENABLE
#include "auth/Authenticator.h"
#endif

#ifdef SERIALUI_PYTHONMODULES_SUPPORT_ENABLE
#include "python/ExternalModule.h"
#endif

namespace SerialUI {

class Globals {
public:

	static State * state();
	static Comm::Source * commSource();
	static Comm::Channel * commChannel();
	static Menu::Structure * menuStructure();
	static Menu::Tracking * trackedStates();

#ifdef SERIALUI_PYTHONMODULES_SUPPORT_ENABLE
	static Python::ExternalModule * pythonModule();
	static bool setPythonModule(Python::ExternalModule * mod);
#endif /* SERIALUI_PYTHONMODULES_SUPPORT_ENABLE */

#ifdef SERIALUI_AUTHENTICATOR_ENABLE
	static Auth::Authenticator * authenticator();
	static void setAuthenticator(Auth::Authenticator* auth);
#endif /* SERIALUI_AUTHENTICATOR_ENABLE */

private:
	Globals();


};

} /* namespace SerialUI */

#endif /* SERIALUIV3_SRC_INCLUDES_SUIGLOBALS_H_ */

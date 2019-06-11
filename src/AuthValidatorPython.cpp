/*
 * AuthValidatorPython.cpp
 *
 *  Created on: Jun 7, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */

#include "includes/auth/AuthValidatorPython.h"

#if defined(SERIALUI_AUTHENTICATOR_ENABLE) and defined(SERIALUI_PYTHONMODULES_SUPPORT_ENABLE)
#include "includes/SUIGlobals.h"

#define SUIHAVE_EXTERNAL_VALIDATOR_AVAILABLE() \
	(Globals::pythonModule() && Globals::pythonModule()->authValidator())

namespace SerialUI {
namespace Auth {

ValidatorPython::ValidatorPython(Storage * storage) : Validator(storage) {


}

Challenge ValidatorPython::challenge(Level::Value forLevel) {
	if (! SUIHAVE_EXTERNAL_VALIDATOR_AVAILABLE()) {
		SERIALUI_DEBUG_OUTLN(F("No ext validator"));
		return NULL;
	}

	return Globals::pythonModule()->authValidator()->challenge(forLevel);

}

Level::Value ValidatorPython::grantAccess(ChallengeResponse resp) {
	if (! SUIHAVE_EXTERNAL_VALIDATOR_AVAILABLE()) {
		SERIALUI_DEBUG_OUTLN(F("No ext validator"));
		return Level::NoAccess;
	}

	return Globals::pythonModule()->authValidator()->grantAccess(resp);

}

Transmission::Type::Value ValidatorPython::communicationType() {
	if (! SUIHAVE_EXTERNAL_VALIDATOR_AVAILABLE()) {
		SERIALUI_DEBUG_OUTLN(F("No ext validator"));
		return Transmission::Type::Custom;
	}

	return Globals::pythonModule()->authValidator()->communicationType();

}

} /* namespace Auth */
} /* namespace SerialUI */

#endif /* SERIALUI_AUTHENTICATOR_ENABLE and SERIALUI_PYTHONMODULES_SUPPORT_ENABLE */



/*
 * AuthValidatorEquality.cpp
 *
 *  Created on: Jun 6, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */

#include "includes/auth/AuthValidatorEquality.h"

#ifdef SERIALUI_AUTHENTICATOR_ENABLE
namespace SerialUI {
namespace Auth {

ValidatorEquality::ValidatorEquality(Storage * storage) : Validator(storage) {

}


Level::Value ValidatorEquality::grantAccess(ChallengeResponse resp) {
	Level::Value testLevels[] = {
			Level::Guest,
			Level::User,
			Level::Admin,
			Level::NoAccess
	};
	SERIALUI_DEBUG_OUT(SUI_STR("ValidatorEQ "));
	if (! (resp && strlen(resp))) {
		SERIALUI_DEBUG_OUTLN(SUI_STR("nothing2chk"));
		return Level::NoAccess;
	}

	if (!storage()->configured()) {
		SERIALUI_DEBUG_OUTLN(SUI_STR("unconfig"));
		return Level::NoAccess;
	}
	uint8_t idx = 0;
	while (testLevels[idx] != Level::NoAccess) {
		Passphrase pass = storage()->passphrase(testLevels[idx]);

		if (pass) {
			SERIALUI_DEBUG_OUT(SUI_STR("comparing '"));
			SERIALUI_DEBUG_OUT(resp);
			SERIALUI_DEBUG_OUT(SUI_STR("' to "));
			SERIALUI_DEBUG_OUTLN(pass);
			int cmpval = strcmp(pass, resp) ;
			if (cmpval == 0) {
				SERIALUI_DEBUG_OUTLN(SUI_STR("HUZZAH"));
				return testLevels[idx];
			} else {
				SERIALUI_DEBUG_OUTLN(cmpval);
			}
		}
		idx++;
	}
	return Level::NoAccess;

}

} /* namespace Auth */
} /* namespace SerialUI */


#endif /* SERIALUI_AUTHENTICATOR_ENABLE */

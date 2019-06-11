/*
 * Authenticator.cpp
 *
 *  Created on: Jun 6, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */

#include "includes/auth/Authenticator.h"

#ifdef SERIALUI_AUTHENTICATOR_ENABLE

namespace SerialUI {
namespace Auth {

Authenticator::Authenticator(Validator * validator) :
		auth_validator(validator),
		current_level(Level::NoAccess)
{

}


bool Authenticator::configured() { return auth_validator->configured();}
bool Authenticator::setPassphrase(Passphrase pass, Level::Value forLevel) {
	return auth_validator->storage()->setPassphrase(pass, forLevel);
}

Challenge Authenticator::challenge(Level::Value forLevel) {
	return auth_validator->challenge(forLevel);
}
Level::Value Authenticator::grantAccess(ChallengeResponse resp) {
	current_level = auth_validator->grantAccess(resp);
	return current_level;
}

bool Authenticator::accessIsAtLeast(Level::Value lev) {
	return ((uint8_t)current_level >= lev);

}

Transmission::Type::Value Authenticator::encoding() {
	return auth_validator->communicationType();
}


} /* namespace Auth */
} /* namespace SerialUI */

#endif /* SERIALUI_AUTHENTICATOR_ENABLE */

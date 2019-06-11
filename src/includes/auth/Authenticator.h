/*
 * Authenticator.h
 *
 *  Created on: Jun 6, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */

#ifndef SERIALUI_SRC_INCLUDES_AUTH_AUTHENTICATOR_H_
#define SERIALUI_SRC_INCLUDES_AUTH_AUTHENTICATOR_H_


#include "AuthValidator.h"

#ifdef SERIALUI_AUTHENTICATOR_ENABLE

namespace SerialUI {
namespace Auth {

class Authenticator {
public:
	Authenticator(Validator * validator);

	bool configured();
	bool setPassphrase(Passphrase pass, Level::Value forLevel);

	Transmission::Type::Value encoding();
	Challenge challenge(Level::Value forLevel=Level::User) ;
	Level::Value grantAccess(ChallengeResponse resp) ;

	bool accessIsAtLeast(Level::Value lev);

	void clearAccess() { current_level = Level::NoAccess;}
	Level::Value accessLevel() { return current_level;}


private:
	Validator * auth_validator;
	Level::Value current_level;
};

} /* namespace Auth */
} /* namespace SerialUI */

#endif /* SERIALUI_AUTHENTICATOR_ENABLE */

#endif /* SERIALUI_SRC_INCLUDES_AUTH_AUTHENTICATOR_H_ */

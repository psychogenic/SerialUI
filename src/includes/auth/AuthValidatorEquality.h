/*
 * AuthValidatorEquality.h
 *
 *  Created on: Jun 6, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */

#ifndef SERIALUI_SRC_INCLUDES_AUTH_AUTHVALIDATOREQUALITY_H_
#define SERIALUI_SRC_INCLUDES_AUTH_AUTHVALIDATOREQUALITY_H_
#include "AuthValidator.h"

#ifdef SERIALUI_AUTHENTICATOR_ENABLE

namespace SerialUI {
namespace Auth {

class ValidatorEquality : public Validator {
public:
	ValidatorEquality(Storage * storage);

	virtual Challenge challenge(Level::Value forLevel=Level::User)
	{ return NULL; }
	virtual Level::Value grantAccess(ChallengeResponse resp);
	virtual Transmission::Type::Value communicationType() {
		return Transmission::Type::Plain;
	}
};

} /* namespace Auth */
} /* namespace SerialUI */


#endif /* SERIALUI_AUTHENTICATOR_ENABLE */

#endif /* SERIALUI_SRC_INCLUDES_AUTH_AUTHVALIDATOREQUALITY_H_ */

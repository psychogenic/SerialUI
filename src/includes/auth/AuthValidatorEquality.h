/*
 * AuthValidatorEquality.h
 *
 *  Created on: Jun 6, 2019
 *      Author: malcalypse
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

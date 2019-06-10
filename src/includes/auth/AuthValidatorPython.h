/*
 * AuthValidatorPython.h
 *
 *  Created on: Jun 7, 2019
 *      Author: malcalypse
 */

#ifndef SERIALUI_SRC_INCLUDES_AUTH_AUTHVALIDATORPYTHON_H_
#define SERIALUI_SRC_INCLUDES_AUTH_AUTHVALIDATORPYTHON_H_
#include "AuthValidator.h"

#if defined(SERIALUI_AUTHENTICATOR_ENABLE) and defined(SERIALUI_PYTHONMODULES_SUPPORT_ENABLE)

namespace SerialUI {
namespace Auth {

class ValidatorPython : public Validator {
public:
	ValidatorPython(Storage * storage);

	virtual Challenge challenge(Level::Value forLevel=Level::User);
	virtual Level::Value grantAccess(ChallengeResponse resp);
	virtual Transmission::Type::Value communicationType() ;
};

} /* namespace Auth */
} /* namespace SerialUI */

#endif /* SERIALUI_PYTHONMODULES_SUPPORT_ENABLE  and SERIALUI_AUTHENTICATOR_ENABLE */

#endif /* SERIALUI_SRC_INCLUDES_AUTH_AUTHVALIDATORPYTHON_H_ */

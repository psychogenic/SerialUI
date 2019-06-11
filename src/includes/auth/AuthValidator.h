/*
 * AuthValidator.h
 *
 *  Created on: Jun 6, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */

#ifndef SERIALUI_SRC_INCLUDES_AUTH_AUTHVALIDATOR_H_
#define SERIALUI_SRC_INCLUDES_AUTH_AUTHVALIDATOR_H_
#include "AuthTypes.h"
#include "AuthStorage.h"

namespace SerialUI {
namespace Auth {

class Validator {
public:
	Validator(Storage * storage);
	virtual ~Validator();
	virtual Transmission::Type::Value communicationType() = 0;
	virtual Challenge challenge(Level::Value forLevel=Level::User) = 0;
	virtual Level::Value grantAccess(ChallengeResponse resp) = 0;

	virtual bool configured() { return auth_store->configured();}
	Storage * storage() { return auth_store;}
private:
	Storage * auth_store;
};

} /* namespace Auth */
} /* namespace SerialUI */


#endif /* SERIALUI_SRC_INCLUDES_AUTH_AUTHVALIDATOR_H_ */

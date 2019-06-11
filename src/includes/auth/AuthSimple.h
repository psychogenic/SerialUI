/*
 * AuthSimple.h
 *
 *  Created on: Jun 6, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */

#ifndef SERIALUI_SRC_INCLUDES_AUTH_AUTHSIMPLE_H_
#define SERIALUI_SRC_INCLUDES_AUTH_AUTHSIMPLE_H_

#include "Authenticator.h"

#ifdef SERIALUI_AUTHENTICATOR_ENABLE

#include "AuthValidatorEquality.h"
#include "AuthStorageStatic.h"

namespace SerialUI {
namespace Auth {

class Simple : public Authenticator {
public:
	Simple(Passphrase adminPass, Passphrase userPass=NULL,
			Passphrase guestPass=NULL);

private:
	StorageStatic pass_store;
	ValidatorEquality eq_validator;
};

} /* namespace Auth */
} /* namespace SerialUI */


#endif /* SERIALUI_AUTHENTICATOR_ENABLE */

#endif /* SERIALUI_SRC_INCLUDES_AUTH_AUTHSIMPLE_H_ */

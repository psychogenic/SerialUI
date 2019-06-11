/*
 * AuthSimple.cpp
 *
 *  Created on: Jun 6, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */

#include "includes/auth/AuthSimple.h"

#ifdef SERIALUI_AUTHENTICATOR_ENABLE

namespace SerialUI {
namespace Auth {

Simple::Simple(Passphrase adminPass, Passphrase userPass,
		Passphrase guestPass) :
				pass_store(adminPass, userPass, guestPass),
				eq_validator(&pass_store),
				Authenticator(&eq_validator)
{


}


} /* namespace Auth */
} /* namespace SerialUI */


#endif /* SERIALUI_AUTHENTICATOR_ENABLE */

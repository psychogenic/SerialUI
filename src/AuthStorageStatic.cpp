/*
 * AuthStorageStatic.cpp
 *
 *  Created on: Jun 6, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */

#include "includes/auth/AuthStorageStatic.h"

#ifdef SERIALUI_AUTHENTICATOR_ENABLE
namespace SerialUI {
namespace Auth {

StorageStatic::StorageStatic(Passphrase adminPass,
		Passphrase userPass,
		Passphrase guestPass) : Storage()
{
	stored_pass[(uint8_t)Level::NoAccess] = NULL;
	stored_pass[(uint8_t)Level::Guest] = guestPass;
	stored_pass[(uint8_t)Level::User] = userPass;
	stored_pass[(uint8_t)Level::Admin] = adminPass;
	for (uint8_t i=0; i<4; i++) {
		uconfiged_pass[i] = NULL;
	}

}

bool StorageStatic::setPassphrase(Passphrase pass, Level::Value forLevel) {
	uint8_t levIdx = (uint8_t)forLevel;
	if (uconfiged_pass[levIdx]) {
		delete[] uconfiged_pass[levIdx];
	}
	if (! pass) {
		uconfiged_pass[levIdx] = NULL;
		stored_pass[levIdx] = NULL;
		return true;
	}
	uint8_t passLen = strlen(pass);

	uconfiged_pass[levIdx] = new char[passLen + 1];
	if (! uconfiged_pass[levIdx]) {
		return false;
	}
	uconfiged_pass[levIdx][passLen] = 0;
	strcpy(uconfiged_pass[levIdx], pass);
	stored_pass[levIdx] = uconfiged_pass[levIdx];
	return true;
}
bool StorageStatic::configured(Level::Value forLevel) {
	if (! stored_pass[(uint8_t)forLevel]) {
		return false;
	}
	return true;
}

Passphrase StorageStatic::passphrase(Level::Value forLevel) {
	return stored_pass[(uint8_t)forLevel];
}

} /* namespace Auth */
} /* namespace SerialUI */


#endif /* SERIALUI_AUTHENTICATOR_ENABLE */

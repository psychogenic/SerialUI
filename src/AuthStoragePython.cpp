/*
 * AuthStoragePython.cpp
 *
 *  Created on: Jun 7, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */

#include "includes/auth/AuthStoragePython.h"

#if defined(SERIALUI_AUTHENTICATOR_ENABLE) and defined(SERIALUI_PYTHONMODULES_SUPPORT_ENABLE)
#include "includes/SUIGlobals.h"

#define SUIHAVE_EXTERNAL_STORAGE_AVAILABLE() \
	(Globals::pythonModule() && Globals::pythonModule()->authStorage())

namespace SerialUI {
namespace Auth {

StoragePython::StoragePython() : Storage() {
}

bool StoragePython::configured(Level::Value forLevel) {
	if (! SUIHAVE_EXTERNAL_STORAGE_AVAILABLE()) {
		return false;
	}
	return Globals::pythonModule()->authStorage()->configured(forLevel);

}

Passphrase StoragePython::passphrase(Level::Value forLevel) {
	if (! SUIHAVE_EXTERNAL_STORAGE_AVAILABLE()) {
			return NULL;
	}
	return Globals::pythonModule()->authStorage()->passphrase(forLevel);

}

bool StoragePython::setPassphrase(Passphrase pass, Level::Value forLevel) {
	if (! SUIHAVE_EXTERNAL_STORAGE_AVAILABLE()) {
		return false;
	}
	return Globals::pythonModule()->authStorage()->setPassphrase(pass, forLevel);

}

} /* namespace Auth */
} /* namespace SerialUI */

#endif /* SERIALUI_AUTHENTICATOR_ENABLE and SERIALUI_PYTHONMODULES_SUPPORT_ENABLE */


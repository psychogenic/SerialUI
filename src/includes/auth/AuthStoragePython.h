/*
 * AuthStoragePython.h
 *
 *  Created on: Jun 7, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */

#ifndef SERIALUI_SRC_INCLUDES_AUTH_AUTHSTORAGEPYTHON_H_
#define SERIALUI_SRC_INCLUDES_AUTH_AUTHSTORAGEPYTHON_H_
#include "AuthStorage.h"

#if defined(SERIALUI_AUTHENTICATOR_ENABLE) and defined(SERIALUI_PYTHONMODULES_SUPPORT_ENABLE)

namespace SerialUI {
namespace Auth {

class StoragePython : public Storage {
public:
	StoragePython();

	virtual bool configured(Level::Value forLevel=Level::User);
	virtual Passphrase passphrase(Level::Value forLevel);
	virtual bool setPassphrase(Passphrase pass, Level::Value forLevel);

};

} /* namespace Auth */
} /* namespace SerialUI */

#endif /* SERIALUI_PYTHONMODULES_SUPPORT_ENABLE  and SERIALUI_AUTHENTICATOR_ENABLE */
#endif /* SERIALUI_SRC_INCLUDES_AUTH_AUTHSTORAGEPYTHON_H_ */

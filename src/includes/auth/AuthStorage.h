/*
 * AuthStorage.h
 *
 *  Created on: Jun 6, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */

#ifndef SERIALUI_SRC_INCLUDES_AUTH_AUTHSTORAGE_H_
#define SERIALUI_SRC_INCLUDES_AUTH_AUTHSTORAGE_H_
#include "../SerialUIPlatform.h"
#include "AuthTypes.h"

namespace SerialUI {
namespace Auth {

class Storage {
public:
	Storage();
	virtual ~Storage();
	virtual bool configured(Level::Value forLevel=Level::User) { return false;}
	virtual Passphrase passphrase(Level::Value forLevel) = 0;
	virtual bool setPassphrase(Passphrase pass, Level::Value forLevel) = 0;
};

} /* namespace Auth */
} /* namespace SerialUI */

#endif /* SERIALUI_SRC_INCLUDES_AUTH_AUTHSTORAGE_H_ */

/*
 * AuthStorage.h
 *
 *  Created on: Jun 6, 2019
 *      Author: malcalypse
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

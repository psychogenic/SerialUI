/*
 * AuthStorageStatic.h
 *
 *  Created on: Jun 6, 2019
 *      Author: malcalypse
 */

#ifndef SERIALUI_SRC_INCLUDES_AUTH_AUTHSTORAGESTATIC_H_
#define SERIALUI_SRC_INCLUDES_AUTH_AUTHSTORAGESTATIC_H_

#include "Authenticator.h"
#ifdef SERIALUI_AUTHENTICATOR_ENABLE

namespace SerialUI {
namespace Auth {

class StorageStatic : public Storage {
public:
	StorageStatic(Passphrase adminPass, Passphrase userPass,
			Passphrase guestPass=NULL);

	virtual bool configured(Level::Value forLevel=Level::User);

	virtual Passphrase passphrase(Level::Value forLevel) ;
	virtual bool setPassphrase(Passphrase pass, Level::Value forLevel);
private:
	Passphrase  stored_pass[4];
	char *  uconfiged_pass[4];

};

} /* namespace Auth */
} /* namespace SerialUI */

#endif /* SERIALUI_AUTHENTICATOR_ENABLE */
#endif /* SERIALUI_SRC_INCLUDES_AUTH_AUTHSTORAGESTATIC_H_ */

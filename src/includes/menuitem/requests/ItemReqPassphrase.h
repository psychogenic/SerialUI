/*
 * ItemReqPassphrase.h
 *
 *  Created on: May 7, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */

#ifndef SERIALUI_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQPASSPHRASE_H_
#define SERIALUI_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQPASSPHRASE_H_

#include "ItemReqString.h"

namespace SerialUI {
namespace Menu {
namespace Item {
namespace Request {

class Passphrase : public ::SerialUI::Menu::Item::Request::String {
public:

	Passphrase(uint8_t maxlen, ValueChangedCallback vchng=NULL,
				ValidatorCallback validcb=NULL) :
					String(maxlen, vchng, validcb) {
		setRequestType(Type::Passphrase);
	}
	Passphrase(
				StaticString key,
				StaticString help, uint8_t maxlen,
				ValueChangedCallback vchng=NULL,
				ValidatorCallback validcb=NULL) :
		String(key, help, maxlen, vchng, validcb) {

		setRequestType(Type::Passphrase);
	}
	Passphrase(TopLevelString initVal,
			StaticString key,
			StaticString help, uint8_t maxlen,
			ValueChangedCallback vchng=NULL,
			ValidatorCallback validcb=NULL) :
				String(initVal, key, help, maxlen, vchng, validcb)
	{

		setRequestType(Type::Passphrase);
	}

};

} /* namespace Request */
} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */




#endif /* SERIALUI_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQPASSPHRASE_H_ */

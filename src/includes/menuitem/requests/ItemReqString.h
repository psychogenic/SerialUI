/*
 * ItemReqString.h
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  ItemReqString is part of the XYZPROJECT project.
 *  Copyright (C) 2018 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUIV3_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQSTRING_H_
#define SERIALUIV3_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQSTRING_H_

#include "../ItemRequest.h"
namespace SerialUI {
namespace Menu {
namespace Item {
namespace Request {

class String : public TypedRequest<Type::String, TopLevelString> {
public:

	String(uint8_t maxlen, ValueChangedCallback vchng=NULL,
				ValidatorCallback validcb=NULL);
	String(
				StaticString key,
				StaticString help, uint8_t maxlen,
				ValueChangedCallback vchng=NULL,
				ValidatorCallback validcb=NULL);
	String(TopLevelString initVal,
			StaticString key,
			StaticString help, uint8_t maxlen,
			ValueChangedCallback vchng=NULL,
			ValidatorCallback validcb=NULL);


	uint8_t maximumLength() { return _maxLen;}
	virtual bool getValue(Menu * callingMenu, TopLevelString * v);

private:
	uint8_t _maxLen;
};

} /* namespace Request */
} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

#endif /* SERIALUIV3_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQSTRING_H_ */

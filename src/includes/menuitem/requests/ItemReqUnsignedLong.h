/*
 * ItemReqUnsignedLong.h
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  ItemReqUnsignedLong is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUIV3_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQUNSIGNEDLONG_H_
#define SERIALUIV3_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQUNSIGNEDLONG_H_

#include "../ItemRequest.h"

namespace SerialUI {
namespace Menu {
namespace Item {
namespace Request {

class UnsignedLong : public
	COUNTABLEREQCLASS_PARENT(Type::UnsignedLongInt, unsigned long int, UnsignedLong) {
public:
	UnsignedLong(unsigned long int initVal=0, ValueChangedCallback vchng=NULL,
			ValidatorCallback validcb=NULL);


	UnsignedLong(unsigned long int initVal, StaticString key,
			StaticString help, ValueChangedCallback vchng=NULL,
			ValidatorCallback validcb=NULL);


	virtual bool getValue(Menu * callingMenu, unsigned long int * v);


	ITEMPYTHONOVERRIDE_VALIDATION_DECL(unsigned long int);
	COUTABLEREQCLASS_USINGALLOPS(Type::UnsignedLongInt, unsigned long int, UnsignedLong);


};

} /* namespace Request */
} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

#endif /* SERIALUIV3_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQUNSIGNEDLONG_H_ */

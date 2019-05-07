/*
 * ItemReqDateTime.h
 *
 *  Created on: Jun 2, 2018
 *      Author: Pat Deegan
 *
 *  ItemReqDateTime is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQDATETIME_H_
#define SERIALUI_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQDATETIME_H_

#include "ItemReqUnsignedLong.h"
namespace SerialUI {
namespace Menu {
namespace Item {
namespace Request {

class DateTime : public UnsignedLong {
public:
	DateTime(unsigned long int initVal=0, ValueChangedCallback vchng=NULL,
			ValidatorCallback validcb=NULL);
	DateTime(unsigned long int initVal, StaticString key,
			StaticString help, ValueChangedCallback vchng=NULL,
			ValidatorCallback validcb=NULL);
};

} /* namespace Request */
} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

#endif /* SERIALUI_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQDATETIME_H_ */

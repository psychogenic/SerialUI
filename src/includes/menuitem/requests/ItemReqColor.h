/*
 * ItemReqColor.h
 *
 *  Created on: May 13, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */

#ifndef SERIALUI_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQCOLOR_H_
#define SERIALUI_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQCOLOR_H_

#include "ItemReqUnsignedLong.h"

namespace SerialUI {
namespace Menu {
namespace Item {
namespace Request {


class Color : public UnsignedLong {
public:
	Color(unsigned long int initVal=0, ValueChangedCallback vchng=NULL,
			ValidatorCallback validcb=NULL) : UnsignedLong(initVal, vchng, validcb) {
		this->setRequestType(Type::Color);
	}

	Color(unsigned long int initVal, StaticString key,
			StaticString help, ValueChangedCallback vchng=NULL,
			ValidatorCallback validcb=NULL) : UnsignedLong(initVal, key, help, vchng, validcb) {
		this->setRequestType(Type::Color);
	}

	uint8_t red() {
		return (((this->currentValue() & 0xff0000) >> 16) & 0xff);
	}
	uint8_t green() {
		return (((this->currentValue() & 0xff00) >> 8) & 0xff);
	}
	uint8_t blue() {
		return (this->currentValue() & 0xff);
	}


};

typedef Color Colour; // 'cause we ain't all united statesians...

} /* namespace Request */
} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */



#endif /* SERIALUI_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQCOLOR_H_ */

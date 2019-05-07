/*
 * ItemReqDateTime.cpp
 *
 *  Created on: Jun 2, 2018
 *      Author: Pat Deegan
 *
 *  ItemReqDateTime is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#include "includes/menuitem/requests/ItemReqDateTime.h"

namespace SerialUI {
namespace Menu {
namespace Item {
namespace Request {


DateTime::DateTime(unsigned long int initVal, StaticString key,
		StaticString help, ValueChangedCallback vchng,
		ValidatorCallback validcb) :
		UnsignedLong(initVal, key, help,
				vchng, validcb) {

	setRequestType(Type::DateTime);
}


DateTime::DateTime(unsigned long int initVal, ValueChangedCallback vchng,
		ValidatorCallback validcb) :
		UnsignedLong(initVal, vchng, validcb) {
	setRequestType(Type::DateTime);
}

} /* namespace Request */
} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

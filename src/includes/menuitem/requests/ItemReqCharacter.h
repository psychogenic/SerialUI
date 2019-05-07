/*
 * ItemReqCharacter.h
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  ItemReqCharacter is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUIV3_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQCHARACTER_H_
#define SERIALUIV3_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQCHARACTER_H_

#include "../ItemRequest.h"
namespace SerialUI {
namespace Menu {
namespace Item {
namespace Request {

class Character : public TypedRequest<Type::Character, char>{
public:
	Character(char initVal='a',
			ValueChangedCallback vchng=NULL, ValidatorCallback validcb=NULL);
	Character(char initVal, StaticString key=NULL,
			StaticString help=NULL, ValueChangedCallback vchng=NULL,
			ValidatorCallback validcb=NULL);

	virtual bool getValue(Menu * callingMenu, char * v);
};

} /* namespace Request */
} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

#endif /* SERIALUIV3_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQCHARACTER_H_ */

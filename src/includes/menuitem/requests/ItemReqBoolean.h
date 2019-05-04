/*
 * ItemReqBoolean.h
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  ItemReqBoolean is part of the XYZPROJECT project.
 *  Copyright (C) 2018 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUIV3_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQBOOLEAN_H_
#define SERIALUIV3_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQBOOLEAN_H_
#include "../ItemRequest.h"

namespace SerialUI {
namespace Menu {
namespace Item {
namespace Request {

class Boolean : public TypedRequest<Type::Boolean, bool> {
public:

	Boolean(bool initVal=false, ValueChangedCallback vchng=NULL, ValidatorCallback validcb=NULL);
	Boolean(bool initVal, StaticString key,
			StaticString help, ValueChangedCallback vchng=NULL, ValidatorCallback validcb=NULL);

	virtual bool getValue(Menu * callingMenu, bool * v);
};

// TODO:FIXME typedef Boolean Toggle;
class Toggle : public TypedRequest<Type::Toggle, bool> {
public:

	Toggle(bool initVal=false, ValueChangedCallback vchng=NULL, ValidatorCallback validcb=NULL);
	Toggle(bool initVal, StaticString key,
			StaticString help, ValueChangedCallback vchng=NULL, ValidatorCallback validcb=NULL);

	virtual bool getValue(Menu * callingMenu, bool * v);
};

} /* namespace Request */
} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

#endif /* SERIALUIV3_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQBOOLEAN_H_ */

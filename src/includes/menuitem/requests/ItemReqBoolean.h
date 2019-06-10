/*
 * ItemReqBoolean.h
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  ItemReqBoolean is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUIV3_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQBOOLEAN_H_
#define SERIALUIV3_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQBOOLEAN_H_
#include "../ItemRequest.h"

namespace SerialUI {
namespace Menu {
namespace Item {
namespace Request {
typedef TypedRequest<Type::Boolean, bool> BooleanBase;
class Boolean : public BooleanBase {
public:

	Boolean(bool initVal=false, ValueChangedCallback vchng=NULL, ValidatorCallback validcb=NULL);
	Boolean(bool initVal, StaticString key,
			StaticString help, ValueChangedCallback vchng=NULL, ValidatorCallback validcb=NULL);

	virtual bool getValue(Menu * callingMenu, bool * v);

	ITEMPYTHONOVERRIDE_VALIDATION_DECL(bool);
};

// TODO:FIXME typedef Boolean Toggle;
typedef TypedRequest<Type::Toggle, bool> ToggleBase;
class Toggle : public ToggleBase {
public:

	Toggle(bool initVal=false, ValueChangedCallback vchng=NULL, ValidatorCallback validcb=NULL);
	Toggle(bool initVal, StaticString key,
			StaticString help, ValueChangedCallback vchng=NULL, ValidatorCallback validcb=NULL);

	virtual bool getValue(Menu * callingMenu, bool * v);
	ITEMPYTHONOVERRIDE_VALIDATION_DECL(bool);
};

} /* namespace Request */
} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

#endif /* SERIALUIV3_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQBOOLEAN_H_ */

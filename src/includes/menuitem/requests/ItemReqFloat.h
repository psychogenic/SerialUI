/*
 * ItemReqFloat.h
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  ItemReqFloat is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUIV3_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQFLOAT_H_
#define SERIALUIV3_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQFLOAT_H_

#include "../ItemRequest.h"


namespace SerialUI {
namespace Menu {
namespace Item {
namespace Request {

class Float: public COUNTABLEREQCLASS_PARENT(Type::Float, float, Float)  {
public:
	Float(float initVal=0.0, ValueChangedCallback vchng=NULL, ValidatorCallback validcb=NULL);


	Float(float initVal, StaticString key=NULL,
			StaticString help=NULL, ValueChangedCallback vchng=NULL, ValidatorCallback validcb=NULL);


	virtual bool getValue(Menu * callingMenu, float * v);

	ITEMPYTHONOVERRIDE_VALIDATION_DECL(float);

	COUTABLEREQCLASS_USINGALLOPS(Type::Float, float, Float);

};

} /* namespace Request */
} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

#endif /* SERIALUIV3_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQFLOAT_H_ */

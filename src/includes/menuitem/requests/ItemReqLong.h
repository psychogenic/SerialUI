/*
 * ItemReqLong.h
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  ItemReqLong is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUIV3_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQLONG_H_
#define SERIALUIV3_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQLONG_H_

#include "../ItemRequest.h"

namespace SerialUI {
namespace Menu {
namespace Item {
namespace Request {

class Long : public COUNTABLEREQCLASS_PARENT(Type::LongInt, long int, Long){
public:
	Long(long int initVal, ValueChangedCallback vchng=NULL, ValidatorCallback validcb=NULL);


	Long(long int initVal=0, StaticString key=NULL,
			StaticString help=NULL, ValueChangedCallback vchng=NULL, ValidatorCallback validcb=NULL);


	virtual bool getValue(Menu * callingMenu, long int * v);


	ITEMPYTHONOVERRIDE_VALIDATION_DECL(long int);

	COUTABLEREQCLASS_USINGALLOPS(Type::LongInt, long int, Long);
};

} /* namespace Request */
} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

#endif /* SERIALUIV3_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQLONG_H_ */

/*
 * ItemReqBoundedLong.h
 *
 *  Created on: May 25, 2018
 *      Author: Pat Deegan
 *
 *  ItemReqBoundedLong is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUIV3_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQBOUNDEDLONG_H_
#define SERIALUIV3_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQBOUNDEDLONG_H_

#include "../ItemRequest.h"

namespace SerialUI {
namespace Menu {
namespace Item {
namespace Request {

class BoundedLong: public COUNTABLEREQCLASS_PARENT(Type::BoundedLongInt, long int, BoundedLong) {
public:
	BoundedLong(long int initVal,
			StaticString key,
			StaticString help,
			long int min, long int max,
			ValueChangedCallback vchng=NULL,
			ValidatorCallback validcb=NULL);

	BoundedLong(
			long int initVal=0,
			long int min=0, long int max=1,
			ValueChangedCallback vchng=NULL,
			ValidatorCallback validcb=NULL);


	virtual bool getValue(Menu * callingMenu, long int * v);
	virtual bool valueIsValid(long int & val) ;

	inline void setStep(uint8_t val) { if (val) { _stepVal = val;}}
	inline long int minimum() { return _minAllowed;}
	inline long int maximum() { return _maxAllowed;}
	inline uint8_t step() { return _stepVal;}

	COUTABLEREQCLASS_USINGALLOPS(Type::BoundedLongInt, long int, BoundedLong);

	ITEMPYTHONOVERRIDE_VALIDATION_DECL(long int);

protected:
	inline void setMinimum(long int i) { _minAllowed = i;}
	inline void setMaximum(long int i) { _maxAllowed = i;}
	void ensureSaneBounds();

private:

	long int _minAllowed;
	long int _maxAllowed;
	uint8_t _stepVal;
};

} /* namespace Request */
} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */

#endif /* SERIALUIV3_SRC_INCLUDES_MENUITEM_REQUESTS_ITEMREQBOUNDEDLONG_H_ */

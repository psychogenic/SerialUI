/*
 * TrackedNum.h
 *
 *  Created on: May 26, 2018
 *      Author: Pat Deegan
 *
 *  TrackedNum is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_TRACKED_TRACKEDNUM_H_
#define SERIALUI_SRC_INCLUDES_TRACKED_TRACKEDNUM_H_

#include "TrackedVariable.h"

namespace SerialUI {
namespace Tracked {


#define DEF_COUNTABLECLASS(name, tval, trackedType) \
class name: public StateImplCountable<Type::tval, trackedType, name> \
{ \
public: \
	name(StaticString n) : StateImplCountable(n, 0) {} \
	name(StaticString n, trackedType v) : StateImplCountable(n, v) {} \
	name(StaticString n, ID sid, trackedType v) : StateImplCountable(n, sid, v) {} \
	using StateImplCountable<Type::tval, trackedType, name>::operator=; \
	using StateImplCountable<Type::tval, trackedType, name>::operator==; \
	using StateImplCountable<Type::tval, trackedType, name>::operator!=; \
	using StateImplCountable<Type::tval, trackedType, name>::operator+; \
	using StateImplCountable<Type::tval, trackedType, name>::operator-; \
	using StateImplCountable<Type::tval, trackedType, name>::operator*; \
	using StateImplCountable<Type::tval, trackedType, name>::operator/; \
	using StateImplCountable<Type::tval, trackedType, name>::operator+=; \
	using StateImplCountable<Type::tval, trackedType, name>::operator-=; \
	using StateImplCountable<Type::tval, trackedType, name>::operator*=; \
	using StateImplCountable<Type::tval, trackedType, name>::operator/=; \
}


DEF_COUNTABLECLASS(UnsignedInteger, UInt, unsigned int);
DEF_COUNTABLECLASS(Integer, Int, int);
DEF_COUNTABLECLASS(Float, Float, float);
DEF_COUNTABLECLASS(Character, Char, unsigned char);
DEF_COUNTABLECLASS(BoundedLongInt, BoundedLongInt, long int);
DEF_COUNTABLECLASS(OptionsList, BoundedLongInt, long int);
DEF_COUNTABLECLASS(DateTime, DateTime, unsigned long int);
DEF_COUNTABLECLASS(Time, Time, unsigned long int);
DEF_COUNTABLECLASS(Event, Event, unsigned long int);


}
}


#endif /* SERIALUI_SRC_INCLUDES_TRACKED_TRACKEDNUM_H_ */

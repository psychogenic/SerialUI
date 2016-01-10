/*
 * SUIStateTracking.cpp
 *
 *  Created on: Aug 10, 2014
 *      Author: malcalypse
 */


#include "includes/SUIStateTracking.h"


#if 1
// def SUI_ENABLE_STATE_TRACKER

namespace SUI {

TrackedStateVariableDetailsStruct::TrackedStateVariableDetailsStruct(SUI_FLASHSTRING_PTR n,
		TrackedType t, void * variable ) : type(t), name(n)
{
	switch (type)
	{

	case SUITracked_Bool:
		ptr_bool = (bool*) variable;
		lastval_bool = *ptr_bool;
		break;
	case SUITracked_UInt:
		ptr_int = (unsigned long*) variable;
		lastval_int = *ptr_int;
		break;
	case SUITracked_Float:
		ptr_float = (float*) variable;
		lastval_float = *ptr_float;
		break;
	}
}


bool TrackedStateVariableDetails::hasChanged()
{

	switch (type)
	{

	case SUITracked_Bool:
		return (lastval_bool != *ptr_bool);
		break;
	case SUITracked_UInt:
		return (lastval_int != *ptr_int);
		break;
	case SUITracked_Float:
		return (lastval_float != *ptr_float);
		break;
	}

	return false;
}

} /* namespace SUI */


#endif

